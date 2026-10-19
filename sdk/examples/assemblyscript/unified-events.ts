/** Complete bounded event application shared by the two explicit backend entries.
 * Grant sub as String subscription and subEvents as event.observe/source=sub with
 * topic-message,resource-error,resource-closed. Default13 is reported, not treated
 * as native delivery. No implicit clock/thread/retry; idle11 is normal once.
 */
import {Api,Status} from '../../assemblyscript/api';
import {Capability,EventSubscription,Executor} from '../../assemblyscript/roswasm';
import {UnifiedEvent,EventKind} from '../../assemblyscript/events';
let last:u32=0;let lastKind:EventKind=EventKind.TopicMessage;
/** Function reference uses explicit application state; no unsupported JS closure.
 * UnifiedEvent owns its body copy. Pick the source's actual codec before use.
 */
function receive(event:UnifiedEvent):u32{lastKind=event.kind;return 0;}
/** Run real imports and always check both acquired owners' cleanup statuses. */
export function run(api:Api):void {
    const source=new Capability(api),observer=new Capability(api);last=source.open('sub');if(last)return;
    last=observer.open('subEvents');
    const events=new EventSubscription(observer,source,receive,256),executor=new Executor();
    if(!last)last=events.status;if(!last)last=executor.add(events);
    if(!last)last=events.readiness().status;
    if(!last){const status=executor.spinOnce()[0];if(status!=Status.Again)last=status;}
    executor.close(); // Drop registration before explicit source lifetime change.
    const cleanup=source.close();if(!last)last=cleanup;
    if(!last){last=events.spinOnce();if(!last&&lastKind!=EventKind.ResourceClosed)last=Status.Invalid;}
    if(observer.isOpen()){const cleanup=observer.close();if(!last)last=cleanup;}
}
/** Primitive embedder verdict; the command entry is implemented per backend. */
export function lastStatus():u32{return last;}
