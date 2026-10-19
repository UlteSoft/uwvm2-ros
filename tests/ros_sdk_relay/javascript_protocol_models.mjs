/**
 * @file javascript_protocol_models.mjs
 * @brief Small-memory independent stream/SDK models; no Wasm or ROS execution.
 *
 * The synthetic peer below emits mathematical protocol records over in-memory
 * streams. It supplies no WebAssembly.Memory/Instance or fake ROS import object.
 * These tests qualify framing/ownership/error handling only. Actual Runtime,
 * Gateway, native effects, credentials and process identity require separate
 * supervised execution evidence. All receipts belong to a new /tmp path.
 */
import assert from 'node:assert/strict';
import { PassThrough } from 'node:stream';
import { createHash } from 'node:crypto';
import { writeFile, readFile } from 'node:fs/promises';
import { specification } from '../../sdk/javascript/raw.mjs';
import { RelayProtocol as P, RelayChannel, RelayFrameDecoder, RelayError, RelayBusyError, encodeRelayFrame } from '../../sdk/javascript/relay.mjs';
import { AsyncSession, AsyncNode, AsyncExecutor, StringMessage, RosError } from '../../sdk/javascript/async.mjs';

let checks=0;
function check(condition,message) {assert.ok(condition,message);checks++;}
const MAX=0xffffffffffffffffn,HI=0xfedcba9876543210n;
function dv(bytes) {return new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);}
function canonical(version) {
  const spec=specification(version);
  return JSON.stringify({abi_version:1,functions:spec.functions.map(f=>({name:f.name,parameters:f.parameters.map(p=>p.type),result:'i32'})),module:spec.module});
}
function hello(version) {
  const body=new Uint8Array(80),data=dv(body);
  data.setUint32(0,1,true);data.setUint32(4,specification(version).functions.length,true);
  body.set(createHash('sha256').update(canonical(version)).digest(),8);
  data.setBigUint64(40,131072n,true);data.setBigUint64(48,16384n,true);
  data.setBigUint64(56,20464n,true);data.setBigUint64(64,20472n,true);
  data.setUint32(72,4080,true);data.setUint32(76,4080,true);
  return encodeRelayFrame({version,kind:P.hello,sequence:0n,index:65535,arity:0},body);
}
function result(frame,status,slot,snapshot,flags=1) {
  const body=new Uint8Array(20+snapshot.length),data=dv(body);
  data.setUint32(0,status,true);data.setBigUint64(4,slot,true);
  data.setUint32(12,snapshot.length,true);data.setUint32(16,flags,true);body.set(snapshot,20);
  return encodeRelayFrame({...frame,kind:P.result},body);
}
class SyntheticPeer {
  constructor(version,handler=null) {
    this.version=version;this.spec=specification(version);this.responses=new PassThrough();this.requests=new PassThrough();
    this.calls=[];this.handler=handler;this.fragment=0;
    this.decoder=new RelayFrameDecoder(version,8192,frame=>{
      if(frame.kind===P.shutdown) {this.emit(result(frame,0,0n,new Uint8Array()));return;}
      const entry=this.spec.functions[frame.index],data=dv(frame.body);
      assert.equal(frame.arity,entry.parameters.length);
      const args=Array.from({length:frame.arity},(_,i)=>data.getBigUint64(i*8,true));
      const seed=data.getBigUint64(frame.arity*8,true),size=data.getUint32(frame.arity*8+8,true);
      const request={...frame,name:entry.name,args,seed,size,input:frame.body.slice(frame.arity*8+12)};this.calls.push(request);
      const reply=this.handler?.(request)??{status:entry.name==='abi_version'?1:entry.name==='capability_open'?13:9,slot:seed};
      if(reply===false) return;
      const snapshot=new Uint8Array(size).fill(0xa5);if(reply.bytes) snapshot.set(reply.bytes);
      this.emit(result(frame,reply.status,reply.slot??seed,snapshot,reply.flags??1));
    });
    this.requests.on('data',chunk=>this.decoder.feed(chunk));
  }
  emit(frame) {
    if(this.fragment) for(let i=0;i<frame.length;i+=this.fragment) this.responses.write(frame.subarray(i,i+this.fragment));
    else this.responses.write(frame);
  }
  async attach(timeoutMs=250) {
    const attached=RelayChannel.attach({version:this.version,readable:this.responses,writable:this.requests,timeoutMs});
    this.emit(hello(this.version));this.channel=await attached;return this.channel;
  }
  async close() {if(this.channel?.state==='ready') await this.channel.shutdown();this.responses.end();this.requests.end();}
}
function argumentsFor(entry,layout,handle=HI,input=new Uint8Array()) {
  // The additive forms are selected by name before the legacy arity forms.
  // Their two handles, correlation, fixed readiness record and submit ID slot
  // have distinct meanings; a seven-argument RPC is not a substitute for them.
  if(entry.name==='event_watch') return [handle,handle-1n,37n];
  if(entry.name==='event_ready') return [handle,0,layout.output];
  if(entry.name==='service_submit') return [handle,handle-1n,0,layout.input,BigInt(input.length),layout.slot];
  const n=entry.parameters.length;
  if(n===0) return [];
  if(n===1) return [handle];
  if(n===4&&entry.name==='capability_open') return [0,layout.input,BigInt(input.length),layout.slot];
  if(n===4) return [handle,0,layout.input,BigInt(input.length)];
  if(n===5) return [handle,0,layout.output,64n,layout.slot];
  assert.equal(n,7,'fixture must explicitly implement every selected ABI form');
  return [handle,0,layout.input,BigInt(input.length),layout.output,64n,layout.slot];
}
function snapshotFor(entry) {
  if(entry.name==='event_watch'||entry.name==='service_submit') return 0;
  if(entry.name==='event_ready') return 32;
  return 64;
}

