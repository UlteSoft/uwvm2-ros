/** Future Linux Node contract fixtures for a freshly generated demo RPC module.
 * This file executes no generator. Run under the existing bounded Linux owner:
 * node sdk/origin/custom-types/tests/rpc_cleanup_real_session_fixtures.mjs \
 *   /actual/frozen/source/sdk/origin/custom-types/generated-rpc/bounded_interfaces.mjs
 * Genuine Session.open creates every Capability. The copied binding below is
 * an instrumented local delegate double, never an actual Wasm/ROS permission
 * witness. No Capability constructor, closed flag mutation, source substring
 * assertion, implicit retry, network or subprocess is used.
 */
import assert from 'node:assert/strict';
import {realpathSync} from 'node:fs';
import {isAbsolute} from 'node:path';
import {pathToFileURL} from 'node:url';
import {Session,Capability,RosError,Status} from '../../../../../../../sdk/origin/javascript/roswasm.mjs';
import {CopiedTransport} from '../../../../../../../sdk/origin/javascript/copied_transport.mjs';
import {specification} from '../../../../../../../sdk/origin/javascript/raw.mjs';
import {EventBinding} from '../../../../../../../sdk/origin/javascript/events.mjs';

assert.equal(process.argv.length,3,'one explicit freshly generated module required');
assert.ok(isAbsolute(process.argv[2]),'generated module path must be absolute');
const modulePath=realpathSync(process.argv[2]);
const generated=await import(pathToFileURL(modulePath).href);
const {RpcConstructionFailure:ConstructionFailure,
  Ros2Demo_rpcSrvIncrementClient:ServiceClient,
  Ros2Demo_rpcSrvIncrementServer:ServiceServer,
  Ros2Demo_rpcActionCountClient:ActionClient,
  Ros2Demo_rpcActionCountServer:ActionServer}=generated;
for(const item of [ConstructionFailure,ServiceClient,ServiceServer,ActionClient,ActionServer])
  assert.equal(typeof item,'function','current generated demo exports required');
const roles=Object.freeze({send:'fixture-send',cancelOwn:'fixture-own',
  feedback:'fixture-feedback',result:'fixture-result',cancelAny:'fixture-any'});
const roleNames=Object.freeze(Object.values(roles));
const firstHandle=0xfedcba9876543201n,requestId=0xf123456789abcde1n;
const empty=new Uint8Array(),decoder=new TextDecoder('utf-8',{fatal:true});
const results=[];
async function test(name,body){
  const bindingDescriptors=Object.getOwnPropertyDescriptors(EventBinding.prototype);
  const assignDescriptor=Object.getOwnPropertyDescriptor(Object,'assign');
  try{await body();}
  finally{assert.deepEqual(Object.getOwnPropertyDescriptors(EventBinding.prototype),bindingDescriptors,
      'every test must leave the entire EventBinding prototype unchanged');
    assert.deepEqual(Object.getOwnPropertyDescriptor(Object,'assign'),assignDescriptor,
      'every test must restore the complete Object.assign descriptor');}
  results.push({name,status:'CONTRACT_MODEL_PASS'});
}
function captured(body){let threw=false,value;try{body();}catch(error){threw=true;value=error;}
  assert.equal(threw,true,'expected an actual throw, even if its value is undefined');return value;}
