/** Source-only regression for actual local Future/DeferredQueue implementation.
 * Execute only on the admitted SSH Linux64GiB keeper. No ROS, grant, native
 * endpoint, Runtime effect, host callback or performance result is modeled.
 * A process zero exit requires every branch below; none has run for this patch.
 */
import { Future, DeferredQueue, SchedulingError } from '../scheduling.mjs';
import { Status } from '../raw.mjs';

function check(condition,label) { if(!condition)throw new Error(label); }
function exactThrow(callback,expected,label) {
  let threw=false,actual;
  try { callback(); } catch(error) { threw=true;actual=error; }
  check(threw&&Object.is(actual,expected),label);
}
async function exactRejection(promise,expected,label) {
  let threw=false,actual;
  try { await promise; } catch(error) { threw=true;actual=error; }
  check(threw&&Object.is(actual,expected),label);
}
function pendingRefusal(future,label) {
  let threw=false,actual;
  try { future.result(); } catch(error) { threw=true;actual=error; }
  check(threw&&actual instanceof SchedulingError&&actual.code===Status.AGAIN,label);
}

export async function runFutureRejectionContracts() {
  const object={marker:'exact object'},error=new Error('exact Error'),symbol=Symbol('exact symbol');
  const reasons=[0,-0,null,undefined,false,'',0n,symbol,error,object];
  for(const reason of reasons) {
    const direct=new Future();
    check(direct.start(),'direct starts once');direct.reject(reason);
    check(direct.done&&direct.state==='rejected','direct actual rejected state');
    exactThrow(()=>direct.result(),reason,'direct result retains exact rejection');
    await exactRejection(direct.promise,reason,'direct promise retains same rejection');
    exactThrow(()=>direct.result(),reason,'repeat observation never turns failure into value');
    check(Object.is(direct.error,reason),'observing failure does not replace its original value');

    let syncCalls=0;
    const queue=new DeferredQueue({maxPending:1,maxTasksPerPass:1});
    const scheduled=queue.defer(()=>{syncCalls++;throw reason;});
    const tasks=queue.snapshot();queue.run(tasks);queue.finishSnapshot();
    check(syncCalls===1&&scheduled.done&&scheduled.state==='rejected','actual synchronous queue rejects once');
    exactThrow(()=>scheduled.result(),reason,'sync queue result does not hide falsy throw');
    await exactRejection(scheduled.promise,reason,'sync queue promise and result agree');
    queue.run(tasks);check(syncCalls===1,'completed task cannot replay callback');queue.close();

    let asyncCalls=0;
    const asyncQueue=new DeferredQueue({maxPending:1,maxTasksPerPass:1});
    const asynchronous=asyncQueue.defer(async()=>{asyncCalls++;throw reason;});
    const asyncTasks=asyncQueue.snapshot();await asyncQueue.runAsync(asyncTasks);asyncQueue.finishSnapshot();
    check(asyncCalls===1&&asynchronous.done&&asynchronous.state==='rejected','actual asynchronous queue rejects once');
    exactThrow(()=>asynchronous.result(),reason,'async queue result retains exact rejection');
    await exactRejection(asynchronous.promise,reason,'async promise and result agree');
    await asyncQueue.runAsync(asyncTasks);check(asyncCalls===1,'async completed callback cannot replay');asyncQueue.close();
  }

  for(const value of [0,-0,null,undefined,false,'',0n,object]) {
    const direct=new Future();check(direct.start(),'success starts');direct.resolve(value);
    check(direct.done&&direct.state==='fulfilled','actual fulfilled state');
    check(Object.is(direct.result(),value),'falsy success remains a successful exact value');
    check(Object.is(await direct.promise,value),'success promise agrees with result');
    const queue=new DeferredQueue({maxPending:1,maxTasksPerPass:1});
    const future=queue.defer(()=>value);queue.run(queue.snapshot());queue.finishSnapshot();
    check(Object.is(future.result(),value)&&Object.is(await future.promise,value),'queue falsy success preserved');
    queue.close();
  }

  const pending=new Future();pendingRefusal(pending,'pending result remains unavailable');
  check(pending.start(),'pending becomes running');pendingRefusal(pending,'running result remains unavailable');
  check(pending.cancel()===false,'running work is not falsely cancelled');pending.resolve(undefined);
  check(pending.result()===undefined,'fulfilled undefined differs from thrown undefined');

  const cancelled=new Future();check(cancelled.cancel(),'pending cancellation succeeds once');
  const original=cancelled.error;
  check(cancelled.done&&cancelled.state==='cancelled'&&original instanceof SchedulingError,'actual cancellation state');
  exactThrow(()=>cancelled.result(),original,'cancelled result throws exact retained local error');
  await exactRejection(cancelled.promise,original,'cancelled promise agrees with result');
  check(cancelled.cancel()===false&&!cancelled.start(),'cancelled future never starts or cancels twice');

  let cancelledCalls=0;
  const queue=new DeferredQueue({maxPending:1,maxTasksPerPass:1});
  const cancelledTask=queue.defer(()=>{cancelledCalls++;return 0;});
  const held=queue.snapshot();queue.close();queue.run(held);queue.finishSnapshot();
  check(cancelledCalls===0&&cancelledTask.state==='cancelled','closing borrowed local queue never runs pending callback');
  exactThrow(()=>cancelledTask.result(),cancelledTask.error,'queue cancellation remains a failure result');
  await exactRejection(cancelledTask.promise,cancelledTask.error,'queue cancelled promise agrees');
}

await runFutureRejectionContracts();
