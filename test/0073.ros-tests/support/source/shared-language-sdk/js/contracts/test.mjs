/** Pure specification/adapter tests. This suite creates no native ROS endpoints,
 * does not execute uwvm, and does not establish a production trusted bridge.
 * Its in-memory fake only verifies memory/precision/status/ownership contracts.
 * Run from any directory: node sdk/origin/javascript/tests/test.mjs.
 */
import assert from 'node:assert/strict';
import fs from 'node:fs';
import { ABI } from '../../../../../../../sdk/origin/javascript/abi.mjs';
import { Status, u64, resultU32, createRawAdapter, specification, memoryRange, readU64 } from '../../../../../../../sdk/origin/javascript/raw.mjs';
import * as C from '../../../../../../../sdk/origin/javascript/codecs.mjs';
import { Session, Node, Executor, RosError } from '../../../../../../../sdk/origin/javascript/roswasm.mjs';
let checks = 0;
const check = (actual, expected, label) => { assert.deepEqual(actual, expected, label); checks++; };
const throws = (fn, error, label) => { assert.throws(fn, error, label); checks++; };
const fromHex = value => Uint8Array.from(Buffer.from(value, 'hex'));
const hex = value => Buffer.from(value).toString('hex');
const vectors = JSON.parse(fs.readFileSync(new URL('../../../../../../../sdk/origin/abi/wire-vectors-v1.json', import.meta.url), 'utf8'));
const schema = JSON.parse(fs.readFileSync(new URL('../../../../../../../sdk/origin/abi/abi-v1.json', import.meta.url), 'utf8'));

// Compare the full inventory/signatures with the independent authoritative manifest.
for (const version of [1, 2]) {
  const spec = ABI[String(version)], authoritative = schema.modules[spec.module].functions;
  check(spec.functions.map(x => [x.name, x.parameters]), authoritative.map(x => [x.name, x.params]), `ROS${version} signatures`);
  check(Object.isFrozen(spec.functions[0].parameters), true, 'deep immutable ABI');
}

const num = value => value === 'negative-zero' ? -0 : Number(value);
const scalarArray = kind => C.parameterArray(kind);
const codecs = {
  string: C.StringMessage, bool: C.BoolMessage, 'set-bool-request': C.BoolMessage,
  'parameter-int32': C.Parameter.Int32, 'parameter-int64': C.Parameter.Int64,
  'parameter-float64': C.Parameter.Float64, twist: C.TwistMessage,
  'parameter-bool-array': scalarArray('Bool'), 'parameter-byte-array': scalarArray('Byte'),
  'parameter-int32-array': scalarArray('Int32'), 'parameter-int64-array': scalarArray('Int64'),
  'parameter-float64-array': scalarArray('Float64'), 'parameter-string-array': scalarArray('String'),
  image: C.ImageMessage, transform: C.TransformMessage, 'trigger-request': C.TriggerRequest,
  'service-response': C.ServiceResponse, 'fibonacci-goal': C.FibonacciGoal,
  'goal-id': { encode: C.encodeGoalId, decode: C.decodeGoalId },
  'incoming-goal': { encode: v => new C.Writer().u64(v.goalId).i32(v.order).finish(), decode: C.decodeIncomingGoal },
  'action-feedback': { encode: v => C.encodeActionSequence(v, false), decode: b => C.decodeActionSequence(b, false) },
  'action-result': { encode: C.encodeActionSequence, decode: C.decodeActionSequence },
  'action-response': { encode: C.encodeActionSequence, decode: C.decodeActionSequence },
  'lifecycle-state': C.LifecycleState, time: C.TimeNanoseconds,
  'parameter-event': C.ParameterEvent, 'typed-parameter-event': C.TypedParameterEvent,
  'service-request-event': { encode: v => new C.Writer().u64(v.requestId).bytes(fromHex(v.requestHex)).finish(), decode: b => { const r = new C.Reader(b), requestId = r.u64(), requestHex = hex(r.take(b.length - r.position)); r.end(); return { requestId, requestHex }; } },
  'service-response-envelope': { encode: v => new C.Writer().u64(v.requestId).bytes(fromHex(v.responseHex)).finish(), decode: b => { const r = new C.Reader(b), requestId = r.u64(), responseHex = hex(r.take(b.length - r.position)); r.end(); return { requestId, responseHex }; } }
};