function refused(body,code){const error=captured(body);assert.ok(error instanceof RosError);assert.equal(error.code,code);return error;}
function exact(actual,expected,message){assert.ok(Object.is(actual,expected),message);}
function names(state){return state.calls.map(row=>row.name);}
function calls(state,name){return state.calls.filter(row=>row.name===name);}
function closes(state){return state.calls.filter(row=>row.name==='capability_close'||row.name==='capability_revoke');}
function handles(state){return state.accepted.slice();}
function reverseOwners(state){return handles(state).reverse();}
function closeHandles(state){return closes(state).map(row=>row.args[0]);}
function assertNoBusinessReplay(state,before){
  assert.deepEqual(state.calls.filter(row=>!row.name.startsWith('capability_close')&&!row.name.startsWith('capability_revoke')),
    before,'cleanup must not open, advertise, submit or replay a business operation');
}
function businessCalls(state){return state.calls.filter(row=>row.name!=='capability_close'&&row.name!=='capability_revoke');}
function word(value){const bytes=new Uint8Array(4);new DataView(bytes.buffer).setInt32(0,value,true);return bytes;}
function ticket(id,body=empty){const bytes=new Uint8Array(8+body.length);new DataView(bytes.buffer).setBigUint64(0,id,true);bytes.set(body,8);return bytes;}
function envelope(source,id,body){const bytes=new Uint8Array(64+body.length),v=new DataView(bytes.buffer);
  v.setUint32(0,0x45525755,true);v.setUint16(4,1,true);v.setUint16(6,3,true);bytes[8]=2;
  v.setBigUint64(16,source,true);v.setBigUint64(24,id,true);v.setBigUint64(32,1n,true);
  v.setBigUint64(40,0xe000000000000101n,true);v.setBigUint64(48,0xe000000000000102n,true);
  v.setBigUint64(56,BigInt(body.length),true);bytes.set(body,64);return bytes;}

/** This is the actual public CopiedTransport binding shape, not a duck Session.
 * Every function index/arity/scalar/range and invocation ticket passes through
 * the real transport/raw/Session implementation before this delegate records it.
 * A binding result always owns exactly the requested snapshot. Local model live
 * deletion is an after-effect test schedule, not proof of actual Host retirement.
 */
function fixture({openHandles=null,hook=null}={}){
  const spec=specification(2),layout=Object.freeze({backend:2,abiVersion:1,
    functionCount:spec.functions.length,input:8n,inputCapacity:4096,slot:4104n,
    output:4112n,outputCapacity:4096});
  const state={calls:[],accepted:[],aliases:new Map(),live:new Set(),queues:new Map(),
    openCount:0,captureFault:null,captureReads:0,hook,requestId,goalId:0xe123456789abcdefn};
  function answer(row,status=0,payload=empty,slot=row.seed){
    assert.ok(payload.length<=row.snapshot);const snapshot=new Uint8Array(row.snapshot);
    snapshot.set(payload);return {status,slot,snapshot};
  }
  function output(row,payload){return answer(row,0,payload,BigInt(payload.length));}
  const invoke=function(index,args,input,seed,snapshot){
    const entry=spec.functions[index];assert.ok(entry,'exact actual ABI index');
    const row={name:entry.name,args:args.slice(),input:input.slice(),seed,snapshot};
    if(row.name==='capability_open')row.alias=decoder.decode(input);
    state.calls.push(row);
    if(row.name==='capability_close'||row.name==='capability_revoke')state.live.delete(args[0]);
    const override=state.hook?.(row,state,{answer,output});
    if(override!==undefined)return override;
    if(row.name==='abi_version')return answer(row,1);
    if(row.name==='capability_open'){
      const index=state.openCount++,id=openHandles===null?firstHandle+BigInt(index):openHandles[index];
      assert.equal(typeof id,'bigint');state.accepted.push(id);state.live.add(id);state.aliases.set(row.alias,id);
      return answer(row,0,empty,id);
    }
    if(row.name==='capability_close'||row.name==='capability_revoke'||row.name==='service_advertise')return answer(row);
    if(row.name==='service_submit')return answer(row,0,empty,state.requestId);
    if(row.name==='service_call')return output(row,word(42));
    if(row.name==='action_send_goal')return output(row,ticket(state.goalId));
    if(row.name==='action_advertise'||row.name==='service_respond'||row.name==='action_respond')return output(row,empty);
    if(row.name==='event_poll'||row.name==='event_poll_v1'){
      const queue=state.queues.get(args[0]);return queue?.length?output(row,queue.shift()):answer(row,Status.AGAIN);
    }
    return answer(row,Status.UNSUPPORTED);
  };
  const binding={describe(){return layout;},get invoke(){
    state.captureReads+=1;
    if(state.captureFault!==null){const fault=state.captureFault;state.captureFault=null;throw fault.value;}
    return invoke;
  }};
  state.transport=new CopiedTransport(2,binding);
  state.session=new Session(2,state.transport);
  assert.deepEqual(names(state),['abi_version']);state.calls.length=0;
  state.enqueue=(alias,body)=>{const id=state.aliases.get(alias);assert.equal(typeof id,'bigint');
    let queue=state.queues.get(id);if(!queue){queue=[];state.queues.set(id,queue);}queue.push(body);};
  return state;
}
function construction(body){const value=captured(body);assert.ok(value instanceof ConstructionFailure);
  assert.equal(value.primaryKnown,true);assert.ok(value.cleanupAttempted);return value;}
