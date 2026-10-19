/** Lesson05: actual guest JS closures, bounded Future queue and owner lifetime.
 * This local computation opens no alias or clock. A Future becomes ready only
 * when the explicit executor advances; the Promise job pump is not a ROS thread.
 */
import { createGuestSession, Context, Executor, CallbackGroup, Timer } from '../../javascript/guest.mjs';
import { check, marker, clean } from './support.mjs';
export async function main() {
  const context = new Context(createGuestSession()), node = context.createNode('guest-local');
  const executor = new Executor({ maxPending: 4, maxTasksPerPass: 1 }), group = new CallbackGroup();
  let count = 0; const timer = new Timer(() => true, () => { ++count; }, { group });
  executor.add(timer);
  const future = executor.defer(() => 6 * 7);
  check(!future.done && count === 0, 'No implicit execution');
  check(executor.spinOnce().every(result => result === 0) && count === 1 && future.result() === 42, 'Guest timer/deferred closure');
  check(await future.promise === 42, 'Engine Promise continuation');
  executor.close(); timer.close(); clean(context.shutdown());
  let refused = false; try { node.open('ungranted'); } catch (error) { refused = error.code === 9; }
  check(refused, 'Closed Context rejects owner creation before any import');
  marker('PASS_JS_GUEST_OWNERSHIP'); return 0;
}
