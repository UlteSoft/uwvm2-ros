/** Expected failure fixture. Keep allocations rooted so collection cannot make
 * an unbounded loop appear harmless. The configured counted heap must refuse
 * well before256 MiB; actual owner RSS/cgroup records remain independent proof.
 * An allocation refusal poisons this engine and exits with lifecycle LIMIT3.
 */
import { write } from 'roswasm:guest_native';
export function main() {
  if (write(new TextEncoder().encode('JS_GUEST_LIMIT_ARMED\n')) !== 0) throw new Error('Arm marker');
  const buffers = [];
  for (let index = 0; index < 256; ++index) buffers.push(new Uint8Array(1048576));
  throw new Error('Selected heap limit was not enforced');
}