function assertFrozenReport(report){assert.ok(Object.isFrozen(report));assert.ok(Object.isFrozen(report.statuses));
  for(const row of report.statuses)assert.ok(Object.isFrozen(row));}
function assertAllEntered(report,count){assert.equal(report.blocked,false);assert.equal(report.invoked,count);
  assert.equal(report.attempted,count);assert.equal(report.retained,0);assertFrozenReport(report);
  for(const row of report.statuses.filter(row=>row.acquired)){assert.equal(row.attempted,true);assert.equal(row.retained,false);}}

await test('genuine Session opens owners; closed is read-only and full-u64; preflight differs from entered TypeError',()=>{
  const state=fixture();const owner=state.session.open('direct-real-owner');
  assert.ok(owner instanceof Capability);assert.equal(owner.session,state.session);assert.equal(owner.handle,firstHandle);
  const descriptor=Object.getOwnPropertyDescriptor(owner,'closed');assert.equal(typeof descriptor.get,'function');
  assert.equal(descriptor.set,undefined);assert.equal(descriptor.configurable,false);assert.equal(owner.closed,false);
  // A real nested delegate holds CopiedTransport's private invocation set. No
  // diagnostic busy/closed property is written by the fixture.
  let preflight;state.hook=(row)=>{if(row.name==='service_call')preflight=captured(()=>owner.close());};
  const reply=owner.request('service_call',word(1));assert.equal(reply.status,0);
  assert.ok(preflight instanceof TypeError);assert.equal(owner.closed,false);assert.equal(closes(state).length,0);
  const after=new TypeError('same class; delegate was actually attempted');
  state.hook=row=>{if(row.name==='capability_close')throw after;};
  exact(captured(()=>owner.close()),after);assert.equal(owner.closed,true);assert.equal(closes(state).length,1);
  assert.equal(Reflect.set(owner,'closed',false),false);assert.equal(owner.closed,true);
  refused(()=>owner.close(),Status.BAD_HANDLE);refused(()=>owner.request('service_call',word(2)),Status.BAD_HANDLE);
  assert.deepEqual(closeHandles(state),[firstHandle]);assert.equal(calls(state,'service_call').length,1);
});

await test('distinct aliases and backend are checked before any open',()=>{
  const state=fixture();refused(()=>new ActionClient(state.session,{...roles,result:roles.send}),Status.INVALID);
  refused(()=>new ServiceClient(state.session,'call','call'),Status.INVALID);
  refused(()=>new ServiceClient({version:2},'call'),Status.INVALID);assert.deepEqual(state.calls,[]);
});

await test('default open denial has an explicit carrier but no invented owner or cleanup delegate',()=>{
  const state=fixture({hook(row,s,{answer}){if(row.name==='capability_open')return answer(row,Status.DENIED);}});
  const failure=construction(()=>new ServiceServer(state.session,'ad','reply'));
  assert.ok(failure.primary instanceof RosError);assert.equal(failure.primary.code,13);
  assert.deepEqual(names(state),['capability_open']);assert.equal(failure.cleanup.invoked,0);
  assert.equal(failure.cleanup.attempted,0);assert.equal(failure.cleanup.retained,0);
  const row=failure.diagnostics()[0];assert.equal(row.openInvoked,true);assert.equal(row.openThrew,true);
  assert.equal(row.openReturned,false);assert.equal(row.acquired,false);assert.equal(row.handle,null);
});

