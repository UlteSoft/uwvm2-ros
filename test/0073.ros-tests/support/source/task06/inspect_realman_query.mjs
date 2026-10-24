// Whole original values and query timing data model. No ROS/Guest/device stub.
import assert from 'node:assert/strict';
import { readFileSync, writeFileSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { resolve } from 'node:path';
import { pathToFileURL } from 'node:url';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const options = new Map();
for (let index = 2; index < process.argv.length; index += 2) options.set(process.argv[index], process.argv[index + 1]);
const root = resolve(options.get('--root'));
const base = resolve(root, 'sdk/origin/platform/realman/query-core');
const kernelPath = resolve(base, 'prototype/get_arm_state.mjs');
const { ArmStateQuery } = await import(pathToFileURL(kernelPath).href);
const records = await import(pathToFileURL(resolve(root, 'sdk/origin/custom-types/realman-ros1-develop-complete/bounded_records.mjs')).href);
const { Ros1Std_msgsEmptyCodec: empty } = await import(pathToFileURL(resolve(base, 'generated/bounded_records.mjs')).href);
const version = records.Ros1Rm_msgsArm_Software_VersionCodec, quaternion = records.Ros1Rm_msgsArmStateCodec;
const euler = records.Ros1Rm_msgsArm_Current_StateCodec, force = records.Ros1Rm_msgsSix_ForceCodec;
const command = records.Ros1Rm_msgsGetArmState_CommandCodec;
const cases = [];
function test(name, callback) { callback(); cases.push({ name, status: 'PASS', layer: 'ACTUAL_WHOLE_CODEC_HOST_QUERY_MODEL' }); }
function code(expected, callback) { assert.throws(callback, error => error?.code === expected); }
function setup(write = () => 0) {
  const owner = new ArmStateQuery(write), effects = [];
  const publish = (kind, value) => { const codec = kind === 'state' ? command : empty;
    effects.push({ kind, value: codec.decode(codec.encode(value)), bytes: codec.encode(value) }); return 0; };
  return { owner, effects, publish };
}
function ready() {
  const context = setup(); context.owner.onTime(0n, context.publish, () => 0n);
  assert.equal(context.owner.onTime(1000000000n, context.publish, () => 0n).setupRequired, true);
  context.owner.onReady(1000000000n); return context;
}
function textSink() {
  let text = ''; return { write(bytes) { text += new TextDecoder().decode(bytes); return 0; }, text: () => text };
}

test('empty-is-genuine-generated-ROS1-message-with-exact-zero-body', () => {
  assert.equal(empty.minWireBytes, 0); assert.equal(empty.maxWireBytes, 0);
  assert.equal(empty.encode(empty.defaultValue()).length, 0); assert.deepEqual(empty.decode(new Uint8Array()), {});
  code(90, () => empty.decode(new Uint8Array([0])));
});
test('initial-one-second-delay-precedes-endpoint-setup', () => {
  const { owner, publish, effects } = setup();
  assert.equal(owner.onTime(50n, publish, () => 0n).phase, 'waitSetup');
  assert.equal(owner.onTime(1000000049n, publish, () => 0n).setupRequired, false);
  assert.equal(owner.onTime(1000000050n, publish, () => 0n).setupRequired, true);
  assert.equal(effects.length, 0); owner.close();
});
test('setup-and-two-second-wait-do-not-publish-early', () => {
  const { owner, publish, effects } = ready();
  assert.equal(owner.onTime(2999999999n, publish, () => 2999999999n).phase, 'software');
  assert.equal(effects.length, 0); owner.onTime(3000000000n, publish, () => 3000000000n);
  assert.deepEqual(effects.map(row => row.kind), ['software']); assert.equal(effects[0].bytes.length, 0); owner.close();
});
test('all-three-source-publications-keep-full-types-order-and-final-delay', () => {
  const output = textSink(), { owner, publish, effects } = setup(output.write);
  owner.onTime(0n, publish, () => 0n); owner.onTime(1000000000n, publish, () => 0n); owner.onReady(1000000000n);
  for (const now of [3000000000n, 4000000000n, 5000000000n]) owner.onTime(now, publish, () => now);
  assert.deepEqual(effects.map(row => row.kind), ['software', 'state', 'force']);
  assert.deepEqual(effects[1].value, { command: 'get_current_arm_state' });
  assert.equal(owner.onTime(5999999999n, publish, () => 0n).complete, false);
  assert.equal(owner.onTime(6000000000n, publish, () => 0n).complete, true);
  assert.equal(output.text(), '*******Get Arm Software Version Pub show on the rm_driver node.*******\n' +
    '*******Get Arm State Pub*******\n*******Get Arm Six Force Pub*******\n*******All command is over please click ctrl+c end*******\n');
  owner.onTime(9000000000n, publish, () => 0n); assert.equal(effects.length, 3); owner.close();
});
test('late-tick-does-not-catch-up-or-collapse-post-effect-waits', () => {
  const { owner, publish, effects } = ready();
  owner.onTime(90000000000n, publish, () => 95000000000n); assert.equal(effects.length, 1);
  owner.onTime(95999999999n, publish, () => 95999999999n); assert.equal(effects.length, 1);
  owner.onTime(96000000000n, publish, () => 97000000000n); assert.equal(effects.length, 2); owner.close();
});
test('time-and-setup-phase-validation-precede-effects', () => {
  const { owner, publish } = setup(); code(17, () => owner.onReady(0n));
  code(22, () => owner.onTime(0, publish, () => 0n));
  code(34, () => owner.onTime(0xffffffffffffffffn, publish, () => 0n));
  owner.onTime(1n, publish, () => 0n); code(22, () => owner.onTime(0n, publish, () => 0n)); owner.close();
});
test('no-query-replay-after-returned-publication-refusal', () => {
  const { owner, effects } = ready(); let attempts = 0;
  code(13, () => owner.onTime(3000000000n, () => { attempts += 1; return 13; }, () => 3000000000n));
  code(17, () => owner.onTime(4000000000n, () => { attempts += 1; return 0; }, () => 4000000000n));
  assert.equal(attempts, 1); assert.equal(effects.length, 0); owner.close();
});
test('publication-throw-and-promise-do-not-become-success', () => {
  const first = ready(), primary = new Error('query publisher failed');
  assert.throws(() => first.owner.onTime(3000000000n, () => { throw primary; }, () => 3000000000n), error => error === primary);
  code(17, () => first.owner.onTime(4000000000n, first.publish, () => 4000000000n)); first.owner.close();
  const second = ready(); code(22, () => second.owner.onTime(3000000000n, () => Promise.resolve(0), () => 3000000000n)); second.owner.close();
});
test('post-publication-clock-refusal-retains-completed-effect-without-replay', () => {
  const { owner, publish, effects } = ready();
  code(22, () => owner.onTime(3000000000n, publish, () => 2999999999n)); assert.equal(effects.length, 1);
  code(17, () => owner.onTime(4000000000n, publish, () => 4000000000n)); assert.equal(effects.length, 1); owner.close();
});
test('post-effect-clock-overflow-is-not-retroactive-success', () => {
  const { owner, publish, effects } = ready();
  code(34, () => owner.onTime(3000000000n, publish, () => 0xffffffffffffffffn)); assert.equal(effects.length, 1);
  code(17, () => owner.onTime(4000000000n, publish, () => 4000000000n)); owner.close();
});
test('complete-version-callback-prints-all-twelve-original-fields', () => {
  const output = textSink(), { owner } = setup(output.write), value = version.defaultValue();
  value.product_version = 'p'; value.controller_version = 'c'; value.algorithm_info = 'a'; value.dynamic_info = 'd';
  for (const [index, key] of ['ctrl_info', 'com_info', 'program_info', 'plan_info'].entries()) {
    value[key].build_time = 't' + index; value[key].version = 'v' + index;
  }
  owner.onVersion(value);
  assert.equal(output.text(), 'Product_version is: p\ncontroller_version is: c\nalgorithm_info is: a\n' +
    'ctrl_info build_time is: t0\nctrl_info version is: v0\ncom_info build_time is: t1\ncom_info version is: v1\n' +
    'program_info build_time is: t2\nprogram_info version is: v2\ndynamic_info is: d\nplan_info build_time is: t3\nplan_info version is: v3\n'); owner.close();
});
test('whole-version-unprinted-storage-bound-refuses-before-any-output', () => {
  const output = textSink(), { owner } = setup(output.write), value = version.defaultValue();
  value.plan_info.version = 'x'.repeat(257); code(22, () => owner.onVersion(value)); assert.equal(output.text(), ''); owner.close();
});
test('quaternion-callback-keeps-six-joint-label-and-XYZ-XYZW-order', () => {
  const output = textSink(), { owner } = setup(output.write), value = quaternion.defaultValue();
  value.joint = [1, 2, 3, 4, 5, 6]; value.dof = 6;
  value.Pose.position = { x: 7, y: 8, z: 9 }; value.Pose.orientation = { x: 10, y: 11, z: 12, w: 13 };
  owner.onArmState(value); assert.equal(output.text(), 'joint radian state is: [1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000]\n\n' +
    'pose Quaternion is: [7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000]\n\n'); owner.close();
});
test('Euler-callback-keeps-seven-joint-branch-and-complete-six-pose', () => {
  const output = textSink(), { owner } = setup(output.write), value = euler.defaultValue();
  value.joint = [1, 2, 3, 4, 5, 6, 7]; value.dof = 7; value.Pose = [-0, 2, 3, 4, 5, 6];
  owner.onEulerState(value); assert.equal(output.text(), 'joint angle state is: [1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000]\n\n' +
    'pose Euler angle is: [-0.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000]\n\n'); owner.close();
});
test('short-indexed-joints-and-unprinted-error-overflow-are-not-padded', () => {
  const output = textSink(), { owner } = setup(output.write), short = quaternion.defaultValue(), invalid = euler.defaultValue();
  short.joint = [1, 2, 3, 4, 5, 6]; short.dof = 7; code(22, () => owner.onArmState(short));
  invalid.joint = [1, 2, 3, 4, 5, 6]; invalid.err = [65536]; code(22, () => owner.onEulerState(invalid));
  assert.equal(output.text(), ''); owner.close();
});
for (const [kind, label] of [['raw', 'Six force'], ['sensorZero', 'Zero six force'], ['workZero', 'Work zro force'], ['toolZero', 'Tool zero force']]) {
  test('complete-distinct-force-topic-role-' + kind, () => {
    const output = textSink(), { owner } = setup(output.write), value = force.defaultValue();
    Object.assign(value, { force_Fx: 1, force_Fy: -0, force_Fz: 3, force_Mx: 4, force_My: 5, force_Mz: 6 });
    owner.onForce(kind, value); assert.equal(output.text(), label + ' value is: [force_Fx:1.000000,\n force_Fy:-0.000000,\n force_Fz:3.000000,\n force_Mx:4.000000,\n force_My:5.000000,\n force_Mz:6.000000]\n\n'); owner.close();
  });
}
test('last-force-component-and-role-are-validated-before-output', () => {
  const output = textSink(), { owner } = setup(output.write), value = force.defaultValue(); value.force_Mz = NaN;
  code(22, () => owner.onForce('raw', value)); code(22, () => owner.onForce('merged', force.defaultValue()));
  assert.equal(output.text(), ''); owner.close();
});
test('callback-writer-error-retains-primary-cause-with-one-attempt', () => {
  let calls = 0; const primary = new Error('basic output error');
  const { owner } = setup(() => { calls += 1; throw primary; });
  assert.throws(() => owner.onVersion(version.defaultValue()), error => error.code === 5 && error.cause === primary);
  assert.equal(calls, 1); owner.close();
});
test('reentry-retirement-and-foreign-owner-refuse-before-effects', () => {
  let owner; ({ owner } = setup(() => { code(17, () => owner.close()); code(17, () => owner.onVersion(version.defaultValue())); return 0; }));
  owner.onVersion(version.defaultValue()); owner.close(); code(9, () => owner.close());
  code(9, () => owner.onArmState(quaternion.defaultValue())); code(9, () => ArmStateQuery.prototype.onReady.call({}, 0n));
});

const inventory = [];
for (const path of [kernelPath, resolve(base, 'prototype/api_get_arm_state.mjs')]) {
  const source = readFileSync(path, 'utf8'), comments = [];
  const tree = acorn.parse(source, { ecmaVersion: 'latest', sourceType: 'module', locations: true, onComment: comments });
  const hash = value => createHash('sha256').update(value).digest('hex');
  function declaration(name, kind, node, attachment = node) {
    const comment = comments.filter(value => value.end <= attachment.start && /^\s*$/.test(source.slice(value.end, attachment.start))).at(-1);
    assert.ok(comment?.type === 'Block' && comment.value.startsWith('*'), 'actual attached JSDoc required: ' + name);
    for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag), name + ': ' + tag);
    inventory.push({ path, name, kind, line: node.loc.start.line, source_sha256: hash(source),
      declaration_sha256: hash(source.slice(node.start, node.end)), comment_sha256: hash(source.slice(comment.start, comment.end)),
      semantic_review: 'Separate substantive API review required; parser tags alone do not qualify completeness' });
  }
  for (const exported of tree.body.filter(node => node.type === 'ExportNamedDeclaration')) {
    const node = exported.declaration;
    if (node.type === 'ClassDeclaration') {
      declaration(node.id.name, 'class', node, exported);
      for (const method of node.body.body) declaration(node.id.name + '.' + method.key.name, 'method', method);
    } else if (node.type === 'FunctionDeclaration') declaration(node.id.name, 'function', node, exported);
    else assert.fail('Uninventoried original RealMan query API');
  }
}
assert.equal(inventory.length, 10);
writeFileSync(options.get('--out'), JSON.stringify({ status: 'PASS_COMPLETE_HOST_REALMAN_QUERY_ONLY', cases,
  public_api_inventory: inventory, sdk_builds: 0, wasm_guest: 0, native_ros: 0, physical_motion: false }, null, 2) + '\n');
process.stdout.write(JSON.stringify({ status: 'PASS_COMPLETE_HOST_REALMAN_QUERY_ONLY', cases: cases.length, public_api_items: inventory.length }) + '\n');
