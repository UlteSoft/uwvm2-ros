/** Canonical ABI v1 codecs; these bytes are not ROS CDR or TCPROS serialization.
 * Runtime capabilities choose resource/type/identity. Codecs never grant access.
 * Strict decoders reject truncation and trailing bytes before returning a value.
 */
// A leading U+FEFF is literal message content in this length-framed format.
// ignoreBOM:true tells TextDecoder to preserve it rather than consume a marker.
const utf8 = new TextEncoder(), textDecoder = new TextDecoder('utf-8', { fatal: true, ignoreBOM: true });
const MAX_U32 = 0xffffffff;

/** Sequential little-endian encoder with an explicit allocation bound.
 * Use write methods in wire-field order, then finish once; no automatic I/O occurs.
 */
export class Writer {
  /** @param {number} maximum Maximum final wire bytes (default 4 MiB). */
  constructor(maximum = 4193786) { if (!Number.isSafeInteger(maximum) || maximum < 0 || maximum > 4193786) throw new RangeError('Invalid wire allocation bound'); this.maximum = maximum; this.parts = []; this.length = 0; }
  /** Append owned bytes after checking the complete allocation bound. */
  bytes(value) {
    if (!(value instanceof Uint8Array)) throw new TypeError('Uint8Array required');
    if (value.length > this.maximum - this.length) throw new RangeError('Wire allocation limit');
    this.parts.push(value.slice()); this.length += value.length; return this;
  }
  /** Append a validated unsigned byte. */
  u8(value) { if (!Number.isInteger(value) || value < 0 || value > 255) throw new RangeError('u8'); return this.bytes(Uint8Array.of(value)); }
  /** Append a validated u32 little-endian value. */
  u32(value) {
    if (!Number.isInteger(value) || value < 0 || value > MAX_U32) throw new RangeError('u32');
    const b = new Uint8Array(4); new DataView(b.buffer).setUint32(0, value, true); return this.bytes(b);
  }
  /** Append a validated i32 little-endian value, preserving its sign bits. */
  i32(value) {
    if (!Number.isInteger(value) || value < -2147483648 || value > 2147483647) throw new RangeError('i32');
    const b = new Uint8Array(4); new DataView(b.buffer).setInt32(0, value, true); return this.bytes(b);
  }
  /** Append a BigInt u64 without narrowing through Number. */
  u64(value) {
    if (typeof value !== 'bigint' || value < 0n || value > 0xffffffffffffffffn) throw new RangeError('u64 BigInt');
    const b = new Uint8Array(8); new DataView(b.buffer).setBigUint64(0, value, true); return this.bytes(b);
  }
  /** Append a signed i64 BigInt without changing its two's-complement bits. */
  i64(value) { if (typeof value !== 'bigint' || value < -0x8000000000000000n || value > 0x7fffffffffffffffn) throw new RangeError('i64 BigInt'); return this.u64(BigInt.asUintN(64, value)); }
  /** Append a finite IEEE-754 float64; NaN and infinity are rejected. */
  f64(value) {
    if (!Number.isFinite(value)) throw new RangeError('finite float64 required');
    const b = new Uint8Array(8); new DataView(b.buffer).setFloat64(0, value, true); return this.bytes(b);
  }
  /** Append a u32 byte count followed by UTF-8 bytes; lone surrogates are rejected. */
  text(value) {
    if (typeof value !== 'string' || /[\uD800-\uDBFF](?![\uDC00-\uDFFF])|(?<![\uD800-\uDBFF])[\uDC00-\uDFFF]/u.test(value))
      throw new TypeError('Well-formed Unicode string required');
    if (this.maximum - this.length < 4 || value.length > this.maximum - this.length - 4) throw new RangeError('Wire text limit');
    const b = utf8.encode(value); return this.u32(b.length).bytes(b);
  }
  /** Append a u32 byte count followed by a raw byte payload. */
  blob(value) { if (!(value instanceof Uint8Array)) throw new TypeError('Uint8Array blob required'); return this.u32(value.length).bytes(value); }
  /** Return one owned byte array. The writer does not reuse returned storage. */
  finish() { const out = new Uint8Array(this.length); let p = 0; for (const b of this.parts) { out.set(b, p); p += b.length; } return out; }
}

