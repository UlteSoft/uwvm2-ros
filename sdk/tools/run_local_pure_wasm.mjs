/**
 * @file run_local_pure_wasm.mjs
 * @brief Execute an already compiled, validated SDK model from a /tmp run.
 *
 * This local developer lane is separate from Linux cgroup acceptance. It
 * creates no ROS host, WASI implementation, network endpoint or authority.
 * The parent serial runner supplies its process/RSS/deadline limits. Each
 * artifact must belong to that parent's successful compiler report, retain
 * its recorded SHA-256, and contain zero imports before instantiation.
 *
 * Usage: node --max-old-space-size=96 run_local_pure_wasm.mjs
 *        /tmp/fresh-run/wasm-results.json /tmp/fresh-run/pure-models.json
 * Run once per frozen compiler report. The output path must not exist.
 */
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { expectedChecks } from '../javascript/tests/run_pure_wasm.mjs';

/** Accept only files beneath /tmp, including macOS's /private/tmp spelling. */
function temporary(name) {
  const root = fs.realpathSync('/tmp');
  const resolved = path.resolve(name);
  assert.ok(resolved.startsWith(root + path.sep) || resolved.startsWith('/tmp/'));
  return resolved;
}

const report = temporary(process.argv[2]), output = temporary(process.argv[3]);
const root = fs.realpathSync(path.dirname(report));
const rows = JSON.parse(fs.readFileSync(report, 'utf8'));
const selected = rows.filter(row => row.language === 'assemblyscript' && row.backend === 'none');
assert.equal(selected.length, Object.keys(expectedChecks).length);
assert.equal(new Set(selected.map(row => path.basename(row.source, '.ts'))).size, selected.length);
const results = [];
for (const row of selected) {
  const fixture = path.basename(row.source, '.ts');
  assert.ok(Object.hasOwn(expectedChecks, fixture));
  assert.equal(row.status, 'COMPILE_IMPORT_FULL_VALIDATION_PASS');
  const artifact = fs.realpathSync(row.artifact);
  assert.ok(artifact.startsWith(root + path.sep));
  const descriptor = fs.openSync(artifact, fs.constants.O_RDONLY | fs.constants.O_NONBLOCK);
  let bytes;
  try {
    const before = fs.fstatSync(descriptor, { bigint: true });
    assert.ok(before.isFile() && before.size <= 67108864n);
    bytes = fs.readFileSync(descriptor);
    const after = fs.fstatSync(descriptor, { bigint: true });
    for (const field of ['dev', 'ino', 'size', 'mtimeNs', 'ctimeNs']) assert.equal(after[field], before[field]);
  } finally { fs.closeSync(descriptor); }
  const digest = crypto.createHash('sha256').update(bytes).digest('hex');
  assert.equal(digest, row.artifact_sha256);
  const module = new WebAssembly.Module(bytes);
  assert.deepEqual(WebAssembly.Module.imports(module), []);
  const instance = new WebAssembly.Instance(module, {}), expected = expectedChecks[fixture];
  assert.equal(typeof instance.exports._start, 'function');
  if (expected !== null) {
    assert.equal(typeof instance.exports.completedChecks, 'function');
    assert.equal(instance.exports.completedChecks(), 0);
  }
  instance.exports._start();
  const completed = expected === null ? null : instance.exports.completedChecks();
  assert.equal(completed, expected);
  results.push({ fixture, artifact_sha256: digest, completed_checks: completed,
    status: expected === null ? 'SEMANTIC_MODEL_EXECUTED' : 'PURE_ASSERTIONS_PASS' });
}
const receipt = { status: 'LOCAL_IMPORT_FREE_WASM_MODELS_PASS',
  scope: 'Guest-local SDK models only; no native ROS, Runtime permission or Linux cgroup qualification',
  compile_report_sha256: crypto.createHash('sha256').update(fs.readFileSync(report)).digest('hex'),
  model_count: results.length, assertion_count: results.reduce((n, row) => n + (row.completed_checks ?? 0), 0),
  actual_ROS_delivery: 'NOT_EXECUTED', results };
fs.writeFileSync(output, JSON.stringify(receipt, null, 2) + '\n', { flag: 'wx' });
console.log(JSON.stringify({ status: receipt.status, models: receipt.model_count, assertions: receipt.assertion_count }));
