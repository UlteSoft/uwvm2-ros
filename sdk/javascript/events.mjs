/** Additive unified event v1 copied codecs. No native/ROS/network operation,
 * permission, thread, clock or alias is created by decoding these values. */
export const EventKind=Object.freeze({TOPIC_MESSAGE:1,SERVICE_REQUEST:2,SERVICE_RESPONSE:3,
  ACTION_FEEDBACK:4,ACTION_RESULT:5,RESOURCE_ERROR:6,RESOURCE_CLOSED:7});
/** Decode one complete64-byte unaligned LE envelope. All identities are BigInt.
 * Body remains the source's unchanged canonical codec, including owned ID8.
 * Flags1=retired,2=admitted external effects may remain,4=incoming Action,
 *8=parameter projection. Unknown flags cannot convert source permissions.
 * The returned body is an owning copy and remains valid after memory growth.
 */
export function decodeUnifiedEvent(bytes) {
  if(!(bytes instanceof Uint8Array)||bytes.length<64||bytes.length>4194304)throw new TypeError('Complete bounded event bytes required');
  const v=new DataView(bytes.buffer,bytes.byteOffset,bytes.length);
  const bad=()=>{throw new TypeError('Invalid unified event envelope');};
  if(v.getUint32(0,true)!==0x45525755||v.getUint16(4,true)!==1||v.getUint16(10,true)!==0)bad();
  const kind=v.getUint16(6,true),backend=bytes[8],flags=bytes[9],error=v.getUint32(12,true),
    source=v.getBigUint64(16,true),correlation=v.getBigUint64(24,true),sequence=v.getBigUint64(32,true),
    instance=v.getBigUint64(40,true),session=v.getBigUint64(48,true),length=v.getBigUint64(56,true);
  if(kind<1||kind>7||backend<1||backend>2||!source||!sequence||!instance||!session||length!==BigInt(bytes.length-64))bad();
  if(kind>=6){if(length||(flags&~3)||(kind===6&&!error)||(kind===7&&!(flags&1)))bad();}
  else {
    if(error)bad();
    if(kind===1){if(correlation||(flags!==0&&flags!==8))bad();}
    else if(!correlation||(kind===2?(flags!==0&&flags!==4):flags!==0))bad();
    if((kind===2||kind===4||kind===5)&&(length<8n||v.getBigUint64(64,true)!==correlation))bad();
  }
  return Object.freeze({kind,backend,flags,error,source,correlation,sequence,instance,session,body:bytes.slice(64)});
}
/** Decode exactly32 full-width counters. Positive credit is only a snapshot;
 * submit11 still means that no credit was reserved/admitted for that operation. */
export function decodeEventReadiness(bytes) {
  if(!(bytes instanceof Uint8Array)||bytes.length!==32)throw new TypeError('Exactly32 readiness bytes required');
  const v=new DataView(bytes.buffer,bytes.byteOffset,32);
  return Object.freeze({readyEvents:v.getBigUint64(0,true),nextEventBytes:v.getBigUint64(8,true),admissionSlots:v.getBigUint64(16,true),admissionBytes:v.getBigUint64(24,true)});
}
/** Local audit binding for a caller-owned observer. First verified envelope
 * pins the trusted instance/session; all later events require the same binding
 * and strictly increasing sequence. This guard never replaces Runtime checks.
 */
export class EventBinding {
  constructor(backend,source,correlation=0n) {
    if((backend!==1&&backend!==2)||typeof source!=='bigint'||source<=0n||source>0xffffffffffffffffn||typeof correlation!=='bigint'||correlation<0n||correlation>0xffffffffffffffffn)throw new TypeError('Exact source binding required');
    this.backend=backend;this.source=source;this.correlation=correlation;this.instance=null;this.session=null;this.sequence=0n;
  }
  decode(bytes) {
    const event=decodeUnifiedEvent(bytes);
    if(event.backend!==this.backend||event.source!==this.source||event.sequence!==this.sequence+1n||
      (this.instance!==null&&(event.instance!==this.instance||event.session!==this.session))||
      (this.correlation&&event.correlation&&event.correlation!==this.correlation))throw new TypeError('Event source/session/sequence mismatch');
    this.instance=event.instance;this.session=event.session;this.sequence=event.sequence;return event;
  }
}
