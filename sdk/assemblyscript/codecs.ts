/** Canonical ABI v1 bytes, independent of ROS CDR/TCPROS wire encoding.
 * All output allocations are bounded. Decoders carry a valid flag rather than
 * exposing truncated data. Managed strings are UTF-16 internally: never pass a
 * string object address to an import expecting UTF-8 payload bytes.
 */
export const MAX_WIRE: i32 = 4193786;
export const MAX_ELEMENTS: u32 = 1048576;

/** Reject unpaired UTF-16 surrogates before UTF-8 encoding; do not silently replace user text. */
export function validUnicode(value: string): bool { for (let i = 0; i < value.length; ++i) { const c = value.charCodeAt(i); if (c >= 0xd800 && c <= 0xdbff) { if (++i >= value.length) return false; const next = value.charCodeAt(i); if (next < 0xdc00 || next > 0xdfff) return false; } else if (c >= 0xdc00 && c <= 0xdfff) return false; } return true; }

/** Strict UTF-8 validation with optional embedded NUL rejection. */
export function validUtf8(bytes: Uint8Array, allowNul: bool = true): bool {
  let i = 0;
  while (i < bytes.length) {
    let lead = bytes[i++];
    if (lead < 128) { if (!lead && !allowNul) return false; continue; }
    let count = 0, code: u32 = 0, minimum: u32 = 0;
    if (lead >= 0xc2 && lead <= 0xdf) { count = 1; code = lead & 31; minimum = 128; }
    else if (lead >= 0xe0 && lead <= 0xef) { count = 2; code = lead & 15; minimum = 2048; }
    else if (lead >= 0xf0 && lead <= 0xf4) { count = 3; code = lead & 7; minimum = 65536; }
    else return false;
    if (count > bytes.length - i) return false;
    while (count--) { let b = bytes[i++]; if ((b & 0xc0) != 0x80) return false; code = (code << 6) | (b & 63); }
    if (code < minimum || code > 0x10ffff || (code >= 0xd800 && code <= 0xdfff)) return false;
  }
  return true;
}

/** Bounded sequential little-endian writer. Invalid writes set valid=false. */
export class Writer {
  private values: Array<u8> = new Array<u8>();
  /** Inspect before finish(); an invalid writer returns an empty array. */
  valid: bool = true;
  /** @param maximum Maximum final byte count, including envelopes and length prefixes. */
  constructor(private maximum: i32 = MAX_WIRE) { if (maximum < 0 || maximum > MAX_WIRE) this.valid = false; }
  /** Append one byte after verifying remaining allocation space. */
  u8(value: u8): Writer { if (this.values.length >= this.maximum) this.valid = false; if (this.valid) this.values.push(value); return this; }
  /** Append width low bytes from a u64, little-endian. */
  integer(value: u64, width: i32): Writer { if (width < 0 || width > 8 || width > this.maximum - this.values.length) { this.valid = false; return this; } for (let i = 0; i < width; ++i) this.u8(<u8>(value >> (i * 8))); return this; }
  /** Append u32 in four bytes. */
  u32(value: u32): Writer { return this.integer(value, 4); }
  /** Append a u64 handle/length/goal ID without narrowing. */
  u64(value: u64): Writer { return this.integer(value, 8); }
  /** Append i32 preserving two's-complement bits. */
  i32(value: i32): Writer { return this.u32(<u32>value); }
  /** Append i64 preserving two's-complement bits. */
  i64(value: i64): Writer { return this.u64(<u64>value); }
  /** Append finite IEEE-754 float64. */
  f64(value: f64): Writer { if (!isFinite(value)) this.valid = false; return this.u64(reinterpret<u64>(value)); }
  /** Copy a bounded byte array. */
  bytes(value: Uint8Array): Writer { if (value.length > this.maximum - this.values.length) { this.valid = false; return this; } for (let i = 0; i < value.length; ++i) this.u8(value[i]); return this; }
  /** Encode UTF-8 with a four-byte byte count. Strings are not NUL terminated. */
  text(value: string): Writer { if (!this.valid || !validUnicode(value)) { this.valid = false; return this; } const length = String.UTF8.byteLength(value); if (this.maximum - this.values.length < 4 || length > this.maximum - this.values.length - 4) { this.valid = false; return this; } const b = Uint8Array.wrap(String.UTF8.encode(value)); if (!validUtf8(b)) { this.valid = false; return this; } return this.u32(<u32>b.length).bytes(b); }
  /** Encode a length-prefixed raw byte blob. */
  blob(value: Uint8Array): Writer { return this.u32(<u32>value.length).bytes(value); }
  /** Return one owned immutable-by-convention copy; no native I/O occurs. */
  finish(): Uint8Array { if (!this.valid) return new Uint8Array(0); const out = new Uint8Array(this.values.length); for (let i = 0; i < out.length; ++i) out[i] = this.values[i]; return out; }
}

