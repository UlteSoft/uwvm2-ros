/** Actual embedded-JavaScript vision provider for the existing scalar Wasm ABI.
 * Four supplied synthetic millimetre samples are already measured inputs. This
 * is not image perception. The pure-library build has no ROS or WASI imports.
 */
const INVALID = -2147483648 + 22;
export function initialize() { return 0; }
export function clearance(a, b, c, d) {
  if ([a,b,c,d].some(value => !Number.isInteger(value) || value < 0 || value > 5000)) return INVALID;
  return Math.min(a,b,c,d);
}
