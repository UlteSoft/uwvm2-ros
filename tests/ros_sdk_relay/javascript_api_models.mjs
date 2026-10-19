/**
 * @file javascript_api_models.mjs
 * @brief Serial small-memory client-layer models, never Runtime acceptance.
 *
 * Run only under the task owner's serial test lane:
 * node --max-old-space-size=96 javascript_api_models.mjs /tmp/fresh-receipt.json
 * The mathematical stream peer models selected explicit replies. It creates
 * no Wasm Module/Instance, native endpoint, process, Gateway or permission.
 * Synchronous cases use one real linear Memory with mathematical imports only;
 * no Wasm code is instantiated/executed and no fake model is an SDK example.
 * Each named case is counted once; assertions within a case are not inflated
 * into execution/ROS evidence. Existing historical test receipts stay intact.
 */
import assert from 'node:assert/strict';
import { PassThrough } from 'node:stream';
import { createHash } from 'node:crypto';
import { writeFile } from 'node:fs/promises';
import { specification, Status, memoryRange } from '../../sdk/javascript/raw.mjs';
import { RelayChannel, RelayFrameDecoder, RelayProtocol as P, encodeRelayFrame } from '../../sdk/javascript/relay.mjs';
import * as A from '../../sdk/javascript/async.mjs';
import * as C from '../../sdk/javascript/codecs.mjs';
import * as S from '../../sdk/javascript/roswasm.mjs';
import { Executor } from '../../sdk/javascript/roswasm.mjs';
import { Future, CallbackGroup, Timer, AsyncTimer, DeferredQueue, SchedulingError } from '../../sdk/javascript/scheduling.mjs';

const cases=[],HI=0xfedcba9876543210n;
function test(name,run){cases.push({name,run});}
function data(bytes){return new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);}
function hello(version) {
  const spec=specification(version),body=new Uint8Array(80),d=data(body);
  const canonical=JSON.stringify({abi_version:1,functions:spec.functions.map(f=>({name:f.name,parameters:f.parameters.map(p=>p.type),result:'i32'})),module:spec.module});
  d.setUint32(0,1,true);d.setUint32(4,spec.functions.length,true);body.set(createHash('sha256').update(canonical).digest(),8);
  for(const [offset,value] of [[40,131072n],[48,16384n],[56,20464n],[64,20472n]])d.setBigUint64(offset,value,true);
  d.setUint32(72,4080,true);d.setUint32(76,4080,true);
  return encodeRelayFrame({version,kind:P.hello,sequence:0n,index:65535,arity:0},body);
}
/** Closed synthetic stream, with explicit test grant aliases and a callback
 * determining mathematical replies. Default alias opening refuses13; no model
 * substitutes itself for a production authority-aware embedding.
 */
async function setup(version,allowed=[],handler=()=>({status:0})) {
  const read=new PassThrough(),write=new PassThrough(),calls=[],handles=new Map();let next=HI;
  const decoder=new RelayFrameDecoder(version,8192,frame=>{
    const d=data(frame.body),entry=specification(version).functions[frame.index];
    let reply={status:0,slot:0n},snapshot=0;
    if(frame.kind!==P.shutdown) {
      const args=Array.from({length:frame.arity},(_,i)=>d.getBigUint64(i*8,true));
      snapshot=d.getUint32(frame.arity*8+8,true);
      const q={name:entry.name,args,input:frame.body.slice(frame.arity*8+12),alias:handles.get(args[0])};calls.push(q);
      reply={status:0,slot:d.getBigUint64(frame.arity*8,true)};
      if(q.name==='capability_open') {
        const alias=new TextDecoder('utf-8',{fatal:true}).decode(q.input);
        if(allowed.includes(alias)){handles.set(next,alias);reply.slot=next++;}else reply.status=13;
      } else if(q.name==='capability_close'||q.name==='capability_revoke') {
        reply.status=handles.delete(args[0])?0:9;
      } else reply={...reply,...handler(q)};
    }
    const body=new Uint8Array(20+snapshot),out=data(body);
    out.setUint32(0,reply.status,true);out.setBigUint64(4,reply.slot,true);
    out.setUint32(12,snapshot,true);out.setUint32(16,1,true);body.fill(0xa5,20);
    if(reply.bytes){assert.ok(reply.bytes.length<=snapshot);body.set(reply.bytes,20);out.setBigUint64(4,BigInt(reply.bytes.length),true);}
    read.write(encodeRelayFrame({...frame,kind:P.result},body));
  });
  write.on('data',bytes=>decoder.feed(bytes));
  const attached=RelayChannel.attach({version,readable:read,writable:write,timeoutMs:250});read.write(hello(version));
  const session=new A.AsyncSession(await attached),context=new A.AsyncContext(session),node=context.createNode();
  return {session,context,node,calls,handles,async close(){if(!node.closed)await node.close();if(session.channel.state==='ready')await session.shutdown();read.end();write.end();}};
}
/** Explicit mathematical synchronous source using actual bounded Memory.
 * Default opens refuse13. Only each case's literal allowed aliases can acquire
 * model IDs; the function table is not an authenticated Runtime embedding.
 */
