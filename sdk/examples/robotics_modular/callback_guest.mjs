/** Actual guest JS callback registry, owned by this Wasm engine/instance.
 * Owner and generation are ordinary application protocol labels, not an
 * authenticated Runtime identity or authority grant. No native function pointer
 * or JS object crosses a Wasm memory. Generations never wrap.
 */
const OWNER = 0x4d4f544e, INVALID = -2147483648 + 22, STALE = -2147483648 + 9;
let generation = 0, active = false, busy = false, calls = 0;
export function begin(owner) {
  if (owner !== OWNER || active || busy || generation === 0xffffffff) return 0;
  ++generation; active = true; calls = 0; return generation;
}
export function end(owner, value) {
  if (busy) return 17;
  if (!active || owner !== OWNER || value !== generation) return 9;
  active = false; return 0;
}
export function count(owner, value) { return active && owner === OWNER && value === generation ? calls : 0xffffffff; }
export function denials() { return 0; }
export function onMotion(owner, value, requested, clearance) {
  if (busy) return -2147483648 + 17;
  if (!active || owner !== OWNER || value !== generation) return STALE;
  if (!Number.isInteger(requested) || requested < -100 || requested > 100 ||
      !Number.isInteger(clearance) || clearance < 0 || clearance > 5000 || calls === 0xffffffff) return INVALID;
  busy = true;
  try { ++calls; return clearance < 400 ? 0 : requested; }
  finally { busy = false; }
}
