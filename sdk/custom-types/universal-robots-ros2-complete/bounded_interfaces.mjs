/** Generated custom RPC owners. All aliases must be explicitly granted by the host.
 * Callers own each facade and must inspect closeChecked()/revokeChecked(). No
 * builtin service/action factory, native thread, timer or implicit retry is used.
 * Place beside bounded_records.mjs under sdk/custom-types/<generated-directory>.
 * Relocation requires updating only the SDK module imports below. */
import {Status,RosError,Session,Capability} from '../../javascript/roswasm.mjs';
import {EventKind,EventBinding} from '../../javascript/events.mjs';
import * as records from './bounded_records.mjs';
const empty=new Uint8Array(),MAX_SLOTS=1024;
const apply=Reflect.apply,openCapability=Session.prototype.open;
const closeCapability=Capability.prototype.close,revokeCapability=Capability.prototype.revoke;
const noArgs=Object.freeze([]);
function invalid(message='invalid custom RPC envelope'){throw new RosError(Status.INVALID,message);}
function checked(reply,operation){if(reply.status){const error=new RosError(reply.status,operation);error.required=reply.required;throw error;}return reply.bytes;}
function zero(reply,operation){if(checked(reply,operation).length)invalid();}
function idBytes(id,allowZero=false){if(typeof id!=='bigint'||id<0n||id>0xffffffffffffffffn||(!allowZero&&!id))invalid();const b=new Uint8Array(8);new DataView(b.buffer).setBigUint64(0,id,true);return b;}
function readId(bytes){if(bytes.length<8)invalid();const id=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength).getBigUint64(0,true);if(!id)invalid();return id;}
function framed(id,body,kind=null){const prefix=idBytes(id),out=new Uint8Array(body.length+(kind===null?8:9));out.set(prefix);if(kind!==null)out[8]=kind;out.set(body,kind===null?8:9);return out;}
function decode(codec,bytes){try{return codec.decode(bytes);}catch(error){if(error instanceof RosError)throw error;const failure=new RosError(Status.INVALID,'custom component decode');failure.cause=error;throw failure;}}
function slots(count){if(!Number.isInteger(count)||count<1||count>MAX_SLOTS)invalid('bounded local owner capacity');return count;}

/** Constructor failures always use this preallocated carrier. primaryKnown and
 * primary retain the original thrown value, including null/undefined/false/0
 * and frozen objects; the original object is never modified. cleanupThrew is
 * independent of cleanupException. diagnostics() returns copied facts only.
 * retryCleanup() retries retained local preflight owners, never an owner whose
 * genuine SDK lifetime already entered close/revoke. No RPC is replayed. */