// Header/body splitting exercises every single split position and byte-sized
// fragments. The expected reconstructed frame is outside candidate parsing.
for(const version of [1,2]) {
  const original=hello(version);
  for(let split=0;split<=original.length;split++) {
    const frames=[],decoder=new RelayFrameDecoder(version,8192,f=>frames.push(f));
    decoder.feed(original.subarray(0,split));decoder.feed(original.subarray(split));decoder.finish();
    check(frames.length===1&&frames[0].body.length===80&&frames[0].sequence===0n,'all HELLO split positions');
  }
  const frames=[],decoder=new RelayFrameDecoder(version,8192,f=>frames.push(f));
  for(const byte of original) decoder.feed(Uint8Array.of(byte));decoder.finish();check(frames.length===1,'single-byte fragments');
  for(const [offset,value] of [[0,0],[4,2],[6,version===1?2:1],[7,0],[18,8],[19,1]]) {
    const bad=original.slice();bad[offset]=value;
    assert.throws(()=>new RelayFrameDecoder(version,8192,()=>{}).feed(bad),RelayError);checks++;
  }
  const excessive=original.slice(0,24);dv(excessive).setUint32(20,8193,true);
  assert.throws(()=>new RelayFrameDecoder(version,8192,()=>{}).feed(excessive),RelayError);checks++;
  const truncated=new RelayFrameDecoder(version,8192,()=>{});truncated.feed(original.subarray(0,17));
  assert.throws(()=>truncated.finish(),RelayError);checks++;

  const peer=new SyntheticPeer(version);peer.fragment=1;const channel=await peer.attach();
  for(const entry of specification(version).functions) {
    const input=entry.name==='capability_open'?new TextEncoder().encode('ungranted'):
      entry.name==='service_submit'?Uint8Array.of(1):new Uint8Array();
    const args=argumentsFor(entry,channel.layout,HI,input);
    const snapshot=snapshotFor(entry);
    const reply=await channel.call(entry.name,args,{input,snapshot});
    check(reply.status===(entry.name==='abi_version'?1:entry.name==='capability_open'?13:9),'complete selected ABI mathematical refusal');
    check(reply.slot===MAX&&reply.snapshot.length===snapshot&&reply.snapshot.every(b=>b===0xa5),'protected slot/output retained by synthetic refusal');
    const observed=peer.calls.at(-1);
    for(let i=0;i<args.length;i++) check(observed.args[i]===(typeof args[i]==='bigint'?args[i]:BigInt(args[i])),'exact typed argument bits');
  }
  const before=peer.calls.length;
  await assert.rejects(()=>channel.call('topic_publish',[Number(HI),0,channel.layout.input,0n]),TypeError);checks++;
  await assert.rejects(()=>channel.call('topic_publish',[HI,1,channel.layout.input,0n]),RelayError);checks++;
  await assert.rejects(()=>channel.call('event_poll',[HI,0,channel.layout.input,64n,channel.layout.slot]),RelayError);checks++;
  await assert.rejects(()=>channel.call('event_poll',[HI,0,channel.layout.output,4081n,channel.layout.slot]),RelayError);checks++;
  await assert.rejects(()=>channel.call('function_from_another_version',[]),RelayError);checks++;
  // Every additive form also refuses malformed local arguments before a frame
  // is written. These are transport/scratch checks, not synthetic ROS grants.
  const watch=[HI,HI-1n,37n],ready=[HI,0,channel.layout.output],
    submit=[HI,HI-1n,0,channel.layout.input,0n,channel.layout.slot];
  const invalidAdditiveCalls=[
    ['event_watch',watch.slice(0,-1),{snapshot:0},TypeError],
    ['event_ready',ready.slice(0,-1),{snapshot:32},TypeError],
    ['service_submit',submit.slice(0,-1),{snapshot:0},TypeError],
    ['event_watch',watch,{input:Uint8Array.of(1),snapshot:0},RelayError],
    ['event_watch',watch,{snapshot:1},RelayError],
    ['event_ready',[HI,1,channel.layout.output],{snapshot:32},RelayError],
    ['event_ready',[HI,0,channel.layout.input],{snapshot:32},RelayError],
    ['event_ready',ready,{snapshot:31},RelayError],
    ['event_ready',ready,{snapshot:33},RelayError],
    ['service_submit',[HI,HI-1n,1,channel.layout.input,0n,channel.layout.slot],{snapshot:0},RelayError],
    ['service_submit',[HI,HI-1n,0,channel.layout.output,0n,channel.layout.slot],{snapshot:0},RelayError],
    ['service_submit',[HI,HI-1n,0,channel.layout.input,1n,channel.layout.slot],{snapshot:0},RelayError],
    ['service_submit',[HI,HI-1n,0,channel.layout.input,0n,channel.layout.output],{snapshot:0},RelayError],
    ['service_submit',submit,{snapshot:1},RelayError]
  ];
  for(const [name,args,options,ErrorType] of invalidAdditiveCalls) {
    await assert.rejects(()=>channel.call(name,args,options),ErrorType);checks++;
  }
  check(peer.calls.length===before,'invalid local calls dispatch nothing');await peer.close();
}

