/** Expected failure fixture, not an example that can pass.
 * The actual engine interrupt-poll budget must stop this guest loop with local
 * lifecycle LIMIT/exit3. The independent owner also enforces a hard deadline.
 */
import { write } from 'roswasm:guest_native';
export function main() {
  if (write(new TextEncoder().encode('JS_GUEST_LIMIT_ARMED\n')) !== 0) throw new Error('Arm marker');
  let value = 0; for (;;) value = (value + 1) >>> 0;
}
