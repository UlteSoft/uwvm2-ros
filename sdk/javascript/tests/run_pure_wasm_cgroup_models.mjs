/** Pure filesystem models for the standalone model runner's cgroup admission.
 *
 * This file evaluates only the existing cgroupProof function with a mocked
 * filesystem and process identity. It neither reads a live cgroup nor invokes
 * WebAssembly compilation, instantiation, a guest, a compiler or ROS. The
 * historical Linux evidence records no memory.max file at the hierarchy root;
 * that absence must coexist with strict required files below the root.
 */
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import {fileURLToPath} from 'node:url';

const source = fs.readFileSync(new URL('./run_pure_wasm.mjs', import.meta.url), 'utf8');
const begin = source.indexOf('function cgroupProof(args) {');
const end = source.indexOf('\n/** Read at most64MiB', begin);
assert.ok(begin >= 0 && end > begin, 'The admission function must have one known source boundary');
const functionSource = source.slice(begin, end);
assert.ok(!/WebAssembly|child_process|\.artifact\(/.test(functionSource),
  'The extracted function must remain admission-only');

const root = '/sys/fs/cgroup';
const parent = root + '/system.slice';
const leaf = parent + '/test.scope';
let checks = 0;

/** Make a deterministic cgroup-v2 view; root memory.max is absent by default. */
function fixture() {
  const files = new Map([
    ['/proc/self/cgroup', '0::/system.slice/test.scope\n'],
    [leaf + '/cgroup.procs', '123\n'],
    [leaf + '/memory.max', '68719476736\n'],
    [parent + '/memory.max', 'max\n'],
    [leaf + '/memory.swap.max', '0\n'],
    [leaf + '/cpuset.cpus.effective', '0-31\n'],
    ['/proc/sys/kernel/random/boot_id', 'model-boot\n'],
  ]);
  const errors = new Map();
  const readPaths = [];
  const args = {'expected-leaf': leaf, 'expected-device': '29',
    'expected-inode': '18873', 'expected-boot': 'model-boot',
    'expected-memory-max': '68719476736', 'expected-swap-max': '0', 'expected-cpus': '0-31'};
  const context = {
    path,
    process: {platform: 'linux', pid: 123},
    fs: {
      realpathSync: value => value,
      statSync: () => ({dev: 29n, ino: 18873n}),
      readFileSync(value) {
        readPaths.push(value);
        const code = errors.get(value) ?? (files.has(value) ? undefined : 'ENOENT');
        if (code) {
          const error = new Error(code + ': ' + value);
          error.code = code;
          throw error;
        }
        return files.get(value);
      },
    },
  };
  // The function is the runner's actual current source. The fake filesystem
  // accepts no operations beyond these reads, identity queries and realpaths.
  const admit = vm.runInNewContext(functionSource + '\ncgroupProof', context);
  return {files, errors, readPaths, args, context, admit};
}

/** Check one positive model and retain the explicit identity it admitted. */
function accepts(change = () => {}) {
  const model = fixture();
  change(model);
  const result = model.admit(model.args);
  assert.equal(result.leaf, leaf);
  assert.equal(result.device, '29');
  assert.equal(result.inode, '18873');
  assert.equal(result.boot, 'model-boot');
  assert.equal(result.pid, 123);
  assert.equal(result.expected_memory_max, '68719476736');
  assert.equal(result.leaf_memory_max, '68719476736');
  assert.equal(result.effective_memory_max, '68719476736');
  assert.equal(result.swap_max, '0');
  checks++;
  return model;
}

/** Check one negative model; failures may never become an effective limit. */
function refuses(change) {
  const model = fixture();
  change(model);
  assert.throws(() => model.admit(model.args));
  checks++;
}

// Root file absence reflects the actual supplied Linux host. A present root
// file still participates in parsing/minimum selection and may not be ignored.
accepts();
accepts(model => model.files.set(root + '/memory.max', 'max\n'));
accepts(model => {
  model.files.set(parent + '/memory.max', '68719476736\n');
});
refuses(model => model.files.set(root + '/memory.max', 'not-a-limit\n'));
refuses(model => model.files.set(root + '/memory.max', '34359738368\n'));
refuses(model => model.errors.set(root + '/memory.max', 'EACCES'));
refuses(model => model.errors.set(root + '/memory.max', 'EIO'));
refuses(model => model.files.delete(parent + '/memory.max'));
refuses(model => model.files.delete(leaf + '/memory.max'));
refuses(model => model.files.set(parent + '/memory.max', '34359738368\n'));
refuses(model => model.files.set(leaf + '/memory.max', 'not-a-limit\n'));
refuses(model => {
  model.files.set(leaf + '/memory.max', 'max\n');
  model.files.set(parent + '/memory.max', 'max\n');
});
refuses(model => model.args['expected-device'] = '30');
refuses(model => model.args['expected-inode'] = '18874');
refuses(model => model.args['expected-leaf'] = parent + '/other.scope');
refuses(model => model.files.set(leaf + '/cgroup.procs', '124\n'));
refuses(model => model.files.set('/proc/self/cgroup', '0::/system.slice/test.scope\n1:memory:/other\n'));
refuses(model => model.files.set(leaf + '/memory.swap.max', 'max\n'));
refuses(model => model.files.set(leaf + '/cpuset.cpus.effective', '0-15\n'));
refuses(model => model.files.set('/proc/sys/kernel/random/boot_id', 'changed-boot\n'));
refuses(model => model.context.process.platform = 'darwin');
// A previous48GiB leaf, an inherited-only64GiB limit, or relabelled control
// input must never qualify the newly selected exact64GiB native leaf.
refuses(model => model.files.set(leaf + '/memory.max', '51539607552\n'));
refuses(model => model.args['expected-memory-max'] = '51539607552');
refuses(model => model.args['expected-memory-max'] = '34359738368');
refuses(model => model.args['expected-swap-max'] = 'max');
refuses(model => model.args['expected-boot'] = '');
refuses(model => {
  model.files.set(leaf + '/memory.max', 'max\n');
  model.files.set(parent + '/memory.max', '68719476736\n');
});
refuses(model => {
  let calls = 0;
  model.context.fs.statSync = () => ({dev: 29n, ino: ++calls === 1 ? 18873n : 18874n});
});
refuses(model => {
  const read = model.context.fs.readFileSync;
  let reads = 0;
  model.context.fs.readFileSync = value => value === '/proc/sys/kernel/random/boot_id'
    ? (++reads === 1 ? 'model-boot\n' : 'changed-boot\n') : read(value);
});

// This single-purpose entry point reports source/mock model checks only. It
// intentionally has no artifact argument or route to the runner's run function.
if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  console.log(JSON.stringify({status: 'SOURCE_CGROUP_MODEL_PASS', checks,
    scope: 'Extracted admission function with mocked filesystem and process only',
    live_cgroup_execution: 'NOT_PERFORMED', WebAssembly_execution: 'NOT_PERFORMED',
    native_compilation: 'NOT_PERFORMED', ROS_execution: 'NOT_PERFORMED'}));
}