// Delayed responses hold one borrow. No second frame appears during reentry.
{
  const peer=new SyntheticPeer(2,()=>false),channel=await peer.attach(35);
  const first=channel.call('abi_version',[]);await assert.rejects(()=>channel.call('abi_version',[]),RelayBusyError);checks++;
  await assert.rejects(first,RelayError);checks++;check(peer.calls.length===1&&channel.state==='failed','deadline never retries a request');
  peer.responses.end();peer.requests.end();
}
// A failure while an outgoing callback is stalled must settle the Promise,
// without an unhandled rejection or cancellation/remote-rollback claim.
{
  const read=new PassThrough(),write={on(){},write(){}};
  const attached=RelayChannel.attach({version:2,readable:read,writable:write,timeoutMs:25});read.write(hello(2));
  const channel=await attached;await assert.rejects(()=>channel.call('abi_version',[]),RelayError);checks++;
  check(channel.state==='failed','write-stall deadline bounded');read.end();
}

// Factory rollback, high-bit successful handles, exact decoded copy, callback
// after borrow and nested executor behavior are independent SDK-only models.
{
  const replies=StringMessage.encode('roswasm-js-relay-in');let next=HI,polled=false,closed=0;
  const peer=new SyntheticPeer(2,q=>{
    if(q.name==='capability_open') return {status:0,slot:next++};
    if(q.name==='capability_close') {closed++;return {status:0};}
    if(q.name==='event_poll') {if(polled) return {status:11};polled=true;return {status:0,slot:BigInt(replies.length),bytes:replies};}
    return {status:0};
  });
  const session=new AsyncSession(await peer.attach()),node=new AsyncNode(session);
  const pub=await node.createPublisher('pub');check(pub.capability.handle===HI,'successful high-bit opaque handle retained');
  let received=0;const executor=new AsyncExecutor();
  const sub=await node.createSubscription('sub',StringMessage,async (value,raw)=>{
    check(value==='roswasm-js-relay-in'&&raw.required===23n&&raw.bytes.every((b,i)=>b===replies[i]),'full exact callback bytes/length');
    const before=peer.calls.length;check((await executor.spinOnce())[0]===17&&peer.calls.length===before,'nested executor dispatches nothing');
    check(await pub.publish('callback publication')===0,'callback runs after transport borrow');received++;
  });
  executor.add(sub);check((await executor.spinOnce())[0]===0&&received===1,'one selected callback');
  const statuses=await node.close();check(statuses.every(x=>x.status===0)&&closed===2,'all explicit owned handles closed once');
  await assert.rejects(()=>pub.publish('stale'),RosError);checks++;check(closed===2,'local stale wrapper performs no close retry');
  await peer.close();
}
{
  let opens=0,closed=0;
  const peer=new SyntheticPeer(1,q=>{
    if(q.name==='capability_open') return ++opens===1?{status:0,slot:HI}:{status:13};
    if(q.name==='capability_close') {closed++;return {status:107};}
    return {status:0};
  });
  const node=new AsyncNode(new AsyncSession(await peer.attach()));
  let refused;try {await node.transaction(['first','denied'],()=>assert.fail('construction must not run'));}catch(error){refused=error;}
  check(refused instanceof RosError&&refused.code===13&&closed===1&&refused.cleanupFailures[0].status===107,'factory preserves initial denial plus one failed cleanup');
  check(node.owned.length===0,'failed factory commits no handle');await peer.close();
}
// Capacity refusal may either preserve its slot or expose a consumed required
// length; each model makes exactly one call, with no automatic resize/replay.
for(const slot of [MAX,5000n]) {
  const peer=new SyntheticPeer(2,()=>({status:90,slot})),session=new AsyncSession(await peer.attach());
  const before=peer.calls.length,result=await session.poll(HI,64);
  check(result.status===90&&result.required===(slot===MAX?null:5000n)&&result.bytes.length===0&&peer.calls.length===before+1,'error90 single-consumption sentinel contract');
  await peer.close();
}

const out=process.argv[2];
if(out) {
  assert.ok(out.startsWith('/tmp/')&&!out.includes('/../'),'new temporary receipt required');
  await writeFile(out,JSON.stringify({format_version:1,status:'PASS_PURE_STREAM_AND_SDK_MODELS_ONLY',checks,
    scope:'Synthetic byte streams and SDK ownership only; no real Runtime, Wasm, ROS, Gateway, process or authority result.',
    runtime_tools_executed:[],actual_native_or_Guest_execution:false},null,2)+'\n',{flag:'wx'});
}
console.log(JSON.stringify({status:'PASS_PURE_STREAM_AND_SDK_MODELS_ONLY',checks,receipt:out??null}));
