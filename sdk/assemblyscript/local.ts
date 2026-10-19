/** Caller-driven local Context, Future, Timer and CallbackGroup.
 * These classes import no ROS or clock and create no native resource/authority.
 * A deferred Client call remains synchronous during that later scheduling step.
 * Copy results/events before retaining them. All functions run on the Guest
 * execution thread; an unrecoverable Wasm trap terminates the application and
 * is not translated to a successful callback or guaranteed cleanup.
 */
import { Status } from './api';
import { Reply } from './roswasm';
import { MAX_WIRE } from './codecs';

/** Mutually exclusive local callback guard, with no native callback registration. */
export class CallbackGroup {
  private busy: bool = false;
  /** Return17 on same-group reentry before invoking the nested function. */
  run(callback: () => u32): u32 { if(this.busy) return Status.Conflict; this.busy=true; const status=callback(); this.busy=false; return status; }
}

/** One copied Reply from a deferred local function, not a native async request. */
export class Future {
  private stage: u8 = 0;
  private result: Reply = new Reply(Status.Again);
  /** Whether one result can be consumed without scheduling or blocking. */
  isReady(): bool { return this.stage==2; }
  /** Take exactly once; pending/running11 and already taken9. */
  take(): Reply { if(this.stage==3) return new Reply(Status.BadHandle); if(this.stage!=2) return new Reply(Status.Again); this.stage=3; const result=this.result; this.result=new Reply(Status.BadHandle); return result; }
  /** Cancel only not-started local work. Its125 result does not revoke a host
   * alias, interrupt a started synchronous call or undo an external effect.
   */
  cancel(): u32 { if(this.stage) return Status.BadHandle; this.result=new Reply(Status.Revoked); this.stage=2; return Status.Success; }
  /** Internal scheduler transition; never invoke this to synthesize native data. */
  run(callback: () => Reply): void { if(this.stage) return; this.stage=1; const reply=callback(); this.result=reply.bytes.length>MAX_WIRE?new Reply(Status.TooLarge):new Reply(reply.status,reply.bytes.slice(0),reply.required,reply.requiredValid); this.stage=2; }
}
class DeferredTask {
  constructor(public future: Future, public callback: () => Reply) {}
}

/** Local timer with exact caller-domain nanoseconds. No clock is read implicitly. */
export class Timer {
  /** Local liveness is bookkeeping, not native readiness or permission. */
  live: bool = true;
  /** The scheduler uses one consistent caller-selected monotonic domain. */
  constructor(public deadline: u64, public period: u64, public group: CallbackGroup, public callback: () => u32) {}
  /** Cancel once; a second call is9 and performs no native close. */
  cancel(): u32 { if(!this.live) return Status.BadHandle; this.live=false; return Status.Success; }
}

/** Local result retaining registration status without using zero as an ID. */
export class FutureReply { constructor(public status: u32, public future: Future | null = null) {} }
/** Local result of timer registration; invalid/closed requests never register. */
export class TimerReply { constructor(public status: u32, public timer: Timer | null = null) {} }

/** Bounded local scheduling state, separate from Session's native-handle group. */
export class Context {
  private jobs: Array<DeferredTask> = new Array<DeferredTask>();
  private timers: Array<Timer> = new Array<Timer>();
  private closed: bool = false;
  private spinning: bool = false;
  private hasTime: bool = false;
  private lastTime: u64 = 0;
  /** Initialization status; constructor opens nothing. Budget must be1..1024. */
  status: u32 = Status.Success;
  constructor(private maximum: i32 = 1024) { if(maximum<1 || maximum>1024) this.status=Status.Invalid; }
  private room(): u32 {
    if(this.closed) return Status.BadHandle; if(this.status) return this.status; if(this.spinning) return Status.Conflict;
    for(let i=this.timers.length-1;i>=0;--i) if(!this.timers[i].live) this.timers.splice(i,1);
    return this.jobs.length+this.timers.length>=this.maximum?Status.TooLarge:Status.Success;
  }
  /** Queue one copied-result function, never execute it inline. AssemblyScript
   * uses function references; put callback state in application classes/globals
   * rather than assuming JavaScript closures are supported by this compiler.
   */
  defer(callback: () => Reply): FutureReply { const status=this.room(); if(status) return new FutureReply(status); const future=new Future(); this.jobs.push(new DeferredTask(future,callback)); return new FutureReply(Status.Success,future); }
  /** Register an explicit absolute deadline/positive period; no implicit Clock
   * grant. Overdue passes invoke at most once and do not replay missed ticks.
   */
  timer(first: u64, period: u64, group: CallbackGroup, callback: () => u32): TimerReply { const status=this.room(); if(status) return new TimerReply(status); if(!period) return new TimerReply(Status.Invalid); const timer=new Timer(first,period,group,callback); this.timers.push(timer); return new TimerReply(Status.Success,timer); }
  /** Process the queue present at entry and each due timer at most once.
   * Backwards time is rejected before effects. Overflow cancels without calling
   * the callback. Returned statuses are ordered local outcomes; inspect all.
   */
  spinOnce(now: u64): Array<u32> {
    if(this.closed) return [Status.BadHandle]; if(this.status) return [this.status]; if(this.spinning) return [Status.Conflict]; if(this.hasTime && now<this.lastTime) return [Status.Invalid];
    this.hasTime=true; this.lastTime=now; this.spinning=true;
    const jobs=this.jobs; this.jobs=new Array<DeferredTask>(); const out=new Array<u32>();
    for(let i=0;i<jobs.length;++i) { jobs[i].future.run(jobs[i].callback); out.push(Status.Success); }
    const count=this.timers.length;
    for(let i=0;i<count;++i) { const timer=this.timers[i]; if(!timer.live || now<timer.deadline) continue; if(now>u64.MAX_VALUE-timer.period) { timer.live=false; out.push(Status.Invalid); continue; } timer.deadline=now+timer.period; out.push(timer.group.run(timer.callback)); }
    this.spinning=false; return out;
  }
  /** Cancel queued local results and timers; no ROS close is imported. A started
   * synchronous RPC cannot be interrupted. Close Session separately and check
   * every returned close status. Repeated shutdown/closed operations report9.
   */
  shutdown(): u32 { if(this.closed) return Status.BadHandle; if(this.spinning) return Status.Conflict; this.closed=true; for(let i=0;i<this.jobs.length;++i) this.jobs[i].future.cancel(); this.jobs.length=0; for(let i=0;i<this.timers.length;++i) this.timers[i].live=false; this.timers.length=0; return Status.Success; }
}