function setupSync(version,allowed=[],handler=()=>({status:0,bytes:new Uint8Array()})) {
  const memory=new WebAssembly.Memory({initial:1,maximum:1}),calls=[],handles=new Map();let next=HI;
  const store=(slot,value)=>{const bytes=memoryRange(memory,slot,8n);data(bytes).setBigUint64(0,value,true);};
  const functions=Object.fromEntries(specification(version).functions.map(entry=>[entry.name,(...args)=>{
    const name=entry.name,input=name==='capability_open'?memoryRange(memory,args[1],args[2]).slice():entry.parameters.length===7?memoryRange(memory,args[2],args[3]).slice():new Uint8Array();
    const q={name,args,input,alias:handles.get(args[0])};calls.push(q);
    if(name==='abi_version')return 1;
    if(name==='capability_open') {const alias=new TextDecoder('utf-8',{fatal:true}).decode(input);if(!allowed.includes(alias))return 13;handles.set(next,alias);store(args[3],next++);return 0;}
    if(name==='capability_close'||name==='capability_revoke')return handles.delete(args[0])?0:9;
    if(!handles.has(args[0]))return 9;
    const reply=handler(q);
    if(reply.status)return reply.status;
    if(name==='event_poll'||entry.parameters.length===7) {const output=name==='event_poll'?args[2]:args[4],capacity=name==='event_poll'?args[3]:args[5],slot=name==='event_poll'?args[4]:args[6],bytes=reply.bytes??new Uint8Array();assert.ok(BigInt(bytes.length)<=capacity);memoryRange(memory,output,BigInt(bytes.length)).set(bytes);store(slot,BigInt(bytes.length));}
    return 0;
  }]));
  const session=new S.Session(version,{memory,functions},{offset:1n,inputCapacity:512,outputCapacity:512}),context=new S.Context(session),node=context.createNode();
  return {memory,session,context,node,calls,handles,close(){if(!context.closed)context.shutdown();else node.close();}};
}
function eventFor(kind) {return kind==='subscription'?C.StringMessage.encode('exact'):kind==='service'?new C.Writer().u64(HI).finish():C.IncomingGoal.encode({goalId:HI,order:3});}
function responseFor(kind,label='same callable') {return kind==='service'?{success:true,message:label}:kind==='action'?{kind:2,values:[0,1]}:undefined;}
function ownersOf(resource,kind) {return kind==='subscription'?[resource.capability]:[resource.advertise,resource.respond];}
function install(resource,kind,handler) {if(kind==='subscription')resource.callback=handler;else resource.handler=handler;}
function syncResource(f,kind,handler) {return kind==='subscription'?f.node.createSubscription('sub',C.StringMessage,handler):kind==='service'?f.node.createService('adv','reply',handler):f.node.createActionServer('adv','reply',handler);}
async function asyncResource(f,kind,handler) {return kind==='subscription'?f.node.createSubscription('sub',C.StringMessage,handler):kind==='service'?f.node.createService('adv','reply',handler):f.node.createActionServer('adv','reply',handler);}
function allowedFor(kind) {return kind==='subscription'?['sub']:['adv','reply'];}
function asyncStatuses(items) {return items.map(item=>item.status);}