/** Translate language-neutral JSON integers/field names without losing i64 precision. */
function convert(vector) {
  const { codec, value: v } = vector;
  if (codec === 'parameter-int32' || codec === 'parameter-float64') return num(v);
  if (codec === 'parameter-int64' || codec === 'goal-id' || codec === 'time') return BigInt(v);
  if (codec === 'twist' || codec === 'parameter-float64-array' || codec === 'parameter-int32-array') return v.map(num);
  if (codec === 'parameter-int64-array') return v.map(BigInt);
  if (codec === 'image') return { ...v, frameId: v.frame_id, data: Uint8Array.from(v.data), frame_id: undefined };
  if (codec === 'transform') return { ...v, parentFrame: v.parent_frame, childFrame: v.child_frame, parent_frame: undefined, child_frame: undefined };
  if (codec === 'service-request-event') return { requestId: BigInt(v.request_id), requestHex: v.request_hex };
  if (codec === 'service-response-envelope') return { requestId: BigInt(v.request_id), responseHex: v.response_hex };
  if (codec === 'incoming-goal') return { goalId: BigInt(v.goal_id), order: v.order };
  if (codec.startsWith('action-')) return { goalId: BigInt(v.goal_id), kind: v.kind ?? 0, values: v.values };
  if (codec === 'typed-parameter-event') return { ...v, value: v.kind === 3 ? null : v.tag === 2 ? BigInt(v.value) : v.tag === 7 ? v.value.map(BigInt) : v.value };
  return v;
}

for (const vector of vectors.positive) {
  const codec = codecs[vector.codec]; assert.ok(codec, vector.codec);
  const value = convert(vector), encoded = codec.encode(value), expected = fromHex(vector.hex);
  check(hex(encoded), vector.hex, `${vector.name} encoding`);
  // Re-encoding the decoded independent golden bytes also covers negative zero.
  check(hex(codec.encode(codec.decode(expected))), vector.hex, `${vector.name} decoding`);
  check(encoded.length, vector.bytes, `${vector.name} size`);
}
for (const vector of vectors.negative) throws(() => codecs[vector.codec].decode(fromHex(vector.hex)), Error, vector.reason);

// Private Unicode regression cases: leading U+FEFF is literal framed content,
// not a stream byte-order marker. The shared 57/11 specification fixtures stay unchanged.
const bomText = '\uFEFFrobot';
check(C.StringMessage.decode(C.StringMessage.encode(bomText)), bomText, 'String preserves literal leading BOM');
check(hex(C.StringMessage.encode(bomText)), '08000000efbbbf726f626f74', 'BOM String canonical UTF8 bytes');
check(C.StringMessage.decode(C.StringMessage.encode('\uFEFF')), '\uFEFF', 'BOM-only String is not empty');
check(C.Parameter.String.decode(C.Parameter.String.encode(bomText)), bomText, 'String parameter preserves BOM');
const bomArray = [bomText, 'plain', '\uFEFF'];
check(C.parameterArray('String').decode(C.parameterArray('String').encode(bomArray)), bomArray, 'String-array elements preserve BOM');
const bomService = { success: true, message: bomText };
check(C.ServiceResponse.decode(C.ServiceResponse.encode(bomService)), bomService, 'Service response text preserves BOM');
const bomEvent = { seconds: 1, nanoseconds: 2, kind: 2, tag: 4, node: '/demo', name: 'value', value: bomText };
check(C.TypedParameterEvent.decode(C.TypedParameterEvent.encode(bomEvent)), bomEvent, 'Typed event String preserves BOM');
const bomArrayEvent = { ...bomEvent, tag: 9, value: bomArray };
check(C.TypedParameterEvent.decode(C.TypedParameterEvent.encode(bomArrayEvent)), bomArrayEvent, 'Typed event String-array preserves BOM');
const bomLegacy = { seconds: 1, nanoseconds: 2, kind: 2, node: '/demo', name: 'value', value: bomText };
check(C.ParameterEvent.decode(C.ParameterEvent.encode(bomLegacy)), bomLegacy, 'Legacy event value text preserves BOM');

