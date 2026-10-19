/** Lesson 00: exact raw imports through the same real Wasm's copied C binding.
 * Compile this ESM and the pinned engine into a command with recipes.json.
 * Use no ROS grants. ABI version is public; an ungranted alias must return13
 * and preserve the actual eight-byte slot and64 output sentinel bytes.
 */
import * as native from 'roswasm:guest_native';
import { check, marker } from './support.mjs';
export function main() {
  const layout = native.describe(), seed = 0x8877665544332211n;
  const version = native.invoke(0, [], new Uint8Array(), seed, 0);
  check(version.status === 1 && version.slot === seed, 'Actual ABI version/slot');
  const alias = new TextEncoder().encode('ungranted');
  const result = native.invoke(1, [0, layout.input, BigInt(alias.length), layout.slot], alias, seed, 64);
  check(result.status === 13 && result.slot === seed && result.snapshot.length === 64 &&
    result.snapshot.every(byte => byte === 0xa5), 'Actual default denial and protected output');
  marker('PASS_JS_GUEST_DEFAULT_DENIED'); return 0;
}
