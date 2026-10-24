/** Authored, NOT EXECUTED. Run only after source review in the existing Linux
 * 64GiB cgroup. These are JavaScript lifetime/transport MODEL contracts using
 * explicit fixture delegates. They are not ROS/native/Wasm sandbox acceptance.
 */
import assert from 'node:assert/strict';
import { PassThrough, Writable } from 'node:stream';
import { specification } from '../../../../../../../sdk/origin/javascript/raw.mjs';
import { Session, Capability, Node, Context } from '../../../../../../../sdk/origin/javascript/roswasm.mjs';
import { AsyncSession, AsyncCapability, AsyncContext } from '../../../../../../../sdk/origin/javascript/async.mjs';
import { CopiedTransport } from '../../../../../../../sdk/origin/javascript/copied_transport.mjs';
import { RelayChannel, RelayBusyError, RelayProtocol, encodeRelayFrame } from '../../../../../../../sdk/origin/javascript/relay.mjs';

const high=0xfedcba9876543200n, spec=specification(2);
function copiedFixture() {
  const seen=[];let next=high,closeMode=()=>0,lookupMode=null;
  const layout={backend:2,abiVersion:1,functionCount:spec.functions.length,
    input:16n,slot:4112n,output:4120n,inputCapacity:4096,outputCapacity:4096};
  const invoke=(index,args,input,seed,snapshot)=>{
    const name=spec.functions[index].name;
    let status=0,slot=seed;
    if(name==='abi_version')status=1;
    else if(name==='capability_open')slot=++next;
    else if(name==='capability_close'||name==='capability_revoke'){
      seen.push(args[0]);status=closeMode(args[0]);
    }
    return {status,slot,snapshot:new Uint8Array(snapshot)};
  };
  const binding={describe:()=>layout};
  Object.defineProperty(binding,'invoke',{get(){if(lookupMode)lookupMode();return invoke;}});
  const transport=new CopiedTransport(2,binding),session=new Session(2,transport);
  return {transport,session,seen,setClose(fn){closeMode=fn;},setLookup(fn){lookupMode=fn;}};
}
function memoryFixture() {
  const memory=new WebAssembly.Memory({initial:1}),seen=[];let closeMode=()=>0;
  const functions=Object.fromEntries(spec.functions.map(entry=>[entry.name,(...args)=>{
    if(entry.name==='abi_version')return 1;
    if(entry.name==='capability_close'||entry.name==='capability_revoke'){seen.push(args[0]);return closeMode(args[0]);}
    return 0;
  }]));
  const session=new Session(2,{memory,functions},{offset:16n,inputCapacity:1024,outputCapacity:1024});
  return {session,seen,setClose(fn){closeMode=fn;}};
}
function badHandle(error){return error?.code===9;}

// 1. Full-width validation happens before the original createRawAdapter target.
{
  const f=memoryFixture(),cap=new Capability(f.session,high);
  cap.handle=1;
  assert.throws(()=>cap.close(),TypeError);assert.equal(cap.closed,false);assert.equal(f.seen.length,0);
  cap.handle=high;assert.equal(cap.close(),0);assert.deepEqual(f.seen,[high]);assert.equal(cap.closed,true);
}
// 2. Copied busy-before-invoke retains the owner for a later explicit close.
{
  const f=copiedFixture(),cap=new Capability(f.session,high+1n);
  f.transport.invoking=true;
  assert.throws(()=>cap.close(),TypeError);assert.equal(cap.closed,false);assert.equal(f.seen.length,0);
  f.transport.invoking=false;assert.equal(cap.close(),0);assert.deepEqual(f.seen,[high+1n]);
}
// 3. The actual delegate sees sticky retirement before synchronous reentry.
{
  const f=memoryFixture(),cap=new Capability(f.session,high+2n);
  f.setClose(()=>{assert.equal(cap.closed,true);assert.throws(()=>cap.close(),badHandle);return 107;});
  assert.equal(cap.close(),107);assert.throws(()=>{cap.closed=false;},TypeError);
  assert.equal(cap.closed,true);assert.throws(()=>cap.close(),badHandle);assert.equal(f.seen.length,1);
}
// 4. The same error CLASS thrown after binding.invoke is never a safe retry.
{
  const f=copiedFixture(),cap=new Capability(f.session,high+3n);
  f.setClose(()=>{throw new TypeError('Embedded transport is closed or reentrant');});
  assert.throws(()=>cap.close(),TypeError);assert.equal(cap.closed,true);
  assert.throws(()=>cap.close(),badHandle);assert.equal(f.seen.length,1);
}
// 5. Async copied uses the same deepest entry, not AsyncSession.handle entry.
{
  const f=copiedFixture(),session=AsyncSession.fromCopiedTransport(f.transport),cap=new AsyncCapability(session,high+4n);
  f.transport.invoking=true;await assert.rejects(cap.close(),TypeError);
  assert.equal(cap.closed,false);assert.equal(f.seen.length,0);
  f.transport.invoking=false;f.setClose(()=>{throw new TypeError('same class after attempt');});
  await assert.rejects(cap.close(),TypeError);assert.equal(cap.closed,true);
  await assert.rejects(cap.close(),badHandle);assert.equal(f.seen.length,1);
}