// Reject lossy i64 Number inputs and preserve an unaligned eight-byte slot.
const BIG_HANDLE = 9223372036854775813n;
check(u64(BIG_HANDLE), BIG_HANDLE, 'BigInt above safe integer');
throws(() => u64(Number(BIG_HANDLE)), TypeError, 'Number rejected');
throws(() => u64(-1n), TypeError, 'negative offset rejected');
throws(() => specification(3), RangeError, 'unsupported backend');
check(resultU32(-1), 0xffffffff, 'signed i32 result bit pattern');
check(resultU32(-2147483648), 0x80000000, 'minimum signed i32 result');
check(resultU32(0xffffffff), 0xffffffff, 'unsigned bridge status bit pattern');
throws(() => resultU32(-2147483649), TypeError, 'result below signed i32');
throws(() => resultU32(4294967296), TypeError, 'result above unsigned i32');
throws(() => resultU32(1.25), TypeError, 'fractional result rejected');
const memory = new WebAssembly.Memory({ initial: 1, maximum: 2 });
const unaligned = memoryRange(memory, 3n, 8n);
new DataView(unaligned.buffer, unaligned.byteOffset, 8).setBigUint64(0, BIG_HANDLE, true);
check(readU64(memory, 3n), BIG_HANDLE, 'unaligned little-endian slot');
throws(() => memoryRange(memory, 9007199254740993n, 8n), RangeError, 'huge offset bounds');
throws(() => memoryRange(memory, 65535n, 2n), RangeError, 'range overflow');