/** Strict reader over copied canonical bytes. Call done() before using decoded fields. */
export class Reader {
  /** Current position is public for bounded envelope slicing only. */
  position: i32 = 0;
  /** False after any invalid field or memory bound. */
  valid: bool = true;
  /** The caller keeps the managed input alive for the reader's lifetime. */
  constructor(public bytes: Uint8Array) { if (bytes.length > MAX_WIRE) this.valid = false; }
  /** Read <=8 little-endian bytes with subtraction-based bounds checks. */
  integer(width: i32): u64 { if (!this.valid || width < 0 || width > 8 || width > this.bytes.length - this.position) { this.valid = false; return 0; } let result: u64 = 0; for (let i = 0; i < width; ++i) result |= <u64>this.bytes[this.position++] << (i * 8); return result; }
  /** Read an unsigned byte. */
  u8(): u8 { return <u8>this.integer(1); }
  /** Read a u32. */
  u32(): u32 { return <u32>this.integer(4); }
  /** Read an exact u64, preserving all bits. */
  u64(): u64 { return this.integer(8); }
  /** Read a signed i32. */
  i32(): i32 { return <i32>this.u32(); }
  /** Read a signed i64. */
  i64(): i64 { return <i64>this.u64(); }
  /** Read finite float64; invalid values mark the complete reader invalid. */
  f64(): f64 { const n = reinterpret<f64>(this.u64()); if (!isFinite(n)) this.valid = false; return n; }
  /** Copy a bounded byte range. No borrowed view escapes a native import. */
  take(length: u32): Uint8Array { if (!this.valid || length > <u32>(this.bytes.length - this.position)) { this.valid = false; return new Uint8Array(0); } const result = this.bytes.slice(this.position, this.position + <i32>length); this.position += <i32>length; return result; }
  /** Decode a strict length-prefixed UTF-8 string. */
  text(allowNul: bool = true): string { const b = this.take(this.u32()); if (!this.valid || !validUtf8(b, allowNul)) { this.valid = false; return ""; } return String.UTF8.decode(b.buffer); }
  /** Copy a length-prefixed blob. */
  blob(): Uint8Array { return this.take(this.u32()); }
  /** Require successful complete consumption, rejecting trailing bytes. */
  done(): bool { return this.valid && this.position == this.bytes.length; }
}

