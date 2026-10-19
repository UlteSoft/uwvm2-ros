/** Persistent actual JS-in-Wasm benchmark with the existing finite session ABI.
 * Initialize providers once, emit READY\n, then accept at most256 W/P/Q bytes.
 * W and P reset the same deterministic batch and emit exactly8 checksum bytes;
 * Q exits0. The external owner supplies deadlines and wall/CPU measurements.
 * This protocol separates engine/process cold startup from a retained instance.
 */
import { initialize, batch } from './benchmark_guest.mjs';
import { writeGuestOutput, readGuestByte } from '../../javascript/guest.mjs';
function write(bytes) { if (writeGuestOutput(bytes) !== 0) throw new Error('Exact session output'); }
export function main() {
  initialize(); write(new TextEncoder().encode('READY\n'));
  for (let command = 0; command < 256; ++command) {
    const value = readGuestByte();
    if (value === 81) return 0;
    if (value !== 87 && value !== 80) throw new Error('Closed benchmark command');
    write(batch());
  }
  throw new Error('Finite session command budget exhausted');
}