// A deliberately non-production fake verifies adapter mechanics only.
const calls = [];
let nextEvent = null, mode = 'success', closeStatus = 0, revokeStatus = 0, handleToOpen = BIG_HANDLE;
const functions = Object.fromEntries(specification(2).functions.map(f => [f.name, (...args) => { calls.push([f.name, args]); return Status.UNSUPPORTED; }]));
const writeLength = (offset, value) => { const b = memoryRange(memory, offset, 8n); new DataView(b.buffer, b.byteOffset, 8).setBigUint64(0, value, true); };
functions.abi_version = () => 1;
functions.capability_open = (m, o, n, s) => { calls.push(['capability_open', [m, o, n, s]]); if (mode === 'denied') return 13; writeLength(s, handleToOpen); return 0; };
functions.capability_close = h => { calls.push(['capability_close', [h]]); return closeStatus; };
functions.capability_revoke = h => { calls.push(['capability_revoke', [h]]); return revokeStatus; };
functions.topic_subscribe = h => { calls.push(['topic_subscribe', [h]]); return 0; };
functions.topic_publish = (...args) => { calls.push(['topic_publish', args]); return 0; };
functions.event_poll = (h, m, o, c, s) => { calls.push(['event_poll', [h, m, o, c, s]]); if (!nextEvent) return 11; memoryRange(memory, o, BigInt(nextEvent.length)).set(nextEvent); writeLength(s, BigInt(nextEvent.length)); nextEvent = null; return 0; };
functions.service_call = (h, m, i, n, o, c, s) => { calls.push(['service_call', [h, m, i, n, o, c, s]]); if (mode === 'unknown90') return 90; if (mode === 'known90') { writeLength(s, 200n); return 90; } if (mode === 'grow') memory.grow(1); const b = C.ServiceResponse.encode({ success: true, message: 'ok' }); memoryRange(memory, o, BigInt(b.length)).set(b); writeLength(s, BigInt(b.length)); return 0; };
const bridge = { memory, functions }, scratch = { offset: 1n, inputCapacity: 128, outputCapacity: 128 };
const session = new Session(2, bridge, scratch), node = new Node(session);
const pub = node.createPublisher('greeting-pub'); check(pub.capability.handle, BIG_HANDLE, 'owned precise handle');
check(pub.publish('hello'), 0, 'single publish');
check(calls.filter(c => c[0] === 'topic_publish').length, 1, 'no publish replay');
let callbackCount = 0;
const sub = node.createSubscription('greeting-sub', C.StringMessage, value => { check(value, 'event', 'typed callback'); callbackCount++; });
const executor = new Executor(); executor.add(sub); nextEvent = C.StringMessage.encode('event');
check(callbackCount, 0, 'callback not native'); check(executor.spinOnce(), [0], 'one poll'); check(callbackCount, 1, 'callback after spin');
check(executor.spinOnce(), [11], 'empty queue untouched');
const client = node.createClient('trigger-call');
mode = 'unknown90'; const unknown = client.call(); check([unknown.status, unknown.required, unknown.bytes.length], [90, null, 0], 'unknown length 90');
mode = 'known90'; const known = client.call(); check([known.status, known.required, known.bytes.length], [90, 200n, 0], 'written length 90');
check(calls.filter(c => c[0] === 'service_call').length, 2, 'no capacity retry');
mode = 'grow'; check(client.call().value, { success: true, message: 'ok' }, 'fresh views after memory grow');
mode = 'denied'; const beforeDenied = calls.length; throws(() => session.open('forbidden'), err => err instanceof RosError && err.code === 13, 'default-deny visible'); check(calls.length - beforeDenied, 1, 'no denied escalation/retry');
closeStatus = 107; check(pub.close(), 107, 'close backend failure returned'); throws(() => pub.publish('again'), err => err.code === 9, 'released ownership after close error');
check(calls.filter(c => c[0] === 'capability_close').length, 1, 'one close attempt');
closeStatus = 0; node.close();
// Missing backend operations must fail before opening even an explicitly named alias.
const beforeUnsupported = calls.length;
throws(() => node.createClock('wrong', 'time_read_wall'), err => err.code === 95, 'ROS1 clock import rejected in ROS2');
throws(() => node.createParameters({ delete: 'wrong' }), err => err.code === 95, 'ROS1 parameter import rejected in ROS2');
throws(() => node.createResource('wrong', 'future_import'), err => err.code === 95, 'unknown import rejected');
throws(() => node.createActionClient({}), TypeError, 'missing Action send alias rejected');
check(calls.length, beforeUnsupported, 'no alias open for unsupported factories');
mode = 'success';
const beforeAction = calls.length, limitedAction = node.createActionClient({ send: 'send-only' });
check(calls.length - beforeAction, 1, 'send-only Action opens one alias');
const beforeMissing = calls.length;
throws(() => limitedAction.cancelOwn(BIG_HANDLE), err => err.code === 13, 'cancel-own authority optional');
throws(() => limitedAction.feedback(BIG_HANDLE), err => err.code === 13, 'feedback authority optional');
throws(() => limitedAction.result(BIG_HANDLE), err => err.code === 13, 'result authority optional');
throws(() => limitedAction.cancelAll(), err => err.code === 13, 'cancel-all requires explicit any authority');
check(calls.length, beforeMissing, 'optional Action operation grants nothing');
limitedAction.close();
const cancelAllClient = node.createActionClient({ send: 'send-only', cancelAny: 'cancel-any' });
const beforeAny = calls.length; check(cancelAllClient.cancelAll().status, 95, 'fake unavailable any-cancel unchanged');
check(calls.length - beforeAny, 1, 'cancel-all invokes one explicitly granted operation');
check([...memoryRange(memory, session.input, 8n)], [0, 0, 0, 0, 0, 0, 0, 0], 'cancel-all special zero selector');
throws(() => cancelAllClient.cancelOwn(0n), err => err.code === 13, 'missing own capability never uses any authority');
cancelAllClient.close();
handleToOpen = 0xffffffffffffffffn;
const maximalHandle = session.open('opaque-all-bits'); check(maximalHandle.handle, handleToOpen, 'all opaque nonzero handle bit patterns accepted'); maximalHandle.close();
handleToOpen = BIG_HANDLE;
const revoked = session.open('revoke-test'); revokeStatus = 107;
check(revoked.revoke(), 107, 'revoke backend failure returned');
throws(() => revoked.poll(), err => err.code === 9, 'revoke retires local ownership');
throws(() => revoked.revoke(), err => err.code === 9, 'revoke is not retried');
check(calls.filter(c => c[0] === 'capability_revoke').length, 1, 'single revoke attempt');
throws(() => createRawAdapter(2, { memory, functions: {} }), TypeError, 'incomplete bridge rejected');
throws(() => session.raw.topic_publish(BIG_HANDLE, 0, 1, 2n), TypeError, 'offset Number rejected before fake');
throws(() => C.StringMessage.encode('\ud800'), TypeError, 'lone surrogate rejected');
throws(() => new C.Writer(Infinity), RangeError, 'unbounded writer rejected');
throws(() => new C.Writer(2).text('long'), RangeError, 'text bounded before encoding');
throws(() => C.encodeActionSequence({ goalId: BIG_HANDLE, kind: 2, values: [-1] }), RangeError, 'negative Fibonacci encode rejected');
throws(() => C.decodeActionSequence(new C.Writer().u64(BIG_HANDLE).u8(2).u32(1).i32(-1).finish()), RangeError, 'negative Fibonacci decode rejected');
check(Object.is(C.Parameter.Float64.decode(C.Parameter.Float64.encode(-0)), -0), true, 'negative zero preserved');
console.log(JSON.stringify({ status: 'PASS', scope: 'pure JavaScript codec and in-memory adapter tests; no ROS/uwvm runtime verdict', goldenPositive: vectors.positive.length, goldenNegative: vectors.negative.length, checks }));
