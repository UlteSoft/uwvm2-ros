/** Actual f32/f64 imports into a separate floating-math Wasm library.
 * C checks Number-only input, creates real Wasm floating values, then checks
 * the provider's JS return before its typed scalar export returns. This root
 * checks rounding/Infinity/NaN/negative-zero and rejects implicit coercions.
 * The supervisor must separately inspect actual import/export signatures and
 * retain a same-operation independent IEEE numerical oracle. No NaN payload
 * bit identity or ROS/native performance equivalence is asserted.
 */
import { identity32, identity64, sum32, sum64 } from 'roswasm:library_imports';
import { check, marker } from './support.mjs';
export function main() {
  check(identity32(16777217) === 16777216, 'Real f32 demotion');
  check(identity64(16777217) === 16777217, 'Real f64 precision');
  for (const identity of [identity32, identity64]) {
    check(Object.is(identity(-0), -0), 'Signed zero');
    check(identity(Infinity) === Infinity && identity(-Infinity) === -Infinity, 'Infinity');
    check(Number.isNaN(identity(NaN)), 'NaN class preservation');
    for (const bad of [1n, true, '1', null, undefined, { valueOf() { return 1; } }]) {
      let refused = false; try { identity(bad); } catch { refused = true; }
      check(refused, 'No scalar Number coercion');
    }
  }
  check(sum32(0.5, 0.25) === 0.75 && sum64(0.5, 0.25) === 0.75, 'Two-argument scalar import');
  check(identity32(1e100) === Infinity, 'f32 overflow follows Wasm');
  marker('PASS_JS_GUEST_FLOATING_MODULES'); return 0;
}
