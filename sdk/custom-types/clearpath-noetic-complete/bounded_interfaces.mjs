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

/** Custom clearpath_camera_msgs/InspectPOI client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_camera_msgsInspectPOIClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_camera_msgsInspectPOIGoalCodec,records.Ros1Clearpath_camera_msgsInspectPOIResultCodec,records.Ros1Clearpath_camera_msgsInspectPOIFeedbackCodec,capacity);}}

/** Custom clearpath_camera_msgs/InspectPOI server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_camera_msgsInspectPOIServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_camera_msgsInspectPOIGoalCodec,records.Ros1Clearpath_camera_msgsInspectPOIResultCodec,true,capacity,records.Ros1Clearpath_camera_msgsInspectPOIFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_dock_msgs/LocalDock client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_dock_msgsLocalDockClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_dock_msgsLocalDockGoalCodec,records.Ros1Clearpath_dock_msgsLocalDockResultCodec,records.Ros1Clearpath_dock_msgsLocalDockFeedbackCodec,capacity);}}

/** Custom clearpath_dock_msgs/LocalDock server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_dock_msgsLocalDockServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsLocalDockGoalCodec,records.Ros1Clearpath_dock_msgsLocalDockResultCodec,true,capacity,records.Ros1Clearpath_dock_msgsLocalDockFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_dock_msgs/LocalUndock client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_dock_msgsLocalUndockClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_dock_msgsLocalUndockGoalCodec,records.Ros1Clearpath_dock_msgsLocalUndockResultCodec,records.Ros1Clearpath_dock_msgsLocalUndockFeedbackCodec,capacity);}}

/** Custom clearpath_dock_msgs/LocalUndock server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_dock_msgsLocalUndockServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsLocalUndockGoalCodec,records.Ros1Clearpath_dock_msgsLocalUndockResultCodec,true,capacity,records.Ros1Clearpath_dock_msgsLocalUndockFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_dock_msgs/NetworkDock client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_dock_msgsNetworkDockClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_dock_msgsNetworkDockGoalCodec,records.Ros1Clearpath_dock_msgsNetworkDockResultCodec,records.Ros1Clearpath_dock_msgsNetworkDockFeedbackCodec,capacity);}}

/** Custom clearpath_dock_msgs/NetworkDock server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_dock_msgsNetworkDockServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsNetworkDockGoalCodec,records.Ros1Clearpath_dock_msgsNetworkDockResultCodec,true,capacity,records.Ros1Clearpath_dock_msgsNetworkDockFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_dock_msgs/RadiusDock client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_dock_msgsRadiusDockClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_dock_msgsRadiusDockGoalCodec,records.Ros1Clearpath_dock_msgsRadiusDockResultCodec,records.Ros1Clearpath_dock_msgsRadiusDockFeedbackCodec,capacity);}}

/** Custom clearpath_dock_msgs/RadiusDock server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_dock_msgsRadiusDockServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsRadiusDockGoalCodec,records.Ros1Clearpath_dock_msgsRadiusDockResultCodec,true,capacity,records.Ros1Clearpath_dock_msgsRadiusDockFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_localization_msgs/SurveyBaseStation client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_localization_msgsSurveyBaseStationClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_localization_msgsSurveyBaseStationGoalCodec,records.Ros1Clearpath_localization_msgsSurveyBaseStationResultCodec,records.Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackCodec,capacity);}}

/** Custom clearpath_localization_msgs/SurveyBaseStation server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_localization_msgsSurveyBaseStationServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsSurveyBaseStationGoalCodec,records.Ros1Clearpath_localization_msgsSurveyBaseStationResultCodec,true,capacity,records.Ros1Clearpath_localization_msgsSurveyBaseStationFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_mission_scheduler_msgs/RunScheduleByUuid client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalCodec,records.Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultCodec,records.Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackCodec,capacity);}}

/** Custom clearpath_mission_scheduler_msgs/RunScheduleByUuid server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoalCodec,records.Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResultCodec,true,capacity,records.Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/ExecuteMissionByUuid client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalCodec,records.Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultCodec,records.Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/ExecuteMissionByUuid server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsExecuteMissionByUuidServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoalCodec,records.Ros1Clearpath_navigation_msgsExecuteMissionByUuidResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/ExecuteNetworkGoTo client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalCodec,records.Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultCodec,records.Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/ExecuteNetworkGoTo server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsExecuteNetworkGoToServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoalCodec,records.Ros1Clearpath_navigation_msgsExecuteNetworkGoToResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/ExecuteNetworkMissionByUuid client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalCodec,records.Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultCodec,records.Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/ExecuteNetworkMissionByUuid server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoalCodec,records.Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/GetWaypointPath client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsGetWaypointPathClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsGetWaypointPathGoalCodec,records.Ros1Clearpath_navigation_msgsGetWaypointPathResultCodec,records.Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/GetWaypointPath server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsGetWaypointPathServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsGetWaypointPathGoalCodec,records.Ros1Clearpath_navigation_msgsGetWaypointPathResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsGetWaypointPathFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/Mission client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsMissionClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsMissionGoalCodec,records.Ros1Clearpath_navigation_msgsMissionResultCodec,records.Ros1Clearpath_navigation_msgsMissionFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/Mission server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsMissionGoalCodec,records.Ros1Clearpath_navigation_msgsMissionResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsMissionFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/NetworkGoToPOI client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsNetworkGoToPOIClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalCodec,records.Ros1Clearpath_navigation_msgsNetworkGoToPOIResultCodec,records.Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/NetworkGoToPOI server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsNetworkGoToPOIServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsNetworkGoToPOIGoalCodec,records.Ros1Clearpath_navigation_msgsNetworkGoToPOIResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/RecordPath client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsRecordPathClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsRecordPathGoalCodec,records.Ros1Clearpath_navigation_msgsRecordPathResultCodec,records.Ros1Clearpath_navigation_msgsRecordPathFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/RecordPath server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsRecordPathServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsRecordPathGoalCodec,records.Ros1Clearpath_navigation_msgsRecordPathResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsRecordPathFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/RunNetworkGoToPlanner client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalCodec,records.Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultCodec,records.Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/RunNetworkGoToPlanner server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoalCodec,records.Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/RunNetworkMissionPreviewer client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalCodec,records.Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultCodec,records.Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/RunNetworkMissionPreviewer server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoalCodec,records.Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/SmoothPath client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsSmoothPathClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsSmoothPathGoalCodec,records.Ros1Clearpath_navigation_msgsSmoothPathResultCodec,records.Ros1Clearpath_navigation_msgsSmoothPathFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/SmoothPath server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsSmoothPathServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsSmoothPathGoalCodec,records.Ros1Clearpath_navigation_msgsSmoothPathResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsSmoothPathFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Custom clearpath_navigation_msgs/UITask client; aliases send/cancelOwn/feedback/result are mandatory. */
export class Ros1Clearpath_navigation_msgsUITaskClient extends TypedActionClient {constructor(session,aliases,capacity=8){super(session,1,aliases,records.Ros1Clearpath_navigation_msgsUITaskGoalCodec,records.Ros1Clearpath_navigation_msgsUITaskResultCodec,records.Ros1Clearpath_navigation_msgsUITaskFeedbackCodec,capacity);}}

