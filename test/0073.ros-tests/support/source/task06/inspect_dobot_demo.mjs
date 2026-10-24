// Actual complete-codec local models; no ROS/Guest/service substitute.
import assert from 'node:assert/strict';
import {readFileSync, writeFileSync} from 'node:fs';
import {resolve} from 'node:path';
import {pathToFileURL} from 'node:url';
import {createHash} from 'node:crypto';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const options = new Map();
for (let at = 2; at < process.argv.length; at += 2) options.set(process.argv[at], process.argv[at + 1]);
const root = resolve(options.get('--source')), alarms = resolve(options.get('--alarms'));
const kernel = resolve(root, 'include/demo_cycle.mjs');
const {DemoCycle} = await import(pathToFileURL(kernel).href);
const modules = new Map();
for (const version of [3, 4]) modules.set(version,
  await import(pathToFileURL(resolve(root, `include/ros1/rosdemo-v${version}/bounded_records.mjs`)).href));
const points = [[0, 0, 0, 0, 0, 0], [0.5, 0, 0.5, 0, 0, 0]];
const cases = [], hash = body => createHash('sha256').update(body).digest('hex');
function test(name, callback) { callback(); cases.push({name, status: 'PASS', layer: 'COMPLETE_CODEC_HOST_DATA_MODEL_ONLY'}); }
function code(wanted, callback) { assert.throws(callback, value => value?.code === wanted); }
function setup(version, extra = {}) {
  return new DemoCycle({version, cycles: 1, points,
    controllerJSON: readFileSync(resolve(alarms, `v${version}/alarm_controller.json`), 'utf8'),
    servoJSON: readFileSync(resolve(alarms, `v${version}/alarm_servo.json`), 'utf8'), ...extra});
}
function codec(version, name) { return modules.get(version)[`Ros1Rosdemo_v${version}${name}Codec`]; }
function response(version, name, value) { return codec(version, name + 'Response').encode(value); }
function feedback(version, point = points[0], command = 0, mode = 5, error = 0, enabled = 1, queued = 1) {
  return JSON.stringify({EnableStatus: enabled, ErrorStatus: error, RunQueuedCmd: queued,
    QactualVec: point, robot_mode: mode, currentCommandId: command, unobserved: {whole: ['retained', 17]}});
}
function pointWire(version, point) {
  // Independent original wire oracle: bool for v4, six binary64LE, empty u32 array.
  const body = Buffer.alloc(version === 3 ? 52 : 53), offset = version === 3 ? 0 : 1;
  if (version === 4) body[0] = 1;
  point.forEach((value, index) => body.writeDoubleLE(value, offset + index * 8));
  body.writeUInt32LE(0, offset + 48);
  return body;
}
function alarmWire(version, ids, res = -2147483648) {
  const body = Buffer.alloc(8 + ids.length * 4);
  const countAt = version === 3 ? 4 : 0, dataAt = countAt + 4;
  body.writeInt32LE(res, version === 3 ? 0 : 4 + ids.length * 4);
  body.writeUInt32LE(ids.length, countAt);
  ids.forEach((value, index) => body.writeInt32LE(value, dataAt + index * 4));
  return body;
}

