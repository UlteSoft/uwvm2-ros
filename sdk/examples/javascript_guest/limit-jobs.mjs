/** Expected failure fixture: a Promise chain that never reaches quiescence.
 * Only the engine's explicit finite job pump advances it. No Node timer/thread
 * or hidden guest clock is involved. The configured total-job limit must yield
 * lifecycle LIMIT/exit3, independently bounded by the owner's deadline.
 */
import { write } from 'roswasm:guest_native';
export function main() {
  if (write(new TextEncoder().encode('JS_GUEST_LIMIT_ARMED\n')) !== 0) throw new Error('Arm marker');
  const again = () => Promise.resolve().then(again);
  return again();
}