/** Custom clearpath_navigation_msgs/UITask server; callbacks explicitly choose their responses. */
export class Ros1Clearpath_navigation_msgsUITaskServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsUITaskGoalCodec,records.Ros1Clearpath_navigation_msgsUITaskResultCodec,true,capacity,records.Ros1Clearpath_navigation_msgsUITaskFeedbackCodec);}respondFeedback(id,value){return this.respond(id,value,1);}respondResult(id,kind,value){if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}

/** Complete clearpath_camera_msgs/q62AreaZoom typed client; optional observer is independently granted. */
export class Ros1Clearpath_camera_msgsQ62AreaZoomClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_camera_msgsQ62AreaZoomRequestCodec,records.Ros1Clearpath_camera_msgsQ62AreaZoomResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_camera_msgs/q62AreaZoom typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_camera_msgsQ62AreaZoomServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_camera_msgsQ62AreaZoomRequestCodec,records.Ros1Clearpath_camera_msgsQ62AreaZoomResponseCodec,false,capacity);}}

/** Complete clearpath_control_msgs/SetControlMode typed client; optional observer is independently granted. */
export class Ros1Clearpath_control_msgsSetControlModeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_control_msgsSetControlModeRequestCodec,records.Ros1Clearpath_control_msgsSetControlModeResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_control_msgs/SetControlMode typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_control_msgsSetControlModeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_control_msgsSetControlModeRequestCodec,records.Ros1Clearpath_control_msgsSetControlModeResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/AddDock typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsAddDockClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsAddDockRequestCodec,records.Ros1Clearpath_dock_msgsAddDockResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/AddDock typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsAddDockServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsAddDockRequestCodec,records.Ros1Clearpath_dock_msgsAddDockResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/ExportData typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsExportDataClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsExportDataRequestCodec,records.Ros1Clearpath_dock_msgsExportDataResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/ExportData typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsExportDataServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsExportDataRequestCodec,records.Ros1Clearpath_dock_msgsExportDataResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/GetDockByUuid typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsGetDockByUuidClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsGetDockByUuidRequestCodec,records.Ros1Clearpath_dock_msgsGetDockByUuidResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/GetDockByUuid typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsGetDockByUuidServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsGetDockByUuidRequestCodec,records.Ros1Clearpath_dock_msgsGetDockByUuidResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/ImportData typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsImportDataClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsImportDataRequestCodec,records.Ros1Clearpath_dock_msgsImportDataResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/ImportData typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsImportDataServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsImportDataRequestCodec,records.Ros1Clearpath_dock_msgsImportDataResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/RemoveDock typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsRemoveDockClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsRemoveDockRequestCodec,records.Ros1Clearpath_dock_msgsRemoveDockResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/RemoveDock typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsRemoveDockServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsRemoveDockRequestCodec,records.Ros1Clearpath_dock_msgsRemoveDockResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/SetDockLocationById typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsSetDockLocationByIdClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsSetDockLocationByIdRequestCodec,records.Ros1Clearpath_dock_msgsSetDockLocationByIdResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/SetDockLocationById typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsSetDockLocationByIdServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsSetDockLocationByIdRequestCodec,records.Ros1Clearpath_dock_msgsSetDockLocationByIdResponseCodec,false,capacity);}}

