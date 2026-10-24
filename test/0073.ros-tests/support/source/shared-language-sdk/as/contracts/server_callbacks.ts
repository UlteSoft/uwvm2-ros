/** Real high-level callback/ownership logic over a local fake Api.
 * Prepared for serial Wasm execution; no native Api, ROS resource or clock is
 * imported. Closed/reentrant/refused cases must not dispatch a native operation.
 */
import { Api,Operation,Status } from '../../../../../../../sdk/origin/assemblyscript/api';
import { Node,Session,Service,ActionServer,Subscription } from '../../../../../../../sdk/origin/assemblyscript/roswasm';
import { ServiceRequest,ServiceResponse,IncomingGoal,ActionSequence,Writer,StringCodec,decodeServiceRequest } from '../../../../../../../sdk/origin/assemblyscript/codecs';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';
let checks:u32=0;
function require(value:bool):void { if(!value) unreachable(); checks++; }
class FixtureApi extends Api {
  next:u64=1; polls:u32=0; replies:u32=0; closes:u32=0; revokes:u32=0;
  action:bool=false; topic:bool=false; pollStatus:u32=0;
  constructor(){ super(2); }
  version():u32 { return 1; }
  supports(_operation:Operation):bool { return true; }
  open(_memory:u32,_offset:u64,_length:u64,slot:u64):u32 { store<u64>(<usize>slot,this.next++); return 0; }
  close(_handle:u64):u32 { this.closes++; return 0; }
  revoke(_handle:u64):u32 { this.revokes++; return 0; }
  subscribe(_handle:u64):u32 { return 0; }
  advertiseService(_handle:u64):u32 { return 0; }
  poll(_handle:u64,_memory:u32,output:u64,capacity:u64,slot:u64):u32 {
    this.polls++; if(this.pollStatus) return this.pollStatus;
    const body=this.topic?new StringCodec().encode('copied topic'):this.action?new Writer().u64(7).i32(5).finish():new Writer().u64(7).finish();
    if(capacity< <u64>body.length) return Status.TooLarge;
    memory.copy(<usize>output,body.dataStart,<usize>body.length); store<u64>(<usize>slot,<u64>body.length); return 0;
  }
  request(operation:Operation,_handle:u64,_memory:u32,input:u64,_length:u64,_output:u64,_capacity:u64,slot:u64):u32 {
    if(operation==Operation.service_respond || operation==Operation.action_respond){ this.replies++; if(load<u64>(<usize>input)!=7) unreachable(); }
    store<u64>(<usize>slot,0); return 0;
  }
}
let service:Service|null=null, action:ActionServer|null=null;
let ownerNode:Node|null=null, ownerSession:Session|null=null, ownerApi:FixtureApi|null=null;
let subscription:Subscription<string>|null=null, topicCallbacks:u32=0;
/** Verify the actual grouped/owner cleanup paths, not a separate model guard. */
function cleanupRefused(advertise:u64,respond:u64):void {
  const node=ownerNode, session=ownerSession, api=ownerApi;
  if(node) { const statuses=node.close(); require(statuses.length==1 && statuses[0]==Status.Conflict); }
  if(session) { const statuses=session.close(); require(statuses.length==1 && statuses[0]==Status.Conflict && session.isOpen()); }
  require(advertise!=0 && respond!=0);
  if(api) require(api.closes==0 && api.revokes==0);
}
function serve(request:ServiceRequest):ServiceResponse {
  const server=service;
  if(server){ require(server.spinOnce()==Status.Conflict); require(server.close()[0]==Status.Conflict);
    const api=ownerApi; if(api) { const polls=api.polls; require(server.advertise.poll(256).status==Status.Conflict && api.polls==polls); }
    require(server.advertise.close()==Status.Conflict); require(server.respond.revoke()==Status.Conflict);
    cleanupRefused(server.advertise.rawHandle().handle,server.respond.rawHandle().handle);
  }
  require(request.id==7 && request.request.length==0);
  return new ServiceResponse(true,'owned reply');
}
function act(goal:IncomingGoal):ActionSequence {
  const server=action;
  if(server){ require(server.spinOnce()==Status.Conflict); require(server.revoke()[0]==Status.Conflict);
    const api=ownerApi; if(api) { const polls=api.polls; require(server.advertise.poll(256).status==Status.Conflict && api.polls==polls); }
    require(server.advertise.close()==Status.Conflict); require(server.respond.revoke()==Status.Conflict);
    cleanupRefused(server.advertise.rawHandle().handle,server.respond.rawHandle().handle);
  }
  require(goal.id==7 && goal.order==5);
  return new ActionSequence(goal.id,2,[0,1,1,2,3,5]);
}
function wrong(goal:IncomingGoal):ActionSequence { return new ActionSequence(goal.id+1,2,[0,1]); }
/** One copied String callback must not recursively dequeue or retire its owner. */
function consume(text:string):void {
  topicCallbacks++; require(text=='copied topic');
  const sub=subscription, node=ownerNode, session=ownerSession, api=ownerApi;
  if(sub && api){ const polls=api.polls; require(sub.spinOnce()==Status.Conflict && api.polls==polls);
    require(sub.capability.poll(64).status==Status.Conflict && api.polls==polls);
    require(sub.capability.close()==Status.Conflict); require(sub.capability.revoke()==Status.Conflict);
    require(sub.rawHandle().status==Status.Success && sub.capability.isOpen());
  }
  if(node){ const statuses=node.close(); require(statuses.length==1 && statuses[0]==Status.Conflict); }
  if(session){ const statuses=session.close(); require(statuses.length==1 && statuses[0]==Status.Conflict && session.isOpen()); }
  if(api) require(api.closes==0 && api.revokes==0);
}
/** Run service/Action reentry, identity and closed-state contracts once. */
export function _start():void {
  const api=new FixtureApi(), session=new Session(api,8), node=session.node();
  ownerApi=api; ownerSession=session; ownerNode=node;
  service=node.createService('events','respond',serve,256);
  const selected=service; if(selected){ require(selected.status==0); require(selected.spinOnce()==0); require(api.polls==1 && api.replies==1 && api.closes==0); }
  api.action=true; action=node.createActionServer('goals','results',act,256);
  const chosen=action; if(chosen){ require(chosen.status==0); require(chosen.spinOnce()==0); require(api.polls==2 && api.replies==2 && api.closes==0); }
  const bad=node.createActionServer('bad-goals','bad-results',wrong,256);
  require(bad.spinOnce()==Status.Invalid); require(api.replies==2);
  const outcomes=node.close(); require(outcomes.length==6 && api.closes==6);
  if(selected) require(selected.spinOnce()==Status.BadHandle);
  if(chosen) require(chosen.spinOnce()==Status.BadHandle);
  require(node.open('events').openStatus==Status.BadHandle); require(api.polls==3 && api.replies==2);
  require(!decodeServiceRequest(new Writer().u64(7).u8(2).finish()).valid);
  require(!decodeServiceRequest(new Writer().u64(7).u8(0).u8(0).finish()).valid);
  require(session.close().length==0 && !session.isOpen());
  require(session.close()[0]==Status.BadHandle && api.closes==6 && api.revokes==0);

  // A separate group preserves all original service/Action checks above.
  const topicApi=new FixtureApi(), topicSession=new Session(topicApi,2), topicNode=topicSession.node(); topicApi.topic=true;
  ownerApi=topicApi; ownerSession=topicSession; ownerNode=topicNode;
  subscription=topicNode.createSubscription<string>('topic',new StringCodec(),consume,64);
  const sub=subscription;
  if(sub){
    require(sub.status==Status.Success); require(sub.spinOnce()==Status.Success);
    require(topicApi.polls==1 && topicCallbacks==1 && topicApi.closes==0 && topicApi.revokes==0);
    require(sub.rawHandle().status==Status.Success && sub.rawHandle().handle!=0);
    topicApi.pollStatus=Status.Again; require(sub.spinOnce()==Status.Again);
    require(topicApi.polls==2 && topicCallbacks==1 && topicApi.closes==0);
    topicApi.pollStatus=Status.Invalid; require(sub.spinOnce()==Status.Invalid);
    require(topicApi.polls==3 && topicCallbacks==1 && topicApi.closes==0);
    topicApi.pollStatus=Status.Again;
    require(sub.capability.poll(64).status==Status.Again && topicApi.polls==4 && topicCallbacks==1);
    // Normal idle/error returns release the lease, allowing later checked close.
    const closed=topicNode.close(); require(closed.length==1 && closed[0]==0 && topicApi.closes==1 && !sub.capability.isOpen());
    require(topicSession.close().length==0 && !topicSession.isOpen());
    require(sub.close()==Status.BadHandle && topicApi.closes==1);
    require(sub.spinOnce()==Status.BadHandle && topicApi.polls==4);
    require(topicNode.close()[0]==Status.BadHandle && topicApi.closes==1 && topicApi.revokes==0);
  }
}
/** Compare only after _start completes; fake success is not native authorization. */
export function completedChecks():u32 { return checks; }
