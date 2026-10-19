/** Bounded actual JS-in-Wasm modular benchmark root.
 * The input sequence and checksum match robotics_math.h. Actual separate
 * vision/motion/callback Wasm imports execute every iteration. This measures
 * engine/import/lifetime overhead in addition to fused math; it is not native
 * ROS, image perception or Unitree dynamics evidence. No Guest clock is used.
 */
import { vision_initialize, vision_clearance_mm, callback_begin, callback_end,
  callback_count, motion_step } from 'roswasm:library_imports';
import { writeGuestOutput } from '../../javascript/guest.mjs';
const OWNER = 0x4d4f544e, ITERATIONS = 10000;
export function initialize() {
  if (vision_initialize() !== 0) throw new Error('Vision initialization');
}
export function batch() {
  const generation = callback_begin(OWNER);
  if (!generation) throw new Error('Callback registration');
  let state = 1, checksum = 14695981039346656037n;
  const next = () => { state = (Math.imul(state, 1664525) + 1013904223) >>> 0; return state; };
  for (let iteration = 0; iteration < ITERATIONS; ++iteration) {
    const a = next() % 5001, b = next() % 5001, c = next() % 5001, d = next() % 5001;
    const requested = next() % 201 - 100;
    const clearance = vision_clearance_mm(a,b,c,d), command = motion_step(OWNER,generation,requested,clearance);
    if (command < -100 || command > 100) throw new Error('Scalar callback refusal');
    checksum = BigInt.asUintN(64, (checksum ^ BigInt(command >>> 0)) * 1099511628211n);
  }
  if (callback_count(OWNER,generation) !== ITERATIONS || callback_end(OWNER,generation) !== 0)
    throw new Error('Checked callback retirement');
  const bytes = new Uint8Array(8); new DataView(bytes.buffer).setBigUint64(0,checksum,true);
  return bytes;
}
export function main() {
  initialize();
  if (writeGuestOutput(batch()) !== 0) throw new Error('Exact binary checksum output');
  return 0;
}
