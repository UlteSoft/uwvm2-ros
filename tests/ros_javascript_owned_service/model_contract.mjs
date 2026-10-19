/**
 * Authored LOCAL mathematical contracts, NOT EXECUTED.
 *
 * Run only from an explicit admitted /tmp source/test tree after the complete
 * records-only generator has produced its actual bounded_records.mjs. This
 * model builds byte values and uses a real SDK Session over a JavaScript test
 * adapter. It is not the C builtin, Wasm, Runtime, permission or Native/DDS proof.
 * Every canonical codec/root fact is from the real generated module; no Native
 * type identity or source/canonical hash is invented by these models.
 */
import assert from 'node:assert/strict';
import * as records from '../../sdk/custom-types/generated-control-managed/bounded_records.mjs';
import { ABI } from '../../sdk/javascript/abi.mjs';
import { Session, Status } from '../../sdk/javascript/roswasm.mjs';
import { OwnedServiceBridge, prepareServiceCall, invokeServiceCall, serviceCallAttempted } from '../../sdk/javascript/owned_service/bridge.mjs';
import { OwnedClient, OwnedServer } from '../../sdk/javascript/owned_service/owned.mjs';
import { bindControlServices } from '../../sdk/javascript/owned_service/control.mjs';
import { copyExpectedQos, decodeEndpointReceipt } from '../../sdk/javascript/owned_service/wire.mjs';
import { openDescriptionClient } from '../../sdk/examples/javascript_owned_service/control_client.mjs';
import { openParameterTypesServer } from '../../sdk/examples/javascript_owned_service/control_server.mjs';
import { SetupFailure, applicationCleanupFacts, closeApplication } from '../../sdk/examples/javascript_owned_service/setup.mjs';