const thrownValues=[['null',null],['undefined',undefined],['false',false],['zero',0],['negative-zero',-0],
  ['empty-string',''],['zero-bigint',0n],['nan',NaN],['symbol',Symbol('cleanup')],['frozen-object',Object.freeze({kind:'cleanup'})]];
for(const [label,value] of thrownValues){
  await test(`reverse independent close retains thrown ${label} and never retries entered owners`,()=>{
    const state=fixture(),client=new ActionClient(state.session,roles),primary=Object.freeze({kind:'business'});
    const ids=handles(state);
    assert.deepEqual(calls(state,'capability_open').map(row=>row.alias),roleNames);
    state.hook=row=>{if(row.name==='capability_close'&&row.args[0]===ids[3])throw value;};
    const report=client.closeChecked(primary);assertAllEntered(report,5);exact(report.primary,primary);
    assert.equal(report.failed,1);assert.deepEqual(report.statuses.map(row=>row.alias),roleNames);
    assert.deepEqual(report.statuses.map(row=>row.handle),ids);assert.equal(report.cleanupError,Status.INVALID);assert.equal(report.cleanupThrew,true);
    exact(report.cleanupException,value);const row=report.statuses[3];assert.equal(row.threw,true);assert.equal(row.returned,false);
    assert.equal(row.status,null);exact(row.exception,value);assert.equal(row.firstFailureThrew,true);exact(row.firstFailureException,value);
    assert.deepEqual(closeHandles(state),ids.slice().reverse());assert.equal(state.live.size,0);
    const count=state.calls.length,later=client.closeChecked(primary);assert.equal(later.invoked,0);assert.equal(later.attempted,0);
    assert.equal(later.failed,1);assert.equal(later.cleanupThrew,true);exact(later.cleanupException,value);assert.equal(state.calls.length,count);
    assert.deepEqual(Reflect.ownKeys(primary),['kind']);
  });
  await test(`constructor rollback retains primary ${label} and frozen cleanup identity`,()=>{
    const cleanup=Object.freeze({kind:'cleanup',label}),state=fixture({hook(row,s){
      if(row.name==='capability_open'&&s.accepted.length===2)throw value;
      if(row.name==='capability_close'&&row.args[0]===firstHandle+1n)throw cleanup;
    }});
    const failure=construction(()=>new ActionClient(state.session,roles));exact(failure.primary,value);
    assert.deepEqual(calls(state,'capability_open').map(row=>row.alias),roleNames.slice(0,3));
    assert.equal(failure.cleanupReturned,true);assert.equal(failure.cleanupThrew,false,
      'a returned report containing a thrown delegate is distinct from cleanup() itself throwing');
    assertAllEntered(failure.cleanup,2);assert.equal(failure.cleanup.cleanupThrew,true);exact(failure.cleanup.cleanupException,cleanup);
    const facts=failure.diagnostics();assert.equal(facts.length,5);assert.equal(facts[2].openThrew,true);
    assert.equal(facts[2].openReturned,false);exact(facts[2].openException,value);assert.equal(facts[2].acquired,false);
    assert.deepEqual(closeHandles(state),[firstHandle+1n,firstHandle]);
    const before=businessCalls(state),closeCount=closes(state).length;failure.retryCleanup();
    exact(failure.primary,value);assert.equal(failure.cleanupCalls,2);assertNoBusinessReplay(state,before);
    assert.equal(closes(state).length,closeCount);assert.equal(failure.cleanup.failed,1);
  });
}