/** Bounded reader over one complete copied wire payload. */
export class Reader {
  /** @param {Uint8Array} bytes Owned or immutable wire bytes. */
  constructor(bytes) { if (!(bytes instanceof Uint8Array)) throw new TypeError('Uint8Array required'); this.bytes = bytes; this.position = 0; }
  /** Borrow exactly n remaining bytes. Bounds are checked before advancing. */
  take(n) {
    if (!Number.isInteger(n) || n < 0 || n > this.bytes.length - this.position) throw new RangeError('Truncated wire');
    const out = this.bytes.subarray(this.position, this.position + n); this.position += n; return out;
  }
  /** Read an unsigned byte. */
  u8() { return this.take(1)[0]; }
  /** Read a u32 little-endian value. */
  u32() { const b = this.take(4); return new DataView(b.buffer, b.byteOffset, 4).getUint32(0, true); }
  /** Read an i32 little-endian value. */
  i32() { const b = this.take(4); return new DataView(b.buffer, b.byteOffset, 4).getInt32(0, true); }
  /** Read a u64 as BigInt. */
  u64() { const b = this.take(8); return new DataView(b.buffer, b.byteOffset, 8).getBigUint64(0, true); }
  /** Read a signed i64 as BigInt, retaining precision for all 64 bits. */
  i64() { return BigInt.asIntN(64, this.u64()); }
  /** Read finite float64; reject invalid canonical numeric values. */
  f64() { const b = this.take(8), n = new DataView(b.buffer, b.byteOffset, 8).getFloat64(0, true); if (!Number.isFinite(n)) throw new RangeError('Nonfinite wire'); return n; }
  /** Decode a length-prefixed UTF-8 string, rejecting invalid UTF-8. */
  text() { return textDecoder.decode(this.take(this.u32())); }
  /** Copy a length-prefixed raw byte blob. */
  blob() { return this.take(this.u32()).slice(); }
  /** Require complete consumption; use before exposing any decoded object. */
  end() { if (this.position !== this.bytes.length) throw new RangeError('Trailing wire bytes'); }
}

/** Canonical std_msgs/String value; byte length is UTF-8 length, never JS string length. */
export const StringMessage = Object.freeze({ encode: value => new Writer().text(value).finish(), decode: bytes => { const r = new Reader(bytes), value = r.text(); r.end(); return value; } });
/** Canonical std_msgs/Bool or SetBool request: exactly one byte, either 0 or 1. */
export const BoolMessage = Object.freeze({ encode: value => { if (typeof value !== 'boolean') throw new TypeError('boolean required'); return Uint8Array.of(value ? 1 : 0); }, decode: bytes => { if (!(bytes instanceof Uint8Array) || bytes.length !== 1 || bytes[0] > 1) throw new RangeError('Invalid Bool wire'); return bytes[0] !== 0; } });
/** geometry_msgs/Twist: linear xyz then angular xyz, six finite little-endian doubles. */
export const TwistMessage = Object.freeze({ encode: value => { if (!Array.isArray(value) || value.length !== 6) throw new TypeError('six doubles required'); const w = new Writer(48); for (const n of value) w.f64(n); return w.finish(); }, decode: bytes => { const r = new Reader(bytes), value = Array.from({ length: 6 }, () => r.f64()); r.end(); return value; } });
/** std_srvs/Trigger request has zero bytes; SetBool uses BoolMessage. */
export const TriggerRequest = Object.freeze({ encode: () => new Uint8Array(), decode: bytes => { if (!(bytes instanceof Uint8Array) || bytes.length) throw new RangeError('Trigger request must be empty'); return null; } });
/** Trigger/SetBool response is a bool followed by a u32-prefixed UTF-8 message. */
export const ServiceResponse = Object.freeze({ encode: value => new Writer().bytes(BoolMessage.encode(value.success)).text(value.message).finish(), decode: bytes => { const r = new Reader(bytes), success = BoolMessage.decode(r.take(1)), message = r.text(); r.end(); return { success, message }; } });
/** Poll-delivered service request envelope: nonzero u64 request ID followed by typed request bytes. */
export function decodeServiceRequest(bytes, codec = TriggerRequest) { const r = new Reader(bytes), requestId = r.u64(); if (!requestId) throw new RangeError('Zero request ID'); const request = codec.decode(r.take(bytes.length - r.position)); r.end(); return { requestId, request }; }
/** Reply envelope preserves the exact request ID. It does not select another service or grant response rights. */
export function encodeServiceReply(requestId, response) { if (!requestId) throw new RangeError('Zero request ID'); return new Writer().u64(requestId).bytes(ServiceResponse.encode(response)).finish(); }
/** Fibonacci goal request: one nonnegative i32 order; runtime quota usually limits order to <=46. */
export const FibonacciGoal = Object.freeze({ encode: value => { if (value < 0 || value > 46) throw new RangeError('Fibonacci order must be 0..46'); return new Writer(4).i32(value).finish(); }, decode: bytes => { const r = new Reader(bytes), order = r.i32(); r.end(); if (order < 0 || order > 46) throw new RangeError('Fibonacci order'); return order; } });
/** Encode a nonzero opaque Action goal ID as eight little-endian bytes. */
export function encodeGoalId(goalId) { if (!goalId) throw new RangeError('Zero goal ID'); return new Writer(8).u64(goalId).finish(); }
/** Decode an eight-byte Action goal ID returned by action_send_goal. */
export function decodeGoalId(bytes) { const r = new Reader(bytes), id = r.u64(); r.end(); if (!id) throw new RangeError('Zero goal ID'); return id; }
/** Action server goal event: u64 ID then i32 order (12 bytes). */
export function decodeIncomingGoal(bytes) { const r = new Reader(bytes), goalId = r.u64(), order = r.i32(); r.end(); if (!goalId || order < 0 || order > 46) throw new RangeError('Invalid incoming goal'); return { goalId, order }; }
/** Fibonacci sequence envelope. Feedback reads omit kind; result/server responses include
 * kind 1=feedback, 2=success, 3=abort, 4=cancel. Values have a u32 count and <=48 i32 elements.
 */