const MAX = 0xffffffffffffffffn, definitions = bindControlServices(records);
const requestCodec = records.Ros2Rcl_interfacesSrvGetParameterTypes_RequestCodec;
const responseCodec = records.Ros2Rcl_interfacesSrvGetParameterTypes_ResponseCodec;
function u64(view, at, value) { view.setBigUint64(at, value, true); }
function qos() {
  const b = new Uint8Array(80), d = new DataView(b.buffer);
  for (let at = 0; at < 16; at += 4) d.setUint32(at, 1, true);
  u64(d, 16, 1000n); return b;
}
function model(kind = 3, ownership = 'owned', query = true, historyEntries = 8) {
  const memory = new WebAssembly.Memory({ initial: 2 }), functions = {}, caps = [], closes = [], calls = [];
  let next = 1n, count = 0;
  const m = { kind, caps, closes, calls, deny: false, packet: null, submitTicket: MAX, hook: null, closeHook: null,
    request: requestCodec.defaultValue(), response: responseCodec.defaultValue() };
  for (const row of ABI['2'].functions) functions[row.name] = () => { throw Error('Forbidden legacy business operation ' + row.name); };
  functions.abi_version = () => 1;
  functions.capability_open = (_mem, _p, _n, out) => {
    if (m.deny) return 13;
    const handle = 0x100000000n + next++; new DataView(memory.buffer).setBigUint64(Number(out), handle, true); return 0;
  };
  functions.capability_close = handle => { closes.push(handle); return m.closeHook ? m.closeHook(handle) : 0; };
  const session = new Session(2, { memory, functions }, { offset: 0n, inputCapacity: 4096, outputCapacity: 4096 });
  const a = session.open('MODEL-source'), b = session.open('MODEL-peer'); caps.push(a, b);
  if (kind === 3 && query) caps.push(session.open('MODEL-query'));
  m.source = a.handle; m.peer = b.handle; m.incarnation = 11n;
  m.receipt = operation => {
    const bytes = new Uint8Array(256), d = new DataView(bytes.buffer);
    d.setUint32(0, 0x45524d55, true); d.setUint16(4, 1, true); bytes[6] = operation; bytes[7] = 2;
    d.setUint32(8, 256, true); bytes[12] = kind; bytes[13] = 2;
    u64(d, 16, m.source); u64(d, 24, 7n); u64(d, 32, 8n); u64(d, 40, 9n); u64(d, 48, m.incarnation);
    bytes.set(qos(), 64); bytes.set(qos(), 144); return bytes;
  };
  m.delivery = ({ ticket = MAX, sequence = 1n, nonce = 1n, nativeSequence = -7n, body = kind === 3 ? responseCodec.encode(m.response) : requestCodec.encode(m.request), control = 0, error = 0, controlFlags = 0 } = {}) => {
    const bound = kind === 3 ? responseCodec.maxWireBytes : requestCodec.maxWireBytes;
    const bytes = new Uint8Array(352 + bound), d = new DataView(bytes.buffer);
    d.setUint32(0, 0x43524d55, true); d.setUint16(4, 1, true); bytes[6] = 5; bytes[7] = 2; d.setUint32(8, 160, true);
    bytes[12] = kind; bytes[13] = 16;
    for (const [at, value] of [[16,m.source],[24,7n],[32,8n],[40,9n],[48,m.incarnation],[56,m.peer],[64,ticket],[72,sequence],[80,nonce],[96,BigInt(bytes.length)],[104,192n+BigInt(body.length)]]) u64(d, at, value);
    const at = 160;
    d.setUint32(at, 0x494d5255, true); d.setUint16(at+4, 1, true); bytes[at+6] = control || (kind === 3 ? 3 : 2); bytes[at+7] = 2;
    d.setUint32(at+8, 192, true); d.setUint32(at+12, control ? 0 : 35, true);
    for (const [offset,value] of [[16,m.source],[24,7n],[32,8n],[40,9n],[48,m.incarnation],[56,sequence],[64,ticket],[88,MAX],[96,MAX],[112,BigInt(body.length)]]) u64(d, at+offset, value);
    d.setBigInt64(at+72, control ? 0n : -1n, true); d.setBigInt64(at+80, control ? 0n : 0n, true); d.setBigInt64(at+104, control ? 0n : nativeSequence, true);
    bytes[at+121] = control ? 0 : 16; bytes[at+122] = controlFlags; d.setUint32(at+124,error,true);
    bytes.set(body, 352); return bytes;
  };
  const binding = new OwnedServiceBridge({ invoke(index, args, input, output) {
    calls.push({ index, args: [...args], input: Uint8Array.from(input), outputBytes: output.length });
    const hook = m.hook?.(index,args,input,output); if (hook !== undefined) return hook;
    if (index === 0) return 1;
    if (index === 1 || index === 2 || index === 3) output.set(m.receipt(index === 3 ? 2 : 1));
    if (index === 4) new DataView(output.buffer).setBigUint64(0,m.submitTicket,true);
    if (index === 7) { if (!m.packet) return 11; output.set(m.packet); }
    if (index === 8) { count++; m.packet = null; }
    if (index === 10) output[0] = 1;
    return 0;
  } });
  m.endpoint = kind === 3 ? new OwnedClient(binding,a,b,definitions.getParameterTypes,{ownership,historyEntries,expectedQos:[qos(),qos()],availability:caps[2] ?? null}) :
    new OwnedServer(binding,a,b,definitions.getParameterTypes,{ownership,historyEntries,expectedQos:[qos(),qos()]});
  m.binding = binding; m.session = session; m.commitCount = () => count; return m;
}
function activated(...args) { const m=model(...args);assert.equal(m.calls.length,0);assert.equal(m.endpoint.activate(),0);return m; }
function calls(m,index) { return m.calls.filter(row=>row.index===index); }
let cases = 0;
function test(name, body) { body(); cases++; }

