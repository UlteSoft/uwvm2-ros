/**
 * @file scheduling.mjs
 * @brief Bounded caller-driven scheduling shared by both SDK transports.
 *
 * These objects perform no ROS import, create no thread and read no clock.
 * A local Future describes deferred application work, not a nonblocking native
 * service request. The caller explicitly advances its Executor. Stopping local
 * delivery cannot establish rollback of an already admitted native effect.
 */
import { Status } from './raw.mjs';

/** Local scheduling refusal. Its code is useful for client-library control
 * flow but is not an errno returned by a Runtime import.
 */
export class SchedulingError extends Error {
  constructor(code,message) {super(message);this.name='SchedulingError';this.code=code;this.local=true;}
}

/** Local deferred result. Await promise or inspect result() after a caller
 * advances the executor. cancel() succeeds only before execution begins and
 * never calls a native cancellation operation or retires a Runtime goal ID.
 */
export class Future {
  constructor() {
    this.state='pending';this.value=undefined;this.error=null;
    this.promise=new Promise((resolve,reject)=>{this.resolvePromise=resolve;this.rejectPromise=reject;});
    // Inspecting a Future synchronously is supported. Retain the rejection for
    // awaiters while preventing an unattended rejected Promise warning.
    this.promise.catch(()=>{});
  }
  get done() {return this.state==='fulfilled'||this.state==='rejected'||this.state==='cancelled';}
  result() {
    if(!this.done)throw new SchedulingError(Status.AGAIN,'Deferred work has not completed');
    // JavaScript may throw any value, including undefined/null/false/0.
    // Completion state, not error truthiness, distinguishes a rejection.
    if(this.state==='rejected'||this.state==='cancelled')throw this.error;return this.value;
  }
  cancel() {
    if(this.state!=='pending')return false;
    this.state='cancelled';this.error=new SchedulingError(Status.REVOKED,'Local deferred work cancelled before execution');
    this.rejectPromise(this.error);return true;
  }
  /** Internal completion methods are public only for custom local executors.
   * They confer no Runtime authority or remote completion evidence.
   */
  start() {if(this.state!=='pending')return false;this.state='running';return true;}
  resolve(value) {if(this.state!=='running')throw new SchedulingError(Status.CONFLICT,'Future is not running');this.value=value;this.state='fulfilled';this.resolvePromise(value);}
  reject(error) {if(this.state!=='running')throw new SchedulingError(Status.CONFLICT,'Future is not running');this.error=error;this.state='rejected';this.rejectPromise(error);}
}

/** Local mutually exclusive callback group. It does not create a native ROS
 * callback group or OS thread. The guard is released even if user code throws.
 */
export class CallbackGroup {
  constructor() {this.busy=false;}
  enter(callback) {
    if(typeof callback!=='function')throw new TypeError('Local callback required');
    if(this.busy)throw new SchedulingError(Status.CONFLICT,'Nested callback group invocation');this.busy=true;
  }
  run(callback,...args) {
    this.enter(callback);
    try {const value=callback(...args);if(value&&typeof value.then==='function')throw new TypeError('Synchronous callback returned a Promise');return value;}
    finally {this.busy=false;}
  }
  async runAsync(callback,...args) {this.enter(callback);try{return await callback(...args);}finally{this.busy=false;}}
}

/** Explicit-ready local timer. The application supplies ready(clock) and may
 * supply a previously granted Clock. No hidden timer, sleep, epoch conversion
 * or clock capability is opened. It is polled at most once per executor pass.
 */
export class Timer {
  constructor(ready,callback,{clock=null,group=new CallbackGroup()}={}) {
    if(typeof ready!=='function'||typeof callback!=='function'||!(group instanceof CallbackGroup))throw new TypeError('Explicit timer readiness/callback/group required');
    this.ready=ready;this.callback=callback;this.clock=clock;this.group=group;this.closed=false;this.spinning=false;
  }
  spinOnce() {
    if(this.closed)return Status.BAD_HANDLE;
    if(this.spinning)return Status.CONFLICT;this.spinning=true;
    try {
      const ready=this.ready(this.clock);if(this.closed)return Status.BAD_HANDLE;if(typeof ready!=='boolean')throw new TypeError('Synchronous timer readiness must return boolean');
      if(!ready)return Status.AGAIN;this.group.run(this.callback);return Status.SUCCESS;
    } finally {this.spinning=false;}
  }
  close() {this.closed=true;return Status.SUCCESS;}
}
/** Asynchronous explicit-ready timer; readiness may await a caller-granted
 * Clock. Callback invocation still follows completion/release of that request.
 */
