/** Additive, copied unified event v1 values. Decoding grants no authority.
 * The64-byte header is unaligned little endian and Body retains its original
 * source codec. No native ROS callback enters the guest or receives pointers.
 */
export enum EventKind { TopicMessage=1,ServiceRequest=2,ServiceResponse=3,ActionFeedback=4,ActionResult=5,ResourceError=6,ResourceClosed=7 }
/** Copied event metadata with full-width opaque IDs and unchanged canonical body. */
export class UnifiedEvent {
  kind:EventKind=EventKind.TopicMessage;
  backend:u8=0;
  /** Flags1 retired,2 admitted external effects may remain,4 incoming Action,8 parameter projection. */
  flags:u8=0;
  error:u32=0;
  source:u64=0;correlation:u64=0;sequence:u64=0;instance:u64=0;session:u64=0;
  body:Uint8Array=new Uint8Array(0);
}
/** Validate the complete record before publishing any object/body. Null means22.
 * A kind never substitutes for the source capability's exact operation/type.
 */
export function decodeUnifiedEvent(bytes:Uint8Array):UnifiedEvent|null {
  if(bytes.length<64||bytes.length>4194304)return null;
  const p=bytes.dataStart;
  if(load<u32>(p)!=0x45525755||load<u16>(p+4)!=1||load<u16>(p+10)!=0)return null;
  const kind=load<u16>(p+6),backend=load<u8>(p+8),flags=load<u8>(p+9),error=load<u32>(p+12);
  const source=load<u64>(p+16),correlation=load<u64>(p+24),sequence=load<u64>(p+32),instance=load<u64>(p+40),session=load<u64>(p+48),length=load<u64>(p+56);
  if(kind<1||kind>7||backend<1||backend>2||!source||!sequence||!instance||!session||length!=<u64>(bytes.length-64))return null;
  if(kind>=6){if(length||(flags&~<u8>3)||(kind==6&&!error)||(kind==7&&!(flags&1)))return null;}
  else {
    if(error)return null;
    if(kind==1){if(correlation||(flags!=0&&flags!=8))return null;}
    else if(!correlation||(kind==2?(flags!=0&&flags!=4):flags!=0))return null;
    if((kind==2||kind==4||kind==5)&&(length<8||load<u64>(p+64)!=correlation))return null;
  }
  const result=new UnifiedEvent();result.kind=<EventKind>kind;result.backend=backend;result.flags=flags;result.error=error;result.source=source;result.correlation=correlation;result.sequence=sequence;result.instance=instance;result.session=session;result.body=bytes.slice(64);return result;
}
/** Current credit only; no reservation or permission is created. */
export class EventReadiness { readyEvents:u64=0;nextEventBytes:u64=0;admissionSlots:u64=0;admissionBytes:u64=0; }
/** Exactly32 little-endian bytes with no wasm32 narrowing. Null means22. */
export function decodeEventReadiness(bytes:Uint8Array):EventReadiness|null {
  if(bytes.length!=32)return null;const result=new EventReadiness(),p=bytes.dataStart;
  result.readyEvents=load<u64>(p);result.nextEventBytes=load<u64>(p+8);result.admissionSlots=load<u64>(p+16);result.admissionBytes=load<u64>(p+24);return result;
}

/** Local binding for one watched source. It grants no native authority.
 * Decode failure preserves state; successful sequence/session binding commits
 * before application callbacks, so handler failure cannot replay a record.
 */
export class EventBinding {
  private sequence:u64=0;private instance:u64=0;private session:u64=0;
  /** Invalid construction reports22 through valid=false, without any import. */
  valid:bool;
  constructor(private backend:u32,private source:u64,private correlation:u64=0){this.valid=(backend==1||backend==2)&&source!=0;}
  /** Return a complete owning copy, or null on malformed/foreign/stale data.
   * Nonzero Action correlation still permits uncorrelated error metadata.
   */
  decode(bytes:Uint8Array):UnifiedEvent|null{
    if(!this.valid)return null;const event=decodeUnifiedEvent(bytes);if(event===null)return null;
    if(event.backend!=this.backend||event.source!=this.source||this.sequence==u64.MAX_VALUE||event.sequence!=this.sequence+1||
       (this.instance!=0&&(event.instance!=this.instance||event.session!=this.session))||
       (this.correlation!=0&&event.correlation!=0&&event.correlation!=this.correlation))return null;
    this.sequence=event.sequence;this.instance=event.instance;this.session=event.session;return event;
  }
}
