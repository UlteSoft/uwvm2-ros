/**
 * @file bridge.mjs
 * @brief Explicit copied adapter for the optional uwvm_ros2_rmw_srv_v1 imports.
 *
 * The production binding is the opt-in builtin `roswasm:rmw_service_native`
 * compiled into the SAME QuickJS Wasm principal. It exports only synchronous
 * invoke(index, scalarArgs, inputUint8Array, outputUint8Array) -> unsigned i32.
 * C owns memory zero and every pointer. JavaScript never chooses an address.
 * An injected JavaScript binding is useful for a MODEL, never Native evidence.
 * Neither importing this file nor possessing a matching packet grants rights.
 */
import { u32, u64, resultU32 } from '../raw.mjs';
import { DELIVERY_PREFIX_BYTES, MAXIMUM_PACKET_BYTES } from './wire.mjs';

const bindings = new WeakMap(), preparations = new WeakMap();
const apply = Reflect.apply;
const typedLength = Object.getOwnPropertyDescriptor(Object.getPrototypeOf(Uint8Array.prototype), 'length').get;
const empty = new Uint8Array();
const arities = Object.freeze([0, 2, 2, 2, 2, 3, 4, 3, 4, 4, 4, 2]);
function need(value, message) { if (!value) throw new TypeError(message); }
function length(value) {
  need(value instanceof Uint8Array, 'Actual Uint8Array transfer required');
  return typedLength.call(value);
}

/** Capture a real builtin's single invoke delegate once, before any import.
 * No describe() or automatic fallback is used. ABI version is checked explicitly
 * by endpoint.activate(), after the endpoint has retained its cleanup ledger.
 */
export class OwnedServiceBridge {
  constructor(binding) {
    const invoke = binding?.invoke;
    need(typeof invoke === 'function', 'Explicit optional owned-Service builtin required');
    bindings.set(this, { binding, invoke, busy: false });
    Object.freeze(this);
  }
}

/** @internal Prepare all local validation and dense argument storage FIRST.
 * The private attempt bit changes immediately before the captured delegate.
 * A thrown delegate, invalid result or Host trap remains an attempted effect.
 * Each preparation is single-use; output is owned by the calling endpoint.
 * C independently validates these same forms and preserves output on nonzero.
 */
export function prepareServiceCall(bridge, index, supplied, input = empty, output = empty) {
  const binding = bindings.get(bridge);
  need(binding && !binding.busy, 'Live nonreentrant owned-Service bridge required');
  u32(index); need(index < arities.length && Array.isArray(supplied) && supplied.length === arities[index], 'Exact raw12 index/arity required');
  const args = new Array(supplied.length);
  for (let i = 0; i < args.length; ++i) {
    const value = supplied[i];
    args[i] = index === 7 && i === 2 ? u32(value) : u64(value);
  }
  const n = length(input), m = length(output);
  need(n <= MAXIMUM_PACKET_BYTES - DELIVERY_PREFIX_BYTES, 'Canonical input exceeds explicit transport maximum');
  need((index === 4 || index === 6) || n === 0, 'Only submit/respond have canonical input');
  const expected = index === 1 || index === 2 || index === 3 ? 256 : index === 4 ? 8 : index === 10 ? 1 : 0;
  need(index === 7 ? m >= DELIVERY_PREFIX_BYTES && m <= MAXIMUM_PACKET_BYTES : m === expected, 'Exact raw12 output form required');
  if (index === 7) need(args[2] === 16, 'Service request writer GUID width must be16');
  const callArgs = [index, args, input, output], token = Object.freeze({});
  preparations.set(token, { binding, callArgs, started: false, attempted: false });
  return token;
}

/** @internal Invoke once; return ABI version1 at index0, otherwise a status.
 * No result object or BigInt is allocated after the delegate returns. The C
 * builtin similarly returns JS_NewUint32 and copies output only on status0.
 */
export function invokeServiceCall(token) {
  const entry = preparations.get(token);
  need(entry && !entry.started && !entry.binding.busy, 'Fresh prepared owned-Service call required');
  entry.started = true; entry.binding.busy = true;
  try {
    entry.attempted = true;
    return resultU32(apply(entry.binding.invoke, entry.binding.binding, entry.callArgs));
  } finally { entry.binding.busy = false; }
}

/** @internal This is local delegate-entry evidence only, never a Host receipt. */
export function serviceCallAttempted(token) {
  const entry = preparations.get(token);
  need(entry, 'Actual prepared call token required'); return entry.attempted;
}
