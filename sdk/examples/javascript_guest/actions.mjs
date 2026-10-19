/** Complete bounded application action dispatcher, shared by sync/async lessons.
 * This calls existing SDK objects; it supplies no second ROS implementation.
 * The closed method list prevents accidentally executing arbitrary reflective
 * methods. Every nonzero result is reported as failure, without retry.
 */
import { check, status, gate } from './support.mjs';
const methods = Object.freeze({ publisher:['publish'], client:['call'],
  parameters:['get','set','list','describe','declare','undeclare','delete','exists','search','subscribeEvents','pollEvent'],
  actionClient:['sendGoal','cancelOwn','cancelAny','cancelAll','feedback','result'],
  graph:['read'], clock:['now'], tf:['lookup','broadcast'], lifecycle:['getState','changeState'],
  logger:['write'], resource:['call'] });
export async function perform(application, selected, passes, gated) {
  check(Array.isArray(selected) && selected.length <= 64 && Number.isInteger(passes) && passes >= 0 && passes <= 200, 'Finite explicit actions/passes');
  const futures = [];
  for (const action of selected) {
    check(action && Object.keys(action).every(key => ['object','method','args','index'].includes(key)), 'Closed action');
    const args = action.args ?? []; check(Array.isArray(args) && args.length <= 4, 'Bounded explicit arguments');
    if (gated) gate('R');
    if (action.object === 'executor') {
      check(action.method === 'spinOnce' && !args.length, 'Explicit executor pass');
      for (const result of await application.executor.spinOnce()) check(result === 0 || result === 11, 'Executor result');
      continue;
    }
    const object = action.object === 'clock' ? application.objects.clocks?.[action.index ?? 0] : application.objects[action.object];
    check(object, 'Explicit resource alias required');
    if (action.object === 'client' && action.method === 'callDeferred') {
      futures.push(object.callDeferred(application.executor, ...args)); continue;
    }
    check(methods[action.object]?.includes(action.method), 'Closed SDK method');
    status(await object[action.method](...args));
  }
  for (let pass = 0; pass < passes; ++pass) {
    if (gated) gate('R');
    for (const result of await application.executor.spinOnce()) check(result === 0 || result === 11, 'Executor result');
  }
  for (const future of futures) { check(future.done, 'Deferred call requires an explicit pass'); status(future.result()); }
}
