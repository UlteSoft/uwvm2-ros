/** Pure executor models using arbitrary JavaScript Pollables. No ROS bridge,
 * Wasm guest, capability, native callback, clock or network is created here.
 * Run with node sdk/javascript/tests/executor_contracts.mjs.
 */
import assert from 'node:assert/strict';
import { Executor } from '../roswasm.mjs';
import { Status } from '../raw.mjs';
let checks = 0;
const equal = (actual, expected) => { assert.deepEqual(actual, expected); checks++; };

// Appending a registration in a callback must not extend the active pass.
const selfExecutor = new Executor(); let selfCalls = 0;
const self = { spinOnce() { selfCalls++; selfExecutor.add(self); return 0; } };
selfExecutor.add(self);
equal(selfExecutor.spinOnce(), [0]); equal(selfCalls, 1);
equal(selfExecutor.spinOnce(), [0, 0]); equal(selfCalls, 3);

const deferExecutor = new Executor(); let firstCalls = 0, addedCalls = 0;
const added = { spinOnce() { addedCalls++; return Status.AGAIN; } };
deferExecutor.add({ spinOnce() { if (!firstCalls++) deferExecutor.add(added); return 0; } });
equal(deferExecutor.spinOnce(), [0]); equal([firstCalls, addedCalls], [1, 0]);
equal(deferExecutor.spinOnce(), [0, Status.AGAIN]); equal([firstCalls, addedCalls], [2, 1]);

// Mutation of the original registration array cannot redispatch its first entry
// or skip another registration already captured at pass entry.
const mutation = new Executor(), order = [];
mutation.add({ spinOnce() { order.push('a'); mutation.entities.length = 0; mutation.add({ spinOnce() { order.push('c'); return 0; } }); return 0; } });
mutation.add({ spinOnce() { order.push('b'); return 0; } });
equal(mutation.spinOnce(), [0, 0]); equal(order, ['a', 'b']);
equal(mutation.spinOnce(), [0]); equal(order, ['a', 'b', 'c']);

// A same-executor nested call is refused before invoking any Pollable.
const nestedExecutor = new Executor(); let calls = 0, nested = null;
nestedExecutor.add({ spinOnce() { calls++; nested = nestedExecutor.spinOnce(); return 0; } });
equal(nestedExecutor.spinOnce(), [0]); equal(calls, 1); equal(nested, [Status.CONFLICT]);
equal(nestedExecutor.spinOnce(), [0]); equal(calls, 2); equal(nested, [Status.CONFLICT]);

// Caller-visible exceptions terminate the pass, release busy and require an
// explicit subsequent spin; an earlier callback is never silently replayed.
const recovering = new Executor(), marker = new Error('intentional local callback exception');
let throwerCalls = 0, laterCalls = 0, newCalls = 0;
recovering.add({ spinOnce() { throwerCalls++; if (throwerCalls === 1) { recovering.add({ spinOnce() { newCalls++; return 0; } }); throw marker; } return 0; } });
recovering.add({ spinOnce() { laterCalls++; return 0; } });
assert.throws(() => recovering.spinOnce(), error => error === marker); checks++;
equal([throwerCalls, laterCalls, newCalls], [1, 0, 0]); equal(recovering.spinning, false);
equal(recovering.spinOnce(), [0, 0, 0]); equal([throwerCalls, laterCalls, newCalls], [2, 1, 1]); equal(recovering.spinning, false);
equal(recovering.spinOnce(), [0, 0, 0]); equal([throwerCalls, laterCalls, newCalls], [3, 2, 2]);

console.log(JSON.stringify({ status: 'PASS', scope: 'pure JavaScript bounded executor/mutation/reentrancy models; no ROS or Wasm guest execution', checks }));