export class AsyncTimer extends Timer {
  async spinOnce() {
    if(this.closed)return Status.BAD_HANDLE;
    if(this.spinning)return Status.CONFLICT;this.spinning=true;
    try {
      const ready=await this.ready(this.clock);if(this.closed)return Status.BAD_HANDLE;if(typeof ready!=='boolean')throw new TypeError('Timer readiness must return boolean');
      if(!ready)return Status.AGAIN;await this.group.runAsync(this.callback);return Status.SUCCESS;
    } finally {this.spinning=false;}
  }
}

/** Bounded local queue, used by Executor.defer(). Each pass takes at most the
 * explicit maximum from the entry snapshot; tasks enqueued by a callback wait
 * for the next pass. Native operations within a task keep their original
 * bounded synchronous behavior and unchanged Runtime result/error codes.
 */
export class DeferredQueue {
  constructor({maxPending=64,maxTasksPerPass=1}={}) {
    for(const value of [maxPending,maxTasksPerPass])if(!Number.isSafeInteger(value)||value<1||value>4096)throw new RangeError('Bounded deferred queue size required');
    if(maxTasksPerPass>maxPending)throw new RangeError('Pass limit exceeds queue bound');
    this.maxPending=maxPending;this.maxTasksPerPass=maxTasksPerPass;this.tasks=[];this.active=[];this.closed=false;
  }
  defer(callback) {
    if(typeof callback!=='function')throw new TypeError('Deferred callback required');
    if(this.closed)throw new SchedulingError(Status.BAD_HANDLE,'Deferred queue closed');
    const waiting=this.tasks.length+this.active.filter(task=>task.future.state==='pending').length;
    if(waiting>=this.maxPending)throw new SchedulingError(Status.AGAIN,'Local deferred queue full');
    const future=new Future();this.tasks.push({callback,future});return future;
  }
  snapshot() {this.active=this.tasks.splice(0,this.maxTasksPerPass);return this.active;}
  /** Release the bookkeeping borrow after success or callback failure. */
  finishSnapshot() {this.active=[];}
  /** Cancel pending local work only. Running tasks belong to the current
   * executor pass; this method cannot cancel or replay their native effects.
   */
  close() {this.closed=true;for(const task of [...this.tasks,...this.active])task.future.cancel();this.tasks=[];}
  run(tasks) {
    for(const {callback,future} of tasks) {
      if(!future.start())continue;
      try {const value=callback();if(value&&typeof value.then==='function')throw new TypeError('Synchronous deferred task returned a Promise');future.resolve(value);}
      catch(error) {future.reject(error);}
    }
  }
  async runAsync(tasks) {
    for(const {callback,future} of tasks) {
      if(!future.start())continue;
      try {future.resolve(await callback());}catch(error){future.reject(error);}
    }
  }
}

/** Local parameter/event notifications. Install/remove explicit callbacks;
 * dispatch receives an already copied and decoded result. These callbacks
 * neither register native hooks nor imply parameter mutation authority.
 */
export class ParameterCallbacks {
  constructor() {this.callbacks=[];}
  add(callback) {if(typeof callback!=='function')throw new TypeError('Local parameter callback required');this.callbacks.push(callback);return ()=>{const index=this.callbacks.indexOf(callback);if(index!==-1)this.callbacks.splice(index,1);};}
  dispatch(value) {for(const callback of this.callbacks.slice()){const result=callback(value);if(result&&typeof result.then==='function')throw new TypeError('Synchronous parameter callback returned a Promise');}}
  async dispatchAsync(value) {for(const callback of this.callbacks.slice())await callback(value);}
}
