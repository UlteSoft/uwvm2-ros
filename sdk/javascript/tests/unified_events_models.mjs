/** Real SDK copied event/binding contracts. No bridge, Guest, ROS or actor is
 * supplied. Run only in the parent's serial bounded model stage. All opaque
 * identities remain BigInt; successful decoding does not grant authority.
 */
import assert from 'node:assert/strict';
import {decodeUnifiedEvent,decodeEventReadiness,EventBinding} from '../events.mjs';
let checks=0;
const require=(value)=>{assert.ok(value);checks++;};
const refused=(fn)=>{assert.throws(fn);checks++;};
function record(kind){const bytes=new Uint8Array([2,4,5].includes(kind)?72:64),v=new DataView(bytes.buffer);v.setUint32(0,0x45525755,true);v.setUint16(4,1,true);v.setUint16(6,kind,true);bytes[8]=2;v.setBigUint64(16,0xf000000000000011n,true);v.setBigUint64(32,1n,true);v.setBigUint64(40,0x20000000000001n,true);v.setBigUint64(48,0xe000000000000021n,true);if(kind>=2&&kind<=5)v.setBigUint64(24,17n,true);if(bytes.length===72){v.setBigUint64(56,8n,true);v.setBigUint64(64,17n,true);}if(kind===6){bytes[9]=2;v.setUint32(12,5,true);}if(kind===7)bytes[9]=1;return bytes;}
for(let kind=1;kind<=7;kind++){const input=record(kind),event=decodeUnifiedEvent(input);require(event.kind===kind&&event.backend===2);require(event.source===0xf000000000000011n&&event.instance===0x20000000000001n&&event.session===0xe000000000000021n);require(event.body.length===input.length-64);}
let input=record(6);input[9]=4;refused(()=>decodeUnifiedEvent(input));input=record(2);input[64]=18;refused(()=>decodeUnifiedEvent(input));
for(const at of [4,6,10,56]){input=record(1);input[at]=at===6?8:2;refused(()=>decodeUnifiedEvent(input));}
for(const at of [16,32]){input=record(1);new DataView(input.buffer).setBigUint64(at,0n,true);refused(()=>decodeUnifiedEvent(input));}
input=record(2);const copied=decodeUnifiedEvent(input);input[64]=18;require(copied.body[0]===17);
const readiness=new Uint8Array(32),v=new DataView(readiness.buffer);[0x20000000000001n,69n,2n,0xffffffffffffffffn].forEach((value,i)=>v.setBigUint64(i*8,value,true));const ready=decodeEventReadiness(readiness);require(ready.readyEvents===0x20000000000001n&&ready.nextEventBytes===69n&&ready.admissionSlots===2n&&ready.admissionBytes===0xffffffffffffffffn);refused(()=>decodeEventReadiness(readiness.slice(0,31)));refused(()=>decodeEventReadiness(new Uint8Array(33)));
const binding=new EventBinding(2,0xf000000000000011n);input=record(1);require(binding.decode(input).sequence===1n);refused(()=>binding.decode(input));const next=record(7);new DataView(next.buffer).setBigUint64(32,2n,true);require(binding.decode(next).kind===7);const foreign=record(7);new DataView(foreign.buffer).setBigUint64(32,3n,true);new DataView(foreign.buffer).setBigUint64(48,99n,true);refused(()=>binding.decode(foreign));require(binding.sequence===2n&&binding.session===0xe000000000000021n);refused(()=>new EventBinding(2,Number(0xf000000000000011n)));
assert.equal(checks,39);process.stdout.write('PASS JavaScript unified event models: '+checks+' checks\n');
