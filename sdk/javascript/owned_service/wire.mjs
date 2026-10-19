/**
 * @file wire.mjs
 * @brief Exact value decoders for the optional retained ROS2 Service transport.
 *
 * This module reads UMRE256, UMRC160 and same-record URMI192 values. It performs
 * no import, grant, native discovery, callback dispatch, queue consumption or
 * cleanup. These associations are DATA from a separately authenticated real
 * Runtime invocation. A matching integer or a locally constructed packet never
 * proves Native identity, permission, SDK provenance or successful delivery.
 *
 * The owning endpoint keeps the returned snapshots private. Prepare a complete
 * decoded value before its ONE commit attempt. An associated metadata/codec
 * refusal can cancel the exact sequence+nonce; an unassociated header cannot
 * supply a guessed token. An uncertain commit must never be cancelled as a
 * rollback, retried or turned into a successful callback.
 */
import { Status, u32, u64 } from '../raw.mjs';
const typed = Object.getPrototypeOf(Uint8Array.prototype);
const byteLength = Object.getOwnPropertyDescriptor(typed, 'byteLength').get;
const byteOffset = Object.getOwnPropertyDescriptor(typed, 'byteOffset').get;
const arrayBuffer = Object.getOwnPropertyDescriptor(typed, 'buffer').get;
function length(value) { return byteLength.call(value); }
function offset(value) { return byteOffset.call(value); }
function buffer(value) { return arrayBuffer.call(value); }

export const ENDPOINT_BYTES = 256;
export const CONTROL_BYTES = 160;
export const METADATA_BYTES = 192;
export const DELIVERY_PREFIX_BYTES = 352;
export const MAXIMUM_PACKET_BYTES = 4194304;
export const MAXIMUM_WAIT_NS = 60000000000n;
export const U64_MAX = 0xffffffffffffffffn;
export const CLIENT = 3;
export const SERVICE = 4;

/** A validation result, never a claim about an attempted Host operation.
 * borrow is present only after every UMRC association/extent/freshness check.
 */
export class WireError extends Error {
  constructor(code, message, borrow = null) {
    super(message); this.name = 'OwnedServiceWireError';
    this.code = code; this.borrow = borrow;
  }
}
function need(value, message, code = Status.INVALID, borrow = null) {
  if (!value) throw new WireError(code, message, borrow);
}
function bytes(value) {
  need(value instanceof Uint8Array, 'An actual owned Uint8Array is required');
  return value;
}
function view(value) { return new DataView(buffer(value), offset(value), length(value)); }
function zero(value, first, count) {
  for (let i = first; i < first + count; ++i) if (value[i] !== 0) return false;
  return true;
}
function copy(value, first = 0, count = length(value) - first) {
  // A plain destination avoids a TypedArray subclass species constructor.
  const result = new Uint8Array(count);
  Uint8Array.prototype.set.call(result, new Uint8Array(buffer(value), offset(value) + first, count));
  return result;
}
export function fullHandle(value) {
  u64(value); return (value & 0xffffffffn) !== 0n && (value >> 32n) !== 0n;
}
function binding(d, expectedSource) {
  const source = d.getBigUint64(16, true), instance = d.getBigUint64(24, true);
  const generation = d.getBigUint64(32, true), session = d.getBigUint64(40, true);
  need(d.getUint8(7) === 2 && source === expectedSource && fullHandle(source) &&
    instance !== 0n && generation !== 0n && session !== 0n, 'Complete current ROS2 owner binding required');
  return Object.freeze({ source, instance, generation, session });
}
function sameOwner(d, owner) {
  return d.getUint8(7) === 2 && d.getBigUint64(16, true) === owner.source &&
    d.getBigUint64(24, true) === owner.instance && d.getBigUint64(32, true) === owner.generation &&
    d.getBigUint64(40, true) === owner.session;
}
function sameBytes(a, b) {
  if (length(a) !== length(b)) return false;
  for (let i = 0; i < length(a); ++i) if (a[i] !== b[i]) return false;
  return true;
}

/** Decode the protocol's explicit normalized QoS vocabulary, not native enums.
 * Requested/expected QoS is a separate caller assertion; these are the two
 * actual same-object profiles returned by the real Host, subject to matching.
 */
