// Genuine source graph/AST inspection only. Never evaluate an SDK module.
import assert from 'node:assert/strict';
import { readFileSync, realpathSync, writeFileSync } from 'node:fs';
import { dirname, resolve, relative, sep } from 'node:path';
import { createHash } from 'node:crypto';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const [rootArgument, referenceArgument, output, ...extra] = process.argv.slice(2);
assert.ok(extra.length === 0 || extra.length === 1 && extra[0] === '--include-dobot', 'Exact optional DOBOT graph selector required');
const root = realpathSync(rootArgument), reference = realpathSync(referenceArgument);
const hash = value => createHash('sha256').update(value).digest('hex');
const pairs = [
  ['include/platform/elephant/js/teleop.mjs', 'elephant/javascript/include/teleop.mjs'],
  ['example/platform/elephant/js/official/teleop_keyboard.mjs', 'elephant/javascript/example/official/teleop_keyboard.mjs'],
  ['include/platform/ufactory/js/robot_states.mjs', 'ufactory/javascript/include/robot_states.mjs'],
  ['example/platform/ufactory/js/official/test_robot_states.mjs', 'ufactory/javascript/example/official/test_robot_states.mjs'],
  ['include/platform/flexiv/js/decimal.mjs', 'flexiv/javascript/include/decimal.mjs'],
  ['include/platform/flexiv/js/monitor.mjs', 'flexiv/javascript/include/monitor.mjs'],
  ['example/platform/flexiv/js/official/robot_states_monitor.mjs', 'flexiv/javascript/example/official/robot_states_monitor.mjs'],
  ['include/platform/flexiv/js/trajectory.mjs', 'flexiv/controller-core/prototype/trajectory.mjs'],
  ['include/platform/flexiv/js/sine.mjs', 'flexiv/controller-core/prototype/sine.mjs'],
  ['example/platform/flexiv/js/official/recorded_controllers.mjs', 'flexiv/controller-core/prototype/recorded_controllers.mjs'],
  ['include/platform/realman/js/get_arm_state.mjs', 'realman/query-core/prototype/get_arm_state.mjs'],
  ['example/platform/realman/js/official/api_get_arm_state.mjs', 'realman/query-core/prototype/api_get_arm_state.mjs'],
];
if (extra.length) pairs.push(
  ['include/platform/dobot/js/demo_cycle.mjs', 'dobot/javascript/include/demo_cycle.mjs'],
  ['example/platform/dobot/js/official/demo_cycle.mjs', 'dobot/javascript/example/official/demo_cycle.mjs'],
);
function parse(path) {
  const source = readFileSync(path, 'utf8');
  return { source, tree: acorn.parse(source, { ecmaVersion: 'latest', sourceType: 'module' }) };
}
function normalized(tree) {
  // Only ESM source spellings may differ; order/specifiers/all program AST stay.
  for (const node of tree.body) {
    if (['ImportDeclaration', 'ExportAllDeclaration', 'ExportNamedDeclaration'].includes(node.type) && node.source) node.source.value = 'EXPLICIT_CANONICAL_IMPORT_REBIND';
  }
  return JSON.stringify(tree, (key, value) => ['start', 'end', 'raw'].includes(key) ? undefined :
    typeof value === 'bigint' ? { actualBigInt: value.toString() } : value);
}
const compared = [];
for (const [installed, original] of pairs) {
  const current = parse(resolve(root, installed)), before = parse(resolve(reference, 'sdk/origin/platform', original));
  assert.ok(normalized(current.tree) === normalized(before.tree), 'program AST changed outside canonical import paths: ' + installed);
  compared.push({ installed, original, original_sha256: hash(before.source), installed_sha256: hash(current.source) });
}
const builtins = new Set(['roswasm:utf8', 'roswasm:guest_native']);
const modules = new Map(), work = pairs.filter(([name]) => name.startsWith('example/')).map(([name]) => resolve(root, name));
work.push(resolve(root, 'include/platform/ros/js/guest/bootstrap.mjs'));
while (work.length) {
  const path = work.pop();
  if (modules.has(path)) continue;
  assert.equal(realpathSync(path), path, 'ordinary actual canonical source required');
  const logical = relative(root, path);
  assert.ok(!logical.startsWith('..' + sep) && !logical.split(sep).some(part => ['origin', 'test', 'tests', '.xmake'].includes(part)));
  const parsed = parse(path), edges = [];
  for (const node of parsed.tree.body) {
    if (['ImportDeclaration', 'ExportAllDeclaration', 'ExportNamedDeclaration'].includes(node.type) && node.source) {
      const spelling = node.source.value;
      if (builtins.has(spelling)) edges.push({ spelling, builtin: true });
      else {
        assert.ok(spelling.startsWith('.'), 'unselected external module: ' + spelling);
        const target = resolve(dirname(path), spelling);
        assert.ok(target.startsWith(root + sep), 'canonical import escapes actual source tree');
        edges.push({ spelling, target: relative(root, target) }); work.push(target);
      }
    }
  }
  modules.set(path, { path: logical, bytes: Buffer.byteLength(parsed.source), sha256: hash(parsed.source), edges });
  assert.ok(modules.size <= 64, 'real recipe module bound');
}
writeFileSync(output, JSON.stringify({ status: 'PASS_CANONICAL_SOURCE_GRAPH_ONLY', authored_ast_pairs: compared,
  complete_module_graph: Array.from(modules.values()), source_modules: modules.size,
  builtin_contract: 'Actual r2 C engine module names; no Node import/evaluation or bridge substitute',
  actual_sdk_builds: 0, wasm_guest: 0, native_ros: 0, publication_ready: false }, null, 2) + '\n');