export function encodeActionSequence(value, includeKind = true) { if (!value || !value.goalId || !Array.isArray(value.values) || value.values.length > 48) throw new RangeError('Action sequence'); const w = new Writer().u64(value.goalId); if (includeKind) { if (value.kind < 1 || value.kind > 4) throw new RangeError('Action kind'); w.u8(value.kind); } w.u32(value.values.length); for (const n of value.values) { if (n < 0) throw new RangeError('Negative Fibonacci value'); w.i32(n); } return w.finish(); }
/** Strictly decode the corresponding sequence format; choose hasKind explicitly for feedback. */
export function decodeActionSequence(bytes, hasKind = true) { const r = new Reader(bytes), goalId = r.u64(), kind = hasKind ? r.u8() : 0, count = r.u32(); if (!goalId || count > 48 || (hasKind && (kind < 1 || kind > 4))) throw new RangeError('Action sequence'); const values = Array.from({ length: count }, () => r.i32()); r.end(); if (values.some(n => n < 0)) throw new RangeError('Negative Fibonacci value'); return { goalId, kind, values }; }
/** Common canonical Image validation mirrors unconditional Runtime checks,
 * including the six supported encodings, timestamp, frame, stride and data
 * length. It cannot authorize dimensions/byte quotas or native interoperability.
 * Use imageMessage(version) for the additional selected ROS Header profile.
 */