async function relayFixture() {
  const readable=new PassThrough();let channel,hold=null,throwWrite=false,writes=0;
  const writable=new Writable({write(frame,_encoding,done){
    ++writes;
    if(throwWrite)throw new RelayBusyError();
    const data=new DataView(frame.buffer,frame.byteOffset,frame.byteLength);
    const reply=()=>{const body=new Uint8Array(20),out=new DataView(body.buffer);
      out.setUint32(0,0,true);out.setBigUint64(4,0n,true);out.setUint32(12,0,true);out.setUint32(16,1,true);
      readable.write(encodeRelayFrame({version:2,kind:RelayProtocol.result,sequence:data.getBigUint64(8,true),index:data.getUint16(16,true),arity:frame[18]},body));done();};
    if(hold===true)hold=reply;else reply();
  }});
  const attached=RelayChannel.attach({version:2,readable,writable,inputCapacity:4080,outputCapacity:4080,timeoutMs:10000});
  const hello=new Uint8Array(80),data=new DataView(hello.buffer);
  data.setUint32(0,1,true);data.setUint32(4,spec.functions.length,true);
  hello.set(Buffer.from('f1f2ef80fee1a40b3de7daa584d497431ef0439f3bdd3553a2c384c69139ec68','hex'),8);
  data.setBigUint64(40,65536n,true);data.setBigUint64(48,16n,true);data.setBigUint64(56,4096n,true);data.setBigUint64(64,4104n,true);
  data.setUint32(72,4080,true);data.setUint32(76,4080,true);
  readable.write(encodeRelayFrame({version:2,kind:RelayProtocol.hello,sequence:0n,index:65535,arity:0},hello));
  channel=await attached;
  return {channel,writable,readable,get writes(){return writes;},hold(){hold=true;},release(){const resume=hold;hold=null;resume();},throwOnWrite(){throwWrite=true;},destroy(){clearTimeout(channel.timer);if(channel.pending)clearTimeout(channel.pending.timer);readable.destroy();writable.destroy();}};
}
// 6. A genuine outstanding relay frame rejects cleanup before another write.
{
  const f=await relayFixture();try{
    f.hold();const prior=f.channel.call('capability_close',[high+10n]);
    const session=new AsyncSession(f.channel),cap=new AsyncCapability(session,high+11n);
    await assert.rejects(cap.close(),RelayBusyError);assert.equal(cap.closed,false);assert.equal(f.writes,1);
    f.release();await prior;assert.equal(await cap.close(),0);assert.equal(cap.closed,true);assert.equal(f.writes,2);
  }finally{f.destroy();}
}
// 7. A RelayBusyError originating in the captured actual write is unknown.
{
  const f=await relayFixture();try{
    const cap=new AsyncCapability(new AsyncSession(f.channel),high+12n);f.throwOnWrite();
    await assert.rejects(cap.close(),RelayBusyError);assert.equal(cap.closed,true);
    await assert.rejects(cap.close(),badHandle);assert.equal(f.writes,1);
  }finally{f.destroy();}
}
// 8. A throwing write getter is still before the captured write attempt.
{
  const f=await relayFixture();try{
    const cap=new AsyncCapability(new AsyncSession(f.channel),high+13n),write=f.writable.write;
    Object.defineProperty(f.writable,'write',{configurable:true,get(){throw new RelayBusyError();}});
    await assert.rejects(cap.close(),RelayBusyError);assert.equal(cap.closed,false);assert.equal(f.writes,0);
    Object.defineProperty(f.writable,'write',{configurable:true,writable:true,value:write});
    assert.equal(await cap.close(),0);assert.equal(f.writes,1);
  }finally{f.destroy();}
}
// 9. Mixed batch keeps primary statuses, every full ID and the unentered owner.
{
  const f=copiedFixture(),node=new Node(f.session),caps=[1n,2n,3n].map(n=>new Capability(f.session,high+20n+n));
  node.owned.push(...caps);let first=true;
  f.setLookup(()=>{if(first){first=false;throw new TypeError('before invoke');}});
  f.setClose(id=>{if(id===caps[1].handle)return 107;if(id===caps[2].handle)throw new TypeError('after invoke');return 0;});
  let failure;try{node.close();}catch(error){failure=error;}
  assert(failure instanceof AggregateError);assert.equal(failure.results[1],107);
  assert.deepEqual(failure.attempts.map(row=>row.handle),caps.map(cap=>cap.handle));
  assert.deepEqual(failure.attempts.map(row=>row.attempted),[false,true,true]);
  assert.deepEqual(caps.map(cap=>cap.closed),[false,true,true]);
  assert.deepEqual(node.pendingCleanup().map(row=>row.handle),[caps[0].handle]);
  assert.deepEqual(node.close(),[0]);assert.deepEqual(f.seen,[caps[1].handle,caps[2].handle,caps[0].handle]);
}
// 10. A factory cannot erase its private acquired-owner ledger by mutating caps.
{
  const f=copiedFixture(),node=new Node(f.session),primary=Object.freeze(new Error('factory primary'));
  assert.throws(()=>node.transaction(['one','two'],caps=>{caps.length=0;f.transport.invoking=true;throw primary;}),error=>error===primary);
  assert.equal(node.pendingCleanup().length,2);assert.equal(f.seen.length,0);
  f.transport.invoking=false;assert.deepEqual(node.close(),[0,0]);assert.equal(f.seen.length,2);
}
// 11. Async Context retains unentered owners and declines transport shutdown.
{
  const f=copiedFixture(),session=AsyncSession.fromCopiedTransport(f.transport),context=new AsyncContext(session),node=context.createNode();
  await node.open('one');f.transport.invoking=true;
  const failed=await context.shutdown({transport:true});
  assert.equal(context.closed,false);assert.equal(node.closed,false);assert.equal(failed.transport.cleanupPending,true);assert.equal(f.transport.closed,false);
  f.transport.invoking=false;const completed=await context.shutdown({transport:true});
  assert.equal(context.closed,true);assert.equal(node.closed,true);assert.equal(completed.transport.status,0);assert.equal(f.transport.closed,true);assert.equal(f.seen.length,1);
}
// 12. Async factory's failed cleanup retains its primary and both real owners.
{
  const f=copiedFixture(),session=AsyncSession.fromCopiedTransport(f.transport),context=new AsyncContext(session),node=context.createNode(),primary=new Error('async primary');
  await assert.rejects(node.transaction(['one','two'],caps=>{caps.length=0;f.transport.invoking=true;throw primary;}),error=>error===primary);
  assert.equal(node.pendingCleanup().length,2);assert.equal(primary.cleanupAttempts.length,2);
  assert(primary.cleanupAttempts.every(row=>row.attempted===false&&row.retained===true));
  f.transport.invoking=false;const results=await node.close();assert(results.every(row=>row.status===0));assert.equal(f.seen.length,2);
}
// 13. A non-Error thrown value is still a reported unknown attempt, not success.
{
  const f=memoryFixture(),node=new Node(f.session),cap=new Capability(f.session,high+30n);node.owned.push(cap);
  f.setClose(()=>{throw null;});let failure;try{node.close();}catch(error){failure=error;}
  assert(failure instanceof AggregateError);assert.deepEqual(failure.errors,[null]);
  assert.equal(failure.attempts[0].threw,true);assert.equal(failure.attempts[0].attempted,true);
  assert.equal(cap.closed,true);assert.deepEqual(node.close(),[]);assert.equal(f.seen.length,1);
}
// 14. Async factory diagnostics preserve even null cleanup errors and primary.
{
  const f=copiedFixture(),session=AsyncSession.fromCopiedTransport(f.transport),context=new AsyncContext(session),node=context.createNode(),primary=new Error('primary before null cleanup');
  f.setClose(()=>{throw null;});
  await assert.rejects(node.transaction(['one'],()=>{throw primary;}),error=>error===primary);
  assert.equal(primary.cleanupFailures.length,1);assert.equal(primary.cleanupFailures[0].error,null);
  assert.equal(primary.cleanupAttempts[0].threw,true);assert.equal(primary.cleanupAttempts[0].attempted,true);
  assert.equal(node.pendingCleanup().length,0);assert.deepEqual(await node.close(),[]);assert.equal(f.seen.length,1);
}
// 15. Frozen primaries keep completed cleanup errors in the Node diagnostic.
{
  const f=copiedFixture(),node=new Node(f.session),primary=Object.freeze(new Error('frozen completed cleanup'));
  f.setClose(()=>107);
  assert.throws(()=>node.transaction(['one'],()=>{throw primary;}),error=>error===primary);
  const pending=node.pendingCleanup();assert.equal(pending.length,0);assert.equal(pending.factory.primary,primary);
  assert.equal(pending.factory.attempts[0].status,107);assert.equal(pending.factory.attempts[0].attempted,true);
  assert.equal(pending.factory.attempts[0].retained,false);assert.deepEqual(node.close(),[]);assert.equal(f.seen.length,1);
}
// 16. A primitive async primary is preserved beside an unknown null cleanup.
{
  const f=copiedFixture(),session=AsyncSession.fromCopiedTransport(f.transport),context=new AsyncContext(session),node=context.createNode();
  f.setClose(()=>{throw null;});let caught=false;
  try{await node.transaction(['one'],()=>{throw 0;});}catch(error){caught=true;assert.equal(error,0);}
  assert.equal(caught,true);const pending=node.pendingCleanup();assert.equal(pending.length,0);
  assert.equal(pending.factory.primary,0);assert.equal(pending.factory.attempts[0].error,null);
  assert.equal(pending.factory.attempts[0].threw,true);assert.equal(pending.factory.attempts[0].attempted,true);
  assert.deepEqual(await node.close(),[]);assert.equal(f.seen.length,1);
}
console.log('PASS_JAVASCRIPT_CLEANUP_LOCAL_MODELS');