/** Typed message codec contract for guest-local publisher/subscription wrappers. */
export interface Codec<T> { encode(value: T): Uint8Array; valid(bytes: Uint8Array): bool; decode(bytes: Uint8Array): T; }
/** std_msgs/String: u32 UTF-8 byte count followed by bytes. */
export class StringCodec implements Codec<string> {
  /** Encode one canonical String value. */
  encode(value: string): Uint8Array { return new Writer().text(value).finish(); }
  /** Check exact length and strict UTF-8 before decode(). */
  valid(bytes: Uint8Array): bool { const r = new Reader(bytes); r.text(); return r.done(); }
  /** Decode validated bytes; callers first call valid() when input is untrusted. */
  decode(bytes: Uint8Array): string { const r = new Reader(bytes); return r.text(); }
}
/** std_msgs/Bool and SetBool request: one byte, either 0 or 1. */
export class BoolCodec implements Codec<bool> {
  /** Encode one boolean. */
  encode(value: bool): Uint8Array { const b = new Uint8Array(1); b[0] = value ? 1 : 0; return b; }
  /** Check exact size/value. */
  valid(bytes: Uint8Array): bool { return bytes.length == 1 && bytes[0] <= 1; }
  /** Decode after valid(). */
  decode(bytes: Uint8Array): bool { return bytes.length == 1 && bytes[0] == 1; }
}
/** geometry_msgs/Twist in linear xyz / angular xyz order. */
export class Twist {
  /** Six finite fields. Constructor creates no ROS message or publisher. */
  constructor(public lx: f64 = 0, public ly: f64 = 0, public lz: f64 = 0, public ax: f64 = 0, public ay: f64 = 0, public az: f64 = 0) {}
}
/** Exactly six little-endian float64 values. */
export class TwistCodec implements Codec<Twist> {
  /** Encode a Twist, rejecting nonfinite fields through the bounded Writer. */
  encode(v: Twist): Uint8Array { return new Writer(48).f64(v.lx).f64(v.ly).f64(v.lz).f64(v.ax).f64(v.ay).f64(v.az).finish(); }
  /** Validate size and finiteness. */
  valid(b: Uint8Array): bool { const r = new Reader(b); for (let i = 0; i < 6; ++i) r.f64(); return r.done(); }
  /** Decode validated values. */
  decode(b: Uint8Array): Twist { const r = new Reader(b); return new Twist(r.f64(), r.f64(), r.f64(), r.f64(), r.f64(), r.f64()); }
}
/** Trigger/SetBool response value. valid is false after malformed decoding. */
export class ServiceResponse { valid: bool = true; constructor(public success: bool = false, public message: string = "") {} }
/** Encode bool + length-prefixed UTF-8 message. */
export function encodeServiceResponse(v: ServiceResponse): Uint8Array { return new Writer().u8(v.success ? 1 : 0).text(v.message).finish(); }
/** Strictly decode a Trigger/SetBool response and inspect result.valid. */
export function decodeServiceResponse(b: Uint8Array): ServiceResponse { const r = new Reader(b), flag = r.u8(), v = new ServiceResponse(flag == 1, r.text()); v.valid = flag <= 1 && r.done(); return v; }
/** Encode service.respond's owned request ID followed by its typed response. */
export function encodeServiceReply(id: u64, value: ServiceResponse): Uint8Array { if (!id) return new Uint8Array(0); return new Writer().u64(id).bytes(encodeServiceResponse(value)).finish(); }
/** Incoming Trigger/SetBool request envelope; request bytes need their type-specific codec. */
export class ServiceRequest { valid: bool = true; constructor(public id: u64 = 0, public request: Uint8Array = new Uint8Array(0)) {} }
/** Extract a nonzero request ID and complete supported Trigger/SetBool body.
 * Arbitrary custom service bodies have no trusted native adapter in this SDK.
 */