await test('revoke is one actual reverse pass even when delegate returns 107 or 110',()=>{
  const state=fixture(),client=new ActionClient(state.session,roles),ids=handles(state);
  state.hook=(row,s,{answer})=>row.name==='capability_revoke'?answer(row,row.args[0]===ids[4]?107:110):undefined;
  const report=client.revokeChecked(13);assertAllEntered(report,5);assert.equal(report.failed,5);assert.equal(report.cleanupError,110);
  assert.deepEqual(closeHandles(state),ids.slice().reverse());assert.ok(closes(state).every(row=>row.name==='capability_revoke'));
  client.closeChecked();client.revokeChecked();assert.equal(closes(state).length,5);
});

await test('busy before send retains all actual owners; later explicit cleanup sends no RPC again',()=>{
  const state=fixture(),client=new ServiceClient(state.session,'call','observer');let first;
  state.hook=row=>{if(row.name==='service_call')first=client.closeChecked('during-actual-invoke');};
  assert.deepEqual(client.call({value:1}),{value:42});assert.equal(first.invoked,2);assert.equal(first.attempted,0);
  assert.equal(first.retained,2);assert.equal(first.failed,2);assert.equal(closes(state).length,0);
  for(const row of first.statuses){assert.equal(row.threw,true);assert.ok(row.exception instanceof TypeError);assert.equal(row.retained,true);}
  state.hook=null;const before=businessCalls(state),later=client.closeChecked('explicit-later');assertAllEntered(later,2);
  assert.equal(later.failed,2,'historical local failures remain reported');assertNoBusinessReplay(state,before);
  assert.deepEqual(closeHandles(state),reverseOwners(state));client.closeChecked();assert.equal(closes(state).length,2);
});

await test('constructor cleanup pre-entry capture refusal remains retryable without reopen/advertise replay',()=>{
  const refusal=false,state=fixture({hook(row,s,{answer}){if(row.name==='service_advertise'){
    s.captureFault={value:refusal};return answer(row,Status.DENIED);}}});
  const failure=construction(()=>new ServiceServer(state.session,'advertise','respond'));
  assert.ok(failure.primary instanceof RosError);assert.equal(failure.primary.code,Status.DENIED);
  const first=failure.cleanup;assert.equal(first.retained,1);assert.equal(first.attempted,1);assert.equal(first.invoked,2);
  exact(first.cleanupException,refusal);assert.equal(first.cleanupThrew,true);assert.equal(first.statuses[1].retained,true);
  assert.equal(first.statuses[1].attempted,false);assert.deepEqual(closeHandles(state),[firstHandle]);
  const before=businessCalls(state),primary=failure.primary,later=failure.retryCleanup();assert.equal(later.retained,0);
  assert.equal(later.attempted,1);assert.equal(later.invoked,1);assert.equal(later.failed,1);exact(later.cleanupException,refusal);
  exact(failure.primary,primary);assertNoBusinessReplay(state,before);assert.deepEqual(closeHandles(state),[firstHandle,firstHandle+1n]);
  failure.retryCleanup();assert.equal(closes(state).length,2);
});

await test('raw close hook cannot reenter template cleanup, revoke, or capability-backed send',()=>{
  const state=fixture(),client=new ActionClient(state.session,roles);let nested=false;
  state.hook=row=>{if(row.name==='capability_close'&&!nested){nested=true;const count=state.calls.length;
    const blocked=client.closeChecked(),revoke=client.revokeChecked();
    for(const report of [blocked,revoke]){assert.equal(report.blocked,true);assert.equal(report.cleanupError,17);
      assert.equal(report.invoked,0);assert.equal(report.attempted,0);assert.equal(report.retained,4);
      assert.equal(report.statuses[4].inFlight,true);assert.equal(report.statuses[4].attempted,true);
      assert.equal(report.statuses[4].returned,false);assert.equal(report.statuses[4].status,null);}
    refused(()=>client.sendGoal({count:1}),17);assert.equal(state.calls.length,count);}};
  assertAllEntered(client.closeChecked(),5);assert.equal(nested,true);assert.deepEqual(closeHandles(state),reverseOwners(state));
  assert.equal(calls(state,'action_send_goal').length,0);
});

