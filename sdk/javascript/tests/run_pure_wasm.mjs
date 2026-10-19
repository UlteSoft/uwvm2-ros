/** Execute one compiled, import-free AssemblyScript SDK model in Linux64GiB.
 *
 * Invoke this entry point only through tools/run_cgroup_models.py. That parent
 * binds Node and the input artifact, samples owned processes and memory, and
 * enforces a deadline. This additional check refuses a direct invocation outside
 * the explicitly pinned cgroup. No ROS bridge, WASI imports or host callbacks
 * are supplied to the module. A pass concerns guest-local model assertions only.
 */
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {fileURLToPath} from 'node:url';

export const expectedChecks = Object.freeze({
  codec_vectors: 175,
  adapter_contracts: 14,
  executor_mutation: 9,
  factory_transactions: 75,
  resource_roles: 25,
  unicode_strings: 8,
  highlevel_coverage: null,
  local_schedule: 47,
  server_callbacks: 60,
  unified_events: 33,
  event_dispatch: 20,
});

/** Parse a closed option set; missing/duplicate values never select defaults. */
export function options(argv) {
  const allowed = new Set(['wasm', 'sha256', 'fixture', 'expected-leaf',
    'expected-device', 'expected-inode', 'expected-boot', 'expected-memory-max', 'expected-swap-max', 'expected-cpus']);
  const result = {};
  for (let i = 0; i < argv.length; i += 2) {
    const key = argv[i]?.slice(2);
    if (!argv[i]?.startsWith('--') || !allowed.has(key) || key in result || i + 1 >= argv.length) {
      throw new Error('Unknown, duplicate or incomplete option');
    }
    result[key] = argv[i + 1];
  }
  for (const key of allowed) if (!(key in result)) throw new Error('Missing --' + key);
  if (!Object.hasOwn(expectedChecks, result.fixture)) throw new Error('Unknown pure fixture');
  if (!/^[a-f0-9]{64}$/.test(result.sha256)) throw new Error('Invalid expected SHA-256');
  for (const key of ['expected-device', 'expected-inode']) {
    if (!/^[0-9]+$/.test(result[key])) throw new Error('Invalid cgroup identity');
  }
  if (result['expected-memory-max'] !== '68719476736' || result['expected-swap-max'] !== '0'
      || result['expected-cpus'] !== '0-31' || !result['expected-boot']) {
    throw new Error('Exact current64GiB/zero-swap/cpuset/boot controls required');
  }
  if (!path.isAbsolute(result.wasm) || !path.isAbsolute(result['expected-leaf'])) {
    throw new Error('Artifact and cgroup paths must be absolute');
  }
  return result;
}

/** Prove current membership and effective memory limit, not a copied config. */
function cgroupProof(args) {
  if (process.platform !== 'linux') throw new Error('Execution requires Linux');
  if (args['expected-memory-max'] !== '68719476736' || args['expected-swap-max'] !== '0'
      || args['expected-cpus'] !== '0-31' || !args['expected-boot']) {
    throw new Error('Exact current64GiB/zero-swap/cpuset/boot controls required');
  }
  const bootBefore = fs.readFileSync('/proc/sys/kernel/random/boot_id', 'utf8').trim();
  const membership = fs.readFileSync('/proc/self/cgroup', 'utf8').trim().split('\n');
  if (membership.length !== 1 || !membership[0].startsWith('0::/')) {
    throw new Error('A single cgroup-v2 membership is required');
  }
  const leaf = fs.realpathSync('/sys/fs/cgroup' + membership[0].slice(3));
  const expectedLeaf = fs.realpathSync(args['expected-leaf']);
  const stat = fs.statSync(leaf, {bigint: true});
  if (leaf !== expectedLeaf || stat.dev !== BigInt(args['expected-device']) || stat.ino !== BigInt(args['expected-inode'])) {
    throw new Error('Current cgroup leaf differs from the pinned incarnation');
  }
  if (!fs.readFileSync(path.join(leaf, 'cgroup.procs'), 'utf8').trim().split(/\s+/).includes(String(process.pid))) {
    throw new Error('Current PID is absent from the pinned cgroup');
  }
  const root = fs.realpathSync('/sys/fs/cgroup');
  let effective = null;
  for (let current = leaf;; current = path.dirname(current)) {
    if (current !== root && !current.startsWith(root + '/')) throw new Error('Invalid cgroup hierarchy');
    let value;
    try { value = fs.readFileSync(path.join(current, 'memory.max'), 'utf8').trim(); }
    catch (error) {
      // The cgroup-v2 hierarchy root normally has no memory.max. Its absent
      // file is not a finite limit. Every nonroot controller file must remain
      // readable; skipping one would conceal an unavailable permission proof.
      if (current === root && error.code === 'ENOENT') break;
      throw error;
    }
    if (value !== 'max') {
      if (!/^[0-9]+$/.test(value)) throw new Error('Invalid memory.max');
      const finite = BigInt(value);
      if (effective === null || finite < effective) effective = finite;
    }
    if (current === root) break;
  }
  if (effective !== 68719476736n
      || fs.readFileSync(path.join(leaf, 'memory.max'), 'utf8').trim() !== args['expected-memory-max']) {
    throw new Error('Actual leaf and effective memory.max must both equal64GiB');
  }
  for (const [file, option] of [['memory.swap.max', 'expected-swap-max'], ['cpuset.cpus.effective', 'expected-cpus']]) {
    if (fs.readFileSync(path.join(leaf, file), 'utf8').trim() !== args[option]) throw new Error('Cgroup ' + file + ' changed');
  }
  const boot = fs.readFileSync('/proc/sys/kernel/random/boot_id', 'utf8').trim();
  const after = fs.statSync(leaf, {bigint: true});
  if (boot !== args['expected-boot'] || bootBefore !== boot || after.dev !== stat.dev || after.ino !== stat.ino
      || fs.realpathSync(args['expected-leaf']) !== leaf
      || fs.readFileSync('/proc/self/cgroup', 'utf8').trim().split('\n').join('\n') !== membership.join('\n')
      || !fs.readFileSync(path.join(leaf, 'cgroup.procs'), 'utf8').trim().split(/\s+/).includes(String(process.pid))
      || fs.readFileSync(path.join(leaf, 'memory.max'), 'utf8').trim() !== args['expected-memory-max']
      || fs.readFileSync(path.join(leaf, 'memory.swap.max'), 'utf8').trim() !== '0'
      || fs.readFileSync(path.join(leaf, 'cpuset.cpus.effective'), 'utf8').trim() !== args['expected-cpus']) {
    throw new Error('Cgroup leaf/boot/membership/limit incarnation changed');
  }
  return {leaf, device: String(stat.dev), inode: String(stat.ino), boot, pid: process.pid,
    expected_memory_max: args['expected-memory-max'], leaf_memory_max: args['expected-memory-max'],
    effective_memory_max: String(effective), swap_max: '0', cpus_effective: args['expected-cpus']};
}