for (const version of [3, 4]) {
  const movement = version === 3 ? 'JointMovJ' : 'MovJ';
  test(`v${version}-two-whole-original-point-commands-and-completion`, () => {
    const owner = setup(version);
    for (let index = 0; index < 2; index += 1) {
      const wire = owner.takeMove();
      assert.deepEqual(Buffer.from(wire), pointWire(version, points[index]));
      const value = codec(version, movement + 'Request').decode(wire);
      assert.deepEqual(value.paramValue, []);
      if (version === 4) assert.equal(value.mode, true);
      assert.equal(owner.takeMove(), null);
      owner.onMoveResponse(response(version, movement, {res: index + 1}));
      assert.equal(owner.takeMove(), null);
      owner.onFeedback(feedback(version, points[index], index + 1));
    }
    assert.equal(owner.progress().done, true);
    assert.equal(owner.progress().cycles, 1);
    assert.equal(owner.takeMove(), null);
    owner.close();
  });
  test(`v${version}-whole-response-refuses-trailing-bytes-with-pending-retained`, () => {
    const owner = setup(version); owner.takeMove();
    code(90, () => owner.onMoveResponse(new Uint8Array([0, 0, 0, 0, 0])));
    assert.equal(owner.progress().movePending, true);
    assert.equal(owner.takeMove(), null);
    owner.onMoveResponse(response(version, movement, {res: -2147483648}));
    assert.equal(owner.progress().waiting, true);
    owner.close();
  });
  test(`v${version}-empty-feedback-and-no-response-cannot-complete-reserved-move`, () => {
    const owner = setup(version); owner.takeMove(); owner.onFeedback('');
    owner.onFeedback(feedback(version));
    assert.equal(owner.progress().movePending, true);
    assert.equal(owner.progress().waiting, false);
    code(22, () => owner.onErrorResponse(alarmWire(version, [])));
    owner.close();
  });
  test(`v${version}-all-original-controller-and-servo-alarm-rows`, () => {
    const owner = setup(version);
    owner.onFeedback(feedback(version, points[0], 0, 5, 1));
    const tables = ['controller', 'servo'].map(kind => JSON.parse(readFileSync(
      resolve(alarms, `v${version}/alarm_${kind}.json`), 'utf8')));
    let now = 0n, described = 0;
    for (const table of tables) for (const row of table) {
      const action = owner.onTime(now); now += 5000000000n;
      assert.equal(action.kind, 'E'); assert.equal(action.bytes.length, 0);
      assert.deepEqual(Buffer.from(action.bytes), Buffer.alloc(0));
      const sentinel = -2147483648 + described;
      assert.ok(!tables.some(value => value.some(item => item.id === sentinel)));
      const result = owner.onErrorResponse(alarmWire(version, [row.id, sentinel], -17));
      assert.equal(result.value.res, -17);
      const preferred = tables[0].find(item => item.id === row.id) ?? tables[1].find(item => item.id === row.id);
      const kind = tables[0].some(item => item.id === row.id) ? 'Control' : 'Servo';
      assert.equal(result.text, `${kind} ErrorID : ${row.id},  ${preferred.zh_CN.description}, ${preferred.en.description}\n`);
      described += 1;
    }
    assert.ok(described > 100);
    cases.push({name: `v${version}-every-captured-alarm-row-count`, status: 'PASS', rows: described,
      layer: 'CAPTURED_WHOLE_TABLE_DATA_ONLY'});
    owner.close();
  });
  test(`v${version}-malformed-feedback-is-atomic-and-finite`, () => {
    const owner = setup(version); owner.takeMove();
    for (const text of ['[]', '{', '{"ErrorStatus":1e400}', '{"EnableStatus":2147483648}',
        '{"unobserved":{"value":1e400}}'])
      code(22, () => owner.onFeedback(text));
    assert.equal(owner.onTime(0n), null);
    if (version === 3) for (const text of ['{"QactualVec":[0]}', '{"QactualVec":[0,0,0,0,0,1e400]}'])
      code(22, () => owner.onFeedback(text));
    assert.equal(owner.progress().movePending, true);
    owner.close();
  });
  test(`v${version}-no-replay-unmatched-response-cancel-and-retirement`, () => {
    const owner = setup(version); code(22, () => owner.onMoveResponse(response(version, movement, {res: 0})));
    owner.takeMove(); assert.equal(owner.takeMove(), null);
    owner.cancel(); code(125, () => owner.takeMove()); owner.close(); owner.close();
    code(9, () => owner.progress()); code(9, () => DemoCycle.prototype.takeMove.call({}));
  });
  test(`v${version}-time-overflow-and-backward-observation-refuse`, () => {
    const owner = setup(version);
    assert.equal(owner.onTime(5n), null); code(22, () => owner.onTime(4n));
    code(34, () => owner.onTime(0xffffffffffffffffn)); owner.close();
  });
  test(`v${version}-bad-points-and-full-table-rows-refuse-at-construction`, () => {
    for (const invalid of [[[0], points[1]], [[0, 0, 0, 0, 0, NaN], points[1]], new Array(2)])
      code(22, () => setup(version, {points: invalid}));
    code(22, () => setup(version, {controllerJSON: '[{"id":1,"zh_CN":{"description":"a"},"en":{}}]'}));
    code(22, () => setup(version, {cycles: 0}));
  });
  test(`v${version}-copied-points-and-optional-feedback-fields-retain-source-values`, () => {
    const requested = points.map(point => point.slice()), owner = setup(version, {points: requested});
    requested[0][0] = 999;
    assert.deepEqual(Buffer.from(owner.takeMove()), pointWire(version, points[0]));
    owner.onMoveResponse(response(version, movement, {res: 7}));
    owner.onFeedback(feedback(version, points[0], 7, 5, 1, 1, 1));
    owner.onFeedback('{"EnableStatus":"ignored","ErrorStatus":null,"RunQueuedCmd":"ignored","QactualVec":"ignored"}');
    assert.equal(owner.onTime(0n).kind, 'E');
    owner.close();
  });
  test(`v${version}-oversized-whole-error-vector-does-not-clear-pending`, () => {
    const owner = setup(version); owner.onFeedback(feedback(version, points[0], 0, 5, 1));
    owner.onTime(0n);
    code(90, () => owner.onErrorResponse(alarmWire(version, Array(17).fill(999999))));
    assert.equal(owner.progress().errorPending, true);
    owner.onErrorResponse(alarmWire(version, []));
    assert.equal(owner.progress().errorPending, false); owner.close();
  });
}
test('v3-strict-six-joint-tolerance-does-not-use-command-id', () => {
  const owner = setup(3); owner.takeMove(); owner.onMoveResponse(response(3, 'JointMovJ', {res: -3}));
  owner.onFeedback(feedback(3, [0.001, 0, 0, 0, 0, 0], 2147483647));
  assert.equal(owner.progress().waiting, true);
  owner.onFeedback(feedback(3, [0.000999999, 0, 0, 0, 0, 0]));
  assert.equal(owner.progress().point, 1); owner.close();
});
test('v3-enabled-unqueued-reserves-only-active-Continue', () => {
  const owner = setup(3); owner.onFeedback(feedback(3, points[0], 0, 5, 0, 1, 0));
  const action = owner.onTime(0n); assert.equal(action.kind, 'C'); assert.equal(action.bytes.length, 0);
  assert.equal(owner.onTime(5000000000n), null);
  assert.equal(owner.onContinueResponse(response(3, 'Continues', {res: -27})).res, -27);
  assert.equal(owner.onTime(5000000000n).kind, 'C'); owner.close();
});
test('v3-error-branch-requires-enabled-and-never-enables-if-zero-ClearError', () => {
  const owner = setup(3); owner.onFeedback(feedback(3, points[0], 0, 5, 1, 0, 0));
  assert.equal(owner.onTime(0n), null);
  owner.onFeedback(feedback(3, points[0], 0, 5, 1, 1, 0));
  assert.equal(owner.onTime(5000000000n).kind, 'E');
  owner.onErrorResponse(alarmWire(3, [])); assert.equal(owner.progress().continuePending, false); owner.close();
});
test('v4-id-completion-greater-or-equal-mode-five', () => {
  const owner = setup(4); owner.takeMove(); owner.onMoveResponse(response(4, 'MovJ', {res: 77}));
  owner.onFeedback(feedback(4, points[1], 77, 4)); assert.equal(owner.progress().waiting, true);
  owner.onFeedback(feedback(4, points[1], 78, 4)); assert.equal(owner.progress().point, 1); owner.close();
});
test('v4-error-query-ignores-enabled-and-never-reserves-Continue', () => {
  const owner = setup(4); owner.onFeedback(feedback(4, points[0], 0, 5, 1, 0, 0));
  assert.equal(owner.onTime(0n).kind, 'E'); owner.onErrorResponse(alarmWire(4, []));
  owner.onFeedback(feedback(4, points[0], 0, 5, 0, 1, 0));
  assert.equal(owner.onTime(5000000000n), null);
  code(22, () => owner.onContinueResponse(response(3, 'Continues', {res: 0}))); owner.close();
});
test('v4-sorted-multiset-suppression-keeps-duplicates-and-response-order', () => {
  const owner = setup(4), table = JSON.parse(readFileSync(resolve(alarms, 'v4/alarm_controller.json'), 'utf8'));
  const [first, second] = table.slice(0, 2).map(row => row.id);
  owner.onFeedback(feedback(4, points[0], 0, 5, 1));
  owner.onTime(0n); const initial = owner.onErrorResponse(alarmWire(4, [second, first, first]));
  assert.equal(initial.suppressed, false);
  assert.ok(initial.text.indexOf(`: ${second},`) < initial.text.indexOf(`: ${first},`));
  owner.onTime(5000000000n); assert.equal(owner.onErrorResponse(alarmWire(4, [first, second, first])).suppressed, true);
  owner.onTime(10000000000n); assert.equal(owner.onErrorResponse(alarmWire(4, [first, second])).suppressed, false);
  owner.close();
});