test('typed service response copied once',async()=>{
  const wire=C.ServiceResponse.encode({success:true,message:'exact'}),f=await setup(2,['call'],q=>({status:0,bytes:q.name==='service_call'?wire:undefined}));
  try {const client=await f.node.createClient('call'),r=await client.call();assert.deepEqual(r.value,{success:true,message:'exact'});assert.equal(f.calls.filter(q=>q.name==='service_call').length,1);}finally{await f.close();}
});
test('service callback after borrow and exact request ID',async()=>{
  const request=new C.Writer().u64(HI).finish();let response;
  const f=await setup(1,['adv','reply','pub'],q=>{if(q.name==='event_poll')return {status:0,bytes:request};if(q.name==='service_respond')response=q.input;return {status:0,bytes:new Uint8Array()};});
  try {const pub=await f.node.createPublisher('pub'),server=await f.node.createService('adv','reply',async()=>{assert.equal(await pub.publish('inside handler'),0);return {success:true,message:'ok'};});assert.equal(await server.spinOnce(),0);assert.deepEqual(response,C.encodeServiceReply(HI,{success:true,message:'ok'}));}finally{await f.close();}
});
test('service activation failure rolls both handles back',async()=>{
  const f=await setup(2,['adv','reply'],q=>({status:q.name==='service_advertise'?107:0}));
  try {await assert.rejects(()=>f.node.createService('adv','reply',()=>({success:true,message:''})),e=>e.code===107);assert.equal(f.handles.size,0);assert.equal(f.node.owned.length,0);assert.equal(f.calls.filter(q=>q.name==='capability_close').length,2);}finally{await f.close();}
});
test('invalid codec/handler opens nothing',async()=>{
  const f=await setup(2,['a']);try {for(const call of [()=>f.node.createPublisher('a',{}),()=>f.node.createSubscription('a',C.StringMessage,1),()=>f.node.createService('a','a',null),()=>f.node.createClient('a',{})])await assert.rejects(call,TypeError);assert.equal(f.calls.length,0);}finally{await f.close();}
});
test('version-specific factories refuse before opening',async()=>{
  const f=await setup(1,['a']);try {await assert.rejects(()=>f.node.createLifecycle('a'),e=>e.code===95);await assert.rejects(()=>f.node.createParameters({events:'a'}),e=>e.code===95);assert.equal(f.calls.length,0);}finally{await f.close();}
});
test('Fibonacci send preserves full goal ID',async()=>{
  const f=await setup(2,['send'],q=>({status:0,bytes:q.name==='action_send_goal'?C.GoalId.encode(HI):undefined}));
  try {const client=await f.node.createActionClient({send:'send'});assert.equal((await client.sendGoal(4)).goalId,HI);const before=f.calls.length;await assert.rejects(()=>client.cancelAll(),e=>e.code===13);assert.equal(f.calls.length,before);}finally{await f.close();}
});
test('Action cancel-all is zero only on explicit cancel-any',async()=>{
  const f=await setup(1,['send','any'],q=>({status:0,bytes:q.name==='action_cancel_any_goal'?Uint8Array.of(1):undefined}));
  try {const client=await f.node.createActionClient({send:'send',cancelAny:'any'});await client.cancelAll();assert.deepEqual(f.calls.at(-1).input,new Uint8Array(8));assert.throws(()=>C.GoalId.encode(0n),RangeError);}finally{await f.close();}
});
test('Action feedback and terminal codecs remain distinct',async()=>{
  const feedback=C.ActionFeedback.encode({goalId:HI,values:[0,1]}),result=C.ActionResult.encode({goalId:HI,kind:2,values:[0,1]});
  const f=await setup(2,['send','feedback','result'],q=>({status:0,bytes:q.name==='action_receive_feedback'?feedback:q.name==='action_receive_result'?result:undefined}));
  try {const client=await f.node.createActionClient({send:'send',feedback:'feedback',result:'result'});assert.equal((await client.feedback(HI)).value.kind,0);assert.equal((await client.result(HI)).value.kind,2);assert.throws(()=>C.ActionResult.decode(C.ActionResponse.encode({goalId:HI,kind:1,values:[]})),RangeError);}finally{await f.close();}
});
test('Action server preserves received ID',async()=>{
  const f=await setup(2,['adv','reply'],q=>({status:0,bytes:q.name==='event_poll'?C.IncomingGoal.encode({goalId:HI,order:3}):new Uint8Array()}));
  try {const server=await f.node.createActionServer('adv','reply',goal=>({kind:2,values:[goal.order],goalId:1n}));assert.equal(await server.spinOnce(),0);assert.equal(C.ActionResponse.decode(f.calls.at(-1).input).goalId,HI);}finally{await f.close();}
});
test('exact typed int64 parameter roundtrip',async()=>{
  const value=-0x7fffffffffffffffn,f=await setup(2,['get','set'],q=>({status:0,bytes:q.name==='parameter_get'?C.Parameter.Int64.encode(value):new Uint8Array()}));
  try {const p=await f.node.createParameters({get:'get',set:'set'},C.Parameter.Int64);assert.equal((await p.get()).value,value);assert.equal((await p.set(value)).status,0);assert.deepEqual(f.calls.at(-1).input,C.Parameter.Int64.encode(value));}finally{await f.close();}
});
test('ROS1 exists/search/delete remain separately granted',async()=>{
  const f=await setup(1,['exists','search','delete'],q=>({status:0,bytes:q.name==='parameter_exists'?C.BoolMessage.encode(true):q.name==='parameter_search'?C.StringMessage.encode('/exact/name'):new Uint8Array()}));
  try {const p=await f.node.createParameters({exists:'exists',search:'search',delete:'delete'});assert.equal((await p.exists()).value,true);assert.equal((await p.search()).value,'/exact/name');assert.equal((await p.delete()).status,0);await assert.rejects(()=>p.get(),e=>e.code===13);}finally{await f.close();}
});
test('typed parameter event local callback after copy',async()=>{
  const event={seconds:1,nanoseconds:2,kind:2,tag:2,node:'/node',name:'counter',value:HI-0x8000000000000000n};
  const f=await setup(2,['events','pub'],q=>({status:0,bytes:q.name==='event_poll'?C.TypedParameterEvent.encode(event):undefined}));
  try {const p=await f.node.createParameters({events:'events'}),pub=await f.node.createPublisher('pub');let count=0;const remove=p.onEvent(async v=>{assert.deepEqual(v,event);assert.equal(await pub.publish('callback'),0);count++;});assert.equal(await p.subscribeEvents(),0);await p.pollEvent();remove();assert.equal(count,1);}finally{await f.close();}
});
test('graph exact rows and mutation guard',async()=>{
  const f=await setup(2,['graph'],q=>({status:0,bytes:C.StringMessage.encode('/a\tstd_msgs/msg/String\n/b\n')}));
  try {const graph=await f.node.createGraph('graph','graph_list_topics');assert.deepEqual((await graph.read()).rows,[{name:'/a',type:'std_msgs/msg/String'},{name:'/b',type:''}]);const before=f.calls.length;graph.operation='parameter_set';await assert.rejects(()=>graph.read(),e=>e.code===22);assert.equal(f.calls.length,before);}finally{await f.close();}
});
test('clock BigInt epoch and read-only selector',async()=>{
  const f=await setup(1,['clock'],()=>({status:0,bytes:C.TimeNanoseconds.encode(HI)}));
  try {const clock=await f.node.createClock('clock','time_read_ros');assert.equal((await clock.now()).nanoseconds,HI);clock.operation='parameter_set';await assert.rejects(()=>clock.now(),e=>e.code===22);}finally{await f.close();}
});
test('TF preserves explicit timestamp and transform',async()=>{
  const value={sequence:0,seconds:1,nanoseconds:2,parentFrame:'map',childFrame:'base',translation:[1,2,3],quaternion:[0,0,0,1]};
  const f=await setup(2,['lookup','broadcast'],q=>({status:0,bytes:q.name==='tf_lookup'?C.TransformMessage.encode(value):new Uint8Array()}));
  try {const tf=await f.node.createTf('lookup','broadcast');assert.deepEqual((await tf.lookup(HI)).value,value);assert.deepEqual(f.calls.at(-1).input,C.encodeLookupTime(HI));assert.equal((await tf.broadcast(value)).status,0);}finally{await f.close();}
});
test('lifecycle empty requests and typed responses',async()=>{
  const f=await setup(2,['get','change'],q=>({status:0,bytes:q.name==='lifecycle_get_state'?C.LifecycleState.encode({id:3,label:'active'}):C.BoolMessage.encode(true)}));
  try {const life=await f.node.createLifecycle('get','change');assert.deepEqual((await life.getState()).value,{id:3,label:'active'});assert.equal((await life.changeState()).accepted,true);assert.ok(f.calls.filter(q=>q.name.startsWith('lifecycle_')).every(q=>q.input.length===0));}finally{await f.close();}
});
test('logger is exact log_write String',async()=>{
  const f=await setup(2,['log']);try {const log=await f.node.createLogger('log');assert.equal(await log.write('hello'),0);assert.equal(f.calls.at(-1).name,'log_write');assert.equal(C.StringMessage.decode(f.calls.at(-1).input),'hello');}finally{await f.close();}
});
test('all ABI RPC wrappers preserve unsupported95',async()=>{
  const f=await setup(2,['r'],()=>({status:95}));try {const r=await f.node.createResource('r','parameter_get');assert.equal((await r.call()).status,95);}finally{await f.close();}
});
test('overlapping Node factory refuses and close conflicts',async()=>{
  const f=await setup(2,['a']);try {let release;const hold=new Promise(r=>release=r);const pending=f.node.transaction(['a'],async caps=>{await hold;return caps[0];});await assert.rejects(()=>f.node.createPublisher('a'),e=>e.code===17);await assert.rejects(()=>f.node.close(),e=>e.code===17);release();await pending;assert.equal(f.handles.size,1);}finally{await f.close();}
});
test('local Context shutdown closes all and prohibits new opens',async()=>{
  const f=await setup(1,['a']);try {const context=new A.AsyncContext(f.session),node=context.createNode();await node.createPublisher('a');const result=await context.shutdown();assert.equal(result.nodes[0].results[0].status,0);const before=f.calls.length;await assert.rejects(()=>node.open('a'),e=>e.code===9);assert.equal(f.calls.length,before);}finally{await f.close();}
});
test('Future requires explicit caller-driven pass',()=>{
  const executor=new Executor(),trace=[];const future=executor.defer(()=>{trace.push('call');return 7;});assert.equal(future.done,false);assert.throws(()=>future.result(),e=>e.code===11);assert.deepEqual(trace,[]);executor.spinOnce();assert.equal(future.result(),7);executor.close();
});
test('deferred service call remains one bounded original RPC',async()=>{
  const f=await setup(2,['call'],()=>({status:0,bytes:C.ServiceResponse.encode({success:true,message:'one'})}));
  try {const client=await f.node.createClient('call'),executor=new A.AsyncExecutor(),future=client.callDeferred(executor);assert.equal(f.calls.filter(q=>q.name==='service_call').length,0);await executor.spinOnce();assert.equal((await future.promise).value.message,'one');assert.equal(f.calls.filter(q=>q.name==='service_call').length,1);executor.close();}finally{await f.close();}
});
test('bounded deferred queue and local cancellation',()=>{
  const executor=new Executor({maxPending:1,maxTasksPerPass:1});let count=0;const future=executor.defer(()=>count++);assert.throws(()=>executor.defer(()=>0),e=>e.code===11);assert.equal(future.cancel(),true);executor.spinOnce();assert.equal(count,0);assert.equal(future.state,'cancelled');executor.close();
});
test('callbacks enqueue next-pass tasks and nested spin refuses',()=>{
  const executor=new Executor(),trace=[];let once=false;executor.add({spinOnce(){if(!once){once=true;executor.defer(()=>trace.push('later'));assert.deepEqual(executor.spinOnce(),[17]);}return 0;}});executor.spinOnce();assert.deepEqual(trace,[]);executor.spinOnce();assert.deepEqual(trace,['later']);executor.close();
});
test('callback failure rejects unstarted Future without executing it',()=>{
  const executor=new Executor();let effects=0;const error=new Error('local');const future=executor.defer(()=>effects++);executor.add({spinOnce(){throw error;}});assert.throws(()=>executor.spinOnce(),e=>e===error);assert.throws(()=>future.result(),e=>e===error);assert.equal(effects,0);assert.equal(executor.spinning,false);executor.close();
});
test('deferred exception completes Future once',async()=>{
  const executor=new A.AsyncExecutor(),error=new Error('local');const future=executor.defer(()=>{throw error;});await executor.spinOnce();await assert.rejects(future.promise,e=>e===error);assert.equal(future.done,true);assert.equal(future.cancel(),false);executor.close();
});
test('callback group nested exclusion and finally release',()=>{
  const group=new CallbackGroup();assert.throws(()=>group.run(()=>group.run(()=>{})),e=>e.code===17);assert.equal(group.busy,false);assert.equal(group.run(()=>9),9);
});
test('explicit timer readiness owns no clock',async()=>{
  let count=0,ready=false;const clock={label:'caller-owned'},timer=new Timer(c=>{assert.equal(c,clock);return ready;},()=>count++,{clock});assert.equal(timer.spinOnce(),11);ready=true;assert.equal(timer.spinOnce(),0);assert.equal(count,1);timer.close();assert.equal(timer.spinOnce(),9);const a=new AsyncTimer(async()=>true,async()=>count++);assert.equal(await a.spinOnce(),0);a.close();assert.equal(count,2);
});
test('close from entity cancels the entry task snapshot',()=>{
  const executor=new Executor(),trace=[];const future=executor.defer(()=>trace.push('forbidden'));
  executor.add({spinOnce(){executor.close();return 0;}});executor.add({spinOnce(){trace.push('forbidden poll');return 0;}});
  assert.deepEqual(executor.spinOnce(),[0]);assert.deepEqual(trace,[]);assert.equal(future.state,'cancelled');assert.deepEqual(executor.spinOnce(),[9]);assert.throws(()=>executor.add({spinOnce(){}}),e=>e.code===9);
});
test('async close during first task cancels later active tasks',async()=>{
  const executor=new A.AsyncExecutor({maxTasksPerPass:2,maxPending:2}),trace=[];
  const first=executor.defer(async()=>{executor.close();await Promise.resolve();return 'completed running task';});
  const later=executor.defer(()=>trace.push('forbidden'));await executor.spinOnce();assert.equal(first.result(),'completed running task');assert.equal(later.state,'cancelled');assert.deepEqual(trace,[]);assert.deepEqual(await executor.spinOnce(),[9]);
});
test('closing async timer during readiness prevents callback',async()=>{
  let release,count=0;const ready=new Promise(resolve=>release=resolve),timer=new AsyncTimer(()=>ready,()=>count++);
  const pending=timer.spinOnce();timer.close();release(true);assert.equal(await pending,9);assert.equal(count,0);
});
test('entry snapshot remains charged to pending queue bound',()=>{
  const executor=new Executor({maxPending:1,maxTasksPerPass:1}),future=executor.defer(()=>7);
  executor.add({spinOnce(){assert.throws(()=>executor.defer(()=>9),e=>e.code===11);return 0;}});
  executor.spinOnce();assert.equal(future.result(),7);executor.close();
});
test('timer readiness rejects direct reentry before callback',async()=>{
  let nested,count=0,timer;timer=new Timer(()=>{nested=timer.spinOnce();return true;},()=>count++);
  assert.equal(timer.spinOnce(),0);assert.equal(nested,17);assert.equal(count,1);
  let release;const ready=new Promise(resolve=>release=resolve),asyncTimer=new AsyncTimer(()=>ready,()=>count++);
  const pending=asyncTimer.spinOnce();assert.equal(await asyncTimer.spinOnce(),17);release(true);assert.equal(await pending,0);assert.equal(count,2);
});
test('Image profile rejects mismatch/bool2/trailing and ROS2 Header',()=>{
  const image={sequence:0,seconds:1,nanoseconds:2,frameId:'camera',height:1,width:1,encoding:'rgb8',bigendian:false,step:3,data:Uint8Array.of(1,2,3)},codec=C.imageMessage(2),wire=codec.encode(image);assert.deepEqual(codec.decode(wire),image);for(const v of [{...image,sequence:1},{...image,seconds:2147483648},{...image,step:2},{...image,encoding:'jpeg'},{...image,frameId:'/bad'}])assert.throws(()=>codec.encode(v));assert.throws(()=>codec.decode(Uint8Array.from([...wire,0])));const b=wire.slice();b[4*3+4+6+4*2+4+4]=2;assert.throws(()=>codec.decode(b));
});
test('Transform strict frames/norm/timestamp and no normalization',()=>{
  const value={sequence:0,seconds:1,nanoseconds:2,parentFrame:'map',childFrame:'base',translation:[0,0,0],quaternion:[0,0,0,1]},codec=C.transformMessage(2);assert.deepEqual(codec.decode(codec.encode(value)),value);for(const v of [{...value,quaternion:[0,0,0,2]},{...value,parentFrame:'1bad'},{...value,childFrame:'map'},{...value,nanoseconds:1000000000},{...value,sequence:1}])assert.throws(()=>codec.encode(v));
});
test('typed elementary refusal formats',()=>{
  assert.throws(()=>C.BoolMessage.decode([-1]));assert.throws(()=>C.BoolMessage.decode(Uint8Array.of(2)));assert.throws(()=>C.TriggerRequest.decode([]));assert.throws(()=>C.Parameter.Int64.encode(Number(HI)));assert.throws(()=>C.TwistMessage.encode([0,0,0,0,0,NaN]));assert.throws(()=>C.StringMessage.encode('\ud800'));assert.throws(()=>C.parameterArray('Bool').encode([0]));assert.throws(()=>C.StringList.encode(['x\0']));assert.throws(()=>C.IncomingGoal.encode({goalId:0n,order:1}));
});

