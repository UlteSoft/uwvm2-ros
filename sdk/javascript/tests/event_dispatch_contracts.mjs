/** Actual sync/async SDK event entities with finite in-memory fake imports.
 * No ROS/Gateway/native success is inferred. Source prepared without execution.
 * Run only inside the Root-owned serial model resource envelope.
 */
import assert from 'node:assert/strict';
import {Session,EventSubscription,Executor} from '../roswasm.mjs';
import {AsyncSession,AsyncEventSubscription,AsyncExecutor} from '../async.mjs';
import {CopiedTransport} from '../copied_transport.mjs';
import {specification,memoryRange,Status} from '../raw.mjs';
let checks=0;const equal=(a,b)=>{assert.deepEqual(a,b);checks++;};
function record(state){const bytes=new Uint8Array(64),v=new DataView(bytes.buffer);v.setUint32(0,0x45525755,true);v.setUint16(4,1,true);v.setUint16(6,1,true);bytes[8]=2;
  for(const [at,n] of [[16,state.source],[32,state.sequence],[40,17n],[48,state.session],[56,state.malformed?1n:0n]])v.setBigUint64(at,n,true);return bytes;}
function state(){return {source:2n,sequence:1n,session:19n,malformed:false,idle:false,polls:0,closes:0,next:1n};}
function fakeSync(){const s=state(),memory=new WebAssembly.Memory({initial:1}),functions=Object.fromEntries(specification(2).functions.map(f=>[f.name,()=>95]));
  functions.abi_version=()=>1;functions.capability_open=(m,a,n,slot)=>{new DataView(memory.buffer).setBigUint64(Number(slot),s.next++,true);return 0;};
  functions.capability_close=()=>{s.closes++;return 0;};functions.event_watch=(o,src,c)=>o===1n&&src===2n&&!c?0:13;
  functions.event_poll_v1=(o,m,out,cap,slot)=>{s.polls++;if(s.idle)return 11;new DataView(memory.buffer).setBigUint64(Number(slot),64n,true);if(cap<64n)return 90;memoryRange(memory,out,64n).set(record(s));return 0;};
  return {s,session:new Session(2,{memory,functions},{offset:1n,inputCapacity:128,outputCapacity:128})};}
const {s,session}=fakeSync(),observer=session.open('observer'),source=session.open('source'),executor=new Executor();let calls=0;
const entity=new EventSubscription(observer,source,function(event){calls++;equal(event.sequence,1n);const before=s.polls;
  equal(this.close(),17);equal(observer.close(),17);equal(executor.spinOnce(),[17]);equal(s.polls,before);
  this.callback=event=>{equal(event.sequence,2n);calls++;};executor.add({spinOnce(){return 11;}});
});executor.add(entity);equal(executor.spinOnce(),[0]);equal(calls,1);s.sequence=2n;equal(executor.spinOnce(),[0,11]);equal(calls,2);
for(const [index,mutate] of [()=>{},()=>{s.source=3n;},()=>{s.session=20n;},()=>{s.malformed=true;}].entries()){
  s.sequence=index===0?2n:3n;s.source=2n;s.session=19n;s.malformed=false;mutate();assert.throws(()=>entity.spinOnce());checks++;equal(calls,2);
}
s.source=2n;s.session=19n;s.malformed=false;s.sequence=3n;const marker=new Error('callback failure');entity.callback=()=>{throw marker;};assert.throws(()=>entity.spinOnce(),error=>error===marker);checks++;
equal(entity.spinning,false);equal(observer.closed,false);executor.close();equal(entity.close(),0);equal(s.closes,1);equal(source.closed,false);equal(source.close(),0);
// Promise-facing entity holds its lease across the awaited callback, including
// direct observer cleanup and a rejected recursive pass. No relay is fabricated.
const a=state(),layout={backend:2,abiVersion:1,functionCount:specification(2).functions.length,input:1n,inputCapacity:128,slot:129n,output:137n,outputCapacity:128};
const transport=new CopiedTransport(2,{describe(){return layout;},invoke(index,args,input,seed,snapshot){
  const name=specification(2).functions[index].name;let status=0,slot=seed,bytes=new Uint8Array(snapshot);
  if(name==='capability_open')slot=a.next++;else if(name==='event_watch'){if(args[0]!==1n||args[1]!==2n||args[2]!==0n)status=13;}
  else if(name==='event_poll_v1'){a.polls++;if(a.idle)status=11;else{slot=64n;if(snapshot<64)status=90;else bytes.set(record(a));}}
  else if(name==='capability_close')a.closes++;else status=95;return {status,slot,snapshot:bytes};
}});
const asyncSession=AsyncSession.fromCopiedTransport(transport),ao=await asyncSession.open('observer'),as=await asyncSession.open('source'),ax=new AsyncExecutor();let asyncCalls=0;
const ae=await AsyncEventSubscription.watch(ao,as,async function(event){asyncCalls++;equal(event.sequence,1n);await Promise.resolve();equal(await this.close(),17);equal(await ao.close(),17);equal(await ax.spinOnce(),[17]);});
ax.add(ae);equal(await ax.spinOnce(),[0]);equal(asyncCalls,1);a.sequence=2n;ae.callback=async()=>{await Promise.resolve();throw marker;};await assert.rejects(ae.spinOnce(),error=>error===marker);checks++;
equal(ae.spinning,false);equal(await ae.close(),0);equal(a.closes,1);equal(await as.close(),0);ax.close();
console.log(JSON.stringify({status:'PASS',scope:'source-bound finite sync/async event dispatch models; no ROS/Guest verdict',checks}));
