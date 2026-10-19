/** Actual JS motion composition through a real Wasm callback import.
 * library_imports contains only the manifest-selected scalar Wasm trampoline.
 * It supplies no ROS, host JS engine, callback pointer, shared memory or grant.
 */
import { on_motion } from 'roswasm:library_imports';
export function step(owner, generation, requested, clearance) {
  if (!Number.isInteger(requested) || requested < -100 || requested > 100 ||
      !Number.isInteger(clearance) || clearance < 0 || clearance > 5000) return -2147483648 + 22;
  return on_motion(owner, generation, requested, clearance);
}