test('sync Action mismatched IDs refuse without modifying prior copied output',()=>{
  let mismatch=false;const f=setupSync(2,['send','feedback','result'],q=>({status:0,bytes:q.name==='action_receive_feedback'?C.ActionFeedback.encode({goalId:mismatch?HI+1n:HI,values:[0,1]}):q.name==='action_receive_result'?C.ActionResult.encode({goalId:mismatch?HI+1n:HI,kind:2,values:[0,1]}):new Uint8Array()}));
  try {const client=f.node.createActionClient({send:'send',feedback:'feedback',result:'result'});for(const method of ['feedback','result']) {mismatch=false;const good=client[method](HI),saved=structuredClone(good);mismatch=true;const before=f.calls.length,bad=client[method](HI);assert.equal(bad.status,22);assert.equal(bad.runtimeStatus,0);assert.equal(bad.value,null);assert.equal(bad.required,BigInt(bad.bytes.length));assert.equal((method==='feedback'?C.ActionFeedback:C.ActionResult).decode(bad.bytes).goalId,HI+1n);assert.equal(f.calls.length,before+1);assert.deepEqual(good,saved);mismatch=false;assert.equal(client[method](HI).value.goalId,HI);}}finally {f.close();}
});
test('async Action mismatched IDs refuse without replay or prior result mutation',async()=>{
  let mismatch=false;const f=await setup(1,['send','feedback','result'],q=>({status:0,bytes:q.name==='action_receive_feedback'?C.ActionFeedback.encode({goalId:mismatch?HI+1n:HI,values:[0,1]}):q.name==='action_receive_result'?C.ActionResult.encode({goalId:mismatch?HI+1n:HI,kind:4,values:[]}):new Uint8Array()}));
  try {const client=await f.node.createActionClient({send:'send',feedback:'feedback',result:'result'});for(const method of ['feedback','result']) {mismatch=false;const good=await client[method](HI),saved=structuredClone(good);mismatch=true;const before=f.calls.length,bad=await client[method](HI);assert.equal(bad.status,22);assert.equal(bad.runtimeStatus,0);assert.equal(bad.value,null);assert.equal(bad.required,BigInt(bad.bytes.length));assert.equal(data(bad.slotBytes).getBigUint64(0,true),bad.required);assert.equal((method==='feedback'?C.ActionFeedback:C.ActionResult).decode(bad.bytes).goalId,HI+1n);assert.equal(f.calls.length,before+1);assert.deepEqual(good,saved);mismatch=false;assert.equal((await client[method](HI)).value.goalId,HI);}}finally {await f.close();}
});
/** One callback checks all retirement entry paths against the same shared
 * owner lease. The genuine synchronous API runs against a mathematical table;
 * import counts and real linear-memory snapshots must remain unchanged on17.
 */
