// Actual whole-codec callback models; no ROS/Guest import or substitute device.
import assert from 'node:assert/strict';
import { readFileSync, writeFileSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { resolve } from 'node:path';
import { pathToFileURL } from 'node:url';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const options = new Map();
for (let index = 2; index < process.argv.length; index += 2) options.set(process.argv[index], process.argv[index + 1]);
const root = resolve(options.get('--root'));
const base = resolve(root, 'sdk/origin/platform/ufactory/javascript');
const modulePath = resolve(base, 'include/robot_states.mjs');
const { RobotStateObserver } = await import(pathToFileURL(modulePath).href);
const { Ros2Sensor_msgsMsgJointStateCodec: joint } = await import(pathToFileURL(resolve(base, 'include/core/humble/bounded_records.mjs')).href);
const { Ros2Xarm_msgsMsgRobotMsgCodec: robot, Ros2Xarm_msgsMsgCIOStateCodec: gpio } = await import(pathToFileURL(resolve(root,
  'sdk/origin/custom-types/ufactory-xarm-ros2-humble-gz-complete/bounded_records.mjs')).href);
const numeric = JSON.parse(readFileSync(options.get('--numeric'), 'utf8'));
const cases = [];
function test(name, callback) { callback(); cases.push({ name, status: 'PASS', layer: 'ACTUAL_WHOLE_CODEC_HOST_CALLBACK_MODEL' }); }
function code(expected, callback) { assert.throws(callback, error => error?.code === expected); }
function sink() {
  const chunks = [];
  return { chunks, write(bytes) { assert.ok(bytes instanceof Uint8Array); chunks.push(bytes.slice()); return 0; },
    text() { return new TextDecoder('utf-8', { fatal: true }).decode(Buffer.concat(chunks)); } };
}

test('original-empty-position-keeps-spaces-and-source-typo', () => {
  const output = sink(), owner = new RobotStateObserver(output.write);
  owner.onJointStates(joint.defaultValue());
  assert.equal(output.text(), '[joint_states] positon: [ ]\n'); owner.close();
});
test('every-original-joint-position-is-printed-without-seven-joint-projection', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = joint.defaultValue();
  value.name = ['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight'];
  value.position = [0, -0, 0.5, -0.125, 10, -10, 2.345, 42];
  value.velocity = value.position.map(() => 0); value.effort = value.velocity.slice();
  owner.onJointStates(value);
  assert.equal(output.text(), '[joint_states] positon: [ 0.000000 -0.000000 0.500000 -0.125000 10.000000 -10.000000 2.345000 42.000000 ]\n');
  owner.close();
});
test('fixed-six-is-compared-with-independent-python-binary64-oracle', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = joint.defaultValue();
  for (const row of numeric) {
    const view = new DataView(new ArrayBuffer(8)); view.setBigUint64(0, BigInt('0x' + row.bits), true);
    value.position = [view.getFloat64(0, true)]; owner.onJointStates(value);
  }
  assert.equal(output.text(), numeric.map(row => '[joint_states] positon: [ ' + row.fixed + ' ]\n').join(''));
  owner.close();
});
test('whole-joint-state-rejects-unprinted-nonfinite-velocity', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = joint.defaultValue();
  value.position = [1]; value.velocity = [NaN]; code(22, () => owner.onJointStates(value));
  assert.equal(output.chunks.length, 0); owner.close();
});
test('whole-joint-state-rejects-unprinted-invalid-stamp-and-name', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), stamp = joint.defaultValue(), name = joint.defaultValue();
  stamp.header.stamp.nanosec = 1000000000; code(22, () => owner.onJointStates(stamp));
  name.name = ['bad\0name']; code(22, () => owner.onJointStates(name));
  assert.equal(output.chunks.length, 0); owner.close();
});
test('complete-joint-array-shape-and-canonical-capacity-remain-explicit', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), uneven = joint.defaultValue(), over = joint.defaultValue();
  uneven.name = ['a', 'b']; uneven.position = [1]; code(22, () => owner.onJointStates(uneven));
  over.position = Array(33).fill(0); code(22, () => owner.onJointStates(over));
  assert.equal(output.chunks.length, 0); owner.close();
});
test('robot-state-keeps-source-signed-integer-error-with-all-fields-decoded', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = robot.defaultValue();
  value.state = 4; value.err = -9; value.mode = 2; value.cmdnum = 120; value.angle = [1, 2, 3, 4, 5, 6, 7];
  value.pose = [1, 2, 3, 4, 5, 6]; value.offset = [-1, -2, -3, -4, -5, -6];
  owner.onRobotStates(value); assert.equal(output.text(), '[robot_states] state: 4, error: -9\n'); owner.close();
});
test('robot-state-refuses-unprinted-nonfinite-last-offset', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = robot.defaultValue();
  value.offset[5] = Infinity; code(22, () => owner.onRobotStates(value)); assert.equal(output.chunks.length, 0); owner.close();
});
test('robot-state-refuses-narrow-scalar-overflow-before-logging', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = robot.defaultValue();
  value.state = 32768; code(22, () => owner.onRobotStates(value)); assert.equal(output.chunks.length, 0); owner.close();
});
test('gpio-notification-still-consumes-all-original-fixed-arrays', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = gpio.defaultValue();
  value.input_digitals = [1, 65535]; value.output_digitals = [2, 3];
  value.input_analogs = [1.25, -2.5]; value.output_analogs = [3.5, -4.5];
  value.input_conf[15] = 255; value.output_conf[15] = 254;
  owner.onCgpioStates(value); assert.equal(output.text(), '[xarm_cgpio_states] callback\n'); owner.close();
});
test('gpio-refuses-last-unprinted-configuration-overflow-and-analog-nan', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), narrow = gpio.defaultValue(), analog = gpio.defaultValue();
  narrow.output_conf[15] = 256; code(22, () => owner.onCgpioStates(narrow));
  analog.output_analogs[1] = NaN; code(22, () => owner.onCgpioStates(analog));
  assert.equal(output.chunks.length, 0); owner.close();
});
test('gpio-refuses-short-fixed-array-instead-of-padding-original-type', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = gpio.defaultValue();
  value.input_conf.pop(); code(22, () => owner.onCgpioStates(value)); assert.equal(output.chunks.length, 0); owner.close();
});
test('callback-output-has-independent-ownership-after-source-mutation', () => {
  const output = sink(), owner = new RobotStateObserver(output.write), value = joint.defaultValue();
  value.position = [1]; owner.onJointStates(value); value.position[0] = 99;
  assert.equal(output.text(), '[joint_states] positon: [ 1.000000 ]\n'); owner.close();
});
test('writer-error-preserves-cause-and-does-not-retry', () => {
  let attempts = 0; const primary = new Error('bounded output refused');
  const owner = new RobotStateObserver(() => { attempts += 1; throw primary; });
  assert.throws(() => owner.onCgpioStates(gpio.defaultValue()), error => error.code === 5 && error.cause === primary);
  assert.equal(attempts, 1); owner.close();
});
test('nonzero-and-promise-writers-are-not-success', () => {
  const first = new RobotStateObserver(() => 90); code(5, () => first.onRobotStates(robot.defaultValue())); first.close();
  const second = new RobotStateObserver(() => Promise.resolve(0)); code(5, () => second.onRobotStates(robot.defaultValue())); second.close();
});
test('reentry-and-mid-callback-close-refuse-without-retiring-owner', () => {
  let owner, calls = 0;
  owner = new RobotStateObserver(() => { calls += 1; code(17, () => owner.close()); code(17, () => owner.onCgpioStates(gpio.defaultValue())); return 0; });
  owner.onRobotStates(robot.defaultValue()); assert.equal(calls, 1); owner.close();
});
test('closed-and-foreign-owners-refuse-before-output', () => {
  const output = sink(), owner = new RobotStateObserver(output.write); owner.close();
  code(9, () => owner.close()); code(9, () => owner.onJointStates(joint.defaultValue()));
  code(9, () => RobotStateObserver.prototype.onRobotStates.call({}, robot.defaultValue()));
  assert.equal(output.chunks.length, 0); code(22, () => new RobotStateObserver(null));
});

