// Genuine Node host data-model tests over actual generated complete codecs.
// No source substitution, native ROS stub, robot, SDK build or Guest is used.
import assert from 'node:assert/strict';
import { readFileSync, writeFileSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { resolve } from 'node:path';
import { pathToFileURL } from 'node:url';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const options = new Map();
for (let at = 2; at < process.argv.length; at += 2) options.set(process.argv[at], process.argv[at + 1]);
const root = resolve(options.get('--root'));
const prototype = resolve(root, 'platform/flexiv/controller-core/prototype');
const load = relative => import(pathToFileURL(resolve(root, relative)).href);
const { TrajectoryController } = await load('platform/flexiv/controller-core/prototype/trajectory.mjs');
const { SineSweepController, SWING_AMPLITUDE_RADIANS, SWING_FREQUENCY_HZ } =
  await load('platform/flexiv/controller-core/prototype/sine.mjs');
const humble = await load('platform/flexiv/controller-core/generated/humble/bounded_records.mjs');
const foxy = await load('platform/flexiv/controller-core/generated/foxy/bounded_records.mjs');
const vendor = await load('custom-types/flexiv-ros2-foxy-complete/bounded_records.mjs');
const stateCodec = humble.Ros2Sensor_msgsMsgJointStateCodec;
const trajectoryCodec = humble.Ros2Trajectory_msgsMsgJointTrajectoryCodec;
const positionCodec = foxy.Ros2Std_msgsMsgFloat64MultiArrayCodec;
const impedanceCodec = vendor.Ros2Flexiv_msgsMsgJointPosVelCodec;
const cases = [];
function test(name, callback) {
  callback();
  cases.push({ name, status: 'PASS', layer: 'HOST_COMPLETE_RECORD_MODEL_ONLY' });
}
function code(expected, callback) {
  assert.throws(callback, error => error?.code === expected);
}
function state(names = ['a', 'b'], positions = [0, 0], codec = stateCodec) {
  const value = codec.defaultValue();
  value.header.stamp.sec = 19;
  value.header.stamp.nanosec = 123456789;
  value.header.frame_id = '\uFEFFbase/\u8f74';
  value.name = names.slice();
  value.position = positions.slice();
  value.velocity = names.map((_, index) => index + 0.125);
  value.effort = names.map((_, index) => index - 0.5);
  return value;
}
function configuration(check = true) {
  return { joints: ['a', 'b'], goals: [[1, 2], [3, 4]], checkStartingPoint: check,
    limits: { a: [-1, 1], b: [-2, 2] } };
}
function seven(codec = foxy.Ros2Sensor_msgsMsgJointStateCodec) {
  return state(['j1', 'j2', 'j3', 'j4', 'j5', 'j6', 'j7'], [0, 1, 2, 3, 4, 5, 6], codec);
}
function trajectoryOutput(controller) {
  const output = [];
  const diagnostic = controller.onTimer(value => { output.push(trajectoryCodec.decode(trajectoryCodec.encode(value))); return 0; });
  return { output, diagnostic };
}

test('whole-record-preserves-unobserved-header-velocity-effort', () => {
  const value = state();
  assert.deepEqual(stateCodec.decode(stateCodec.encode(value)), value);
});
test('whole-record-rejects-unobserved-nonfinite-field', () => {
  const value = state(); value.effort[1] = NaN;
  code(22, () => stateCodec.encode(value));
});
test('whole-record-rejects-unobserved-invalid-time', () => {
  const value = state(); value.header.stamp.nanosec = 1000000000;
  code(22, () => stateCodec.encode(value));
});
test('whole-record-rejects-trailing-bytes', () => {
  const bytes = stateCodec.encode(state());
  const trailing = new Uint8Array(bytes.length + 1); trailing.set(bytes);
  code(22, () => stateCodec.decode(trailing));
});
test('irregular-jointstate-is-explicitly-refused', () => {
  const value = state(); value.position = [0];
  code(22, () => stateCodec.encode(value));
});
test('optional-empty-jointstate-arrays-remain-empty', () => {
  const value = state(); value.velocity = []; value.effort = [];
  assert.deepEqual(stateCodec.decode(stateCodec.encode(value)), value);
});
test('trajectory-gated-before-complete-initial-state', () => {
  const controller = new TrajectoryController(configuration());
  assert.equal(trajectoryOutput(controller).diagnostic, null);
  assert.deepEqual(controller.onJointState(state(['a'], [0])), { received: false, startingPointOk: false });
  assert.equal(trajectoryOutput(controller).output.length, 0);
  controller.close();
});
test('trajectory-inclusive-limits-extra-joints-and-cycle', () => {
  const controller = new TrajectoryController(configuration());
  assert.deepEqual(controller.onJointState(state(['gripper', 'b', 'a'], [100, 2, -1])),
    { received: true, startingPointOk: true });
  const first = trajectoryOutput(controller), second = trajectoryOutput(controller), third = trajectoryOutput(controller);
  assert.deepEqual(first.diagnostic, { publishedGoalIndex: 0, nextGoalIndex: 1 });
  assert.deepEqual(second.diagnostic, { publishedGoalIndex: 1, nextGoalIndex: 0 });
  assert.deepEqual(first.output[0].points[0].positions, [1, 2]);
  assert.deepEqual(second.output[0].points[0].positions, [3, 4]);
  assert.deepEqual(third.output, first.output);
  controller.close();
});
test('trajectory-retains-all-original-command-defaults', () => {
  const controller = new TrajectoryController(configuration(false));
  const value = trajectoryOutput(controller).output[0];
  assert.deepEqual(value.header, { stamp: { sec: 0, nanosec: 0 }, frame_id: '' });
  assert.deepEqual(value.joint_names, ['a', 'b']);
  assert.equal(value.points.length, 1);
  assert.deepEqual(value.points[0], { positions: [1, 2], velocities: [], accelerations: [], effort: [],
    time_from_start: { sec: 1, nanosec: 0 } });
  controller.close();
});
test('trajectory-first-complete-rejection-remains-latched', () => {
  const controller = new TrajectoryController(configuration());
  assert.deepEqual(controller.onJointState(state(['a', 'b'], [2, 0])), { received: true, startingPointOk: false });
  assert.deepEqual(controller.onJointState(state()), { received: true, startingPointOk: false });
  assert.equal(trajectoryOutput(controller).diagnostic, null);
  controller.close();
});
test('trajectory-last-duplicate-received-name-wins', () => {
  const controller = new TrajectoryController(configuration());
  assert.equal(controller.onJointState(state(['a', 'a', 'b'], [99, 0, 0])).startingPointOk, true);
  controller.close();
});
test('trajectory-config-and-caller-command-storage-are-independent', () => {
  const original = configuration(false), controller = new TrajectoryController(original);
  original.joints[0] = 'changed'; original.goals[0][0] = 999; original.limits.a[0] = 999;
  const first = trajectoryOutput(controller).output[0]; first.joint_names[0] = 'changed';
  first.points[0].positions[0] = 999;
  trajectoryOutput(controller);
  const repeated = trajectoryOutput(controller).output[0];
  assert.deepEqual(repeated.joint_names, ['a', 'b']);
  assert.deepEqual(repeated.points[0].positions, [1, 2]);
  controller.close();
});
test('trajectory-leading-BOM-is-a-valid-original-name', () => {
  const controller = new TrajectoryController({ joints: ['\uFEFFa'], goals: [[1]], checkStartingPoint: false });
  assert.deepEqual(trajectoryOutput(controller).output[0].joint_names, ['\uFEFFa']); controller.close();
});
test('trajectory-explicit-config-rejections', () => {
  code(22, () => new TrajectoryController({ ...configuration(), joints: ['a', 'a'] }));
  code(22, () => new TrajectoryController({ ...configuration(), joints: ['\uD800'] }));
  code(22, () => new TrajectoryController({ ...configuration(), goals: [[Infinity]] }));
});
test('trajectory-preserves-original-goal-length-without-padding', () => {
  const controller = new TrajectoryController({ ...configuration(false), goals: [[0.25]] });
  assert.deepEqual(trajectoryOutput(controller).output[0].points[0].positions, [0.25]); controller.close();
});
test('trajectory-publisher-error-is-retained-and-no-replay-occurs', () => {
  const controller = new TrajectoryController(configuration(false)); let calls = 0;
  code(5, () => controller.onTimer(() => { calls += 1; return 5; }));
  code(17, () => controller.onTimer(() => { calls += 1; return 0; }));
  assert.equal(calls, 1); controller.close();
});
test('trajectory-publisher-exception-is-primary-and-no-replay-occurs', () => {
  const controller = new TrajectoryController(configuration(false)), primary = new Error('actual output exception');
  assert.throws(() => controller.onTimer(() => { throw primary; }), error => error === primary);
  code(17, () => trajectoryOutput(controller)); controller.close();
});
test('trajectory-async-publisher-is-refused-and-blocked', () => {
  const controller = new TrajectoryController(configuration(false));
  code(22, () => controller.onTimer(() => Promise.resolve(0)));
  code(17, () => trajectoryOutput(controller)); controller.close();
});
test('trajectory-reentry-close-and-retired-state-errors', () => {
  const controller = new TrajectoryController(configuration(false));
  controller.onTimer(() => { code(17, () => controller.close()); code(17, () => controller.onJointState(state())); return 0; });
  controller.close(); code(9, () => controller.close()); code(9, () => trajectoryOutput(controller));
});
test('sine-original-constant-values', () => {
  assert.equal(SWING_AMPLITUDE_RADIANS, 0.035); assert.equal(SWING_FREQUENCY_HZ, 0.3);
});
test('sine-gated-before-first-state', () => {
  const controller = new SineSweepController({ mode: 'position' }); let calls = 0;
  assert.equal(controller.onTimer(() => { calls += 1; return 0; }), false);
  assert.equal(calls, 0); controller.close();
});
test('sine-position-full-output-and-original-seven-joint-order', () => {
  const controller = new SineSweepController({ mode: 'position' }); controller.onJointState(seven());
  controller.onTimer(value => {
    assert.deepEqual(positionCodec.decode(positionCodec.encode(value)), {
      layout: { dim: [], data_offset: 0 }, data: [3, 0, 6, 4, 1, 2, 5] }); return 0;
  }); controller.close();
});
test('sine-impedance-full-fixed-zero-velocity-defaults', () => {
  const controller = new SineSweepController({ mode: 'impedance' }); controller.onJointState(seven());
  controller.onTimer(value => {
    assert.deepEqual(impedanceCodec.decode(impedanceCodec.encode(value)), {
      positions: [3, 0, 6, 4, 1, 2, 5], velocities: [0, 0, 0, 0, 0, 0, 0] }); return 0;
  }); controller.close();
});
test('sine-phase-uses-accumulated-configured-period', () => {
  const controller = new SineSweepController({ mode: 'position', periodSeconds: 0.001, speedScaling: 0.5 });
  controller.onJointState(seven()); let first, second;
  controller.onTimer(value => { first = value; return 0; }); controller.onTimer(value => { second = value; return 0; });
  const displacement = 0.035 * Math.sin(2 * Math.PI * 0.3 * 0.5 * 0.001);
  assert.deepEqual(first.data, [3, 0, 6, 4, 1, 2, 5]);
  assert.deepEqual(second.data, [3, 0, 6, 4, 1, 2, 5].map(value => value + displacement)); controller.close();
});
test('sine-first-state-latch-and-extra-positions', () => {
  const controller = new SineSweepController({ mode: 'position', speedScaling: 0 });
  const value = seven(); value.name.push('gripper'); value.position.push(999); value.velocity.push(0); value.effort.push(0);
  controller.onJointState(value); const later = seven(); later.position.fill(99); controller.onJointState(later);
  controller.onTimer(output => { assert.deepEqual(output.data, [3, 0, 6, 4, 1, 2, 5]); return 0; }); controller.close();
});
test('sine-short-first-state-does-not-latch', () => {
  const controller = new SineSweepController({ mode: 'position' });
  code(22, () => controller.onJointState(state(['a', 'b'], [1, 2], foxy.Ros2Sensor_msgsMsgJointStateCodec)));
  controller.onJointState(seven()); assert.equal(controller.onTimer(() => 0), true); controller.close();
});
test('sine-validates-unobserved-whole-input-before-latching', () => {
  const controller = new SineSweepController({ mode: 'position' }), value = seven(); value.effort[6] = Infinity;
  code(22, () => controller.onJointState(value)); assert.equal(controller.onTimer(() => 0), false); controller.close();
});
test('sine-config-is-copied-and-null-period-is-refused', () => {
  code(22, () => new SineSweepController({ mode: 'position', periodSeconds: null }));
  code(22, () => new SineSweepController({ mode: 'unknown' }));
  const config = { mode: 'position', speedScaling: 0 }, controller = new SineSweepController(config);
  config.mode = 'impedance'; config.speedScaling = Infinity; controller.onJointState(seven());
  controller.onTimer(value => { assert.ok(Array.isArray(value.data)); return 0; }); controller.close();
});
test('sine-failed-publication-blocks-time-and-replay', () => {
  const controller = new SineSweepController({ mode: 'position' }); controller.onJointState(seven()); let calls = 0;
  code(5, () => controller.onTimer(() => { calls += 1; return 5; }));
  code(17, () => controller.onTimer(() => { calls += 1; return 0; })); assert.equal(calls, 1); controller.close();
});
test('sine-nonfinite-phase-is-refused-before-publication', () => {
  const controller = new SineSweepController({ mode: 'position', speedScaling: Number.MAX_VALUE });
  controller.onJointState(seven()); let calls = 0;
  code(33, () => controller.onTimer(() => { calls += 1; return 0; })); assert.equal(calls, 0); controller.close();
});
test('sine-reentry-close-and-retired-state-errors', () => {
  const controller = new SineSweepController({ mode: 'position' }); controller.onJointState(seven());
  controller.onTimer(() => { code(17, () => controller.close()); code(17, () => controller.onJointState(seven())); return 0; });
  controller.close(); code(9, () => controller.close()); code(9, () => controller.onTimer(() => 0));
});

const publicApi = [];
for (const file of ['trajectory.mjs', 'sine.mjs', 'recorded_controllers.mjs']) {
  const path = resolve(prototype, file), source = readFileSync(path, 'utf8'), comments = [];
  const tree = acorn.parse(source, { ecmaVersion: 'latest', sourceType: 'module', locations: true, onComment: comments });
  const digest = value => createHash('sha256').update(value).digest('hex');
  function record(name, kind, node, attachment = node) {
    const comment = comments.filter(value => value.end <= attachment.start &&
      /^\s*$/.test(source.slice(value.end, attachment.start))).at(-1);
    assert.ok(comment?.type === 'Block' && comment.value.startsWith('*'), 'exact attached JSDoc required: ' + name);
    for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag), tag + ': ' + name);
    publicApi.push({ file, name, kind, line: node.loc.start.line, source_sha256: digest(source),
      declaration_sha256: digest(source.slice(node.start, node.end)),
      comment_sha256: digest(source.slice(comment.start, comment.end)),
      documentation_scope: 'PARSER_ATTACHMENT_ONLY_SEPARATE_SEMANTIC_REVIEW_REQUIRED' });
  }
  for (const exported of tree.body.filter(node => node.type === 'ExportNamedDeclaration')) {
    const node = exported.declaration;
    if (node.type === 'ClassDeclaration') {
      record(node.id.name, 'class', node, exported);
      for (const method of node.body.body) {
        if (method.type === 'MethodDefinition' && method.key.type === 'Identifier') {
          record(node.id.name + '.' + method.key.name, 'method', method);
        }
      }
    } else if (node.type === 'FunctionDeclaration') record(node.id.name, 'function', node, exported);
    else if (node.type === 'VariableDeclaration') {
      for (const declared of node.declarations) record(declared.id.name, 'constant', declared, exported);
    } else assert.fail('Uninventoried actual export: ' + node.type);
  }
}
assert.equal(publicApi.length, 13);
writeFileSync(options.get('--out'), JSON.stringify({ status: 'PASS_HOST_MODEL_ONLY', cases,
  public_api_inventory: publicApi, actual_sdk_builds: 0, wasm_validation: 0, guest: 0, native_ros: 0,
  original_processes: 0, publication_ready: false }, null, 2) + '\n');
process.stdout.write(JSON.stringify({ status: 'PASS_HOST_MODEL_ONLY', cases: cases.length,
  public_api_inventory_items: publicApi.length, guest: 0, native_ros: 0 }) + '\n');