/** Complete clearpath_dock_msgs/SetDockLocationByName typed client; optional observer is independently granted. */
export class Ros1Clearpath_dock_msgsSetDockLocationByNameClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_dock_msgsSetDockLocationByNameRequestCodec,records.Ros1Clearpath_dock_msgsSetDockLocationByNameResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_dock_msgs/SetDockLocationByName typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_dock_msgsSetDockLocationByNameServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_dock_msgsSetDockLocationByNameRequestCodec,records.Ros1Clearpath_dock_msgsSetDockLocationByNameResponseCodec,false,capacity);}}

/** Complete clearpath_localization_msgs/ConvertCartesianToLatLon typed client; optional observer is independently granted. */
export class Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestCodec,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_localization_msgs/ConvertCartesianToLatLon typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_localization_msgsConvertCartesianToLatLonServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequestCodec,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponseCodec,false,capacity);}}

/** Complete clearpath_localization_msgs/ConvertCartesianToLatLonArray typed client; optional observer is independently granted. */
export class Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestCodec,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_localization_msgs/ConvertCartesianToLatLonArray typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequestCodec,records.Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponseCodec,false,capacity);}}

/** Complete clearpath_localization_msgs/ConvertLatLonToCartesian typed client; optional observer is independently granted. */
export class Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestCodec,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_localization_msgs/ConvertLatLonToCartesian typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_localization_msgsConvertLatLonToCartesianServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequestCodec,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponseCodec,false,capacity);}}

/** Complete clearpath_localization_msgs/ConvertLatLonToCartesianArray typed client; optional observer is independently granted. */
export class Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestCodec,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_localization_msgs/ConvertLatLonToCartesianArray typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequestCodec,records.Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponseCodec,false,capacity);}}

/** Complete clearpath_localization_msgs/ResetLocalization typed client; optional observer is independently granted. */
export class Ros1Clearpath_localization_msgsResetLocalizationClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_localization_msgsResetLocalizationRequestCodec,records.Ros1Clearpath_localization_msgsResetLocalizationResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_localization_msgs/ResetLocalization typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_localization_msgsResetLocalizationServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsResetLocalizationRequestCodec,records.Ros1Clearpath_localization_msgsResetLocalizationResponseCodec,false,capacity);}}

