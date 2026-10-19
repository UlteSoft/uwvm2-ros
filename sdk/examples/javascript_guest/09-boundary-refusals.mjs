/** Actual guest negative cases: strict native scalar/buffer checks and a closed
 * ESM loader. These are caught local input refusals before a ROS import, not
 * fabricated Runtime errno. A final genuine default-denied call still reaches
 * the Runtime. The owner separately verifies no native effects.
 */
import * as native from 'roswasm:guest_native';
import { createAsyncGuestSession } from '../../javascript/guest.mjs';
import { check, marker } from './support.mjs';
function refused(operation) { let rejected = false; try { operation(); } catch { rejected = true; } check(rejected, 'Malformed native input must throw'); }
export async function main() {
  const layout = native.describe(), bytes = new TextEncoder().encode('ungranted'), seed = 0x8877665544332211n;
  const args = [0,layout.input,BigInt(bytes.length),layout.slot];
  for (const index of [-1,0.5,NaN,Infinity,4294967296]) refused(() => native.invoke(index,args,bytes,seed,64));
  for (const handle of [-1n,1n<<64n,1n<<128n,1,false,'1']) refused(() => native.invoke(2,[handle],new Uint8Array(),seed,0));
  refused(() => native.invoke(1,[1,layout.input,BigInt(bytes.length),layout.slot],bytes,seed,64));
  refused(() => native.invoke(1,[0,layout.input+1n,BigInt(bytes.length),layout.slot],bytes,seed,64));
  refused(() => native.invoke(1,args,new Uint16Array(4),seed,64));
  refused(() => native.invoke(1,args,{buffer:new ArrayBuffer(8)},seed,64));
  refused(() => native.invoke(1,args,bytes,Number(seed),64));
  check(typeof process === 'undefined' && typeof require === 'undefined' && typeof WebAssembly === 'undefined', 'No external Node/WebAssembly host globals');
  for (const name of ['node:fs','std','os','../../../../etc/passwd']) {
    let rejected = false; try { await import(name); } catch { rejected = true; }
    check(rejected, 'Closed module loader refusal');
  }
  const result = native.invoke(1,args,bytes,seed,64);
  check(result.status === 13 && result.slot === seed && result.snapshot.every(byte => byte === 0xa5), 'Actual denial survives malformed inputs');
  // Also exercise the shared async Reply diagnostic shape with a real denied
  // import. Its slotBytes are owned little-endian bytes of the C-returned slot;
  // this must not depend on an external relay or fabricated memory object.
  const session = createAsyncGuestSession();
  const reply = await session.raw('capability_open', args, { input: bytes, slotSeed: seed, snapshot: 64 });
  const diagnostic = session.result(reply, 64, 'capability_open');
  check(diagnostic.status === 13 && diagnostic.required === null && diagnostic.bytes.length === 0 &&
    diagnostic.slotBytes.length === 8 && new DataView(diagnostic.slotBytes.buffer).getBigUint64(0,true) === seed,
    'Actual copied async refusal and owned slot diagnostics');
  await session.shutdown();
  marker('PASS_JS_GUEST_BOUNDARY_REFUSALS'); return 0;
}
