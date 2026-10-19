/** Complete unified-event application over a supplied qualified sync bridge or
 * persistent real Guest relay. Every factory/call/cleanup executes; no actors,
 * native DDS connections, grants, clock or implicit retry are created here.
 * Run with run.mjs and plan-unified-events.json after independently qualifying
 * the actual Worker, Gateway and private relay pipes. Default policy returns13.
 */
import {decodeEventReadiness} from '../../javascript/events.mjs';
import {Status} from '../../javascript/raw.mjs';
import {RosError,EventSubscription,Executor} from '../../javascript/roswasm.mjs';
import {AsyncSession,AsyncEventSubscription,AsyncExecutor} from '../../javascript/async.mjs';
/** Associate explicit source/observer owners; read once, close the source, then
 * verify its permitted closed record before checking observer cleanup. An idle
 * first poll is normal. The caller receives primary and independent cleanup
 * statuses; no side-effecting operation is retried on107/110.
 */
export async function runUnifiedEvents(session,sourceAlias,observerAlias) {
  if(typeof sourceAlias!=='string'||!sourceAlias.length||typeof observerAlias!=='string'||!observerAlias.length||sourceAlias===observerAlias)throw new TypeError('Distinct explicit aliases required');
  let source,observer,primary=0,cleanup=0,received=0,ready=null;
  const check=(status,operation)=>{if(status)throw new RosError(status,operation);};
  try {
    source=await session.open(sourceAlias);observer=await session.open(observerAlias);
    let lastKind=0;
    // Both resource classes use actual event_poll_v1 and local owner leases. A
    // Promise-facing callback is awaited with that same observer still borrowed.
    const handler=event=>{lastKind=event.kind;};
    const events=session instanceof AsyncSession?await AsyncEventSubscription.watch(observer,source,handler):new EventSubscription(observer,source,handler);
    const executor=session instanceof AsyncSession?new AsyncExecutor():new Executor();executor.add(events);
    const counters=await events.readiness();check(counters.status,'event_ready');ready=decodeEventReadiness(counters.bytes);
    const initial=(await executor.spinOnce())[0];if(initial!==Status.AGAIN){check(initial,'event_poll_v1');received=1;}
    executor.close(); // Generic JS Executor drops registrations, not native owners.
    check(await source.close(),'capability_close');check(await events.spinOnce(),'event_poll_v1');
    if(lastKind!==7)throw new RosError(Status.INVALID,'resource_closed');
  } catch(error) {primary=Number.isInteger(error?.code)?error.code:Status.INVALID;}
  finally {
    for(const cap of [source,observer])if(cap&&!cap.closed){try {const status=await cap.close();if(!cleanup)cleanup=status;}catch(error){if(!cleanup)cleanup=Number.isInteger(error?.code)?error.code:Status.INVALID;}}
  }
  return {status:primary||cleanup,primary,cleanup,received,ready};
}
