/** Lesson7: local Future/Context/Timer/CallbackGroup with explicit caller time.
 * No NativeApi, capability, implicit clock or ROS import is used. A real
 * application may pass time from a separately granted monotonic Clock.
 * Deferred native Client calls remain synchronous in that later scheduling step.
 */
import { Context, CallbackGroup } from '../../../assemblyscript/local';
import { Reply } from '../../../assemblyscript/roswasm';
import { Status } from '../../../assemblyscript/api';
export { abortGuest } from '../../../assemblyscript/api';
let status:u32=0, ticks:u32=0;
function result():Reply { return new Reply(Status.Success,new Uint8Array(1)); }
function tick():u32 { ticks++; return Status.Success; }
function remember(value:u32):void { if(value && !status) status=value; }
/** Take a copied deferred result, run one tick, cancel and shut down explicitly. */
export function _start():void {
  const context=new Context(2), deferred=context.defer(result); remember(deferred.status);
  const timer=context.timer(10,5,new CallbackGroup(),tick); remember(timer.status);
  const first=context.spinOnce(0); for(let i=0;i<first.length;++i) remember(first[i]);
  const future=deferred.future; if(future) remember(future.take().status);
  const due=context.spinOnce(10); for(let i=0;i<due.length;++i) remember(due[i]);
  const registration=timer.timer; if(registration) remember(registration.cancel());
  remember(context.shutdown()); // Native Session cleanup would be separate.
}
/** Scheduling error diagnostic; no logger permission or ambient console exists. */
export function lastStatus():u32 { return status; }
/** Actual local callbacks completed in this example, without a native delivery claim. */
export function completedTicks():u32 { return ticks; }
