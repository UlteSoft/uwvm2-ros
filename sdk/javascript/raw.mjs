/**
 * ABI v1 adapter for an explicitly supplied, authority-aware Wasm bridge.
 * This file executes JavaScript in an embedder. uwvm does not execute JavaScript
 * source, and this adapter does not implement ROS imports or native networking.
 * A production bridge must preserve the actual guest identity and uwvm policy.
 */
import { ABI } from './abi.mjs';

/** Runtime status codes. Code 11 covers both an empty poll and rate limiting. */
export const Status = Object.freeze({ SUCCESS: 0, UNKNOWN_MODULE: 2, BAD_HANDLE: 9,
  AGAIN: 11, DENIED: 13, CONFLICT: 17, INVALID: 22, TOO_LARGE: 90,
  UNSUPPORTED: 95, DISCONNECTED: 107, TIMEOUT: 110, REVOKED: 125 });

/** @internal SDK-local preparation metadata. These weak records describe a
 * JavaScript delegate attempt only; they are never Runtime identity or authority.
 * No caller-supplied boolean, errno, exception class or returned flag is a witness.
 */
const callPreparations = new WeakMap(), callEntries = new WeakMap();
const applyDelegate = Reflect.apply;
export function registerCallPreparation(callable, prepare) {
  if (typeof callable !== 'function' || typeof prepare !== 'function' || callPreparations.has(callable))
    throw new TypeError('One SDK preparation per callable required');
  callPreparations.set(callable, prepare); return callable;
}
/** @internal Complete local validation/callee capture before issuing a ticket. */
export function prepareCallEntry(callable, args, receiver = undefined) {
  const prepare = callPreparations.get(callable);
  if (!prepare) throw new TypeError('SDK prepared callable required for checked retirement');
  const run = prepare(receiver, args);
  if (typeof run !== 'function') throw new TypeError('SDK prepared invocation required');
  const ticket = Object.freeze({});
  callEntries.set(ticket, { run, started: false, attempted: false }); return ticket;
}
/** @internal A private ticket can be invoked at most once, even if it rejects. */
export function invokeCallEntry(ticket) {
  const entry = callEntries.get(ticket);
  if (!entry || entry.started) throw new TypeError('Fresh SDK invocation ticket required');
  entry.started = true; return entry.run(ticket);
}
/** @internal Only the record paired with the actual captured delegate is read. */
export function callEntryAttempted(ticket) {
  if (ticket === null) return false;
  const entry = callEntries.get(ticket);
  if (!entry) throw new TypeError('SDK invocation ticket required');
  return entry.attempted;
}
/** @internal The only entry commit: no application callback/property setter is
 * invoked between this private record write and the captured delegate attempt.
 * null is used by ordinary raw calls that have no checked owner to retire.
 */
export function delegateCallEntry(ticket, target, receiver, args) {
  if (typeof target !== 'function') throw new TypeError('Captured delegate required');
  // Capture every argument before entry. Reflect.apply receives only this fresh
  // dense literal array, so an accessor/Proxy on an input list cannot throw
  // between the private commit and the actual captured delegate attempt.
  let values;
  switch (args.length) {
    case 0: values=[];break;
    case 1: values=[args[0]];break;
    case 2: values=[args[0],args[1]];break;
    case 3: values=[args[0],args[1],args[2]];break;
    case 4: values=[args[0],args[1],args[2],args[3]];break;
    case 5: values=[args[0],args[1],args[2],args[3],args[4]];break;
    case 6: values=[args[0],args[1],args[2],args[3],args[4],args[5]];break;
    case 7: values=[args[0],args[1],args[2],args[3],args[4],args[5],args[6]];break;
    default: throw new TypeError('Closed SDK delegate arity required');
  }
  if (ticket !== null) {
    const entry = callEntries.get(ticket);
    if (!entry || !entry.started || entry.attempted) throw new TypeError('Unused started SDK invocation required');
    entry.attempted = true;
  }
  return applyDelegate(target, receiver, values);
}

/** Validate an unsigned i64 ABI value without a Number-to-BigInt conversion.
 * @param {bigint} value Handles and offsets must remain BigInt, including small values.
 * @returns {bigint} The unchanged value. Throws before invoking the bridge on invalid input.
 */
export function u64(value) {
  if (typeof value !== 'bigint' || value < 0n || value > 0xffffffffffffffffn)
    throw new TypeError('ABI i64 requires an unsigned 64-bit BigInt');
  return value;
}

/** Validate an unsigned i32 memory index or ABI result.
 * @param {number} value Integer in [0, 2^32-1].
 * @returns {number} The unchanged value; fractional or narrowed values are rejected.
 */
