/** Pure descriptor models for the standalone runner's immutable artifact read.
 *
 * Only the current artifact function is evaluated with mocked fs operations.
 * No real artifact, FIFO/device, WebAssembly API, guest, compiler, cgroup or ROS
 * process is read or executed. These cases distinguish pre-read admission from
 * post-read identity/hash consistency and prove descriptor closure on errors.
 */
import assert from 'node:assert/strict';
import crypto from 'node:crypto';
import fs from 'node:fs';
import vm from 'node:vm';

const source = fs.readFileSync(new URL('./run_pure_wasm.mjs', import.meta.url), 'utf8');
const begin = source.indexOf('function artifact(args) {');
const end = source.indexOf('\n/** Instantiate only', begin);
assert.ok(begin >= 0 && end > begin, 'The bounded reader must have its known source boundary');
const readerSource = source.slice(begin, end);
assert.ok(!/WebAssembly|child_process|cgroupProof/.test(readerSource));
let checks = 0;

/** Independent opened-file facts; the fake file begins regular and immutable. */
function metadata(changes = {}) {
  return {dev: 29n, ino: 1001n, size: 4n, mtimeNs: 2n, ctimeNs: 3n,
    isFile: () => true, ...changes};
}

/** Offer only descriptor operations, recording all opens/reads/closes. */
function fixture(change = () => {}) {
  const model = {data: Buffer.from([0, 97, 98, 99]), before: metadata(), after: null,
    constants: {O_RDONLY: 0, O_NONBLOCK: 2048}, opens: [], reads: [], closes: [], stats: [],
    offset: 0, chunk: null, fail: null};
  change(model);
  model.after ??= model.before;
  model.args = {wasm: '/synthetic/model.wasm',
    sha256: crypto.createHash('sha256').update(model.data).digest('hex')};
  const fakeFs = {
    constants: model.constants,
    openSync(value, flags) {
      model.opens.push([value, flags]);
      if (model.fail === 'open') throw new Error('Synthetic open failure');
      assert.ok(flags & model.constants.O_NONBLOCK, 'Admission requires a nonblocking open');
      return 71;
    },
    fstatSync(fd, options) {
      assert.equal(fd, 71); assert.equal(options.bigint, true);
      model.stats.push(fd);
      if (model.fail === (model.stats.length === 1 ? 'first-fstat' : 'last-fstat')) throw new Error('Synthetic fstat failure');
      return model.stats.length === 1 ? model.before : model.after;
    },
    readSync(fd, bytes, offset, count, position) {
      assert.equal(fd, 71); assert.equal(position, offset);
      model.reads.push(count);
      if (model.fail === 'read') throw new Error('Synthetic read failure');
      const length = Math.min(count, model.chunk ?? count, model.data.length - model.offset);
      model.data.copy(bytes, offset, model.offset, model.offset + length);
      model.offset += length;
      return length;
    },
    closeSync(fd) { assert.equal(fd, 71); model.closes.push(fd); },
  };
  // Cap allocations in the fake context. A preflight size rejection must occur
  // before allocation, and the64MiB boundary model checks admission with a
  // synthetic allocation sentinel rather than allocating a real64MiB buffer.
  const fakeBuffer = {alloc(size) {
    model.allocation = size;
    if (model.fail === 'allocation' || size > 16) throw new Error('Synthetic bounded allocation sentinel');
    return Buffer.alloc(size);
  }};
  model.readArtifact = vm.runInNewContext(readerSource + '\nartifact', {fs: fakeFs, Buffer: fakeBuffer, crypto});
  return model;
}

/** Run one case, proving exact success bytes or a refusal with owned cleanup. */
function check(name, change = () => {}, {refuses = false, noReads = false, noOpen = false} = {}) {
  const model = fixture(change);
  if (refuses) assert.throws(() => model.readArtifact(model.args), undefined, name);
  else {
    const result = model.readArtifact(model.args);
    assert.deepEqual(result.bytes, model.data, name);
    assert.equal(result.sha256, model.args.sha256, name);
  }
  if (noReads) assert.equal(model.reads.length, 0, name);
  if (noOpen) assert.deepEqual(model.opens, [], name);
  else assert.deepEqual(model.opens, [[model.args.wasm, model.constants.O_RDONLY | model.constants.O_NONBLOCK]], name);
  assert.deepEqual(model.closes, noOpen || model.fail === 'open' ? [] : [71], name);
  ++checks;
  return model;
}

check('regular binary bytes bind the exact expected SHA');
check('empty regular file admitted', model => { model.data = Buffer.alloc(0); model.before = metadata({size: 0n}); });
check('partial reads accumulate', model => model.chunk = 1);
for (const kind of ['FIFO', 'character device', 'directory', 'socket']) {
  const result = check(kind + ' rejected before read', model => model.before = metadata({isFile: () => false}),
    {refuses: true, noReads: true});
  assert.equal(result.allocation, undefined, 'Nonregular inputs must also precede allocation');
}
for (const size of [-1n, 67108865n]) {
  const result = check('invalid declared size rejected', model => model.before = metadata({size}), {refuses: true, noReads: true});
  assert.equal(result.allocation, undefined);
}
const boundary = check('exact64MiB preflight reaches bounded allocation sentinel', model => model.before = metadata({size: 67108864n}),
  {refuses: true, noReads: true});
assert.equal(boundary.allocation, 67108864);
for (const flag of [undefined, 0, 1.5]) {
  check('unavailable nonblocking flag rejected before open', model => model.constants.O_NONBLOCK = flag,
    {refuses: true, noReads: true, noOpen: true});
}
check('truncated data rejected', model => model.data = Buffer.from('abc'), {refuses: true});
for (const field of ['dev', 'ino', 'size', 'mtimeNs', 'ctimeNs']) {
  check(field + ' mutation rejected', model => model.after = metadata({[field]: model.before[field] + 1n}), {refuses: true});
}
check('post-read kind mutation rejected', model => model.after = metadata({isFile: () => false}), {refuses: true});
check('another binary payload receives its own exact SHA', model => model.data = Buffer.from('abcd'));
const wrongHash = fixture(); wrongHash.args.sha256 = '0'.repeat(64);
assert.throws(() => wrongHash.readArtifact(wrongHash.args)); assert.deepEqual(wrongHash.closes, [71]); ++checks;
for (const fail of ['open', 'first-fstat', 'allocation', 'read', 'last-fstat']) {
  check(fail + ' failure keeps descriptor ownership', model => model.fail = fail,
    {refuses: true, noReads: ['open', 'first-fstat', 'allocation'].includes(fail)});
}
assert.equal(fs.readFileSync(new URL('./run_pure_wasm.mjs', import.meta.url), 'utf8'), source);
console.log(JSON.stringify({status: 'SOURCE_ARTIFACT_READ_MODEL_PASS', checks,
  source_sha256: crypto.createHash('sha256').update(source).digest('hex'),
  scope: 'Actual extracted artifact function with synthetic descriptor operations only',
  filesystem_artifact_execution: 'NOT_PERFORMED', WebAssembly_execution: 'NOT_PERFORMED',
  native_compilation: 'NOT_PERFORMED', cgroup_execution: 'NOT_PERFORMED', ROS_execution: 'NOT_PERFORMED'}));