function checkedImage(v,version=1) {
  const channels={mono8:1,mono16:2,rgb8:3,bgr8:3,rgba8:4,bgra8:4}[v?.encoding];
  if(!v||!channels||typeof v.frameId!=='string'||!/^[A-Za-z_0-9]+(?:\/[A-Za-z_0-9]+)*$/.test(v.frameId)||utf8.encode(v.frameId).length>255||
    !Number.isInteger(v.nanoseconds)||v.nanoseconds<0||v.nanoseconds>=1000000000||
    !Number.isInteger(v.width)||v.width<=0||v.width>MAX_U32||!Number.isInteger(v.height)||v.height<=0||v.height>MAX_U32||
    !Number.isInteger(v.step)||v.step<0||v.step>MAX_U32||!(v.data instanceof Uint8Array)||typeof v.bigendian!=='boolean'||
    BigInt(v.width)*BigInt(channels)>BigInt(v.step)||BigInt(v.height)*BigInt(v.step)!==BigInt(v.data.length)||
    !Number.isInteger(v.seconds)||v.seconds<0||v.seconds>(version===2?2147483647:MAX_U32)||
    !Number.isInteger(v.sequence)||v.sequence<0||v.sequence>MAX_U32||(version===2&&v.sequence!==0))throw new RangeError('Image canonical profile');
  return v;
}
export const ImageMessage = Object.freeze({ encode: v => { checkedImage(v);return new Writer().u32(v.sequence).u32(v.seconds).u32(v.nanoseconds).text(v.frameId).u32(v.height).u32(v.width).text(v.encoding).bytes(BoolMessage.encode(v.bigendian)).u32(v.step).blob(v.data).finish(); }, decode: bytes => { const r = new Reader(bytes), v = { sequence: r.u32(), seconds: r.u32(), nanoseconds: r.u32(), frameId: r.text(), height: r.u32(), width: r.u32(), encoding: r.text(), bigendian: BoolMessage.decode(r.take(1)), step: r.u32(), data: r.blob() }; r.end(); return checkedImage(v); } });
/** Return an immutable ROS1/ROS2 Image codec. ROS2 seconds fit signed int32
 * and sequence is exactly zero; no image conversion or normalization occurs.
 */
export function imageMessage(version) { selectedVersion(version);return Object.freeze({encode:v=>ImageMessage.encode(checkedImage(v,version)),decode:b=>checkedImage(ImageMessage.decode(b),version)}); }
/** Canonical TransformStamped: header, parent/child frame strings, xyz and quaternion xyzw. */
function checkedTransform(v,version=1) {
  const frame=value=>typeof value==='string'&&value.length<=255&&/^[A-Za-z_][A-Za-z_0-9]*(?:\/[A-Za-z_][A-Za-z_0-9]*)*$/.test(value);
  if(!v||!frame(v.parentFrame)||!frame(v.childFrame)||v.parentFrame===v.childFrame||
    !Array.isArray(v.translation)||v.translation.length!==3||!Array.isArray(v.quaternion)||v.quaternion.length!==4||
    [...v.translation,...v.quaternion].some(n=>!Number.isFinite(n))||
    !Number.isInteger(v.seconds)||v.seconds<0||v.seconds>2147483647||!Number.isInteger(v.nanoseconds)||v.nanoseconds<0||v.nanoseconds>=1000000000||
    !Number.isInteger(v.sequence)||v.sequence<0||v.sequence>MAX_U32||(version===2&&v.sequence!==0))throw new RangeError('Transform canonical profile');
  const norm=v.quaternion.reduce((sum,n)=>sum+n*n,0);
  if(!Number.isFinite(norm)||Math.abs(norm-1)>0.001)throw new RangeError('Transform quaternion norm');return v;
}
export const TransformMessage = Object.freeze({ encode: v => { checkedTransform(v);const w = new Writer().u32(v.sequence).u32(v.seconds).u32(v.nanoseconds).text(v.parentFrame).text(v.childFrame); for (const n of [...v.translation, ...v.quaternion]) w.f64(n); return w.finish(); }, decode: bytes => { const r = new Reader(bytes), v = { sequence: r.u32(), seconds: r.u32(), nanoseconds: r.u32(), parentFrame: r.text(), childFrame: r.text(), translation: Array.from({ length: 3 }, () => r.f64()), quaternion: Array.from({ length: 4 }, () => r.f64()) }; r.end(); return checkedTransform(v); } });
/** Selected TransformStamped Header profile. No implicit quaternion
 * normalization is performed, and policy can impose tighter field bounds.
 */
export function transformMessage(version) { selectedVersion(version);return Object.freeze({encode:v=>TransformMessage.encode(checkedTransform(v,version)),decode:b=>checkedTransform(TransformMessage.decode(b),version)}); }
function selectedVersion(version) {if(version!==1&&version!==2)throw new RangeError('Explicit ROS version1 or2 required');}
/** Encode the tf.lookup request timestamp; zero selects latest and resource fixes the frames. */
export function encodeLookupTime(nanoseconds = 0n) { return new Writer(8).u64(nanoseconds).finish(); }

/** Return an exact-size scalar codec; used only for policy-selected parameter types. */
function scalarCodec(write, read) { return Object.freeze({ encode: value => { const w = new Writer(); w[write](value); return w.finish(); }, decode: bytes => { const r = new Reader(bytes), value = r[read](); r.end(); return value; } }); }
/** Scalar parameter formats. Strings reuse StringMessage; each array has a u32 element count.
 * ROS 2 native integers use int64. int32 is retained for supported ROS 1 configured types.
 */