function qos(value, at) {
  const d = view(value), history = d.getUint32(at, true), reliability = d.getUint32(at + 4, true);
  const durability = d.getUint32(at + 8, true), liveliness = d.getUint32(at + 12, true);
  const depth = d.getBigUint64(at + 16, true);
  need(history >= 1 && history <= 2 && reliability >= 1 && reliability <= 2 &&
    durability >= 1 && durability <= 2 && liveliness >= 1 && liveliness <= 2 &&
    (history !== 1 || depth !== 0n) && value[at + 72] <= 1 && zero(value, at + 73, 7),
    'Complete concrete normalized actual QoS required');
  const duration = offset => {
    const seconds = d.getBigUint64(at + offset, true), nanoseconds = d.getBigUint64(at + offset + 8, true);
    need(nanoseconds < 1000000000n, 'Normalized protocol nanoseconds required');
    return Object.freeze({ seconds, nanoseconds });
  };
  return Object.freeze({ history, reliability, durability, liveliness, depth,
    deadline: duration(24), lifespan: duration(40), lease: duration(56),
    avoidRosNamespaceConventions: value[at + 72] !== 0 });
}

/** Validate/copy TWO independently selected concrete protocol QoS80 rows.
 * This pure preparation runs before activation. Expected values are assertions,
 * never an actual endpoint receipt, native type proof or permission grant.
 */
export function copyExpectedQos(value) {
  need(Array.isArray(value) && value.length === 2, 'Two explicit expected QoS rows required');
  const a = bytes(value[0]), b = bytes(value[1]);
  need(length(a) === 80 && length(b) === 80, 'Expected QoS rows each have80 bytes');
  const first = copy(a), second = copy(b);
  qos(first, 0); qos(second, 0); return Object.freeze([first, second]);
}

/** Decode exactly one actual activation/inspection receipt.
 * expectedQos, when supplied, is TWO separately reviewed protocol80-byte rows.
 * No native hash, type or writer GUID is invented from this receipt's GID.
 */
export function decodeEndpointReceipt(value, source, kind, operation = 1, expectedQos = null) {
  bytes(value); u64(source);
  need(length(value) === ENDPOINT_BYTES && (kind === CLIENT || kind === SERVICE) &&
    (operation === 1 || operation === 2), 'Exact Service UMRE256 role/operation required');
  const d = view(value);
  need(d.getUint32(0, true) === 0x45524d55 && d.getUint16(4, true) === 1 && value[6] === operation &&
    d.getUint32(8, true) === ENDPOINT_BYTES && value[12] === kind && value[13] === 2 &&
    value[14] <= 32 && value[15] === 0 && zero(value, 56, 8), 'Malformed complete UMRE256');
  const owner = binding(d, source), incarnation = d.getBigUint64(48, true), gidBytes = value[14];
  need(incarnation !== 0n && (kind !== SERVICE || gidBytes === 0) &&
    zero(value, 224 + gidBytes, 32 - gidBytes), 'Actual native incarnation and exact full GID extent required');
  const actualQos = Object.freeze([qos(value, 64), qos(value, 144)]);
  if (expectedQos !== null) {
    need(Array.isArray(expectedQos) && expectedQos.length === 2, 'Two explicit expected QoS rows required');
    for (let i = 0; i < 2; ++i) {
      const expected = bytes(expectedQos[i]); need(length(expected) === 80, 'Expected QoS row has80 bytes');
      qos(expected, 0);
      need(sameBytes(new Uint8Array(buffer(value), offset(value) + 64 + i * 80, 80), expected),
        'Actual same-object QoS differs from the independently selected expected profile');
    }
  }
  return Object.freeze({ owner, kind, incarnation, actualQos, gid: copy(value, 224, gidBytes), wire: copy(value) });
}

/** Inspection may change only the operation byte from activate1 to inspect2.
 * The caller separately admits both complete receipts before this comparison.
 */
export function sameEndpointReceipt(original, inspected) {
  need(original && inspected && original.wire instanceof Uint8Array && inspected.wire instanceof Uint8Array,
    'Actual private receipt snapshots required');
  const a = original.wire, b = inspected.wire;
  if (length(a) !== ENDPOINT_BYTES || length(b) !== ENDPOINT_BYTES || a[6] !== 1 || b[6] !== 2) return false;
  for (let i = 0; i < ENDPOINT_BYTES; ++i) if (i !== 6 && a[i] !== b[i]) return false;
  return true;
}

/** Decode one borrowed full packet from the SAME successful peek invocation.
 * Bytes after160+record_bytes are scratch capacity, never another record.
 * Sequence/nonce/ticket remain unsigned BigInt; native timestamps and request
 * sequence retain signed64, including negative/zero/all-one bit patterns.
 */