/** Read at most64MiB from one opened regular file and reject concurrent edits.
 * Open nonblocking before checking the descriptor. A pathname stat followed by
 * a blocking open would leave a replacement FIFO able to stall the controller.
 * Refuse platforms lacking that flag; same-descriptor checks are consistency
 * evidence around this read, not a filesystem-wide lock or signed attestation.
 */
function artifact(args) {
  const nonblocking = fs.constants.O_NONBLOCK;
  if (!Number.isInteger(nonblocking) || nonblocking <= 0) throw new Error('Artifact reads require O_NONBLOCK support');
  const fd = fs.openSync(args.wasm, fs.constants.O_RDONLY | nonblocking);
  try {
    const before = fs.fstatSync(fd, {bigint: true});
    if (!before.isFile() || before.size < 0n || before.size > 67108864n) throw new Error('Artifact must be a bounded regular file');
    const bytes = Buffer.alloc(Number(before.size));
    let offset = 0;
    while (offset < bytes.length) {
      const count = fs.readSync(fd, bytes, offset, bytes.length - offset, offset);
      if (!count) throw new Error('Truncated artifact');
      offset += count;
    }
    const after = fs.fstatSync(fd, {bigint: true});
    if (!after.isFile()) throw new Error('Artifact changed file kind during read');
    for (const field of ['dev', 'ino', 'size', 'mtimeNs', 'ctimeNs']) {
      if (before[field] !== after[field]) throw new Error('Artifact changed during read');
    }
    const sha256 = crypto.createHash('sha256').update(bytes).digest('hex');
    if (sha256 !== args.sha256) throw new Error('Artifact SHA-256 differs from compiler evidence');
    return {bytes, sha256};
  } finally { fs.closeSync(fd); }
}

/** Instantiate only after every import is refused; invoke the model once. */
export function run(args) {
  const before = cgroupProof(args);
  const input = artifact(args);
  const module = new WebAssembly.Module(input.bytes);
  const imports = WebAssembly.Module.imports(module);
  if (imports.length) throw new Error('Pure fixture has imports: ' + JSON.stringify(imports));
  const instance = new WebAssembly.Instance(module, {});
  if (typeof instance.exports._start !== 'function') throw new Error('Missing exported _start');
  const expected = expectedChecks[args.fixture];
  if (expected !== null) {
    if (typeof instance.exports.completedChecks !== 'function' || instance.exports.completedChecks() !== 0) {
      throw new Error('Assertion counter must start at zero');
    }
  }
  instance.exports._start();
  const actual = expected === null ? null : instance.exports.completedChecks();
  if (actual !== expected) throw new Error('Assertion count differs: ' + actual + ' vs ' + expected);
  const after = cgroupProof(args);
  return {status: expected === null ? 'SEMANTIC_MODEL_EXECUTED' : 'PURE_MODEL_ASSERTIONS_PASS',
    fixture: args.fixture, sha256: input.sha256, imports, expected_checks: expected,
    completed_checks: actual, before, after, ROS_execution: 'NOT_PERFORMED'};
}

// Importing this file for text/option models performs no Wasm compilation.
if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  try { console.log(JSON.stringify(run(options(process.argv.slice(2))))); }
  catch (error) {
    console.error(JSON.stringify({status: 'FAIL_PURE_MODEL_EXECUTION', reason: String(error.message)}));
    process.exitCode = 1;
  }
}
