// Pure diagnostic-data checks over the actual parser-selected helper bodies.
// No SDK, ROS adapter, host import, engine substitute or Guest is instantiated.
import assert from 'node:assert/strict';
import {readFileSync, writeFileSync} from 'node:fs';
import {createHash} from 'node:crypto';
import {compileFunction} from 'node:vm';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

if (process.platform !== 'linux' || process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== '1')
  throw new Error('same finite Linux owner/common lock required');
const options = new Map();
for (let at = 2; at < process.argv.length; at += 2) options.set(process.argv[at], process.argv[at + 1]);
const paths = JSON.parse(readFileSync(options.get('--sources'), 'utf8'));
assert.equal(paths.length, 6);
const cases = [], pins = [], hash = body => createHash('sha256').update(body).digest('hex');
for (const path of [process.argv[1], '/usr/bin/node', '/usr/share/nodejs/acorn/dist/acorn.mjs', ...paths]) {
  const body = readFileSync(path);
  pins.push({path, bytes: body.length, sha256: hash(body)});
}
for (const path of paths) {
  const source = readFileSync(path, 'utf8');
  const tree = acorn.parse(source, {ecmaVersion: 'latest', sourceType: 'module', locations: true});
  const declaration = tree.body.find(node => node.type === 'FunctionDeclaration' && node.id.name === 'failureRecord');
  assert.ok(declaration, 'Exact private failureRecord declaration required');
  // Compile only this exact source slice. No import is resolved or replaced,
  // no complete application is executed and no transport is constructed.
  const helper = compileFunction('return (' + source.slice(declaration.start, declaration.end) + ');', [],
    {filename: path + ':actual-private-diagnostic-only'})();
  function test(name, callback) {
    callback(); cases.push({path, name, status: 'PASS', layer: 'ACTUAL_PRIVATE_DIAGNOSTIC_DATA_ONLY'});
  }
  test('unsigned-returned-status-retains-exact-value', () => {
    for (const value of [0, 9, 13, 22, 125, 0xffffffff]) assert.equal(helper(value).code, value);
  });
  test('primary-before-cleanup-does-not-become-secondary-denial', () => {
    const primary = Object.assign(new Error('invalid complete input'), {code: 22});
    const cleanup = Object.assign(new Error('close denied'), {statuses: [0, 13]});
    const value = helper(new AggregateError([primary, cleanup], 'primary then cleanup'));
    assert.equal(value.errors[0].code, 22);
    assert.equal(value.errors[1].statuses[1].code, 13);
    assert.equal(value.code, null);
  });
  test('actual-SDK-results-and-attempt-shapes-remain-visible', () => {
    const failed = Object.assign(new Error('delegate threw'), {code: 5});
    const cleanup = Object.assign(new AggregateError([failed], 'cleanup'), {
      results: [0, 9, failed], attempts: [
        {status: 0, error: null, attempted: true, retained: false, threw: false},
        {status: 9, error: null, attempted: true, retained: false, threw: false},
        {status: null, error: failed, attempted: true, retained: false, threw: true}
      ]});
    const value = helper(cleanup);
    assert.equal(value.results[1].code, 9);
    assert.equal(value.attempts[1].status, 9);
    assert.equal(value.attempts[2].threw, true);
    assert.equal(value.attempts[2].error.code, 5);
    assert.equal(value.attempts[0].retained, false);
  });
  test('factory-rollback-keeps-primary-and-returned-cleanup-journal', () => {
    const value = helper(Object.assign(new Error('open'), {code: 13, cleanupFailures: [9],
      cleanupAttempts: [{status: 9, attempted: true, retained: false, threw: false}]}));
    assert.equal(value.code, 13); assert.equal(value.cleanupFailures[0].code, 9);
    assert.equal(value.cleanupAttempts[0].status, 9);
  });
  test('causes-and-messages-are-bounded-with-no-BigInt-serialization', () => {
    const value = helper(Object.assign(new Error('x'.repeat(4096)), {cause: {handle: 7n, status: 13}}));
    assert.equal(value.message.length, 256); assert.equal(value.cause.status, 13);
    assert.doesNotThrow(() => JSON.stringify(value));
  });
  test('deep-or-large-diagnostic-data-is-explicitly-truncated', () => {
    const value = helper({errors: Array.from({length: 20}, () => ({cause: {cause: {cause: {code: 13}}}}))});
    assert.equal(value.errors.length, 16); assert.equal(value.errorsTruncated, 20);
    assert.equal(value.errors[0].cause.cause.cause.truncated, true);
  });
  test('missing-fields-do-not-invent-returned-native-status', () => {
    const value = helper(new Error('engine failure'));
    assert.equal(value.code, null); assert.equal(value.status, undefined);
    assert.equal(value.results, undefined);
  });
}
for (const row of pins) assert.equal(hash(readFileSync(row.path)), row.sha256, 'immutable input/tool changed');
const summary = {status: 'PASS_PRIVATE_DIAGNOSTIC_DATA_ONLY', cases, source_pins: pins,
  actual_application_runs: 0, sdk_builds: 0, wasm_validation: 0, guest: 0, native_ros: 0,
  actual_native_cleanup_failure_paths: 0, source_extraction_scope: 'Exact Acorn function slice only; no application/module execution'};
writeFileSync(options.get('--out'), JSON.stringify(summary, null, 2) + '\n');
process.stdout.write(JSON.stringify({status: summary.status, helper_bodies: paths.length, cases: cases.length}) + '\n');