export const Parameter = Object.freeze({ Bool: BoolMessage, Int32: scalarCodec('i32', 'i32'),
  Int64: scalarCodec('i64', 'i64'), Float64: scalarCodec('f64', 'f64'), String: StringMessage });

/** Create a strict bounded homogeneous parameter-array codec.
 * @param {string} kind Exactly Byte, Bool, Int32, Int64, Float64 or String.
 * @returns {object} encode/decode, validating maximum count and complete wire size.
 */
export function parameterArray(kind) {
  const scalar = { Byte: ['u8', 'u8', 1], Bool: ['u8', 'u8', 1], Int32: ['i32', 'i32', 4],
    Int64: ['i64', 'i64', 8], Float64: ['f64', 'f64', 8], String: ['text', 'text', 4] }[kind];
  if (!scalar) throw new TypeError('Unknown parameter array kind');
  return Object.freeze({
    /** Encode <=1,048,576 elements, rejecting NUL in string-array elements. */
    encode(values) { if (!Array.isArray(values) && !(kind === 'Byte' && values instanceof Uint8Array)) throw new TypeError('Explicit parameter array required'); if (values.length > 1048576) throw new RangeError('Array count'); const w = new Writer().u32(values.length); for (let value of values) { if (kind === 'Bool') { if (typeof value !== 'boolean') throw new TypeError('boolean required'); value = value ? 1 : 0; } if (kind === 'String' && (typeof value !== 'string' || value.includes('\0'))) throw new TypeError('NUL-free array string required'); w[scalar[0]](value); } return w.finish(); },
    /** Validate available bytes/count before allocating the result collection. */
    decode(bytes) { if (bytes.length > 4193786) throw new RangeError('Array bytes'); const r = new Reader(bytes), count = r.u32(); if (count > 1048576 || count > Math.floor((bytes.length - 4) / scalar[2])) throw new RangeError('Array count'); const out = []; for (let i = 0; i < count; ++i) { let value = r[scalar[1]](); if (kind === 'Bool') { if (value > 1) throw new RangeError('Bool array value'); value = value !== 0; } if (kind === 'String' && value.includes('\0')) throw new RangeError('NUL in array string'); out.push(value); } r.end(); return out; }
  });
}

/** Validated string-array parameter; graph/parameter-list responses instead use one StringMessage containing newline-separated rows. */
export const StringList = parameterArray('String');

/** Validate the restricted canonical node/parameter names used by typed parameter events. */
function eventName(value, node) { return typeof value === 'string' && encoderLength(value) <= (node ? 1024 : 255) && (node ? /^\/(?:[A-Za-z_][A-Za-z_0-9]*)(?:\/[A-Za-z_][A-Za-z_0-9]*)*$/ : /^[A-Za-z_][A-Za-z_0-9]*(?:\.[A-Za-z_][A-Za-z_0-9]*)*$/).test(value); }
function encoderLength(value) { return utf8.encode(value).length; }
const eventCodecs = [null, Parameter.Bool, Parameter.Int64, Parameter.Float64, Parameter.String,
  parameterArray('Byte'), parameterArray('Bool'), parameterArray('Int64'), parameterArray('Float64'), StringList];

/** Typed parameter-event envelope v2 within ROS ABI v1.
 * kind 1=new, 2=changed, 3=deleted; tag 0=deletion, 1..9 follow ROS parameter types.
 * A deletion may retain a known type tag but always has an empty payload.
 */