function syncGuardCase(kind) {
  const f=setupSync(2,allowedFor(kind),q=>({status:0,bytes:q.name==='event_poll'?eventFor(kind):new Uint8Array()}));let resource,count=0,next=0;
  try {
    resource=syncResource(f,kind,function(){
      assert.equal(this,resource);++count;const calls=f.calls.length,memory=new Uint8Array(f.memory.buffer).slice();assert.equal(resource.spinOnce(),17);
      assert.deepEqual(resource.close(),kind==='subscription'?17:[17]);assert.deepEqual(resource.revoke(),kind==='subscription'?17:[17]);
      for(const owner of ownersOf(resource,kind)) {assert.throws(()=>owner.poll(),e=>e.code===17);assert.equal(owner.close(),17);assert.equal(owner.revoke(),17);assert.equal(owner.closed,false);}
      assert.deepEqual(f.node.close(),[17]);assert.throws(()=>f.context.shutdown(),e=>e.code===17);assert.equal(f.context.closed,false);
      assert.equal(f.calls.length,calls);assert.deepEqual(new Uint8Array(f.memory.buffer),memory);
      install(resource,kind,()=>{++next;return responseFor(kind,'next callable');});return responseFor(kind);
    });
    assert.equal(resource.spinOnce(),0);assert.equal(resource.spinning,false);assert.equal(count,1);assert.equal(next,0);
    assert.equal(resource.spinOnce(),0);assert.equal(count,1);assert.equal(next,1);
    for(const q of f.calls.filter(q=>q.name==='service_respond'||q.name==='action_respond'))assert.equal(data(q.input).getBigUint64(0,true),HI);
    assert.deepEqual(resource.close(),kind==='subscription'?0:[0,0]);assert.throws(()=>resource.spinOnce(),e=>e.code===9);
  } finally {f.close();}
}
test('sync Subscription direct and group retirement reentry is side-effect free',()=>syncGuardCase('subscription'));
test('sync Service direct and group retirement reentry is side-effect free',()=>syncGuardCase('service'));
test('sync ActionServer direct and group retirement reentry is side-effect free',()=>syncGuardCase('action'));
/** Hold the callback Promise after poll has already released transport scratch.
 * This distinguishes intrinsic ownership guards from a transient pipe-busy
 * refusal. Every pending owner/Node/Context remains retryably open after17.
 */
