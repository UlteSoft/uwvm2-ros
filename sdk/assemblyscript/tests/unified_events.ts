/** Source-free real SDK event decoder contracts, prepared for serial Wasm use.
 * No NativeApi, WASI import, clock, thread or ROS resource is instantiated.
 * Seven complete kinds, full-width identities, malformed envelopes, owning
 * body copy and exact readiness length execute33 meaningful assertions.
 */
import {decodeUnifiedEvent,decodeEventReadiness} from '../events';
export {abortGuest} from '../api';
let count:u32=0;
function require(value:bool):void{if(!value)unreachable();count++;}
function record(kind:u16):Uint8Array{
  const length=(kind==2||kind==4||kind==5)?72:64,bytes=new Uint8Array(length),p=bytes.dataStart;
  store<u32>(p,0x45525755);store<u16>(p+4,1);store<u16>(p+6,kind);store<u8>(p+8,2);
  store<u64>(p+16,0xf000000000000011);store<u64>(p+32,1);store<u64>(p+40,0x20000000000001);store<u64>(p+48,0xe000000000000021);
  if(kind>=2&&kind<=5)store<u64>(p+24,17);if(length==72){store<u64>(p+56,8);store<u64>(p+64,17);}
  if(kind==6){store<u8>(p+9,2);store<u32>(p+12,5);}if(kind==7)store<u8>(p+9,1);return bytes;
}
/** Execute once; every refusal is tested without a substitute backend. */
export function _start():void{
  count=0;
  for(let kind:u16=1;kind<=7;kind++){const bytes=record(kind),event=decodeUnifiedEvent(bytes);require(event!==null);if(event){require(event.kind==kind&&event.backend==2&&event.source==0xf000000000000011&&event.instance==0x20000000000001&&event.session==0xe000000000000021);require(event.body.length==bytes.length-64);}}
  let bad=record(6);store<u8>(bad.dataStart+9,4);require(decodeUnifiedEvent(bad)===null);
  bad=record(1);store<u16>(bad.dataStart+6,8);require(decodeUnifiedEvent(bad)===null);
  bad=record(1);store<u16>(bad.dataStart+10,1);require(decodeUnifiedEvent(bad)===null);
  bad=record(1);store<u16>(bad.dataStart+4,2);require(decodeUnifiedEvent(bad)===null);
  bad=record(2);store<u64>(bad.dataStart+64,18);require(decodeUnifiedEvent(bad)===null);
  bad=record(1);store<u64>(bad.dataStart+56,1);require(decodeUnifiedEvent(bad)===null);
  bad=record(1);store<u64>(bad.dataStart+16,0);require(decodeUnifiedEvent(bad)===null);
  bad=record(1);store<u64>(bad.dataStart+32,0);require(decodeUnifiedEvent(bad)===null);
  const bytes=new Uint8Array(32),p=bytes.dataStart;store<u64>(p,0x20000000000001);store<u64>(p+8,69);store<u64>(p+16,2);store<u64>(p+24,u64.MAX_VALUE);
  require(decodeEventReadiness(bytes.slice(0,31))===null);const ready=decodeEventReadiness(bytes);require(ready!==null&&ready.readyEvents==0x20000000000001&&ready.nextEventBytes==69&&ready.admissionSlots==2&&ready.admissionBytes==u64.MAX_VALUE);require(decodeEventReadiness(new Uint8Array(33))===null);
  const original=record(2),copy=decodeUnifiedEvent(original);store<u8>(original.dataStart+64,18);require(copy!==null&&copy.body[0]==17);
}
/** Exact fixture inventory; it describes local codec checks, not ROS delivery. */
export function checks():u32{return count;}
/** Canonical pure-model runner counter; retain checks() for existing callers. */
export function completedChecks():u32{return count;}