await test('direct genuine owner is already retired and cannot issue a request from its close delegate',()=>{
  const state=fixture(),owner=state.session.open('real-direct');let observed=false;
  state.hook=row=>{if(row.name==='capability_close'){observed=true;assert.equal(owner.closed,true);
    const count=state.calls.length;refused(()=>owner.close(),9);refused(()=>owner.request('service_call',word(1)),9);
    assert.equal(state.calls.length,count);assert.equal(Reflect.set(owner,'closed',false),false);}};
  assert.equal(owner.close(),0);assert.equal(observed,true);assert.deepEqual(closeHandles(state),[firstHandle]);
});

await test('duplicate full handle is diagnosed and suppressed, never fake-retired or ambiguously closed twice',()=>{
  const state=fixture({openHandles:[firstHandle,firstHandle]}),failure=construction(()=>new ServiceServer(state.session,'ad','reply'));
  assert.ok(failure.primary instanceof RosError);assert.equal(failure.primary.code,17);
  const report=failure.cleanup;assert.equal(report.attempted,1);assert.equal(report.retained,1);assert.equal(report.failed,1);
  const facts=failure.diagnostics();assert.equal(facts[0].handle,firstHandle);assert.equal(facts[1].handle,firstHandle);
  assert.equal(facts[1].duplicateOf,0);assert.equal(facts[1].attempted,false);assert.equal(facts[1].retained,true);
  assert.deepEqual(closeHandles(state),[firstHandle]);assert.equal(calls(state,'service_advertise').length,0);
  const before=businessCalls(state);failure.retryCleanup();assertNoBusinessReplay(state,before);
  assert.deepEqual(closeHandles(state),[firstHandle]);assert.equal(failure.cleanup.retained,1);
});

await test('service and action advertise constructor throws keep frozen primary and close both real owners',()=>{
  for(const [Constructor,operation] of [[ServiceServer,'service_advertise'],[ActionServer,'action_advertise']]){
    const primary=Object.freeze({kind:operation}),state=fixture({hook(row){if(row.name===operation)throw primary;}});
    const failure=construction(()=>new Constructor(state.session,'ad','reply'));exact(failure.primary,primary);
    assertAllEntered(failure.cleanup,2);assert.deepEqual(names(state),['capability_open','capability_open',operation,'capability_close','capability_close']);
    assert.deepEqual(closeHandles(state),[firstHandle+1n,firstHandle]);assert.deepEqual(Reflect.ownKeys(primary),['kind']);
  }
});

await test('real EventBinding constructor failure after two genuine opens uses the owned rollback carrier',()=>{
  // Explicit local constructor fault, not a malformed native handle. The
  // actual class is invoked by the template; finally restores its prototype.
  const sourceDescriptor=Object.getOwnPropertyDescriptor(EventBinding.prototype,'source');
  assert.equal(sourceDescriptor,undefined);
  const primary=Object.freeze({kind:'EventBinding local constructor fault'}),state=fixture();let failure;
  Object.defineProperty(EventBinding.prototype,'source',{configurable:true,set(){throw primary;}});
  try{failure=construction(()=>new ServiceClient(state.session,'call','observer'));}
  finally{if(sourceDescriptor===undefined)delete EventBinding.prototype.source;
    else Object.defineProperty(EventBinding.prototype,'source',sourceDescriptor);}
  exact(failure.primary,primary);assertAllEntered(failure.cleanup,2);assert.deepEqual(closeHandles(state),[firstHandle+1n,firstHandle]);
  assert.deepEqual(names(state),['capability_open','capability_open','capability_close','capability_close']);
  assert.equal(new EventBinding(2,firstHandle).source,firstHandle,'fault is fully restored');
});