test('LOCAL constructor is effect-free and activates exact raw12 pair',()=>{
  const m=activated();assert.equal(calls(m,1).length,1);assert.deepEqual(calls(m,1)[0].args,[m.source,m.peer]);m.endpoint.close();
});
test('LOCAL successful metadata uses signed native ID and full unsigned ticket',()=>{
  const m=activated();assert.equal(m.endpoint.submit(m.request).ticket,MAX);m.packet=m.delivery();const r=m.endpoint.take();
  assert.equal(r.status,0);assert.equal(r.delivery.ticket,MAX);assert.equal(r.delivery.information.requestId.sequence,-7n);
  assert.equal(r.delivery.information.sourceTimestamp,-1n);assert.equal(r.delivery.information.requestId.writer.length,16);
  assert.deepEqual(r.delivery.value,m.response);assert.equal(m.commitCount(),1);m.endpoint.close();
});
test('LOCAL associated malformed metadata exact cancel without ACK',()=>{
  const m=activated();m.endpoint.submit(m.request);m.packet=m.delivery();m.packet[160+123]=1;
  assert.equal(m.endpoint.take().status,22);assert.deepEqual(calls(m,9)[0].args,[m.source,11n,1n,1n]);assert.equal(calls(m,8).length,0);
  m.packet=m.delivery({nonce:2n});assert.equal(m.endpoint.take().status,0);m.endpoint.close();
});
test('LOCAL unassociated UMRC retires without guessed cancellation',()=>{
  const m=activated();m.endpoint.submit(m.request);m.packet=m.delivery();m.packet[0]=0;
  assert.equal(m.endpoint.take().status,22);assert.equal(calls(m,9).length,0);assert.equal(calls(m,11).length,1);m.endpoint.close();assert.equal(calls(m,11).length,1);
});
test('LOCAL unknown ACK never cancels or retries and keeps original exception',()=>{
  const m=activated();m.endpoint.submit(m.request);m.packet=m.delivery();const error=new Error('MODEL unknown commit');
  m.hook=index=>{if(index===8)throw error;};assert.throws(()=>m.endpoint.take(),e=>e===error);
  assert.equal(calls(m,8).length,1);assert.equal(calls(m,9).length,0);assert.equal(calls(m,11).length,1);
  m.endpoint.close();assert.equal(m.closes.length,3);assert.equal(m.endpoint.facts().calls.commit.threw,true);
});
test('LOCAL duplicate admitted ticket is terminal without narrowing MAX',()=>{
  const m=activated();assert.equal(m.endpoint.submit(m.request).status,0);assert.equal(m.endpoint.submit(m.request).status,13);
  assert.equal(calls(m,11).length,1);m.endpoint.close();
});
test('LOCAL abandoned late response fully decodes and does not deliver success',()=>{
  const m=activated();const a=m.endpoint.submit(m.request);assert.equal(m.endpoint.abandon(a.ticket),0);
  const before=m.calls.length;assert.equal(m.endpoint.abandon(a.ticket),9);assert.equal(m.calls.length,before);assert.equal(calls(m,5).length,1);m.packet=m.delivery();
  const r=m.endpoint.take();assert.equal(r.status,0);assert.equal(r.discarded,true);assert.equal(r.delivery,null);assert.equal(m.commitCount(),1);m.endpoint.close();
});
test('LOCAL malformed abandoned body is cancelled before ACK',()=>{
  const m=activated();m.endpoint.submit(m.request);m.endpoint.abandon(MAX);m.packet=m.delivery({body:new Uint8Array([255])});
  assert.equal(m.endpoint.take().status,22);assert.equal(calls(m,9).length,1);assert.equal(calls(m,8).length,0);m.endpoint.close();
});
test('LOCAL history lifetime is not reset after abandon',()=>{
  const m=activated(3,'owned',false,1);m.endpoint.submit(m.request);m.endpoint.abandon(MAX);const n=m.calls.length;
  assert.equal(m.endpoint.submit(m.request).status,90);assert.equal(m.calls.length,n);m.endpoint.close();
});
test('LOCAL availability requires third independent owner and genuine wait value',()=>{
  const no=activated(3,'owned',false);assert.equal(no.endpoint.available(0n).status,13);assert.equal(calls(no,10).length,0);no.endpoint.close();
  const m=activated();assert.deepEqual(m.endpoint.available(123456789n),{status:0,available:true});assert.equal(calls(m,10)[0].args[3],123456789n);m.endpoint.close();
});
test('LOCAL server native-ID duplicate remains denied after response',()=>{
  const m=activated(4);m.packet=m.delivery({ticket:1n});let r=m.endpoint.take();assert.equal(r.status,0);assert.equal(m.endpoint.respond(r.delivery.ticket,m.response),0);
  m.packet=m.delivery({ticket:2n,sequence:2n,nonce:2n});r=m.endpoint.take();assert.equal(r.status,13);assert.equal(calls(m,9).length,1);assert.equal(calls(m,11).length,1);m.endpoint.close();
});
test('LOCAL server one response attempt even if raw response throws',()=>{
  const m=activated(4);m.packet=m.delivery({ticket:MAX});const r=m.endpoint.take();m.hook=index=>{if(index===6)throw Error('MODEL unknown response');};
  assert.throws(()=>m.endpoint.respond(r.delivery.ticket,m.response));assert.notEqual(m.endpoint.respond(r.delivery.ticket,m.response),0);assert.equal(calls(m,6).length,1);m.endpoint.close();
});
test('LOCAL independent reverse SDK closes continue through another exception',()=>{
  const m=activated();m.closeHook=handle=>{if(handle===m.caps[2].handle)throw Error('MODEL raw close');return 0;};
  assert.notEqual(m.endpoint.close(),0);assert.deepEqual(m.closes,[m.caps[2].handle,m.peer,m.source]);assert.equal(m.endpoint.facts().owners[2].cleanupAttempted,true);assert(m.endpoint.facts().owners[2].cleanupError instanceof Error);
  m.endpoint.close();assert.equal(m.closes.length,3);
});
test('LOCAL borrowed endpoint retires Native only and caller closes owners',()=>{
  const m=activated(3,'borrowed');assert.equal(m.endpoint.close(),0);assert.equal(m.closes.length,0);assert.equal(m.endpoint.facts().borrowedOwnerCleanupRequired,true);
  for(const cap of m.caps)cap.close();assert.equal(m.closes.length,3);
});
test('LOCAL inspection drift refuses before queue or submit effect',()=>{
  const m=activated();m.hook=(index,args,input,out)=>{if(index===3){out.set(m.receipt(2));out[143]^=1;return 0;}};
  assert.notEqual(m.endpoint.submit(m.request).status,0);assert.equal(calls(m,4).length,0);m.endpoint.close();
});
test('LOCAL concrete expected QoS refuses bad rows before any Native call',()=>{
  const a=qos();a[73]=1;assert.throws(()=>copyExpectedQos([a,qos()]));assert.throws(()=>copyExpectedQos([new Uint8Array(81),qos()]));
});
test('LOCAL raw12 forms prepare first and tickets invoke once',()=>{
  const m=activated();const out=new Uint8Array(1), token=prepareServiceCall(m.binding,10,[m.source,11n,m.caps[2].handle,0n],new Uint8Array(),out);
  assert.equal(serviceCallAttempted(token),false);assert.equal(invokeServiceCall(token),0);assert.equal(serviceCallAttempted(token),true);assert.throws(()=>invokeServiceCall(token));
  assert.throws(()=>prepareServiceCall(m.binding,7,[m.source,11n,16n],new Uint8Array(),new Uint8Array(352)));m.endpoint.close();
});
test('LOCAL default denial preserves original output and activates no endpoint',()=>{
  const m=model();m.deny=true;assert.throws(()=>m.session.open('MODEL-ungiven'),e=>e.code===13);assert.equal(m.calls.length,0);m.endpoint.close();
});
test('LOCAL close delegate reentry is17 without losing actual own attempt attribution',()=>{
  const m=activated();let nested=null,once=false;
  m.closeHook=()=>{if(!once){once=true;nested=m.endpoint.close();}return 0;};
  assert.equal(m.endpoint.close(),0);assert.equal(nested,17);assert.equal(m.closes.length,3);
  const facts=m.endpoint.facts();assert.equal(facts.owners[2].cleanupAttempted,true);assert.equal(facts.owners[2].externallyClosed,false);
  m.endpoint.close();assert.equal(m.closes.length,3);
});
test('LOCAL client setup retains frozen primary and independent partial-close failures',()=>{
  const m=model(), primary=Object.freeze(new Error('MODEL frozen client setup')), cleanup=new Error('MODEL query close');
  let closed=0,failure=null,nativeCalls=0;
  m.closeHook=()=>{closed++;if(closed===1)throw cleanup;return closed===2?107:0;};
  const selection={callAlias:'MODEL-new-call',observerAlias:'MODEL-new-observe',availabilityAlias:'MODEL-new-query',historyEntries:2,
    get expectedQos(){throw primary;}};
  assert.throws(()=>openDescriptionClient(m.session,{invoke(){nativeCalls++;return 22;}},records,selection),error=>{failure=error;return error instanceof SetupFailure;});
  assert.equal(failure.primary,primary);assert.equal(Object.isFrozen(primary),true);assert.equal(nativeCalls,0);
  assert.equal(failure.cleanupStatus,22);const facts=applicationCleanupFacts(failure.application);
  assert.equal(facts.owners[2].error,cleanup);assert.equal(facts.owners[1].code,107);assert.equal(facts.owners[0].code,0);
  assert(facts.owners.every(row=>row.attempted&&!row.retained));assert.equal(closed,3);
  m.closeHook=null;m.endpoint.close();
});
test('LOCAL server setup retains caller-visible rollback state before any Native activation',()=>{
  const m=model(),primary=Object.freeze(new Error('MODEL frozen server setup'));let failure=null,nativeCalls=0;
  m.closeHook=()=>107;
  const selection={advertiseAlias:'MODEL-new-advertise',responderAlias:'MODEL-new-respond',historyEntries:2,get expectedQos(){throw primary;}};
  assert.throws(()=>openParameterTypesServer(m.session,{invoke(){nativeCalls++;return 22;}},records,selection,()=>4),error=>{failure=error;return error instanceof SetupFailure;});
  assert.equal(failure.primary,primary);assert.equal(failure.cleanupStatus,107);assert.equal(nativeCalls,0);
  const facts=applicationCleanupFacts(failure.application);assert.equal(facts.owners.length,2);assert(facts.owners.every(row=>row.code===107&&row.attempted&&!row.retained));
  m.closeHook=null;m.endpoint.close();
});
test('LOCAL actual SDK close retains arbitrary thrown values and repeated close stays nonzero without replay',()=>{
  for(const thrown of [null,undefined,false,0]){
    const m=activated();let once=false;
    m.closeHook=()=>{if(!once){once=true;throw thrown;}return 0;};
    assert.notEqual(m.endpoint.close(),0);const facts=m.endpoint.facts();
    assert.equal(facts.owners[2].cleanupThrew,true);assert.equal(facts.owners[2].cleanupError,thrown);
    assert.equal(facts.owners[2].cleanupAttempted,true);assert.equal(facts.owners[2].retained,false);assert.equal(m.closes.length,3);
    assert.notEqual(m.endpoint.close(),0);assert.equal(m.closes.length,3);
  }
});
test('LOCAL partial setup keeps arbitrary cleanup throws on repeated caller-visible close',()=>{
  for(const thrown of [null,undefined,false,0]){
    const m=model(),primary=Object.freeze(new Error('MODEL setup before partial rollback'));let once=false,failure=null;
    m.closeHook=()=>{if(!once){once=true;throw thrown;}return 0;};
    const selection={advertiseAlias:'MODEL-new-advertise',responderAlias:'MODEL-new-respond',historyEntries:2,get expectedQos(){throw primary;}};
    assert.throws(()=>openParameterTypesServer(m.session,{invoke(){throw Error('No Native activation expected');}},records,selection,()=>4),error=>{failure=error;return error instanceof SetupFailure;});
    assert.equal(failure.primary,primary);assert.equal(failure.cleanupStatus,22);
    const facts=applicationCleanupFacts(failure.application);assert.equal(facts.owners[1].threw,true);assert.equal(facts.owners[1].error,thrown);assert.equal(facts.owners[1].attempted,true);
    const before=m.closes.length;assert.equal(closeApplication(failure.application),22);assert.equal(m.closes.length,before);
    m.closeHook=null;m.endpoint.close();
  }
});
assert.equal(cases,23);
// A MODEL marker names its scope; it is never a Wasm/Native acceptance marker.
process.stdout.write('LOCAL_JS_OWNED_SERVICE_MODEL_CASES='+cases+'\n');
