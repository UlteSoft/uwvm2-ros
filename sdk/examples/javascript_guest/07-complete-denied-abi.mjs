/** Lesson07: retain and enter every selected raw ABI import under default deny.
 * Valid copied-memory shapes carry an ungranted alias or zero opaque handles.
 * No native success, Runtime rights or ROS delivery is claimed. The independent
 * owner must verify no native effects and inspect every actual binary signature.
 */
import * as native from 'roswasm:guest_native';
import { specification } from '../../javascript/raw.mjs';
import { check, marker } from './support.mjs';
export function main() {
  const layout = native.describe(), spec = specification(layout.backend), seed = 0x8877665544332211n;
  for (let index = 0; index < spec.functions.length; ++index) {
    const entry = spec.functions[index], n = entry.parameters.length;
    let args, input = new Uint8Array(), snapshot = 0;
    if (entry.name === 'event_watch') args = [0n, 0n, 0n];
    else if (entry.name === 'event_ready') { args = [0n, 0, layout.output]; snapshot = 32; }
    else if (entry.name === 'service_submit') args = [0n, 0n, 0, layout.input, 0n, layout.slot];
    else if (!n) args = [];
    else if (n === 1) args = [0n];
    else if (n === 4 && entry.name === 'capability_open') {
      input = new TextEncoder().encode('ungranted'); args = [0, layout.input, BigInt(input.length), layout.slot]; snapshot = 64;
    } else if (n === 4) args = [0n, 0, layout.input, 0n];
    else if (n === 5) { args = [0n, 0, layout.output, 64n, layout.slot]; snapshot = 64; }
    else if (n === 7) { args = [0n, 0, layout.input, 0n, layout.output, 64n, layout.slot]; snapshot = 64; }
    else throw new Error('Unknown exact ABI form');
    const result = native.invoke(index, args, input, seed, snapshot);
    if (entry.name === 'abi_version') check(result.status === 1, 'ABI version');
    else if (entry.name === 'capability_open') check(result.status === 13 && result.slot === seed, 'Default alias denial');
    else check([9,13,22,95].includes(result.status), 'Unusable handle may never succeed');
    if (result.status !== 0) check(result.snapshot.every(byte => byte === 0xa5), 'Protected refusal snapshot');
  }
  marker('PASS_JS_GUEST_COMPLETE_DENIED_ABI'); return 0;
}
