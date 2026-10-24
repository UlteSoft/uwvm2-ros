/** Pure failed-factory ownership tests. The fake bridge tracks in-memory handles
 * only; it creates no native ROS endpoint and is not sandbox integration evidence.
 * Run with node sdk/origin/javascript/tests/factory_transactions.mjs.
 */
import assert from 'node:assert/strict';
import { specification, memoryRange } from '../../../../../../../sdk/origin/javascript/raw.mjs';
import { Session, Node, RosError, Graph, Clock } from '../../../../../../../sdk/origin/javascript/roswasm.mjs';
let checks = 0;
const equal = (actual, expected) => { assert.deepEqual(actual, expected); checks++; };
const expectStatus = (fn, code) => { assert.throws(fn, error => error instanceof RosError && error.code === code); checks++; };

/** Fresh fake means repeated cases cannot accidentally share leaked ownership. */
function fake({ openError = 13, failAt = 2, activationError = 0, closeError = 0 } = {}) {
  const memory = new WebAssembly.Memory({ initial: 1 }), live = new Set(), calls = [];
  let opens = 0, next = 1n;
  const functions = Object.fromEntries(specification(2).functions.map(f => [f.name, (...args) => { calls.push([f.name, args]); return 95; }]));
  functions.abi_version = () => 1;
  functions.capability_open = (m, o, n, slot) => {
    calls.push(['open', new TextDecoder().decode(memoryRange(memory, o, n))]);
    if (++opens === failAt) return openError;
    const handle = next++; live.add(handle);
    const out = memoryRange(memory, slot, 8n); new DataView(out.buffer, out.byteOffset, 8).setBigUint64(0, handle, true);
    return 0;
  };
  functions.capability_close = handle => { calls.push(['close', handle]); assert.ok(live.delete(handle), 'close must not repeat'); return closeError; };
  functions.service_advertise = handle => { calls.push(['service_activate', handle]); return activationError; };
  functions.action_advertise = (h, m, i, n, o, c, slot) => { calls.push(['action_activate', h]); if (!activationError) { const out = memoryRange(memory, slot, 8n); new DataView(out.buffer, out.byteOffset, 8).setBigUint64(0, 0n, true); } return activationError; };
  const node = new Node(new Session(2, { memory, functions }, { offset: 1n, inputCapacity: 128, outputCapacity: 128 }));
  return { node, live, calls, configureRetry: () => { opens = 0; } };
}

const factories = [
  node => node.createService('advertise', 'respond', () => ({ success: true, message: 'ok' })),
  node => node.createActionServer('advertise', 'respond', () => ({ kind: 2, values: [0, 1] })),
  node => node.createActionClient({ send: 'send', result: 'result' }),
  node => node.createActionClient({ send: 'send', cancelAny: 'explicit-any' }),
  node => node.createParameters({ get: 'get', set: 'set' }),
  node => node.createTf('lookup', 'broadcast'),
  node => node.createLifecycle('get-state', 'change-state')
];
for (const create of factories) for (const openError of [13, 107]) for (const closeError of [0, 107]) {
  const state = fake({ openError, closeError });
  expectStatus(() => create(state.node), openError);
  equal(state.live.size, 0); equal(state.node.owned.length, 0);
  equal(state.calls.filter(c => c[0] === 'close').length, 1);
  equal(state.calls.filter(c => c[0].endsWith('_activate')).length, 0);
  state.node.close(); equal(state.calls.filter(c => c[0] === 'close').length, 1);
  // Retrying the same failed operation must neither reuse nor retain the first handle.
  state.configureRetry(); expectStatus(() => create(state.node), openError);
  equal(state.live.size, 0); equal(state.node.owned.length, 0);
  equal(state.calls.filter(c => c[0] === 'close').length, 2);
}

// A later optional Action capability failure rolls back every preceding acquisition.
const third = fake({ failAt: 3 });
expectStatus(() => third.node.createActionClient({ send: 'send', feedback: 'feedback', result: 'result' }), 13);
equal(third.live.size, 0); equal(third.calls.filter(c => c[0] === 'close').length, 2); equal(third.node.owned.length, 0);

// Advertise errors preserve the original status and close both acquired handles once.
for (const create of factories.slice(0, 2)) {
  const state = fake({ failAt: 0, activationError: 107 });
  expectStatus(() => create(state.node), 107); equal(state.live.size, 0); equal(state.node.owned.length, 0);
  equal(state.calls.filter(c => c[0].endsWith('_activate')).length, 1);
  equal(state.calls.filter(c => c[0] === 'close').length, 2);
  state.node.close(); equal(state.calls.filter(c => c[0] === 'close').length, 2);
}

// Complete success commits ownership only after opening both aliases, then closes once.
for (const create of factories.slice(0, 2)) {
  const state = fake({ failAt: 0 }); create(state.node);
  equal(state.node.owned.length, 2); equal(state.live.size, 2);
  equal(state.calls.map(c => c[0]), ['open', 'open', create === factories[0] ? 'service_activate' : 'action_activate']);
  state.node.close(); equal(state.live.size, 0); equal(state.calls.filter(c => c[0] === 'close').length, 2);
}

// Named read facades reject a registered write selector before opening or calling.
const typed = fake({ failAt: 0 });
expectStatus(() => typed.node.createGraph('unrelated', 'parameter_set'), 22);
expectStatus(() => typed.node.createClock('unrelated', 'parameter_set'), 22);
equal(typed.calls.length, 0);
const typedCap = typed.node.open('borrowed-explicit'); const callsBefore = typed.calls.length;
expectStatus(() => new Graph(typedCap, 'parameter_set'), 22);
expectStatus(() => new Clock(typedCap, 'parameter_set'), 22);
equal(typed.calls.length, callsBefore);
const graph = new Graph(typedCap, 'graph_list_topics'); graph.operation = 'parameter_set';
expectStatus(() => graph.read(), 22);
const clock = new Clock(typedCap, 'time_read_ros'); clock.operation = 'parameter_set';
expectStatus(() => clock.now(), 22);
equal(typed.calls.length, callsBefore);
typed.node.close();
console.log(JSON.stringify({ status: 'PASS', scope: 'pure JavaScript in-memory failed-factory ownership tests; no ROS runtime verdict', checks }));
