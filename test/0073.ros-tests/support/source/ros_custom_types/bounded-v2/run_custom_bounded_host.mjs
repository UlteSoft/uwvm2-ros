/** @file
 * @brief Real persistent-Wasm relay host for one bounded custom client/server.
 * Usage: node run_custom_bounded_host.mjs 1|2 client|server
 * Only fd3/4 carry copied binary commands/results from the externally supervised
 * Worker. This host spawns/connects nothing and owns no Gateway/native endpoint.
 * Its stdin R is externally paced; stdout markers have checked write callbacks.
 * Runtime authority remains in the actual relay Guest, with explicit aliases.
 */
import {createReadStream,createWriteStream} from 'node:fs';
import {AsyncSession} from '../../../sdk/origin/javascript/async.mjs';
import {runCustomClient} from '../../../sdk/origin/custom-types/bounded-examples/client.mjs';
import {runCustomServer} from '../../../sdk/origin/custom-types/bounded-examples/server.mjs';
const [backend,role,...extra]=process.argv.slice(2);
if(extra.length||!['1','2'].includes(backend)||!['client','server'].includes(role))throw new Error('Closed bounded selection required');
const readable=createReadStream(null,{fd:3,autoClose:false,highWaterMark:8192}),writable=createWriteStream(null,{fd:4,autoClose:false,highWaterMark:8192});
const iterator=process.stdin[Symbol.asyncIterator]();let pending=new Uint8Array();
async function gate(expected='R'){if(!pending.length){const next=await iterator.next();if(next.done||!(next.value instanceof Uint8Array)||!next.value.length||next.value.length>16)throw new Error('Bounded gate EOF/input');pending=next.value.slice();}const byte=pending[0];pending=pending.slice(1);if(byte!==expected.charCodeAt(0))throw new Error('Exact R gate required');return expected;}
const marker=text=>new Promise(resolve=>{process.stdout.write(text,error=>resolve(error?107:0));});
// The supervisor proves this exact host Node birth/executable while stopped,
// before the first copied relay request. This is a host map, not ARM proof.
process.kill(process.pid,'SIGSTOP');
try {
  const session=await AsyncSession.attach({version:Number(backend),readable,writable,inputCapacity:4080,outputCapacity:4080,timeoutMs:10000});
  const code=await (role==='client'?runCustomClient:runCustomServer)(session,gate,marker);
  // A printed SDK completion is insufficient: the supervisor also requires an
  // exact relay shutdown acknowledgement and the actual retained Worker exit0.
  await session.shutdown();if(code)process.exitCode=code;
} catch {await marker('FAIL_BOUNDED_RELAY\n');process.exitCode=1;}
finally {readable.destroy();writable.destroy();process.stdin.pause();}
