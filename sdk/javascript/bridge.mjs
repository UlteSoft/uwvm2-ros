/**
 * Adapt the concrete bridge.c module's exports to the JavaScript SDK.
 * This file supplies no ROS host functions, credentials, sockets or authority.
 * A real embedding must instantiate the bridge with authenticated Runtime imports.
 * Inspect its binary with tools/check_wasm_imports.py before instantiation.
 */
import { specification } from './raw.mjs';

/** Convert exports to a complete ABI-shaped bridge and its reserved scratch.
 * @param {number} version Exactly1 or2, matching the C compile-time backend.
 * @param {WebAssembly.Instance} instance Already instantiated by a trusted embedding.
 * @returns {{bridge: object, scratch: object}} Inputs for new Session(version, bridge, scratch).
 * Export reflection checks names, not binary signatures or authenticated caller identity.
 * The reserved C array is exclusively owned by one serialized Session. Do not
 * create another Session over it or let an unrelated caller use it concurrently.
 */
export function fromGuestBridge(version, instance) {
  const spec = specification(version);
  if (!(instance instanceof WebAssembly.Instance)) throw new TypeError('Wasm bridge instance required');
  const exports = instance.exports;
  if (!(exports.memory instanceof WebAssembly.Memory)) throw new TypeError('Bridge must export its memory');
  const functions = {};
  for (const entry of spec.functions) {
    if (typeof exports[entry.name] !== 'function') throw new TypeError(`Missing guest bridge export ${entry.name}`);
    functions[entry.name] = exports[entry.name];
  }
  for (const name of ['roswasm_scratch_offset', 'roswasm_input_capacity', 'roswasm_output_capacity'])
    if (typeof exports[name] !== 'function') throw new TypeError(`Missing scratch export ${name}`);
  // A Wasm i64 export is represented by signed BigInt in JavaScript. Normalize
  // its bits without ever passing through Number, including wasm64 addresses.
  const offset = BigInt.asUintN(64, exports.roswasm_scratch_offset());
  const inputCapacity = exports.roswasm_input_capacity() >>> 0;
  const outputCapacity = exports.roswasm_output_capacity() >>> 0;
  return { bridge: { memory: exports.memory, functions },
           scratch: { offset, inputCapacity, outputCapacity } };
}
