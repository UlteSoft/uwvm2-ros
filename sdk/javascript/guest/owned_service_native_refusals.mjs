/** Actual embedded-engine fixture. Source only until run by the Linux owner.
 * This exercises the real C builtin, not a JavaScript imitation of its checks.
 * It makes only ABI-version imports: each invalid operation must be refused
 * before its business import. It does not test native ROS delivery or grants.
 */
import {invoke} from 'roswasm:rmw_service_native';

const empty = new Uint8Array(0);
let checked = 0;
function assert(value, label) { if (!value) throw new Error(label); }
function refused(body, label) {
  let caught = false;
  try { body(); } catch (_) { caught = true; }
  assert(caught, 'expected refusal: ' + label);
  // A local validation exception must release the guard for a later call.
  assert(invoke(0, [], empty, empty) === 1, 'guard recovery: ' + label);
  ++checked;
}

export function runRefusals() {
  checked = 0;
  assert(invoke(0, [], empty, empty) === 1, 'actual optional ABI version');
  refused(() => invoke(12, [], empty, empty), 'unknown index');
  refused(() => invoke(0n, [], empty, empty), 'index is Number');
  refused(() => invoke(0, {}, empty, empty), 'actual Array');
  refused(() => invoke(0, new Proxy([], {}), empty, empty), 'Array proxy');
  refused(() => invoke(1, [1, 2n], empty, new Uint8Array(256)), 'u64 Number');
  refused(() => invoke(1, [-1n, 2n], empty, new Uint8Array(256)), 'negative u64');
  refused(() => invoke(1, [1n << 64n, 2n], empty, new Uint8Array(256)), 'overwide u64');
  refused(() => invoke(7, [1n, 2n, 3n], empty, new Uint8Array(352)), 'u32 BigInt');
  refused(() => invoke(7, [1n, 2n, 0.5], empty, new Uint8Array(352)), 'fractional u32');
  refused(() => invoke(7, [1n, 2n, 4294967296], empty, new Uint8Array(352)), 'overwide u32');
  refused(() => invoke(1, [1n, 2n], empty, new Uint8Array(255)), 'short receipt');
  refused(() => invoke(1, [1n, 2n], empty, new Uint8Array(257)), 'long receipt');
  refused(() => invoke(4, [1n, 2n], empty, new Uint8Array(7)), 'short ticket');
  refused(() => invoke(7, [1n, 2n, 0], empty, new Uint8Array(351)), 'short delivery');
  refused(() => invoke(10, [1n, 2n, 3n, 0n], empty, new Uint8Array(2)), 'long availability');
  refused(() => invoke(5, [1n, 2n, 3n], new Uint8Array(1), empty), 'unexpected input');
  refused(() => invoke(0, [], new Uint8ClampedArray(0), empty), 'clamped input');
  refused(() => invoke(0, [], empty, new Int8Array(0)), 'signed output');
  refused(() => invoke(0, [], empty, new Proxy(empty, {})), 'output proxy');

  let finalRead = false;
  const full64 = [(1n << 64n) - 1n, 1n << 63n, 0n];
  Object.defineProperty(full64, '2', {get() { finalRead = true; return (1n << 64n) - 1n; }});
  // The final getter is reached only after both high-bit u64 values converted.
  // Deliberately invalid byte shape still prevents the actual abandon import.
  refused(() => invoke(5, full64, new Uint8Array(1), empty), 'full u64 followed by bad shape');
  assert(finalRead, 'full unsigned high-bit conversion');

  let nestedRefused = false;
  const reentrant = [1n, 2n];
  Object.defineProperty(reentrant, '0', {get() {
    try { invoke(0, [], empty, empty); } catch (_) { nestedRefused = true; }
    return 1n;
  }});
  refused(() => invoke(1, reentrant, empty, empty), 'getter then bad receipt');
  assert(nestedRefused, 'guard set before argument getter');
  const detachedInput = new Uint8Array(0);
  detachedInput.buffer.transfer(0);
  refused(() => invoke(0, [], detachedInput, empty), 'detached empty input');
  const detachedOutput = new Uint8Array(0);
  detachedOutput.buffer.transfer(0);
  refused(() => invoke(0, [], empty, detachedOutput), 'detached empty output');

  // These zero-to-nonzero tracking cases expose the original pinned C API
  // bug directly: stale initial length0 would wrongly admit ABI discovery.
  const grownInputBuffer = new ArrayBuffer(0, {maxByteLength: 8});
  const grownInput = new Uint8Array(grownInputBuffer);
  grownInputBuffer.resize(8);
  assert(grownInput.byteLength === 8, 'actual grown input view extent');
  refused(() => invoke(0, [], grownInput, empty), 'current grown input extent');
  const grownOutputBuffer = new ArrayBuffer(0, {maxByteLength: 8});
  const grownOutput = new Uint8Array(grownOutputBuffer);
  grownOutputBuffer.resize(8);
  assert(grownOutput.byteLength === 8, 'actual grown output view extent');
  refused(() => invoke(0, [], empty, grownOutput), 'current grown output extent');

  // Shrinking a tracking view to empty must remain a valid empty byte input.
  const shrunkBuffer = new ArrayBuffer(8, {maxByteLength: 8});
  const shrunk = new Uint8Array(shrunkBuffer);
  shrunkBuffer.resize(0);
  assert(invoke(0, [], shrunk, empty) === 1, 'current empty tracking input');
  assert(invoke(0, [], empty, shrunk) === 1, 'current empty tracking output');
  const fixedBuffer = new ArrayBuffer(0, {maxByteLength: 8});
  const fixed = new Uint8Array(fixedBuffer, 0, 0);
  fixedBuffer.resize(8);
  assert(invoke(0, [], fixed, fixed) === 1, 'fixed length view stays empty');
  const outOfBoundsBuffer = new ArrayBuffer(8, {maxByteLength: 8});
  const outOfBounds = new Uint8Array(outOfBoundsBuffer, 4, 4);
  outOfBoundsBuffer.resize(0);
  refused(() => invoke(0, [], outOfBounds, empty), 'out of bounds input');
  refused(() => invoke(0, [], empty, outOfBounds), 'out of bounds output');
  return checked;
}

/** Separate actual-Host denial case: run only in an owner-created principal
 * with no Service grants. Exact errno is retained, never mapped to readiness.
 * It cannot be counted as a successful native Service or wire test.
 */
export function deniedActivation() {
  const output = new Uint8Array(256); output.fill(0x3c);
  const status = invoke(1, [0n, 0n], empty, output);
  assert(Number.isInteger(status) && status > 0 && status <= 4294967295,
    'known invalid zero owners must be refused by actual Host');
  for (const byte of output) assert(byte === 0x3c, 'nonzero status preserves caller output');
  return status;
}