export const TypedParameterEvent = Object.freeze({
  /** Encode one bounded event with canonical names and nanosecond range. */
  encode(v) { if (!Number.isInteger(v.seconds) || v.seconds < 0 || v.seconds > 2147483647 || !Number.isInteger(v.nanoseconds) || v.nanoseconds < 0 || v.nanoseconds >= 1000000000 || !Number.isInteger(v.kind) || v.kind < 1 || v.kind > 3 || !Number.isInteger(v.tag) || v.tag < 0 || v.tag > 9 || (!v.tag && v.kind !== 3) || !eventName(v.node, true) || !eventName(v.name, false)) throw new RangeError('Typed event envelope'); const payload = v.kind === 3 ? new Uint8Array() : eventCodecs[v.tag].encode(v.value); return new Writer().u32(v.seconds).u32(v.nanoseconds).u8(v.kind).u8(2).u8(v.tag).text(v.node).text(v.name).blob(payload).finish(); },
  /** Decode only supported envelope version 2; unsupported bytes are never guessed. */
  decode(bytes) { if (bytes.length > 4193786) throw new RangeError('Typed event size'); const r = new Reader(bytes), v = { seconds: r.u32(), nanoseconds: r.u32(), kind: r.u8() }, version = r.u8(); v.tag = r.u8(); v.node = r.text(); v.name = r.text(); const payload = r.blob(); r.end(); if (version !== 2 || v.seconds > 2147483647 || v.nanoseconds >= 1000000000 || v.kind < 1 || v.kind > 3 || v.tag > 9 || (!v.tag && v.kind !== 3) || !eventName(v.node, true) || !eventName(v.name, false) || (v.kind === 3 && payload.length)) throw new RangeError('Typed event envelope'); v.value = v.kind === 3 ? null : eventCodecs[v.tag].decode(payload); return v; }
});

/** Legacy textual projection has its own grant, with strict timestamp, names
 * and empty deletion value. It is not guessed from typed envelope v2 bytes.
 */
function checkedLegacyEvent(v) {
  if(!v||!Number.isInteger(v.seconds)||v.seconds<0||v.seconds>2147483647||!Number.isInteger(v.nanoseconds)||v.nanoseconds<0||v.nanoseconds>=1000000000||
    !Number.isInteger(v.kind)||v.kind<1||v.kind>3||!eventName(v.node,true)||!eventName(v.name,false)||typeof v.value!=='string'||(v.kind===3&&v.value!==''))throw new RangeError('Legacy event envelope');return v;
}
export const ParameterEvent = Object.freeze({ encode: v => { checkedLegacyEvent(v);return new Writer().u32(v.seconds).u32(v.nanoseconds).u8(v.kind).text(v.node).text(v.name).text(v.value).finish(); }, decode: bytes => { const r = new Reader(bytes), v = { seconds: r.u32(), nanoseconds: r.u32(), kind: r.u8(), node: r.text(), name: r.text(), value: r.text() }; r.end(); return checkedLegacyEvent(v); } });
/** Lifecycle GetState response: unsigned state ID then label. Request has zero bytes. */
export const LifecycleState = Object.freeze({ encode: v => new Writer().u8(v.id).text(v.label).finish(), decode: bytes => { const r = new Reader(bytes), v = { id: r.u8(), label: r.text() }; r.end(); return v; } });
/** builtin/time-v1 response is unsigned nanoseconds encoded in eight little-endian bytes.
 * The clock kind is chosen by the explicit alias; do not compare steady and ROS epochs.
 */
export const TimeNanoseconds = scalarCodec('u64', 'u64');

/** Explicit service aliases retain identical canonical bytes; their native
 * type/identity is still selected by the independently granted resource.
 */
export const SetBoolRequest=BoolMessage, TriggerResponse=ServiceResponse, SetBoolResponse=ServiceResponse;
/** Named bounded Action codecs distinguish feedback, terminal result and
 * server response. A terminal result must have outcome2/3/4, never feedback1.
 */
export const GoalId=Object.freeze({encode:encodeGoalId,decode:decodeGoalId});
export const IncomingGoal=Object.freeze({encode:v=>new Writer(12).bytes(encodeGoalId(v.goalId)).bytes(FibonacciGoal.encode(v.order)).finish(),decode:decodeIncomingGoal});
export const ActionFeedback=Object.freeze({encode:v=>encodeActionSequence(v,false),decode:b=>decodeActionSequence(b,false)});
export const ActionResponse=Object.freeze({encode:v=>encodeActionSequence(v,true),decode:b=>decodeActionSequence(b,true)});
export const ActionResult=Object.freeze({encode:v=>{if(v.kind<2||v.kind>4)throw new RangeError('Terminal Action outcome');return encodeActionSequence(v,true);},decode:b=>{const v=decodeActionSequence(b,true);if(v.kind<2)throw new RangeError('Terminal Action outcome');return v;}});