export function decodeDelivery(value, endpoint, peer, maximumBody, lastCommitted, lastReservation) {
  bytes(value); u64(peer); u64(lastCommitted); u64(lastReservation); u32(maximumBody);
  need(endpoint && (endpoint.kind === CLIENT || endpoint.kind === SERVICE) &&
    length(value) >= DELIVERY_PREFIX_BYTES && length(value) <= MAXIMUM_PACKET_BYTES &&
    maximumBody <= MAXIMUM_PACKET_BYTES - DELIVERY_PREFIX_BYTES && fullHandle(peer) &&
    peer !== endpoint.owner.source, 'Finite packet capacity and distinct complete peer required');
  const d = view(value), capacity = BigInt(length(value));
  need(d.getUint32(0, true) === 0x43524d55 && d.getUint16(4, true) === 1 && value[6] === 5 &&
    d.getUint32(8, true) === CONTROL_BYTES && value[12] === endpoint.kind && value[13] === 16 &&
    zero(value, 14, 2) && zero(value, 128, 32) && sameOwner(d, endpoint.owner) &&
    d.getBigUint64(48, true) === endpoint.incarnation && d.getBigUint64(56, true) === peer &&
    d.getBigUint64(88, true) === 0n && d.getBigUint64(96, true) === capacity &&
    d.getBigUint64(112, true) === 0n && d.getBigUint64(120, true) === 0n,
    'Unassociated or malformed complete UMRC160');
  const sequence = d.getBigUint64(72, true), reservation = d.getBigUint64(80, true);
  const ticket = d.getBigUint64(64, true), recordBytes = d.getBigUint64(104, true);
  need(recordBytes >= BigInt(METADATA_BYTES) && recordBytes <= capacity - BigInt(CONTROL_BYTES) &&
    recordBytes - BigInt(METADATA_BYTES) <= BigInt(maximumBody), 'Borrowed record exceeds its complete extent/quota', Status.TOO_LARGE);
  need(sequence !== 0n && reservation !== 0n && sequence > lastCommitted && reservation > lastReservation &&
    (endpoint.kind !== CLIENT || (lastCommitted !== U64_MAX && sequence === lastCommitted + 1n)),
    'Replayed/gapped sequence or reused borrow nonce', Status.DENIED);
  // Only this complete associated UMRC can supply the exact cancel/commit pair.
  const borrow = Object.freeze({ sequence, reservation, ticket });
  const bodyBytes = Number(recordBytes - BigInt(METADATA_BYTES)), at = CONTROL_BYTES;
  const m = new DataView(buffer(value), offset(value) + at, METADATA_BYTES);
  need(m.getUint32(0, true) === 0x494d5255 && m.getUint16(4, true) === 1 &&
    m.getUint32(8, true) === METADATA_BYTES && sameOwner(m, endpoint.owner) &&
    m.getBigUint64(48, true) === endpoint.incarnation && m.getBigUint64(56, true) === sequence &&
    m.getBigUint64(64, true) === ticket && m.getBigUint64(112, true) === BigInt(bodyBytes) &&
    value[at + 123] === 0 && zero(value, at + 176, 16), 'Unassociated complete same-take URMI192', Status.INVALID, borrow);
  const kind = value[at + 6], flags = m.getUint32(12, true), controlFlags = value[at + 122];
  const error = m.getUint32(124, true), gidBytes = value[at + 120], writerBytes = value[at + 121];
  const sourceTimestamp = m.getBigInt64(72, true), receivedTimestamp = m.getBigInt64(80, true);
  const nativeSequence = m.getBigInt64(104, true), publication = m.getBigUint64(88, true), reception = m.getBigUint64(96, true);
  const control = kind === 6 || kind === 7;
  if (control) {
    need(flags === 0 && bodyBytes === 0 && gidBytes === 0 && writerBytes === 0 &&
      zero(value, at + 128, 48) && sourceTimestamp === 0n && receivedTimestamp === 0n &&
      nativeSequence === 0n && publication === U64_MAX && reception === U64_MAX &&
      (controlFlags & ~3) === 0 && (kind === 6 ? error !== 0 : (controlFlags & 1) !== 0),
      'Control record must preserve its actual reason/flags without invented native metadata', Status.INVALID, borrow);
    return Object.freeze({ borrow, kind, control: true, controlFlags, error, body: new Uint8Array(), information: null });
  }
  need(kind === (endpoint.kind === CLIENT ? 3 : 2) && flags === 35 && ticket !== 0n &&
    error === 0 && controlFlags === 0 && gidBytes === 0 && writerBytes === 16 &&
    zero(value, at + 128, 32) && publication === U64_MAX && reception === U64_MAX,
    'Complete service request-ID/timestamp metadata required', writerBytes !== 16 ? Status.UNSUPPORTED : Status.INVALID, borrow);
  const information = Object.freeze({ sourceTimestamp, receivedTimestamp,
    requestId: Object.freeze({ sequence: nativeSequence, writer: copy(value, at + 160, 16) }) });
  return Object.freeze({ borrow, kind, control: false, controlFlags: 0, error: 0,
    body: copy(value, DELIVERY_PREFIX_BYTES, bodyBytes), information });
}