async function asyncGuardCase(kind) {
  const f=await setup(1,allowedFor(kind),q=>({status:0,bytes:q.name==='event_poll'?eventFor(kind):new Uint8Array()}));let resource,count=0,next=0,release,entered;
  const hold=new Promise(resolve=>release=resolve),ready=new Promise(resolve=>entered=resolve);
  try {
    resource=await asyncResource(f,kind,async function(){assert.equal(this,resource);++count;entered();await hold;return responseFor(kind);});
    const pending=resource.spinOnce();await ready;const before=f.calls.length;
    assert.equal(await resource.spinOnce(),17);assert.deepEqual(kind==='subscription'?await resource.close():asyncStatuses(await resource.close()),kind==='subscription'?17:[17]);
    assert.deepEqual(kind==='subscription'?await resource.revoke():asyncStatuses(await resource.revoke()),kind==='subscription'?17:[17]);
    for(const owner of ownersOf(resource,kind)) {await assert.rejects(()=>owner.poll(),e=>e.code===17);assert.equal(await owner.close(),17);assert.equal(await owner.revoke(),17);assert.equal(owner.closed,false);}
    await assert.rejects(()=>f.node.close(),e=>e.code===17);await assert.rejects(()=>f.context.shutdown(),e=>e.code===17);
    assert.equal(f.node.closed,false);assert.equal(f.context.closed,false);assert.equal(f.session.channel.state,'ready');assert.equal(f.calls.length,before);
    install(resource,kind,async()=>{++next;return responseFor(kind,'next callable');});release();assert.equal(await pending,0);
    assert.equal(count,1);assert.equal(next,0);assert.equal(resource.spinning,false);assert.equal(await resource.spinOnce(),0);assert.equal(next,1);
    for(const q of f.calls.filter(q=>q.name==='service_respond'||q.name==='action_respond'))assert.equal(data(q.input).getBigUint64(0,true),HI);
    assert.deepEqual(kind==='subscription'?await resource.close():asyncStatuses(await resource.close()),kind==='subscription'?0:[0,0]);await assert.rejects(()=>resource.spinOnce(),e=>e.code===9);
  } finally {release();await f.close();}
}
test('async Subscription awaited callback blocks every owner cleanup bypass',()=>asyncGuardCase('subscription'));
test('async Service awaited handler blocks every owner cleanup bypass',()=>asyncGuardCase('service'));
test('async ActionServer awaited handler blocks every owner cleanup bypass',()=>asyncGuardCase('action'));
function syncThrowCase(kind) {
  const f=setupSync(1,allowedFor(kind),q=>({status:0,bytes:q.name==='event_poll'?eventFor(kind):new Uint8Array()})),error=new Error('explicit local callback failure');
  try {const resource=syncResource(f,kind,()=>{throw error;});assert.throws(()=>resource.spinOnce(),e=>e===error);assert.equal(resource.spinning,false);assert.equal(f.calls.filter(q=>q.name==='service_respond'||q.name==='action_respond').length,0);let count=0;install(resource,kind,()=>{++count;return responseFor(kind);});assert.equal(resource.spinOnce(),0);assert.equal(count,1);assert.deepEqual(resource.revoke(),kind==='subscription'?0:[0,0]);assert.equal(f.handles.size,0);}finally {f.close();}
}
test('sync Subscription callback throw releases its intrinsic and shared leases',()=>syncThrowCase('subscription'));
test('sync Service callback throw releases its intrinsic and shared leases',()=>syncThrowCase('service'));
test('sync ActionServer callback throw releases its intrinsic and shared leases',()=>syncThrowCase('action'));
async function asyncRejectCase(kind) {
  const f=await setup(2,allowedFor(kind),q=>({status:0,bytes:q.name==='event_poll'?eventFor(kind):new Uint8Array()})),error=new Error('explicit local callback rejection');
  try {const resource=await asyncResource(f,kind,async()=>{await Promise.resolve();throw error;});await assert.rejects(()=>resource.spinOnce(),e=>e===error);assert.equal(resource.spinning,false);assert.equal(f.calls.filter(q=>q.name==='service_respond'||q.name==='action_respond').length,0);let count=0;install(resource,kind,async()=>{++count;return responseFor(kind);});assert.equal(await resource.spinOnce(),0);assert.equal(count,1);assert.deepEqual(kind==='subscription'?await resource.revoke():asyncStatuses(await resource.revoke()),kind==='subscription'?0:[0,0]);assert.equal(f.handles.size,0);}finally {await f.close();}
}
test('async Subscription rejection releases its intrinsic and shared leases',()=>asyncRejectCase('subscription'));
test('async Service rejection releases its intrinsic and shared leases',()=>asyncRejectCase('service'));
test('async ActionServer rejection releases its intrinsic and shared leases',()=>asyncRejectCase('action'));
test('all six polling facades reset leases after malformed decoded input',async()=>{
  for(const kind of ['subscription','service','action'])for(const asynchronous of [false,true]) {
    let bad=true;const make=asynchronous?setup:setupSync,f=await make(2,allowedFor(kind),q=>({status:0,bytes:q.name==='event_poll'?(bad?Uint8Array.of(0):eventFor(kind)):new Uint8Array()}));let count=0;
    try {const resource=await (asynchronous?asyncResource:syncResource)(f,kind,()=>{++count;return responseFor(kind);});if(asynchronous)await assert.rejects(()=>resource.spinOnce(),RangeError);else assert.throws(()=>resource.spinOnce(),RangeError);assert.equal(count,0);assert.equal(resource.spinning,false);bad=false;assert.equal(await resource.spinOnce(),0);assert.equal(count,1);await f.context.shutdown();assert.equal(f.handles.size,0);}finally {await f.close();}
  }
});
test('async polling retains the original callable before the first awaited poll',async()=>{
  for(const kind of ['subscription','service','action']) {
    const f=await setup(2,allowedFor(kind),q=>({status:0,bytes:q.name==='event_poll'?eventFor(kind):new Uint8Array()}));let release,original=0,next=0;const hold=new Promise(resolve=>release=resolve);
    try {const resource=await asyncResource(f,kind,async()=>{++original;return responseFor(kind,'original');}),poll=f.session.poll.bind(f.session);f.session.poll=async(...args)=>{await hold;return poll(...args);};const pending=resource.spinOnce();install(resource,kind,async()=>{++next;return responseFor(kind,'next');});release();assert.equal(await pending,0);assert.equal(original,1);assert.equal(next,0);f.session.poll=poll;assert.equal(await resource.spinOnce(),0);assert.equal(original,1);assert.equal(next,1);}finally {release();await f.close();}
  }
});
test('async group cleanup excludes a new poll until every attempted owner completes',async()=>{
  const f=await setup(2,['adv','reply'],q=>({status:0,bytes:q.name==='event_poll'?eventFor('service'):new Uint8Array()}));let release,entered;const hold=new Promise(resolve=>release=resolve),ready=new Promise(resolve=>entered=resolve);
  try {const resource=await f.node.createService('adv','reply',()=>({success:true,message:'never called'})),handle=f.session.handle.bind(f.session);let first=true;f.session.handle=async(...args)=>{if(first&&args[0]==='capability_close'){first=false;entered();await hold;}return handle(...args);};const pending=f.node.close();await ready;const before=f.calls.length;assert.equal(await resource.spinOnce(),17);assert.equal(await resource.respond.close(),17);assert.equal(resource.respond.closed,false);await assert.rejects(()=>resource.respond.request('service_respond',C.encodeServiceReply(HI,{success:true,message:''})),e=>e.code===17);await assert.rejects(()=>f.context.shutdown(),e=>e.code===17);assert.equal(f.context.closed,false);assert.equal(f.calls.length,before);release();assert.deepEqual(asyncStatuses(await pending),[0,0]);assert.equal(f.handles.size,0);assert.equal(resource.spinning,false);}finally {release();await f.close();}
});
test('busy Action handler may issue exact-ID feedback before its terminal response',async()=>{
  const f=await setup(1,['adv','reply'],q=>({status:0,bytes:q.name==='event_poll'?eventFor('action'):new Uint8Array()}));let resource;
  try {resource=await f.node.createActionServer('adv','reply',async goal=>{assert.equal((await resource.respond.request('action_respond',C.ActionResponse.encode({goalId:goal.goalId,kind:1,values:[0,1]}))).status,0);return {kind:2,values:[0,1]};});assert.equal(await resource.spinOnce(),0);const replies=f.calls.filter(q=>q.name==='action_respond').map(q=>C.ActionResponse.decode(q.input));assert.deepEqual(replies.map(v=>[v.goalId,v.kind]),[[HI,1],[HI,2]]);}finally {await f.close();}
});

const results=[];
for(const item of cases){await item.run();results.push({name:item.name,status:'PASS'});}
const receipt={format_version:1,status:'PASS_PURE_CLIENT_AND_CODEC_CASES_ONLY',cases:results.length,
  scope:'Serial mathematical streams and bounded real linear Memory/local scheduling/canonical codecs only. No Wasm Module/Instance/code execution, native ROS, Runtime authorization or delivery.',
  actual_native_or_Guest_execution:false,runtime_tools_executed:[],results};
if(process.argv[2]){assert.ok(process.argv[2].startsWith('/tmp/')&&!process.argv[2].includes('/../'));await writeFile(process.argv[2],JSON.stringify(receipt,null,2)+'\n',{flag:'wx'});}
console.log(JSON.stringify({status:receipt.status,cases:receipt.cases,receipt:process.argv[2]??null}));