export class RpcConstructionFailure extends Error {
  #retry;#inspect;
  constructor(retry,inspect){
    super('Custom RPC construction failed; inspect primary and cleanup facts');
    this.name='RpcConstructionFailure';this.primaryKnown=false;this.primary=undefined;
    this.cleanup=null;this.cleanupAttempted=false;this.cleanupCalls=0;this.cleanupReturned=false;
    this.cleanupThrew=false;this.cleanupException=undefined;
    this.#retry=retry;this.#inspect=inspect;
  }
  retryCleanup(){return this.#retry();}
  diagnostics(){return this.#inspect();}
}

function ownerFact(role,alias){return {role,alias,handle:null,acquired:false,openInvoked:false,
  openReturned:false,openThrew:false,openException:undefined,duplicateOf:null,
  calls:0,invoked:false,inFlight:false,attempted:false,returned:false,status:null,threw:false,
  exception:undefined,retained:false,externallyClosed:false,failureKnown:false,
  firstFailureThrew:false,firstFailureStatus:null,firstFailureException:undefined};}
function copyFact(row){return {...row};}
function failedFact(row,threw,status,error){
  if(!row.failureKnown){row.failureKnown=true;row.firstFailureThrew=threw;
    row.firstFailureStatus=status;row.firstFailureException=error;}
}
function cleanupCode(row){
  if(row.firstFailureThrew)return Status.INVALID;
  const value=row.firstFailureStatus;
  return Number.isInteger(value)&&value>0?value:Status.INVALID;
}

/** Private capabilities are never returned to a handler. A local busy lease
 * spans poll/decode/callback/await/respond; callback exceptions release it in
 * finally and leave the already delivered ticket available for manual response.
 * Construction opens all distinct roles before advertising. Rollback closes
 * each acquired owner once and preserves the original refusal and cleanup facts. */
class Owners {
  #caps;#rows;#failure;#busy=false;#cleaning=false;
  constructor(session,version,aliases){
    if(!(session instanceof Session)||session.version!==version)throw new RosError(Status.INVALID,'selected genuine ROS Session/backend');
    if(!Array.isArray(aliases)||!aliases.length||aliases.some(a=>typeof a!=='string'||!a.length||a.includes('\0'))||new Set(aliases).size!==aliases.length)invalid('distinct explicit role aliases');
    // All owner slots, per-role call arguments and the failure carrier exist
    // before the first open. A returned owner is stored before later checks.
    this.#caps=new Array(aliases.length).fill(null);
    this.#rows=aliases.map((alias,i)=>ownerFact(i,alias));
    const argumentsByRole=aliases.map(alias=>[alias]);
    const duplicate=new RosError(Status.CONFLICT,'duplicate role handle');
    this.#failure=new RpcConstructionFailure(()=>this.#retryCleanup(),()=>this.diagnostics());
    try{
      for(let i=0;i<aliases.length;++i){
        const row=this.#rows[i];row.openInvoked=true;
        try{this.#caps[i]=apply(openCapability,session,argumentsByRole[i]);row.openReturned=true;}
        catch(error){row.openThrew=true;row.openException=error;throw error;}
        row.acquired=true;row.retained=true;row.handle=this.#caps[i].handle;
        for(let j=0;j<i;++j)if(this.#rows[j].acquired&&this.#rows[j].handle===row.handle){row.duplicateOf=j;throw duplicate;}
      }
    }catch(error){this.fail(error);}
  }
  cap(i){if(this.#cleaning)throw new RosError(Status.CONFLICT,'active custom cleanup');const cap=this.#caps[i];if(!cap||cap.closed)throw new RosError(Status.BAD_HANDLE,'retired custom RPC owner');return cap;}
  check(){if(this.#busy||this.#cleaning)throw new RosError(Status.CONFLICT,'active custom callback/cleanup');this.cap(0);}
  enter(){this.check();this.#busy=true;}
  leave(){this.#busy=false;}
  #refresh(){for(let i=0;i<this.#caps.length;++i){const cap=this.#caps[i];if(cap!==null){const row=this.#rows[i];row.retained=!cap.closed;if(cap.closed&&row.invoked)row.attempted=true;}}}
  diagnostics(){this.#refresh();return Object.freeze(this.#rows.map(row=>Object.freeze(copyFact(row))));}
  #retryCleanup(){
    const failure=this.#failure;failure.cleanupAttempted=true;failure.cleanupCalls+=1;
    try{failure.cleanup=this.cleanup(failure.primary);failure.cleanupReturned=true;return failure.cleanup;}
    catch(error){if(!failure.cleanupThrew)failure.cleanupException=error;failure.cleanupThrew=true;throw error;}
  }
  fail(primary){
    const failure=this.#failure;failure.primaryKnown=true;failure.primary=primary;
    try{this.#retryCleanup();}catch{/* Same carrier retains independent cleanup exception. */}
    throw failure;
  }
  cleanup(primary=0,revoke=false){
    // Allocate every output slot before any cleanup delegate. The private
    // lifetime ledger survives a later snapshot/freeze allocation failure.
    this.#refresh();const statuses=this.#rows.map(copyFact);
    const report={primary,cleanupError:0,cleanupThrew:false,cleanupException:undefined,
      blocked:this.#busy||this.#cleaning,invoked:0,attempted:0,failed:0,retained:0,statuses};
    if(report.blocked){report.cleanupError=Status.CONFLICT;}
    else{
      this.#cleaning=true;
      try{
        for(let i=this.#caps.length-1;i>=0;--i){
          const cap=this.#caps[i],row=this.#rows[i];if(cap===null)continue;
          if(cap.closed){row.externallyClosed=!row.attempted;row.retained=false;continue;}
          // A duplicate raw handle cannot be made retired by writing the SDK's
          // read-only closed mirror or by issuing an ambiguous second close.
          if(row.duplicateOf!==null){row.retained=true;failedFact(row,false,Status.CONFLICT,undefined);continue;}
          row.calls+=1;row.invoked=true;row.inFlight=true;row.returned=false;row.status=null;row.threw=false;row.exception=undefined;
          report.invoked+=1;
          try{row.status=apply(revoke?revokeCapability:closeCapability,cap,noArgs);row.returned=true;}
          catch(error){row.threw=true;row.exception=error;}
          finally{row.inFlight=false;row.attempted=cap.closed;row.retained=!cap.closed;if(row.attempted)report.attempted+=1;}
          if(row.threw||row.status!==0)failedFact(row,row.threw,row.status,row.exception);
        }
      }finally{this.#cleaning=false;}
    }
    for(let i=0;i<this.#rows.length;++i){
      const row=this.#rows[i];if(row.retained)report.retained+=1;
      if(row.failureKnown){report.failed+=1;if(report.cleanupError===0)report.cleanupError=cleanupCode(row);
        if(row.firstFailureThrew&&!report.cleanupThrew){report.cleanupThrew=true;report.cleanupException=row.firstFailureException;}}
      Object.assign(statuses[i],row);Object.freeze(statuses[i]);
    }
    Object.freeze(statuses);return Object.freeze(report);
  }
}

/** Typed service source with optional independently granted completion observer.
 * observerAlias=null provides synchronous calls only. submit() always uses the
 * real service_submit import; it never defers a blocking call. Its ID indicates
 * admission, not completion. This facade exclusively polls its fresh observer.
 * Completion bodies are decoded by the existing SDK envelope decoder first. */
class TypedServiceClient {
  #owners;#request;#response;#limit;#pending=new Set();#binding=null;#version;
  constructor(session,version,alias,request,response,observerAlias=null,capacity=8){
    this.#limit=slots(capacity);this.#request=request;this.#response=response;this.#version=version;
    this.#owners=new Owners(session,version,observerAlias===null?[alias]:[alias,observerAlias]);
    try{if(observerAlias!==null)this.#binding=new EventBinding(version,this.#owners.cap(0).handle);}
    catch(error){this.#owners.fail(error);}
  }
  call(value){this.#owners.check();return decode(this.#response,checked(this.#owners.cap(0).request('service_call',this.#request.encode(value)),'service_call'));}
  submit(value){this.#owners.check();if(!this.#binding)throw new RosError(Status.DENIED,'explicit completion observer required');if(this.#pending.size===this.#limit)throw new RosError(Status.TOO_LARGE,'pending service capacity');
    const admitted=this.#owners.cap(0).submitService(this.#owners.cap(1),this.#request.encode(value));if(admitted.status)throw new RosError(admitted.status,'service_submit');const id=admitted.requestId;
    if(typeof id!=='bigint'||!id||this.#pending.has(id))invalid('nonzero unique admitted request ID');this.#pending.add(id);return id;}
  pollCompletion(){this.#owners.check();if(!this.#binding)throw new RosError(Status.DENIED,'explicit completion observer required');
    const bytes=checked(this.#owners.cap(1).pollEventV1(),'event_poll_v1'),event=decode(this.#binding,bytes);
    if(event.kind===EventKind.RESOURCE_CLOSED){throw new RosError(Status.BAD_HANDLE,'service source retired; pending requests remain observable');}
    if(!this.#pending.has(event.correlation))invalid('foreign service correlation');
    if(event.kind===EventKind.RESOURCE_ERROR){this.#pending.delete(event.correlation);return Object.freeze({requestId:event.correlation,error:event.error,value:null,event});}
    if(event.kind!==EventKind.SERVICE_RESPONSE)invalid('service completion kind');
    const value=decode(this.#response,event.body);this.#pending.delete(event.correlation);return Object.freeze({requestId:event.correlation,error:0,value,event});}
  closeChecked(primary=0){const r=this.#owners.cleanup(primary);if(!r.blocked&&r.retained===0)this.#pending.clear();return r;}
  revokeChecked(primary=0){const r=this.#owners.cleanup(primary,true);if(!r.blocked&&r.retained===0)this.#pending.clear();return r;}
}

/** Two separate advertise/respond grants; response IDs must have been polled
 * by this owner. Successful response removes one ID; malformed values and host
 * refusal retain it. Host ownership, deadline and revocation remain mandatory. */
class TypedServer {
  #owners;#input;#output;#feedback;#action;#live=new Set();#limit;
  constructor(session,version,advertise,respond,input,output,action,capacity=8,feedback=null){
    this.#limit=slots(capacity);this.#input=input;this.#output=output;this.#feedback=feedback;this.#action=action;this.#owners=new Owners(session,version,[advertise,respond]);
    try{if(action)zero(this.#owners.cap(0).request('action_advertise'),'action_advertise');else{const c=this.#owners.cap(0),code=c.session.raw.service_advertise(c.handle);if(code)throw new RosError(code,'service_advertise');}}
    catch(error){this.#owners.fail(error);}
  }
  #poll(){if(this.#live.size===this.#limit)throw new RosError(Status.TOO_LARGE,'incoming ticket capacity');const body=checked(this.#owners.cap(0).poll(),'event_poll'),id=readId(body);if(this.#live.has(id))invalid('duplicate incoming ID');const value=decode(this.#input,body.subarray(8));this.#live.add(id);return Object.freeze({id,value});}
  pollOnce(){this.#owners.check();return this.#poll();}
  respond(id,value,kind=null){if(!this.#live.has(id))throw new RosError(Status.BAD_HANDLE,'foreign or completed ticket');if(this.#action){if(!Number.isInteger(kind)||kind<1||kind>4)invalid('action response kind');}else if(kind!==null)invalid();
    const codec=this.#action&&kind===1?this.#feedback:this.#output;
    zero(this.#owners.cap(1).request(this.#action?'action_respond':'service_respond',framed(id,codec.encode(value),kind)),this.#action?'action_respond':'service_respond');if(!this.#action||kind!==1)this.#live.delete(id);}
  async spinOnce(handler){if(typeof handler!=='function')invalid('local typed handler required');this.#owners.enter();try{const incoming=this.#poll(),value=await handler(incoming.value,incoming.id);if(!this.#action)this.respond(incoming.id,value);return incoming;}finally{this.#owners.leave();}}
  closeChecked(primary=0){const r=this.#owners.cleanup(primary);if(!r.blocked&&r.retained===0)this.#live.clear();return r;}
  revokeChecked(primary=0){const r=this.#owners.cleanup(primary,true);if(!r.blocked&&r.retained===0)this.#live.clear();return r;}
}

/** Custom action client: send/cancel-own/feedback/result are distinct aliases;
 * cancel-any is optional and never inferred from cancel-own. Cancellation ACK
 * does not retire a local goal. Only a validated terminal result does so. */
class TypedActionClient {
  #owners;#goal;#feedback;#result;#goals=new Set();#limit;#any;
  constructor(session,version,aliases,goal,result,feedback,capacity=8){
    const keys=['send','cancelOwn','feedback','result'];if(!aliases||Object.keys(aliases).some(k=>![...keys,'cancelAny'].includes(k)))invalid('explicit action roles');
    this.#limit=slots(capacity);this.#goal=goal;this.#feedback=feedback;this.#result=result;this.#any=aliases.cancelAny!==undefined;
    this.#owners=new Owners(session,version,[...keys.map(k=>aliases[k]),...(this.#any?[aliases.cancelAny]:[])]);
  }
  #owns(id){idBytes(id);if(!this.#goals.has(id))throw new RosError(Status.BAD_HANDLE,'foreign goal');}
  sendGoal(value){this.#owners.check();if(this.#goals.size===this.#limit)throw new RosError(Status.TOO_LARGE,'goal capacity');const b=checked(this.#owners.cap(0).request('action_send_goal',this.#goal.encode(value)),'action_send_goal');if(b.length!==8)invalid();const id=readId(b);if(this.#goals.has(id))invalid('duplicate goal');this.#goals.add(id);return id;}
  cancelOwn(id){this.#owners.check();this.#owns(id);const b=checked(this.#owners.cap(1).request('action_cancel_own_goal',idBytes(id)),'action_cancel_own_goal');if(b.length!==1||b[0]>1)invalid();return b[0]===1;}
  cancelAny(id){this.#owners.check();if(!this.#any)throw new RosError(Status.DENIED,'cancel-any grant required');const b=checked(this.#owners.cap(4).request('action_cancel_any_goal',idBytes(id,true)),'action_cancel_any_goal');if(b.length!==1||b[0]>1)invalid();return b[0]===1;}
  cancelAll(){return this.cancelAny(0n);}
  feedback(id){this.#owners.check();this.#owns(id);const b=checked(this.#owners.cap(2).request('action_receive_feedback',idBytes(id)),'action_receive_feedback');if(readId(b)!==id)invalid();return decode(this.#feedback,b.subarray(8));}
  result(id){this.#owners.check();this.#owns(id);const b=checked(this.#owners.cap(3).request('action_receive_result',idBytes(id)),'action_receive_result');if(b.length<9||readId(b)!==id||b[8]<2||b[8]>4)invalid();const value=decode(this.#result,b.subarray(9));this.#goals.delete(id);return Object.freeze({id,kind:b[8],value});}
  closeChecked(primary=0){const r=this.#owners.cleanup(primary);if(!r.blocked&&r.retained===0)this.#goals.clear();return r;}
  revokeChecked(primary=0){const r=this.#owners.cleanup(primary,true);if(!r.blocked&&r.retained===0)this.#goals.clear();return r;}
}

/** Custom ur_dashboard_msgs/action/SetMode client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros2Ur_dashboard_msgsActionSetModeClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,2,aliases,records.Ros2Ur_dashboard_msgsActionSetMode_GoalCodec,records.Ros2Ur_dashboard_msgsActionSetMode_ResultCodec,records.Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec,capacity);}}

/** Custom ur_dashboard_msgs/action/SetMode server; callbacks explicitly choose their responses. */
export class Ros2Ur_dashboard_msgsActionSetModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsActionSetMode_GoalCodec,records.Ros2Ur_dashboard_msgsActionSetMode_ResultCodec,true,capacity,records.Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Complete ur_dashboard_msgs/srv/AddToLog typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvAddToLogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec,records.Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/AddToLog typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvAddToLogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec,records.Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadProgram typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvDownloadProgramClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec,records.Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadProgram typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvDownloadProgramServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec,records.Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadSupportFile typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec,records.Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/DownloadSupportFile typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFileServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec,records.Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateFlightReport typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateFlightReport typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReportServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateSupportFile typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GenerateSupportFile typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFileServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetLoadedProgram typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetLoadedProgram typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgramServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetOperationalMode typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetOperationalModeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetOperationalMode typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetOperationalModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPolyScopeVersion typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPolyScopeVersion typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetProgramState typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetProgramStateClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetProgramState typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetProgramStateServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPrograms typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetProgramsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetPrograms typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetProgramsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotMode typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotMode typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotModel typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModelClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetRobotModel typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModelServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyMode typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyModeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyMode typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyStatus typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSafetyStatus typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatusServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSerialNumber typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetSerialNumberClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetSerialNumber typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetSerialNumberServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetUserRole typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvGetUserRoleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/GetUserRole typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvGetUserRoleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec,records.Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsInRemoteControl typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec,records.Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsInRemoteControl typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControlServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec,records.Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramRunning typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvIsProgramRunningClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec,records.Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramRunning typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvIsProgramRunningServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec,records.Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramSaved typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvIsProgramSavedClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec,records.Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/IsProgramSaved typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvIsProgramSavedServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec,records.Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/Load typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvLoadClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvLoad_RequestCodec,records.Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/Load typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvLoadServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvLoad_RequestCodec,records.Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/Popup typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvPopupClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvPopup_RequestCodec,records.Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/Popup typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvPopupServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvPopup_RequestCodec,records.Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/RawRequest typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvRawRequestClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec,records.Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/RawRequest typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvRawRequestServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec,records.Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetOperationalMode typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvSetOperationalModeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetOperationalMode typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvSetOperationalModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec,records.Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetUserRole typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvSetUserRoleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec,records.Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/SetUserRole typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvSetUserRoleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec,records.Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec,false,capacity);}}

/** Complete ur_dashboard_msgs/srv/UploadProgram typed client; optional observer is independently granted. */
export class Ros2Ur_dashboard_msgsSrvUploadProgramClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec,records.Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_dashboard_msgs/srv/UploadProgram typed server; callback replacement is caller-managed. */
export class Ros2Ur_dashboard_msgsSrvUploadProgramServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec,records.Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec,false,capacity);}}

/** Custom ur_msgs/action/FollowJointTrajectoryUntil client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntilClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,2,aliases,records.Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec,records.Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec,records.Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec,capacity);}}

/** Custom ur_msgs/action/FollowJointTrajectoryUntil server; callbacks explicitly choose their responses. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntilServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec,records.Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec,true,capacity,records.Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom ur_msgs/action/SendScript client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros2Ur_msgsActionSendScriptClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,2,aliases,records.Ros2Ur_msgsActionSendScript_GoalCodec,records.Ros2Ur_msgsActionSendScript_ResultCodec,records.Ros2Ur_msgsActionSendScript_FeedbackCodec,capacity);}}

/** Custom ur_msgs/action/SendScript server; callbacks explicitly choose their responses. */
export class Ros2Ur_msgsActionSendScriptServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsActionSendScript_GoalCodec,records.Ros2Ur_msgsActionSendScript_ResultCodec,true,capacity,records.Ros2Ur_msgsActionSendScript_FeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom ur_msgs/action/ToolContact client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros2Ur_msgsActionToolContactClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,2,aliases,records.Ros2Ur_msgsActionToolContact_GoalCodec,records.Ros2Ur_msgsActionToolContact_ResultCodec,records.Ros2Ur_msgsActionToolContact_FeedbackCodec,capacity);}}

/** Custom ur_msgs/action/ToolContact server; callbacks explicitly choose their responses. */
export class Ros2Ur_msgsActionToolContactServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsActionToolContact_GoalCodec,records.Ros2Ur_msgsActionToolContact_ResultCodec,true,capacity,records.Ros2Ur_msgsActionToolContact_FeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Complete ur_msgs/srv/GetRobotSoftwareVersion typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvGetRobotSoftwareVersionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec,records.Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/GetRobotSoftwareVersion typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvGetRobotSoftwareVersionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec,records.Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetAnalogOutput typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetAnalogOutputClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec,records.Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetAnalogOutput typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetAnalogOutputServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec,records.Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetForceMode typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetForceModeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetForceMode_RequestCodec,records.Ros2Ur_msgsSrvSetForceMode_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetForceMode typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetForceModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetForceMode_RequestCodec,records.Ros2Ur_msgsSrvSetForceMode_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetFrictionModelParameters typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetFrictionModelParametersClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec,records.Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetFrictionModelParameters typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetFrictionModelParametersServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec,records.Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetGravity typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetGravityClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetGravity_RequestCodec,records.Ros2Ur_msgsSrvSetGravity_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetGravity typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetGravityServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetGravity_RequestCodec,records.Ros2Ur_msgsSrvSetGravity_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetIO typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetIOClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetIO_RequestCodec,records.Ros2Ur_msgsSrvSetIO_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetIO typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetIOServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetIO_RequestCodec,records.Ros2Ur_msgsSrvSetIO_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetPayload typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetPayloadClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetPayload_RequestCodec,records.Ros2Ur_msgsSrvSetPayload_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetPayload typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetPayloadServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetPayload_RequestCodec,records.Ros2Ur_msgsSrvSetPayload_ResponseCodec,false,capacity);}}

/** Complete ur_msgs/srv/SetSpeedSliderFraction typed client; optional observer is independently granted. */
export class Ros2Ur_msgsSrvSetSpeedSliderFractionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,2,alias,records.Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec,records.Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec,observerAlias,capacity);}}

/** Complete ur_msgs/srv/SetSpeedSliderFraction typed server; callback replacement is caller-managed. */
export class Ros2Ur_msgsSrvSetSpeedSliderFractionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,2,advertise,respond,records.Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec,records.Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec,false,capacity);}}
