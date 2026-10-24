// Actual complete-codec host data model. No ROS Node, fake device or Guest.
import assert from 'node:assert/strict';
import { readFileSync, writeFileSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { resolve } from 'node:path';
import { pathToFileURL } from 'node:url';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const options = new Map();
for (let index = 2; index < process.argv.length; index += 2) options.set(process.argv[index], process.argv[index + 1]);
const root = resolve(options.get('--root'));
const modulePath = resolve(root, 'sdk/origin/platform/elephant/javascript/include/teleop.mjs');
const { TopicKeyboardController } = await import(pathToFileURL(modulePath).href);
const bindings = await import(pathToFileURL(resolve(root, 'sdk/origin/custom-types/elephant-mycobot-noetic-complete/bounded_records.mjs')).href);
const axes = ['x', 'y', 'z', 'rx', 'ry', 'rz'];
const joints = ['joint_1', 'joint_2', 'joint_3', 'joint_4', 'joint_5', 'joint_6'];
const coordinatesCodec = bindings.Ros1Mycobot_communicationMycobotCoordsCodec;
const anglesCodec = bindings.Ros1Mycobot_communicationMycobotAnglesCodec;
const codecs = { coordinates: bindings.Ros1Mycobot_communicationMycobotSetCoordsCodec,
  angles: bindings.Ros1Mycobot_communicationMycobotSetAnglesCodec,
  gripper: bindings.Ros1Mycobot_communicationMycobotGripperStatusCodec,
  pump: bindings.Ros1Mycobot_communicationMycobotPumpStatusCodec };
const cases = [];
function test(name, callback) { callback(); cases.push({ name, status: 'PASS', layer: 'HOST_COMPLETE_SOURCE_WORKFLOW_MODEL_ONLY' }); }
function code(expected, callback) { assert.throws(callback, error => error?.code === expected); }
function coordinates(values = [100, 10, 100, 0, 0, 0]) {
  const value = coordinatesCodec.defaultValue(); axes.forEach((axis, index) => { value[axis] = values[index]; }); return value;
}
function angles(values = [1, 2, 3, 4, 5, 6]) {
  const value = anglesCodec.defaultValue(); joints.forEach((joint, index) => { value[joint] = values[index]; }); return value;
}
function sink() {
  const records = [];
  return { records, publish(kind, value) {
    const codec = codecs[kind]; records.push({ kind, value: codec.decode(codec.encode(value)), bytes: Array.from(codec.encode(value)) }); return 0;
  } };
}
function ready(configuration = {}) {
  const owner = new TopicKeyboardController(configuration); owner.onCoordinates(coordinates()); return owner;
}

test('all-zero-and-negative-zero-startup-gates-keys', () => {
  const owner = new TopicKeyboardController(), out = sink();
  assert.equal(owner.onCoordinates(coordinates([0, -0, 0, 0, 0, 0])), false);
  assert.equal(owner.onKey('w', out.publish).status, 11); assert.equal(out.records.length, 0); owner.close();
});
test('whole-state-codec-validation-precedes-latching', () => {
  const owner = new TopicKeyboardController(), value = coordinates(); value.rz = NaN;
  code(22, () => owner.onCoordinates(value)); assert.equal(owner.onKey('w', sink().publish).status, 11); owner.close();
});
test('initial-request-latch-is-independent-of-later-observations', () => {
  const owner = ready(), out = sink(); owner.onCoordinates(coordinates([200, 20, 200, 0, 0, 0]));
  owner.onKey('w', out.publish); assert.equal(out.records[0].value.x, 112.5); owner.close();
});
const sourceMovement = [['w', 0, 12.5], ['s', 0, -12.5], ['a', 1, 12.5], ['d', 1, -12.5],
  ['z', 2, -12.5], ['x', 2, 12.5], ['u', 3, 9], ['j', 3, -9], ['i', 4, 9], ['k', 4, -9], ['o', 5, 9], ['l', 5, -9]];
for (const [key, axis, change] of sourceMovement) {
  test('original-key-' + key + '-and-uppercase', () => {
    for (const actual of [key, key.toUpperCase()]) {
      const owner = ready(), out = sink(); owner.onKey(actual, out.publish);
      const expected = [100, 10, 100, 0, 0, 0]; expected[axis] += change;
      assert.equal(out.records.length, 1); assert.equal(out.records[0].kind, 'coordinates');
      axes.forEach((field, index) => assert.equal(out.records[0].value[field], expected[index]));
      assert.equal(out.records[0].value.speed, 50); assert.equal(out.records[0].value.model, 1); owner.close();
    }
  });
}
test('plus-publishes-unchanged-coordinates-minus-does-not', () => {
  const owner = ready(), out = sink(); owner.onKey('+', out.publish);
  assert.equal(out.records.length, 1); assert.equal(out.records[0].value.x, 100);
  owner.onKey('w', out.publish); assert.equal(out.records[1].value.x, 115);
  owner.onKey('-', out.publish); assert.equal(out.records.length, 2);
  owner.onKey('w', out.publish); assert.equal(out.records[2].value.x, 127.5); owner.close();
});
test('source-step-adjustment-clamps-one-to-twenty-percent', () => {
  const owner = ready(), out = sink();
  for (let index = 0; index < 30; index += 1) owner.onKey('+', out.publish);
  owner.onKey('w', out.publish); assert.equal(out.records.at(-1).value.x, 150);
  for (let index = 0; index < 30; index += 1) owner.onKey('-', out.publish);
  owner.onKey('w', out.publish); assert.equal(out.records.at(-1).value.x, 152.5); owner.close();
});
test('gripper-keeps-separate-first-effect-and-coordinate-fallthrough', () => {
  const owner = ready(), out = sink(); owner.onKey('G', out.publish); owner.onKey('h', out.publish);
  assert.deepEqual(out.records.map(row => row.kind), ['gripper', 'coordinates', 'gripper', 'coordinates']);
  assert.deepEqual(out.records[0].value, { Status: true }); assert.deepEqual(out.records[2].value, { Status: false }); owner.close();
});
test('pump-preserves-pins-booleans-and-coordinate-fallthrough', () => {
  const owner = ready(), out = sink(); owner.onKey('b', out.publish); owner.onKey('M', out.publish);
  assert.deepEqual(out.records[0].value, { Status: true, Pin1: 2, Pin2: 5 });
  assert.deepEqual(out.records[2].value, { Status: false, Pin1: 2, Pin2: 5 }); owner.close();
});
test('source-range-error-retains-request-mutation-without-clamp', () => {
  const owner = ready(), out = sink(); owner.close();
  const near = new TopicKeyboardController(); near.onCoordinates(coordinates([280, 10, 100, 0, 0, 0]));
  const refused = near.onKey('w', out.publish); assert.equal(refused.status, 22); assert.equal(refused.value, 292.5);
  assert.equal(refused.mutationRetained, true); assert.equal(out.records.length, 0);
  near.onKey('s', out.publish); assert.equal(out.records[0].value.x, 280); near.close();
});
test('gripper-effect-can-precede-later-coordinate-range-refusal', () => {
  const owner = new TopicKeyboardController(), out = sink(); owner.onCoordinates(coordinates([280, 10, 100, 0, 0, 0]));
  owner.onKey('w', out.publish); const result = owner.onKey('g', out.publish);
  assert.equal(result.status, 22); assert.deepEqual(out.records.map(row => row.kind), ['gripper']); owner.close();
});
test('init-sends-angles-first-and-snapshots-latest-coordinates-after-two-seconds', () => {
  const owner = ready(), out = sink(); owner.onTime(10n, out.publish); owner.onKey('1', out.publish);
  assert.equal(out.records.length, 1); assert.equal(out.records[0].kind, 'angles');
  joints.forEach(joint => assert.equal(out.records[0].value[joint], 0));
  owner.onCoordinates(coordinates([200, 20, 200, 1, 2, 3]));
  assert.equal(owner.onTime(2000000009n, out.publish).awaitingHomeDelay, true); assert.equal(out.records.length, 1);
  code(17, () => owner.onKey('w', out.publish));
  assert.equal(owner.onTime(2000000010n, out.publish).awaitingHomeDelay, false);
  assert.equal(out.records[1].kind, 'coordinates'); assert.equal(out.records[1].value.x, 200); owner.close();
});
test('default-home-and-saved-home-remain-distinct-complete-angle-values', () => {
  const first = ready(), out = sink(); first.onKey('2', out.publish);
  assert.deepEqual(joints.map(joint => out.records[0].value[joint]), [0, 8, -127, 40, 0, 0]); first.close();
  const saved = ready(), second = sink(); saved.onAngles(angles()); saved.onKey('3', second.publish);
  assert.equal(second.records[0].kind, 'coordinates'); saved.onKey('2', second.publish);
  assert.deepEqual(joints.map(joint => second.records[1].value[joint]), [1, 2, 3, 4, 5, 6]); saved.close();
});
test('whole-angle-input-nonfinite-field-does-not-overwrite-home', () => {
  const owner = ready(), out = sink(), bad = angles(); bad.joint_6 = Infinity;
  code(22, () => owner.onAngles(bad)); owner.onKey('3', out.publish); owner.onKey('2', out.publish);
  joints.forEach(joint => assert.equal(out.records[1].value[joint], 0)); owner.close();
});
test('local-time-refuses-backwards-and-future-wait-overflow-before-effects', () => {
  const owner = ready(), out = sink(); owner.onTime(9n, out.publish); code(22, () => owner.onTime(8n, out.publish));
  owner.onTime(0xffffffffffffffffn, out.publish); code(34, () => owner.onKey('1', out.publish));
  assert.equal(out.records.length, 0); owner.close();
});
test('signed-byte-speed-and-copied-configuration-use-original-wire-types', () => {
  code(22, () => new TopicKeyboardController({ speed: 128 }));
  code(22, () => new TopicKeyboardController({ changePercent: NaN }));
  const config = { speed: 100, changePercent: 5 }, owner = ready(config), out = sink(); config.speed = 1;
  owner.onKey('w', out.publish); assert.equal(out.records[0].value.speed, 100); owner.close();
});
test('failed-publication-retains-primary-code-and-refuses-replay', () => {
  const owner = ready(); let attempts = 0;
  code(5, () => owner.onKey('w', () => { attempts += 1; return 5; }));
  code(17, () => owner.onKey('s', () => { attempts += 1; return 0; })); assert.equal(attempts, 1); owner.close();
});
test('publisher-exception-and-async-return-do-not-become-success', () => {
  const first = ready(), primary = new Error('data sink error');
  assert.throws(() => first.onKey('g', () => { throw primary; }), error => error === primary); first.close();
  const second = ready(); code(22, () => second.onKey('w', () => Promise.resolve(0)));
  code(17, () => second.onKey('w', sink().publish)); second.close();
});
test('reentry-quit-unknown-key-and-retirement-are-explicit', () => {
  const owner = ready(), out = sink(); owner.onKey('?', out.publish); owner.onKey('Q', out.publish);
  assert.equal(out.records.length, 0);
  owner.onKey('w', () => { code(17, () => owner.close()); code(17, () => owner.onCoordinates(coordinates())); return 0; });
  assert.equal(owner.onKey('q', out.publish).quit, true); code(17, () => owner.onKey('w', out.publish));
  owner.close(); code(9, () => owner.close()); code(9, () => owner.onAngles(angles()));
});

const inventory = [];
for (const path of [modulePath, resolve(root, 'sdk/origin/platform/elephant/javascript/example/official/teleop_keyboard.mjs')]) {
  const source = readFileSync(path, 'utf8'), comments = [];
  const tree = acorn.parse(source, { ecmaVersion: 'latest', sourceType: 'module', locations: true, onComment: comments });
  const hash = value => createHash('sha256').update(value).digest('hex');
  function declaration(name, kind, node, attachment = node) {
    const comment = comments.filter(value => value.end <= attachment.start && /^\s*$/.test(source.slice(value.end, attachment.start))).at(-1);
    assert.ok(comment?.type === 'Block' && comment.value.startsWith('*'), 'attached actual JSDoc required: ' + name);
    for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag), name + ': ' + tag);
    inventory.push({ path, name, kind, line: node.loc.start.line, source_sha256: hash(source),
      declaration_sha256: hash(source.slice(node.start, node.end)), comment_sha256: hash(source.slice(comment.start, comment.end)),
      semantic_review: 'Separate manual review required; attachment is not completeness' });
  }
  for (const comment of comments.filter(value => value.value.includes('@typedef'))) {
    for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag));
    inventory.push({ path, name: 'TeleopOutcome', kind: 'JSDocType', line: comment.loc.start.line,
      source_sha256: hash(source), comment_sha256: hash(source.slice(comment.start, comment.end)) });
  }
  for (const exported of tree.body.filter(node => node.type === 'ExportNamedDeclaration')) {
    const node = exported.declaration;
    if (node.type === 'ClassDeclaration') {
      declaration(node.id.name, 'class', node, exported);
      for (const method of node.body.body) declaration(node.id.name + '.' + method.key.name, 'method', method);
    } else if (node.type === 'FunctionDeclaration') declaration(node.id.name, 'function', node, exported);
    else assert.fail('Uninventoried actual public declaration');
  }
}
assert.equal(inventory.length, 9);
writeFileSync(options.get('--out'), JSON.stringify({ status: 'PASS_COMPLETE_HOST_TELEOP_MODEL_ONLY', cases,
  public_api_inventory: inventory, actual_sdk_builds: 0, wasm_guest: 0, native_ros: 0, physical_motion: false }, null, 2) + '\n');
process.stdout.write(JSON.stringify({ status: 'PASS_COMPLETE_HOST_TELEOP_MODEL_ONLY', cases: cases.length, public_api_items: inventory.length }) + '\n');