/** Complete clearpath_localization_msgs/SetDatum typed client; optional observer is independently granted. */
export class Ros1Clearpath_localization_msgsSetDatumClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_localization_msgsSetDatumRequestCodec,records.Ros1Clearpath_localization_msgsSetDatumResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_localization_msgs/SetDatum typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_localization_msgsSetDatumServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_localization_msgsSetDatumRequestCodec,records.Ros1Clearpath_localization_msgsSetDatumResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/DeleteLog typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsDeleteLogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsDeleteLogRequestCodec,records.Ros1Clearpath_logger_msgsDeleteLogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/DeleteLog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsDeleteLogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsDeleteLogRequestCodec,records.Ros1Clearpath_logger_msgsDeleteLogResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/DownloadLog typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsDownloadLogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsDownloadLogRequestCodec,records.Ros1Clearpath_logger_msgsDownloadLogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/DownloadLog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsDownloadLogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsDownloadLogRequestCodec,records.Ros1Clearpath_logger_msgsDownloadLogResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/GetAllLogs typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsGetAllLogsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsGetAllLogsRequestCodec,records.Ros1Clearpath_logger_msgsGetAllLogsResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/GetAllLogs typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsGetAllLogsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsGetAllLogsRequestCodec,records.Ros1Clearpath_logger_msgsGetAllLogsResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/RecordCustomEvent typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsRecordCustomEventClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsRecordCustomEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordCustomEventResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/RecordCustomEvent typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsRecordCustomEventServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsRecordCustomEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordCustomEventResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/RecordErrorEvent typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsRecordErrorEventClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsRecordErrorEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordErrorEventResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/RecordErrorEvent typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsRecordErrorEventServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsRecordErrorEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordErrorEventResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/RecordEvent typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsRecordEventClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsRecordEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordEventResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/RecordEvent typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsRecordEventServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsRecordEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordEventResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/RecordLocationEvent typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsRecordLocationEventClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsRecordLocationEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordLocationEventResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/RecordLocationEvent typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsRecordLocationEventServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsRecordLocationEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordLocationEventResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/RecordMediaEvent typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsRecordMediaEventClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsRecordMediaEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordMediaEventResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/RecordMediaEvent typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsRecordMediaEventServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsRecordMediaEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordMediaEventResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/RecordPowerEvent typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsRecordPowerEventClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsRecordPowerEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordPowerEventResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/RecordPowerEvent typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsRecordPowerEventServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsRecordPowerEventRequestCodec,records.Ros1Clearpath_logger_msgsRecordPowerEventResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/StartRecording typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsStartRecordingClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsStartRecordingRequestCodec,records.Ros1Clearpath_logger_msgsStartRecordingResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/StartRecording typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsStartRecordingServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsStartRecordingRequestCodec,records.Ros1Clearpath_logger_msgsStartRecordingResponseCodec,false,capacity);}}