export function decodeServiceRequest(b: Uint8Array): ServiceRequest { const r = new Reader(b), id = r.u64(), v = new ServiceRequest(id, r.take(<u32>(b.length - r.position))); v.valid = id != 0 && r.done() && (v.request.length==0 || (v.request.length==1 && v.request[0]<=1)); return v; }
/** Fibonacci goal request has one bounded nonnegative i32. Empty output means invalid order. */
export function encodeFibonacciGoal(order: i32): Uint8Array { if (order < 0 || order > 46) return new Uint8Array(0); return new Writer(4).i32(order).finish(); }
/** Encode a nonzero owned Action goal ID, exactly eight little-endian bytes. */
export function encodeGoalId(id: u64): Uint8Array { return id ? new Writer(8).u64(id).finish() : new Uint8Array(0); }
/** Decode an exact eight-byte nonzero goal ID; zero means invalid. */
export function decodeGoalId(b: Uint8Array): u64 { const r = new Reader(b), id = r.u64(); return r.done() ? id : 0; }
/** Incoming Action server goal: owned ID and requested Fibonacci order. */
export class IncomingGoal { valid: bool = true; constructor(public id: u64 = 0, public order: i32 = 0) {} }
/** Decode the exact 12-byte event envelope. */
export function decodeIncomingGoal(b: Uint8Array): IncomingGoal { const r = new Reader(b), v = new IncomingGoal(r.u64(), r.i32()); v.valid = v.id != 0 && v.order >= 0 && v.order <= 46 && r.done(); return v; }
/** Action feedback/result/server-response envelope: owned goal ID, optional kind and <=48 i32 values. */
export class ActionSequence { valid: bool = true; constructor(public id: u64 = 0, public kind: u8 = 0, public values: Array<i32> = new Array<i32>()) {} }
/** Encode kind 1=feedback, 2=success, 3=abort, 4=cancel. Feedback reads omit the kind byte. */
export function encodeActionSequence(v: ActionSequence, includeKind: bool = true): Uint8Array { if (!v.id || v.values.length > 48 || (includeKind && (v.kind < 1 || v.kind > 4))) return new Uint8Array(0); const w = new Writer().u64(v.id); if (includeKind) w.u8(v.kind); w.u32(<u32>v.values.length); for (let i = 0; i < v.values.length; ++i) { if (v.values[i] < 0) return new Uint8Array(0); w.i32(v.values[i]); } return w.finish(); }
/** Strictly decode feedback with hasKind=false, result with hasKind=true. */
export function decodeActionSequence(b: Uint8Array, hasKind: bool = true): ActionSequence { const r = new Reader(b), v = new ActionSequence(r.u64(), hasKind ? r.u8() : 0), count = r.u32(); if (count > 48 || count > <u32>(b.length - r.position) / 4) { v.valid = false; return v; } for (let i: u32 = 0; i < count; ++i) { const value = r.i32(); if (value < 0) r.valid = false; v.values.push(value); } v.valid = v.id != 0 && (!hasKind || (v.kind >= 1 && v.kind <= 4)) && r.done(); return v; }
/** Canonical sensor_msgs/Image; encoding/step/shape are further checked by runtime policy. */
export class Image { valid: bool = true; sequence: u32 = 0; seconds: u32 = 0; nanoseconds: u32 = 0; frameId: string = ""; height: u32 = 0; width: u32 = 0; encoding: string = ""; bigendian: bool = false; step: u32 = 0; data: Uint8Array = new Uint8Array(0); }
/** Encode the complete Image envelope with copied pixel bytes. */
export function encodeImage(v: Image): Uint8Array { return new Writer().u32(v.sequence).u32(v.seconds).u32(v.nanoseconds).text(v.frameId).u32(v.height).u32(v.width).text(v.encoding).u8(v.bigendian ? 1 : 0).u32(v.step).blob(v.data).finish(); }
/** Strictly decode one complete Image. Inspect valid before exposing pixels. */
export function decodeImage(b: Uint8Array): Image { const r = new Reader(b), v = new Image(); v.sequence = r.u32(); v.seconds = r.u32(); v.nanoseconds = r.u32(); v.frameId = r.text(); v.height = r.u32(); v.width = r.u32(); v.encoding = r.text(); const flag = r.u8(); v.bigendian = flag == 1; v.step = r.u32(); v.data = r.blob(); v.valid = flag <= 1 && r.done(); return v; }
/** Canonical TransformStamped with quaternion in xyzw order. */
export class Transform { valid: bool = true; sequence: u32 = 0; seconds: u32 = 0; nanoseconds: u32 = 0; parentFrame: string = ""; childFrame: string = ""; x: f64 = 0; y: f64 = 0; z: f64 = 0; qx: f64 = 0; qy: f64 = 0; qz: f64 = 0; qw: f64 = 1; }
/** Encode one Transform; runtime additionally validates frame binding/quaternion policy. */
export function encodeTransform(v: Transform): Uint8Array { return new Writer().u32(v.sequence).u32(v.seconds).u32(v.nanoseconds).text(v.parentFrame).text(v.childFrame).f64(v.x).f64(v.y).f64(v.z).f64(v.qx).f64(v.qy).f64(v.qz).f64(v.qw).finish(); }
/** Decode one complete finite-valued Transform. */
export function decodeTransform(b: Uint8Array): Transform { const r = new Reader(b), v = new Transform(); v.sequence = r.u32(); v.seconds = r.u32(); v.nanoseconds = r.u32(); v.parentFrame = r.text(); v.childFrame = r.text(); v.x = r.f64(); v.y = r.f64(); v.z = r.f64(); v.qx = r.f64(); v.qy = r.f64(); v.qz = r.f64(); v.qw = r.f64(); v.valid = r.done(); return v; }
/** Scalar and array parameter kinds. Int32 belongs to ROS 1; Int64/Bytes belong to ROS 2. */
export enum ParameterKind { Deletion=0, Bool=1, Int64=2, Float64=3, String=4, Bytes=5, BoolArray=6, Int64Array=7, Float64Array=8, StringArray=9, Int32=10, Int32Array=11 }
/** Typed parameter value storage; only the field selected by kind is serialized. */
export class ParameterValue { valid: bool = true; boolValue: bool = false; int32Value: i32 = 0; int64Value: i64 = 0; float64Value: f64 = 0; stringValue: string = ""; byteArray: Uint8Array = new Uint8Array(0); boolArray: Array<bool> = new Array<bool>(); int32Array: Array<i32> = new Array<i32>(); int64Array: Array<i64> = new Array<i64>(); float64Array: Array<f64> = new Array<f64>(); stringArray: Array<string> = new Array<string>(); constructor(public kind: ParameterKind = ParameterKind.String) {} }
/** Encode a supported canonical scalar or array. Unsupported/deletion kinds return empty bytes. */
export function encodeParameter(v: ParameterValue): Uint8Array {
  const w = new Writer();
  switch (v.kind) {
    case ParameterKind.Bool: w.u8(v.boolValue ? 1 : 0); break;
    case ParameterKind.Int32: w.i32(v.int32Value); break;
    case ParameterKind.Int64: w.i64(v.int64Value); break;
    case ParameterKind.Float64: w.f64(v.float64Value); break;
    case ParameterKind.String: w.text(v.stringValue); break;
    case ParameterKind.Bytes: if (<u32>v.byteArray.length > MAX_ELEMENTS) return new Uint8Array(0); w.blob(v.byteArray); break;
    case ParameterKind.BoolArray: if (<u32>v.boolArray.length > MAX_ELEMENTS) return new Uint8Array(0); w.u32(<u32>v.boolArray.length); for (let i = 0; i < v.boolArray.length; ++i) w.u8(v.boolArray[i] ? 1 : 0); break;
    case ParameterKind.Int32Array: if (<u32>v.int32Array.length > MAX_ELEMENTS) return new Uint8Array(0); w.u32(<u32>v.int32Array.length); for (let i = 0; i < v.int32Array.length; ++i) w.i32(v.int32Array[i]); break;
    case ParameterKind.Int64Array: if (<u32>v.int64Array.length > MAX_ELEMENTS) return new Uint8Array(0); w.u32(<u32>v.int64Array.length); for (let i = 0; i < v.int64Array.length; ++i) w.i64(v.int64Array[i]); break;
    case ParameterKind.Float64Array: if (<u32>v.float64Array.length > MAX_ELEMENTS) return new Uint8Array(0); w.u32(<u32>v.float64Array.length); for (let i = 0; i < v.float64Array.length; ++i) w.f64(v.float64Array[i]); break;
    case ParameterKind.StringArray: if (<u32>v.stringArray.length > MAX_ELEMENTS) return new Uint8Array(0); w.u32(<u32>v.stringArray.length); for (let i = 0; i < v.stringArray.length; ++i) { if (v.stringArray[i].indexOf("\0") >= 0) return new Uint8Array(0); w.text(v.stringArray[i]); } break;
    default: return new Uint8Array(0);
  }
  return w.finish();
}
/** Decode according to the type fixed in a capability, not an untrusted payload tag. */
export function decodeParameter(b: Uint8Array, kind: ParameterKind): ParameterValue {
  const r = new Reader(b), v = new ParameterValue(kind);
  if (kind < ParameterKind.Bool || kind > ParameterKind.Int32Array) { v.valid = false; return v; }
  if (kind == ParameterKind.Bool) { const flag = r.u8(); v.boolValue = flag == 1; if (flag > 1) r.valid = false; }
  else if (kind == ParameterKind.Int32) v.int32Value = r.i32();
  else if (kind == ParameterKind.Int64) v.int64Value = r.i64();
  else if (kind == ParameterKind.Float64) v.float64Value = r.f64();
  else if (kind == ParameterKind.String) v.stringValue = r.text();
  else {
    const count = r.u32(); let width: u32 = kind == ParameterKind.Bytes || kind == ParameterKind.BoolArray ? 1 : kind == ParameterKind.Int32Array || kind == ParameterKind.StringArray ? 4 : 8;
    if (count > MAX_ELEMENTS || count > <u32>(b.length - r.position) / width) { v.valid = false; return v; }
    if (kind == ParameterKind.Bytes) v.byteArray = r.take(count);
    else for (let i: u32 = 0; i < count; ++i) {
      if (kind == ParameterKind.BoolArray) { const flag = r.u8(); if (flag > 1) r.valid = false; v.boolArray.push(flag == 1); }
      else if (kind == ParameterKind.Int32Array) v.int32Array.push(r.i32());
      else if (kind == ParameterKind.Int64Array) v.int64Array.push(r.i64());
      else if (kind == ParameterKind.Float64Array) v.float64Array.push(r.f64());
      else if (kind == ParameterKind.StringArray) v.stringArray.push(r.text(false));
      else r.valid = false;
    }
  }
  v.valid = r.done() && kind != ParameterKind.Deletion; return v;
}
/** Typed parameter-event envelope v2 carried inside ABI v1. */
export class TypedParameterEvent { valid: bool = true; seconds: u32 = 0; nanoseconds: u32 = 0; kind: u8 = 1; node: string = ""; name: string = ""; value: ParameterValue = new ParameterValue(); }
/** Validate the restricted ASCII canonical node/parameter name grammar. */
export function validEventName(value: string, node: bool): bool { if (!value.length || value.length > (node ? 1024 : 255) || (node && (value.length < 2 || value.charCodeAt(0) != 47))) return false; let first = true; for (let i = node ? 1 : 0; i < value.length; ++i) { const c = value.charCodeAt(i); if (c == (node ? 47 : 46)) { if (first) return false; first = true; continue; } const alpha = c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == 95; if (!alpha && !(c >= 48 && c <= 57 && !first)) return false; first = false; } return !first; }
/** Encode typed events with a strict timestamp/name/tag envelope and empty deletion payload. */
export function encodeTypedParameterEvent(v: TypedParameterEvent): Uint8Array { if (v.seconds > 2147483647 || v.nanoseconds >= 1000000000 || v.kind < 1 || v.kind > 3 || v.value.kind > ParameterKind.StringArray || (v.value.kind == ParameterKind.Deletion && v.kind != 3) || !validEventName(v.node, true) || !validEventName(v.name, false)) return new Uint8Array(0); const payload = v.kind == 3 ? new Uint8Array(0) : encodeParameter(v.value); if (v.kind != 3 && !payload.length) return new Uint8Array(0); return new Writer().u32(v.seconds).u32(v.nanoseconds).u8(v.kind).u8(2).u8(<u8>v.value.kind).text(v.node).text(v.name).blob(payload).finish(); }
/** Strictly decode version 2; deletion retains a permitted tag but no value bytes. */
export function decodeTypedParameterEvent(b: Uint8Array): TypedParameterEvent { const r = new Reader(b), v = new TypedParameterEvent(); v.seconds = r.u32(); v.nanoseconds = r.u32(); v.kind = r.u8(); const version = r.u8(), tag = r.u8(); v.node = r.text(); v.name = r.text(); const payload = r.blob(); v.valid = version == 2 && v.seconds <= 2147483647 && v.nanoseconds < 1000000000 && v.kind >= 1 && v.kind <= 3 && tag <= 9 && (tag != 0 || v.kind == 3) && validEventName(v.node, true) && validEventName(v.name, false) && r.done(); if (v.kind == 3) { v.value = new ParameterValue(<ParameterKind>tag); v.valid = v.valid && payload.length == 0; } else { v.value = decodeParameter(payload, <ParameterKind>tag); v.valid = v.valid && v.value.valid; } return v; }
/** Legacy textual parameter events: timestamp, change kind and canonical node/name/value strings. */
export class ParameterEvent { valid: bool = true; seconds: u32 = 0; nanoseconds: u32 = 0; kind: u8 = 1; node: string = ""; name: string = ""; value: string = ""; }
/** Encode the legacy textual event; kind must be new/changed/deleted (1..3). */
export function encodeParameterEvent(v: ParameterEvent): Uint8Array { return v.kind >= 1 && v.kind <= 3 ? new Writer().u32(v.seconds).u32(v.nanoseconds).u8(v.kind).text(v.node).text(v.name).text(v.value).finish() : new Uint8Array(0); }
/** Decode one complete legacy event; callers explicitly choose this instead of typed envelope v2. */
export function decodeParameterEvent(b: Uint8Array): ParameterEvent { const r = new Reader(b), v = new ParameterEvent(); v.seconds = r.u32(); v.nanoseconds = r.u32(); v.kind = r.u8(); v.node = r.text(); v.name = r.text(); v.value = r.text(); v.valid = v.kind >= 1 && v.kind <= 3 && r.done(); return v; }
/** Lifecycle GetState response: state ID then canonical label string. */
export class LifecycleState { valid: bool = true; constructor(public id: u8 = 0, public label: string = "") {} }
/** Decode the exact lifecycle response; changing state instead returns canonical Bool. */
export function decodeLifecycleState(b: Uint8Array): LifecycleState { const r = new Reader(b), v = new LifecycleState(r.u8(), r.text()); v.valid = r.done(); return v; }
/** Decode builtin/time-v1's eight-byte unsigned nanoseconds; valid via exact length. */
export function decodeTime(b: Uint8Array): u64 { return b.length == 8 ? new Reader(b).u64() : 0; }