export function u32(value) {
  if (!Number.isInteger(value) || value < 0 || value > 0xffffffff)
    throw new TypeError('ABI i32 requires an unsigned 32-bit Number');
  return value;
}

/** Normalize a Wasm i32 result bit pattern to an unsigned ABI status.
 * JS Wasm functions may return signed i32 values such as -1 for 0xffffffff.
 * Fractional and out-of-range values are rejected before normalization.
 */
export function resultU32(value) { if (!Number.isInteger(value) || value < -2147483648 || value > 0xffffffff) throw new TypeError('Invalid Wasm i32 result'); return value >>> 0; }

/** Select only ROS 1 or ROS 2 ABI v1; never silently fall back to another version.
 * @param {number} version Exactly 1 or 2.
 * @returns {object} The import inventory. No native resource is created.
 */
export function specification(version) {
  if (version !== 1 && version !== 2) throw new RangeError('ROS version must be 1 or 2');
  return ABI[String(version)];
}

/** Inspect real WebAssembly imports against the ABI inventory.
 * This checks module/field names only: JS reflection does not expose function
 * signatures, so use the repository Wasm inspection checker for signature proof.
 * @param {WebAssembly.Module} module A compiled module, not a pathname.
 * @param {number} version Desired ROS version.
 * @returns {string[]} Recognized ROS function names; throws on unknown/version-mixed imports.
 */
export function inspectImports(module, version) {
  const spec = specification(version), names = new Set(spec.functions.map(f => f.name));
  const found = [];
  for (const entry of WebAssembly.Module.imports(module)) {
    if (!entry.module.startsWith('uwvm_ros')) continue;
    if (entry.module !== spec.module || entry.kind !== 'function' || !names.has(entry.name))
      throw new TypeError(`Unsupported ROS import ${entry.module}.${entry.name}`);
    found.push(entry.name);
  }
  return found;
}

/** Create a complete raw callable table from a trusted guest bridge.
 * @param {number} version Exactly 1 or 2.
 * @param {{memory: WebAssembly.Memory, functions: object}} bridge Explicit bridge
 * supplied by the authority-aware embedder. functions contains every ABI name.
 * Its wrappers must call the same guest's actual imports; an arbitrary host ROS
 * implementation is not equivalent and must not be used as sandbox evidence.
 * @returns {object} Validating methods with exact ABI names and immutable metadata.
 */
export function createRawAdapter(version, bridge) {
  const spec = specification(version);
  if (!bridge || !(bridge.memory instanceof WebAssembly.Memory) || !bridge.functions)
    throw new TypeError('Explicit Wasm memory and trusted guest bridge are required');
  const result = { version, module: spec.module, memory: bridge.memory };
  for (const entry of spec.functions) {
    const target = bridge.functions[entry.name];
    if (typeof target !== 'function') throw new TypeError(`Missing bridge function ${entry.name}`);
    const prepare = (_receiver, supplied) => {
      const args = Array.from(supplied);
      if (args.length !== entry.parameters.length) throw new TypeError(`${entry.name}: arity mismatch`);
      for (let i = 0; i < args.length; ++i)
        (entry.parameters[i].type === 'i64' ? u64 : u32)(args[i]);
      return ticket => resultU32(delegateCallEntry(ticket, target, undefined, args));
    };
    const callable = (...args) => prepare(undefined, args)(null);
    result[entry.name] = registerCallPreparation(callable, prepare);
  }
  return Object.freeze(result);
}

/** Convert a validated range only after checking the memory's actual byte length.
 * A new view is created on every call because memory.grow detaches earlier views.
 * @param {WebAssembly.Memory} memory Shared bridge memory.
 * @param {bigint} offset Unsigned ABI offset.
 * @param {bigint} length Unsigned ABI size, checked using subtraction (no overflow).
 * @returns {Uint8Array} Borrowed view. Do not retain it across a bridge invocation.
 */
export function memoryRange(memory, offset, length) {
  u64(offset); u64(length);
  const size = BigInt(memory.buffer.byteLength);
  if (offset > size || length > size - offset) throw new RangeError('Wasm memory range exceeds bounds');
  return new Uint8Array(memory.buffer, Number(offset), Number(length));
}

/** Read an opaque handle or length from exactly eight little-endian bytes.
 * @param {WebAssembly.Memory} memory The actual bridge memory.
 * @param {bigint} offset Output slot offset; alignment is not assumed.
 * @returns {bigint} The exact 64-bit value, even when above Number.MAX_SAFE_INTEGER.
 */
export function readU64(memory, offset) {
  const bytes = memoryRange(memory, offset, 8n);
  return new DataView(bytes.buffer, bytes.byteOffset, 8).getBigUint64(0, true);
}