/** Complete clearpath_logger_msgs/StopRecording typed client; optional observer is independently granted. */
export class Ros1Clearpath_logger_msgsStopRecordingClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_logger_msgsStopRecordingRequestCodec,records.Ros1Clearpath_logger_msgsStopRecordingResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_logger_msgs/StopRecording typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_logger_msgsStopRecordingServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_logger_msgsStopRecordingRequestCodec,records.Ros1Clearpath_logger_msgsStopRecordingResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddNetworkEdge typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddNetworkEdge typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsAddNetworkEdgeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddNetworkPath typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsAddNetworkPathClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddNetworkPath typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsAddNetworkPathServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsAddNetworkPathRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddNetworkPathResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddNetworkPoint typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsAddNetworkPointClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddNetworkPoint typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsAddNetworkPointServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsAddNetworkPointRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddNetworkPointResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddRemoveById typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddRemoveById typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsAddRemoveByIdServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddRemoveTag typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsAddRemoveTagClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/AddRemoveTag typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsAddRemoveTagServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsAddRemoveTagRequestCodec,records.Ros1Clearpath_mission_manager_msgsAddRemoveTagResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CloneMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCloneMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCloneMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCloneMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CloneMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCloneMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCloneMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCloneMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CloneNetworkMap typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CloneNetworkMap typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCloneNetworkMapServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CloneNetworkMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CloneNetworkMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCloneNetworkMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ClonePointOfInterest typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ClonePointOfInterest typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsClonePointOfInterestServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreateMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreateMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreateMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreateMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateNetworkMap typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateNetworkMap typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreateNetworkMapServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateNetworkMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateNetworkMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreateNetworkMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreatePointOfInterest typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreatePointOfInterest typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreatePointOfInterestServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateTask typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreateTaskClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreateTaskRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateTaskResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateTask typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreateTaskServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreateTaskRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateTaskResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateWaypoint typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreateWaypointClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreateWaypointRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateWaypointResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateWaypoint typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreateWaypointServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreateWaypointRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateWaypointResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateWaypointFromPoi typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/CreateWaypointFromPoi typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequestCodec,records.Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/DeleteById typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsDeleteByIdClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsDeleteByIdRequestCodec,records.Ros1Clearpath_mission_manager_msgsDeleteByIdResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/DeleteById typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsDeleteByIdServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsDeleteByIdRequestCodec,records.Ros1Clearpath_mission_manager_msgsDeleteByIdResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/DeleteEverything typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsDeleteEverythingClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestCodec,records.Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/DeleteEverything typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsDeleteEverythingServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsDeleteEverythingRequestCodec,records.Ros1Clearpath_mission_manager_msgsDeleteEverythingResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/DeleteMultiple typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsDeleteMultipleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestCodec,records.Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/DeleteMultiple typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsDeleteMultipleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsDeleteMultipleRequestCodec,records.Ros1Clearpath_mission_manager_msgsDeleteMultipleResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ExportData typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsExportDataClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsExportDataRequestCodec,records.Ros1Clearpath_mission_manager_msgsExportDataResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ExportData typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsExportDataServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsExportDataRequestCodec,records.Ros1Clearpath_mission_manager_msgsExportDataResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllMissions typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetAllMissionsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllMissions typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetAllMissionsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetAllMissionsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllMissionsResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllNetworkMaps typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllNetworkMaps typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllNetworkMissions typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllNetworkMissions typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllPointsOfInterest typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllPointsOfInterest typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllTasks typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetAllTasksClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetAllTasksRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllTasksResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllTasks typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetAllTasksServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetAllTasksRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllTasksResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllWaypoints typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetAllWaypoints typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetAllWaypointsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetEverything typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetEverythingClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetEverythingRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetEverythingResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetEverything typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetEverythingServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetEverythingRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetEverythingResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetNetworkMap typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetNetworkMapClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetNetworkMap typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetNetworkMapServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetNetworkMapResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetNetworkMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetNetworkMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetNetworkMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetPointOfInterest typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetPointOfInterest typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetPointOfInterestServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetTask typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetTaskClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetTaskRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetTaskResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetTask typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetTaskServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetTaskRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetTaskResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetWaypoint typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsGetWaypointClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsGetWaypointRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetWaypointResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/GetWaypoint typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsGetWaypointServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsGetWaypointRequestCodec,records.Ros1Clearpath_mission_manager_msgsGetWaypointResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportData typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsImportDataClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsImportDataRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportDataResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportData typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsImportDataServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsImportDataRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportDataResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsImportMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsImportMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsImportMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsImportMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportNetworkMap typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsImportNetworkMapClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportNetworkMap typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsImportNetworkMapServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsImportNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportNetworkMapResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportNetworkMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ImportNetworkMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsImportNetworkMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/SplitNetworkEdge typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestCodec,records.Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/SplitNetworkEdge typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequestCodec,records.Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkEdge typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkEdge typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkEdges typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkEdges typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkMap typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkMap typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkMapServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkMission typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkMission typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkPoint typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkPoint typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkPoints typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateNetworkPoints typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdatePointOfInterest typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdatePointOfInterest typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateTask typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateTaskClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateTaskRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateTaskResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateTask typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateTaskServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateTaskRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateTaskResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateWaypoint typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsUpdateWaypointClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/UpdateWaypoint typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsUpdateWaypointServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsUpdateWaypointRequestCodec,records.Ros1Clearpath_mission_manager_msgsUpdateWaypointResponseCodec,false,capacity);}}

/** Complete clearpath_mission_manager_msgs/ValidateNetworkMap typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_manager_msgs/ValidateNetworkMap typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_manager_msgsValidateNetworkMapServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequestCodec,records.Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/CloneSchedule typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/CloneSchedule typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsCloneScheduleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/CreateSchedule typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/CreateSchedule typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsCreateScheduleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/EnableSchedule typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/EnableSchedule typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsEnableScheduleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/ExportData typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsExportDataClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsExportDataRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsExportDataResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/ExportData typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsExportDataServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsExportDataRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsExportDataResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/GetAllSchedules typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/GetAllSchedules typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/GetNextSchedule typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/GetNextSchedule typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsGetNextScheduleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/GetSchedule typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsGetScheduleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/GetSchedule typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsGetScheduleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsGetScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsGetScheduleResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/ImportData typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsImportDataClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsImportDataRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsImportDataResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/ImportData typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsImportDataServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsImportDataRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsImportDataResponseCodec,false,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/UpdateSchedule typed client; optional observer is independently granted. */
export class Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_mission_scheduler_msgs/UpdateSchedule typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_mission_scheduler_msgsUpdateScheduleServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequestCodec,records.Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponseCodec,false,capacity);}}

