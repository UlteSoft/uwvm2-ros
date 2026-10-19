/** Executable local scheduler and ownership contracts, prepared for the root's
 * serial AssemblyScript/Wasm test lane. No native Api or ROS import is used.
 * Fake imported results model lifecycle only; passing does not prove DDS/ROS.
 */
import { Api, Status } from '../api';
import { Reply, Session, Executor, Pollable } from '../roswasm';
import { Context, CallbackGroup } from '../local';
export { abortGuest } from '../api';
let checks:u32=0, effects:u32=0;
function require(value:bool):void { if(!value) unreachable(); checks++; }
function result():Reply { effects++; return new Reply(Status.Success,new Uint8Array(1)); }
function tick():u32 { effects++; return Status.Success; }
let shared:CallbackGroup=new CallbackGroup();
function nested():u32 { require(shared.run(tick)==Status.Conflict); return Status.Success; }
class Idle implements Pollable { spinOnce():u32 { return Status.Again; } }
class FixtureApi extends Api {
  opens:u32=0; closes:u32=0; next:u64=1;
  constructor() { super(2); }
  version():u32 { return 1; }
  open(_memory:u32,_offset:u64,_length:u64,slot:u64):u32 { this.opens++; store<u64>(<usize>slot,this.next++); return Status.Success; }
  close(_handle:u64):u32 { this.closes++; return Status.Disconnected; }
}
/** Count only when this function completes without a local trap. */
export function _start():void {
  const context=new Context(2), scheduled=context.defer(result), future=scheduled.future;
  require(scheduled.status==0 && future!=null); require(effects==0);
  if(future) {
    require(future.take().status==Status.Again);
    const timerReply=context.timer(10,5,shared,tick), timer=timerReply.timer;
    require(timerReply.status==0 && timer!=null);
    require(context.defer(result).status==Status.TooLarge);
    require(context.spinOnce(0).length==1); require(future.isReady()); require(future.take().status==0);
    require(future.take().status==Status.BadHandle); require(effects==1);
    require(context.spinOnce(9).length==0); require(context.spinOnce(10).length==1);
    require(context.spinOnce(100).length==1); require(effects==3);
    require(context.spinOnce(99)[0]==Status.Invalid); require(effects==3);
    require(context.shutdown()==0); require(context.spinOnce(101)[0]==Status.BadHandle);
    require(context.shutdown()==Status.BadHandle); if(timer) require(timer.cancel()==Status.BadHandle);
  }
  const cancelled=new Context(1), work=cancelled.defer(result).future;
  if(work) { require(work.cancel()==0); require(work.cancel()==Status.BadHandle); cancelled.spinOnce(0); require(work.take().status==Status.Revoked); require(effects==3); }
  require(shared.run(nested)==0); require(shared.run(tick)==0);
  const overflow=new Context(1), overflowTimer=overflow.timer(u64.MAX_VALUE,1,shared,tick);
  require(overflowTimer.status==0); require(overflow.spinOnce(u64.MAX_VALUE)[0]==Status.Invalid); require(effects==4);
  const api=new FixtureApi(), session=new Session(api,1), node=session.node('local');
  const first=node.open('pub'); require(first.isOpen()); require(first.rawHandle().handle==1);
  require(node.open('sub').openStatus==Status.TooLarge); require(api.opens==1);
  const outcomes=session.close(); require(outcomes.length==1 && outcomes[0]==Status.Disconnected);
  require(!first.isOpen() && first.rawHandle().status==Status.BadHandle); require(node.open('pub').openStatus==Status.BadHandle);
  require(session.close()[0]==Status.BadHandle); require(first.close()==Status.BadHandle); require(api.closes==1);
  const executor=new Executor(1); require(executor.add(new Idle())==0); require(executor.add(new Idle())==Status.TooLarge);
  require(executor.remove(0)==0); require(executor.close()==0); require(executor.spinOnce()[0]==Status.BadHandle);
  require(executor.add(new Idle())==Status.BadHandle); require(executor.close()==Status.BadHandle);
}
/** The independent harness compares this count only after _start succeeds. */
export function completedChecks():u32 { return checks; }