const inventory = [];
for (const path of [kernel, resolve(root, 'example/official/demo_cycle.mjs')]) {
  const source = readFileSync(path, 'utf8'), comments = [];
  const tree = acorn.parse(source, {ecmaVersion: 'latest', sourceType: 'module', locations: true, onComment: comments});
  function declaration(name, kind, node, attachment = node) {
    const comment = comments.filter(value => value.end <= attachment.start && /^\s*$/.test(source.slice(value.end, attachment.start))).at(-1);
    assert.ok(comment?.type === 'Block' && comment.value.startsWith('*'), 'Attached actual public JSDoc: ' + name);
    for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag), name + ': ' + tag);
    inventory.push({path, name, kind, line: node.loc.start.line, source_sha256: hash(source),
      declaration_sha256: hash(source.slice(node.start, node.end)), comment_sha256: hash(source.slice(comment.start, comment.end)),
      semantic_review: 'Separate substantive API review; parser attachment is not semantic completeness'});
  }
  for (const exported of tree.body.filter(node => node.type === 'ExportNamedDeclaration')) {
    const node = exported.declaration;
    if (node.type === 'ClassDeclaration') {
      declaration(node.id.name, 'class', node, exported);
      for (const method of node.body.body) declaration(node.id.name + '.' + method.key.name, 'method', method);
    } else if (node.type === 'FunctionDeclaration') declaration(node.id.name, 'function', node, exported);
    else assert.fail('Uninventoried DOBOT application API');
  }
}
assert.equal(inventory.length, 12);
const summary = {status: 'PASS_COMPLETE_DOBOT_HOST_MODELS_ONLY', cases, public_api_inventory: inventory,
  sdk_builds: 0, wasm_validation: 0, guest: 0, native_ros: 0, physical_motion: false};
writeFileSync(options.get('--out'), JSON.stringify(summary, null, 2) + '\n');
process.stdout.write(JSON.stringify({status: summary.status, cases: cases.length, public_api_items: inventory.length}) + '\n');
