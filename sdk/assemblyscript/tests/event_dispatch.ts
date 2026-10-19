/** Actual production EventSubscription/Executor with finite local fake imports.
 * No NativeApi, WASI, ROS context or authority is supplied. Prepared source only;
 * the guarded model runner must compile and execute this fixture separately.
 */
import {Api,Status} from '../api';
import {Capability,EventSubscription,Executor} from '../roswasm';
import {UnifiedEvent,EventBinding} from '../events';
export {abortGuest} from '../api';
let checks:u32=0,calls:u32=0;let entity:EventSubscription|null=null,observer:Capability|null=null;
function require(ok:bool):void{if(!ok)unreachable();checks++;}
class Fixture extends Api{
  next:u64=1;sequence:u64=1;source:u64=2;session:u64=19;malformed:bool=false;idle:bool=false;polls:u32=0;closes:u32=0;
  constructor(){super(2);}
  version():u32{return 1;}
  open(_m:u32,_a:u64,_n:u64,slot:u64):u32{store<u64>(<usize>slot,this.next++);return 0;}
  close(_id:u64):u32{this.closes++;return 0;}
  watchEvents(o:u64,s:u64,c:u64):u32{return o==1&&s==2&&c==0?0:Status.Denied;}
  pollEventV1(_o:u64,_m:u32,out:u64,capacity:u64,slot:u64):u32{
    this.polls++;if(this.idle)return Status.Again;store<u64>(<usize>slot,64);if(capacity<64)return Status.TooLarge;
    const p=<usize>out;memory.fill(p,0,64);store<u32>(p,0x45525755);store<u16>(p+4,1);store<u16>(p+6,1);store<u8>(p+8,2);
    store<u64>(p+16,this.source);store<u64>(p+32,this.sequence);store<u64>(p+40,17);store<u64>(p+48,this.session);store<u64>(p+56,this.malformed?1:0);return 0;
  }
}
function next(event:UnifiedEvent):u32{require(event.sequence==2);calls++;return 0;}
function first(event:UnifiedEvent):u32{
  calls++;require(event.sequence==1&&event.body.length==0);const selected=entity,owner=observer;
  if(selected){require(selected.spinOnce()==Status.Conflict);require(selected.close()==Status.Conflict);selected.onEvent(next);}
  if(owner)require(owner.close()==Status.Conflict);return 0;
}
/** Execute only explicit source-bound models; no native success is inferred. */
export function _start():void{
  checks=0;calls=0;const api=new Fixture(),o=new Capability(api),s=new Capability(api);observer=o;
  require(o.open('observer')==0&&s.open('source')==0);const stream=new EventSubscription(o,s,first,64);entity=stream;
  require(stream.status==0);const executor=new Executor();require(executor.add(stream)==0);
  require(executor.spinOnce()[0]==0&&calls==1&&api.polls==1&&api.closes==0);
  api.sequence=2;require(executor.spinOnce()[0]==0&&calls==2);require(stream.spinOnce()==Status.Invalid&&calls==2);
  api.sequence=3;api.source=3;require(stream.spinOnce()==Status.Invalid&&calls==2);api.source=2;
  api.session=20;require(stream.spinOnce()==Status.Invalid&&calls==2);api.session=19;
  api.malformed=true;require(stream.spinOnce()==Status.Invalid&&calls==2);api.malformed=false;
  api.idle=true;require(executor.spinOnce()[0]==Status.Again);require(executor.close()==0);
  require(stream.close()==0&&api.closes==1&&s.isOpen());require(stream.spinOnce()==Status.BadHandle);require(s.close()==0);
  // A short record also leaves an independently initialized binding untouched.
  const binding=new EventBinding(2,2);require(binding.decode(new Uint8Array(63))===null);
}
/** Canonical pure-model counter; Root records its actual result only after run. */
export function completedChecks():u32{return checks;}