/** Complete clearpath_navigation_msgs/AutonomyTaskSrv typed client; optional observer is independently granted. */
export class Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestCodec,records.Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_navigation_msgs/AutonomyTaskSrv typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_navigation_msgsAutonomyTaskSrvServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequestCodec,records.Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponseCodec,false,capacity);}}

/** Complete clearpath_navigation_msgs/SetDelayCompensation typed client; optional observer is independently granted. */
export class Ros1Clearpath_navigation_msgsSetDelayCompensationClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_navigation_msgsSetDelayCompensationRequestCodec,records.Ros1Clearpath_navigation_msgsSetDelayCompensationResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_navigation_msgs/SetDelayCompensation typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_navigation_msgsSetDelayCompensationServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsSetDelayCompensationRequestCodec,records.Ros1Clearpath_navigation_msgsSetDelayCompensationResponseCodec,false,capacity);}}

/** Complete clearpath_navigation_msgs/SetPathShifter typed client; optional observer is independently granted. */
export class Ros1Clearpath_navigation_msgsSetPathShifterClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_navigation_msgsSetPathShifterRequestCodec,records.Ros1Clearpath_navigation_msgsSetPathShifterResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_navigation_msgs/SetPathShifter typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_navigation_msgsSetPathShifterServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsSetPathShifterRequestCodec,records.Ros1Clearpath_navigation_msgsSetPathShifterResponseCodec,false,capacity);}}

/** Complete clearpath_navigation_msgs/SetPathSmoother typed client; optional observer is independently granted. */
export class Ros1Clearpath_navigation_msgsSetPathSmootherClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_navigation_msgsSetPathSmootherRequestCodec,records.Ros1Clearpath_navigation_msgsSetPathSmootherResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_navigation_msgs/SetPathSmoother typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_navigation_msgsSetPathSmootherServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsSetPathSmootherRequestCodec,records.Ros1Clearpath_navigation_msgsSetPathSmootherResponseCodec,false,capacity);}}

/** Complete clearpath_navigation_msgs/SetStopDistance typed client; optional observer is independently granted. */
export class Ros1Clearpath_navigation_msgsSetStopDistanceClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_navigation_msgsSetStopDistanceRequestCodec,records.Ros1Clearpath_navigation_msgsSetStopDistanceResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_navigation_msgs/SetStopDistance typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_navigation_msgsSetStopDistanceServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_navigation_msgsSetStopDistanceRequestCodec,records.Ros1Clearpath_navigation_msgsSetStopDistanceResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddCommunicationWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddCommunicationWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddCommunicationWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddHeartbeatWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddHeartbeatWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddHeartbeatWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddInclinationWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddInclinationWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddInclinationWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddInclinationWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddInclinationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddInclinationWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddNodeStatusWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddNodeStatusWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddNodeStatusWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddNodeletStatusWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddNodeletStatusWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddNumPointsWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddNumPointsWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddNumPointsWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddOdomCovarianceWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddOdomCovarianceWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddRtkFixWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddRtkFixWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddRtkFixWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/AddTopicDataWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/AddTopicDataWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsAddTopicDataWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/RemoveWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsRemoveWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsRemoveWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsRemoveWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/RemoveWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsRemoveWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsRemoveWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsRemoveWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateCommunicationWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateCommunicationWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateHeartbeatWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateHeartbeatWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateInclinationWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateInclinationWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateInclinationWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateNodeStatusWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateNodeStatusWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateNodeletStatusWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateNodeletStatusWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateNumPointsWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateNumPointsWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateOdomCovarianceWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateOdomCovarianceWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateRtkFixWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateRtkFixWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponseCodec,false,capacity);}}

/** Complete clearpath_safety_msgs/UpdateTopicDataWatchdog typed client; optional observer is independently granted. */
export class Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient extends TypedServiceClient {constructor(session,alias,observerAlias=null,capacity=8){super(session,1,alias,records.Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseCodec,observerAlias,capacity);}}

/** Complete clearpath_safety_msgs/UpdateTopicDataWatchdog typed server; callback replacement is caller-managed. */
export class Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogServer extends TypedServer {constructor(session,advertise,respond,capacity=8){super(session,1,advertise,respond,records.Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequestCodec,records.Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponseCodec,false,capacity);}}
