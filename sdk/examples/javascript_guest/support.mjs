/** Shared guest-example assertions and bounded basic-stdio markers.
 * These execute inside the embedded Wasm engine. They start no host actor and
 * never interpret a successful publish acknowledgment as native receipt.
 */
import { writeGuestOutput, readGuestByte } from '../../javascript/guest.mjs';
export function check(value, message) { if (!value) throw new Error(message); }
export function marker(text) {
  check(typeof text === 'string' && text.length <= 512 && !text.includes('\n'), 'Bounded marker');
  check(writeGuestOutput(new TextEncoder().encode(text + '\n')) === 0, 'Basic output failed');
}
export function gate(expected) { check(readGuestByte() === expected.charCodeAt(0), 'Explicit supervisor gate'); }
export function status(value) {
  const result = typeof value === 'number' ? value : value?.status;
  check(Number.isInteger(result) && result === 0, 'Operation refused'); return value;
}
/** Cleanup records are application-owned result arrays, not native effects.
 * Every nonzero cleanup is retained as failure rather than retried.
 */
export function clean(value) {
  if (Array.isArray(value)) for (const item of value) clean(item);
  else if (typeof value === 'number') check(value === 0, 'Cleanup refused');
  else if (value && typeof value === 'object') {
    if ('status' in value) check(value.status === 0, 'Cleanup status');
    if ('results' in value) clean(value.results);
    if ('nodes' in value) clean(value.nodes);
  }
}