const inventory = [];
for (const path of [modulePath, resolve(base, 'example/official/test_robot_states.mjs')]) {
  const source = readFileSync(path, 'utf8'), comments = [];
  const tree = acorn.parse(source, { ecmaVersion: 'latest', sourceType: 'module', locations: true, onComment: comments });
  const hash = value => createHash('sha256').update(value).digest('hex');
  function declaration(name, kind, node, attachment = node) {
    const comment = comments.filter(value => value.end <= attachment.start && /^\s*$/.test(source.slice(value.end, attachment.start))).at(-1);
    assert.ok(comment?.type === 'Block' && comment.value.startsWith('*'), 'actual attached JSDoc required: ' + name);
    for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag), name + ': ' + tag);
    inventory.push({ path, name, kind, line: node.loc.start.line, source_sha256: hash(source),
      declaration_sha256: hash(source.slice(node.start, node.end)), comment_sha256: hash(source.slice(comment.start, comment.end)),
      semantic_review: 'Separate manual review required; parser attachment alone does not qualify API semantics' });
  }
  for (const exported of tree.body.filter(node => node.type === 'ExportNamedDeclaration')) {
    const node = exported.declaration;
    if (node.type === 'ClassDeclaration') {
      declaration(node.id.name, 'class', node, exported);
      for (const method of node.body.body) declaration(node.id.name + '.' + method.key.name, 'method', method);
    } else if (node.type === 'FunctionDeclaration') declaration(node.id.name, 'function', node, exported);
    else assert.fail('Uninventoried public xArm declaration');
  }
}
assert.equal(inventory.length, 7);
writeFileSync(options.get('--out'), JSON.stringify({ status: 'PASS_COMPLETE_HOST_XARM_OBSERVER_ONLY', cases,
  numeric_oracle_values: numeric.length, public_api_inventory: inventory,
  sdk_builds: 0, wasm_guest: 0, native_ros: 0, original_processes: 0, physical_motion: false }, null, 2) + '\n');
process.stdout.write(JSON.stringify({ status: 'PASS_COMPLETE_HOST_XARM_OBSERVER_ONLY', cases: cases.length,
  numeric_oracle_values: numeric.length, public_api_items: inventory.length }) + '\n');
