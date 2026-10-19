/** Source-side option models only: importing the runner never compiles Wasm.
 * Actual AssemblyScript assertions require Linux64GiB and compiled artifacts.
 */
import assert from 'node:assert/strict';
import {options, expectedChecks} from './run_pure_wasm.mjs';
let checks = 0;
const argv = ['--wasm', '/tmp/a.wasm', '--sha256', 'a'.repeat(64),
  '--fixture', 'codec_vectors', '--expected-leaf', '/sys/fs/cgroup/x',
  '--expected-device', '29', '--expected-inode', '18', '--expected-boot', 'boot',
  '--expected-memory-max', '68719476736', '--expected-swap-max', '0', '--expected-cpus', '0-31'];
assert.equal(options(argv).fixture, 'codec_vectors'); ++checks;
assert.equal(options(argv)['expected-memory-max'], '68719476736'); ++checks;
assert.equal(Object.values(expectedChecks).filter(x => x !== null).reduce((a, b) => a + b, 0), 466); ++checks;
// Preserve every previously admitted fixture and the explicit event-dispatch
// model. Refuse unknown names; missing artifacts never become implicit skips.
const fixtures=['codec_vectors','adapter_contracts','executor_mutation','factory_transactions',
  'resource_roles','unicode_strings','highlevel_coverage','local_schedule','server_callbacks','unified_events','event_dispatch'];
assert.deepEqual(Object.keys(expectedChecks),fixtures); ++checks;
assert.equal(expectedChecks.factory_transactions,75); ++checks;
assert.equal(expectedChecks.local_schedule,47); ++checks;
assert.equal(expectedChecks.server_callbacks,60); ++checks;
assert.equal(expectedChecks.unified_events,33); ++checks;
assert.equal(expectedChecks.event_dispatch,20); ++checks;
for(const fixture of fixtures) {assert.equal(options(argv.map(x=>x==='codec_vectors'?fixture:x)).fixture,fixture); ++checks;}
for (const bad of [[], argv.slice(0, -1), [...argv, '--fixture', 'codec_vectors'],
  [...argv, '--unknown', 'x'], argv.map(x => x === 'codec_vectors' ? 'coverage_ros2' : x),
  argv.map(x => x === '/tmp/a.wasm' ? 'a.wasm' : x), argv.map(x => x === '29' ? 'x' : x),
  argv.map(x => x === 'a'.repeat(64) ? 'b' : x),
  argv.map(x => x === '68719476736' ? '51539607552' : x),
  argv.map(x => x === '68719476736' ? '34359738368' : x),
  argv.map(x => x === '0' ? 'max' : x),
  argv.map(x => x === '0-31' ? '0-15' : x),
  argv.map(x => x === 'boot' ? '' : x)]) {
  assert.throws(() => options(bad)); ++checks;
}
assert.equal(expectedChecks.highlevel_coverage, null); ++checks;
console.log(JSON.stringify({status: 'PURE_OPTION_MODEL_PASS', checks,
  Wasm_compilation: 'NOT_PERFORMED', Wasm_execution: 'NOT_PERFORMED'}));