await test('report assembly throw retains exact separate frozen cleanup and private already-entered ledger',()=>{
  // This tests a fallible JS report boundary, not terminal engine OOM recovery.
  const primary=Object.freeze({kind:'open primary'}),cleanup=Object.freeze({kind:'report assembly'});
  const state=fixture({hook(row,s){if(row.name==='capability_open'&&s.accepted.length===2)throw primary;}});
  const assignDescriptor=Object.getOwnPropertyDescriptor(Object,'assign');
  const assign=assignDescriptor.value;let injected=false,failure;
  Object.assign=function(target,...sources){if(!injected&&sources[0]?.role===0&&closes(state).length===2){injected=true;throw cleanup;}
    return Reflect.apply(assign,Object,[target,...sources]);};
  try{failure=construction(()=>new ActionClient(state.session,roles));}finally{Object.defineProperty(Object,'assign',assignDescriptor);}
  assert.equal(injected,true);exact(failure.primary,primary);assert.equal(failure.cleanupReturned,false);
  assert.equal(failure.cleanupThrew,true);exact(failure.cleanupException,cleanup);assert.equal(failure.cleanup,null);
  const facts=failure.diagnostics();for(const row of facts.slice(0,2)){assert.equal(row.attempted,true);assert.equal(row.retained,false);}
  assert.deepEqual(closeHandles(state),[firstHandle+1n,firstHandle]);const before=businessCalls(state);
  const report=failure.retryCleanup();assert.equal(report.invoked,0);assert.equal(report.retained,0);
  assert.equal(failure.cleanupReturned,true);assert.equal(failure.cleanupThrew,true);exact(failure.cleanupException,cleanup);
  exact(failure.primary,primary);assertNoBusinessReplay(state,before);assert.equal(closes(state).length,2);
});

await test('awaited server handler holds local lease; thrown false releases it without ticket replay',async()=>{
  const state=fixture(),server=new ServiceServer(state.session,'ad','reply'),id=0xffffffffffffffe1n;
  state.enqueue('ad',ticket(id,word(4)));let handlerCalls=0,caught=false,value;
  try{await server.spinOnce(async(request,actualId)=>{handlerCalls+=1;assert.deepEqual(request,{value:4});assert.equal(actualId,id);
    await Promise.resolve();const count=state.calls.length,report=server.closeChecked();assert.equal(report.blocked,true);
    assert.equal(report.cleanupError,17);assert.equal(report.invoked,0);refused(()=>server.pollOnce(),17);
    assert.equal(state.calls.length,count);throw false;});}catch(error){caught=true;value=error;}
  assert.equal(caught,true);exact(value,false);assert.equal(handlerCalls,1);assert.equal(closes(state).length,0);
  server.respond(id,{value:5});const response=calls(state,'service_respond');assert.equal(response.length,1);
  assert.equal(new DataView(response[0].input.buffer).getBigUint64(0,true),id);
  refused(()=>server.respond(id,{value:6}),9);assertAllEntered(server.closeChecked(),2);
  assert.equal(calls(state,'event_poll').length,1);assert.equal(calls(state,'service_respond').length,1);
});

await test('actual typed client submit and full response decode use genuine source and observer before checked cleanup',()=>{
  const state=fixture(),client=new ServiceClient(state.session,'call','observer'),id=client.submit({value:41});
  assert.equal(id,requestId);const submit=calls(state,'service_submit');assert.equal(submit.length,1);
  assert.equal(submit[0].args[0],firstHandle);assert.equal(submit[0].args[1],firstHandle+1n);
  assert.equal(new DataView(submit[0].input.buffer).getInt32(0,true),41);assert.equal(calls(state,'service_call').length,0);
  state.enqueue('observer',envelope(firstHandle,id,word(42)));const reply=client.pollCompletion();
  assert.equal(reply.requestId,id);assert.deepEqual(reply.value,{value:42});assertAllEntered(client.closeChecked(),2);
  assert.deepEqual(closeHandles(state),[firstHandle+1n,firstHandle]);
});

process.stdout.write(JSON.stringify({status:'RPC_REAL_SESSION_CONTRACT_MODEL_PASS',cases:results.length,results,
  generatedModule:modulePath,generatedHere:false,NativeROS:'NOT_RUN',Wasm:'NOT_RUN',DBG:'NOT_RUN',permissions:'NOT_QUALIFIED'})+'\n');
