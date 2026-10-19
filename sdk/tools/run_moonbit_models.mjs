// Run only the pinned compiler's synchronous local-model test entry points.
// This harness grants no ROS, filesystem, network, update or debugger imports.
// The filesystem-named MoonBit hooks below convert only an explicit filename
// string; they expose no host path operation. Every ROS function always throws.
import assert from 'node:assert/strict';
import {readFileSync} from 'node:fs';
import {createHash} from 'node:crypto';

const sha = bytes => createHash('sha256').update(bytes).digest('hex');
assert.equal(process.argv.length, 3, 'one admitted module manifest required');
const specification = JSON.parse(readFileSync(process.argv[2], 'utf8'));
assert.equal(specification.kind, 'moonbit-synchronous-local-model-module-v1');
const bytes = readFileSync(specification.wasm.path);
assert.equal(bytes.length, specification.wasm.bytes);
assert.equal(sha(bytes), specification.wasm.sha256);
assert(WebAssembly.validate(bytes), 'actual module must validate on this engine');
const infoBytes = readFileSync(specification.test_info.path);
assert.equal(sha(infoBytes), specification.test_info.sha256);
const info = JSON.parse(infoBytes);
assert.deepEqual(info.tests, info.no_args_tests, 'only synchronous no-argument tests');
for (const role of ['with_args_tests', 'with_bench_args_tests', 'async_tests', 'async_tests_with_args']) {
  for (const tests of Object.values(info[role])) assert.deepEqual(tests, [], role);
}
const selected = [];
for (const [file, tests] of Object.entries(info.tests)) {
  assert(/^[a-z0-9_]+\.mbt$/.test(file) && file.length <= 200);
  assert(Array.isArray(tests) && tests.length <= specification.expected_test_count,
    'each compiler-reported file is bounded; empty source-file entries are valid');
  for (const [index, test] of tests.entries()) {
    assert.equal(test.index, index);
    assert.equal(typeof test.name, 'string');
    assert.deepEqual(test.attrs, [], 'skip/panic/async attributes require another runner');
    selected.push({file, index, name: test.name});
  }
}
assert.equal(selected.length, specification.expected_test_count);
assert(selected.length > 0 && selected.length <= 66);

const module = new WebAssembly.Module(bytes);
const imports = WebAssembly.Module.imports(module);
assert.deepEqual(imports, specification.imports, 'exact inspected import identities');
let instance;
let rosCalls = 0;
let chunks = [];
let totalOutput = 0;
const readers = new WeakSet();
let activeReaders = 0;
function append(data) {
  const owned = Buffer.from(data);
  totalOutput += owned.length;
  assert(totalOutput <= 1048576, 'bounded complete test log');
  chunks.push(owned);
}
function memoryRange(offset, length) {
  assert(Number.isInteger(offset) && Number.isInteger(length));
  offset >>>= 0; length >>>= 0;
  const memory = instance.exports.memory;
  assert(memory instanceof WebAssembly.Memory);
  assert(offset <= memory.buffer.byteLength && length <= memory.buffer.byteLength - offset);
  return new Uint8Array(memory.buffer, offset, length);
}
const exception = {
  tag: new WebAssembly.Tag({parameters: specification.exception_tag_parameters}),
  throw() {throw new Error('unexpected MoonBit panic/exception import');},
};
const host = {
  exception,
  wasi_snapshot_preview1: {
    fd_write(fd, vectors, count, written) {
      assert(fd === 1 || fd === 2, 'stdout/stderr only');
      assert(Number.isInteger(count) && count >= 0 && count <= 64);
      const descriptors = new DataView(memoryRange(vectors, count * 8).buffer,
        vectors >>> 0, count * 8);
      const sizes = [];
      let total = 0;
      for (let index = 0; index < count; ++index) {
        const offset = descriptors.getUint32(index * 8, true);
        const length = descriptors.getUint32(index * 8 + 4, true);
        assert(length <= 65536 && total + length <= 65536);
        sizes.push(Buffer.from(memoryRange(offset, length))); total += length;
      }
      const slot = memoryRange(written, 4);
      for (const piece of sizes) append(piece);
      new DataView(slot.buffer, slot.byteOffset, 4).setUint32(0, total, true);
      return 0;
    },
  },
  spectest: {
    print_char(value) {
      assert(Number.isInteger(value) && value >= 0 && value <= 0x10ffff);
      append(Buffer.from(String.fromCodePoint(value), 'utf8'));
    },
  },
  __moonbit_fs_unstable: {
    begin_read_string(value) {
      assert.equal(typeof value, 'string');
      assert(value.length <= 200, 'test filename UTF16 bound');
      const reader = {value, index: 0}; readers.add(reader); ++activeReaders;
      return reader;
    },
    string_read_char(reader) {
      assert(readers.has(reader), 'owned live string reader');
      return reader.index === reader.value.length ? -1 : reader.value.charCodeAt(reader.index++);
    },
    finish_read_string(reader) {
      assert(readers.has(reader) && reader.index === reader.value.length);
      readers.delete(reader); --activeReaders;
    },
  },
};
const allowed = new Set(['exception.tag', 'exception.throw', 'wasi_snapshot_preview1.fd_write',
  'spectest.print_char', '__moonbit_fs_unstable.begin_read_string',
  '__moonbit_fs_unstable.string_read_char', '__moonbit_fs_unstable.finish_read_string']);
