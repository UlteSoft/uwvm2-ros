/** Floating scalar Wasm provider: these functions run in this library's own
 * guest JS engine. The recipe exports exact f32/f64 C trampolines, not boxed
 * host objects or pointers. Real Wasm permits NaN, Infinity and signed zero;
 * that core scalar rule does not relax finite fields in any ROS message codec.
 * This is numerical boundary coverage, not a robotics control or speed claim.
 */
export function identity32(value) { return value; }
export function identity64(value) { return value; }
export function sum32(a, b) { return a + b; }
export function sum64(a, b) { return a + b; }