for (const item of imports) {
  if (/^uwvm_ros[12]_v1$/.test(item.module)) {
    assert.equal(item.kind, 'function');
    host[item.module] ??= {};
    host[item.module][item.name] = () => {++rosCalls; throw new Error('model unexpectedly reached ROS: ' + item.module + '.' + item.name);};
  } else assert(allowed.has(item.module + '.' + item.name), 'closed model host namespace');
}
instance = new WebAssembly.Instance(module, host); // Executes the real start section exactly once.
assert.equal(typeof instance.exports._start, 'function');
assert.equal(typeof instance.exports.moonbit_test_driver_internal_execute, 'function');
assert.equal(typeof instance.exports.moonbit_test_driver_finish, 'function');
instance.exports._start(); // The pinned generated test command body is empty.
assert.equal(chunks.length, 0, 'initialization cannot masquerade as test execution');
const results = [];
for (const test of selected) {
  chunks = [];
  instance.exports.moonbit_test_driver_internal_execute(test.file, test.index);
  assert.equal(activeReaders, 0);
  const raw = Buffer.concat(chunks).toString('utf8');
  const framed = raw.matchAll(/----- BEGIN MOON TEST RESULT -----\n([^\n]*)\n----- END MOON TEST RESULT -----\n/g);
  const records = [...framed].map(match => ({frame: match[0], body: JSON.parse(match[1])}));
  assert.equal(records.length, 2, 'exact actual start/result frames for each requested test');
  assert.equal(records.map(record => record.frame).join(''), raw, 'no ignored output');
  assert.deepEqual(records[0].body, {type: 'start', file: test.file, index: test.index});
  assert.deepEqual(records[1].body, {type: 'result', file: test.file, index: test.index, message: ''},
    'failed or skipped test cannot pass');
  assert.equal(rosCalls, 0, 'synthetic local owners never become Host handles');
  results.push({...test, passed: true, raw_stdout: raw});
}
chunks = [];
instance.exports.moonbit_test_driver_finish();
assert.equal(chunks.length, 0, 'no deferred async or failure output');
assert.equal(rosCalls, 0); assert.equal(activeReaders, 0);
console.log(JSON.stringify({kind: 'ACTUAL_COMPILED_MOONBIT_LOCAL_MODEL_EXECUTION_NOT_UWVM_OR_NATIVE_ROS',
  node: process.version, v8: process.versions.v8, wasm: specification.wasm,
  test_info: specification.test_info, model_tests: results.length, ros_import_calls: rosCalls,
  native_ros_delivery_cases: 0, uwvm_execution_cases: 0, passed: true, results}));
