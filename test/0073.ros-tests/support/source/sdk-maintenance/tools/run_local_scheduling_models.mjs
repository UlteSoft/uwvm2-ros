/**
 * @file run_local_scheduling_models.mjs
 * @brief Execute compiled scheduling lessons with bounded mathematical imports.
 *
 * Pass a /tmp JSON array of successful compiler rows and a fresh /tmp receipt.
 * Each real module uses its exported memory. No Runtime/Gateway, host clock,
 * host environment, native entropy or external resource is supplied. Only
 * basic stdio and an empty guest environment are represented. TinyGo entropy
 * denial and explicitly selected synthetic entropy are separate test cases;
 * synthetic bytes are never described as cryptographic or production authority.
 * Run serially through the task owner's process/RSS/deadline guard.
 */
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';

const root = fs.realpathSync('/tmp');
function temporary(name) {
  const file = path.resolve(name);
  assert.ok(file.startsWith(root + '/') || file.startsWith('/tmp/'));
  return file;
}
const inputs = JSON.parse(fs.readFileSync(temporary(process.argv[2]), 'utf8'));
assert.equal(inputs.length, 10);
class Exit { constructor(code) { this.code = code; } }
const results = [];
for (const row of inputs) {
  assert.equal(row.status, 'COMPILE_IMPORT_FULL_VALIDATION_PASS');
  const bytes = fs.readFileSync(temporary(row.artifact));
  assert.equal(crypto.createHash('sha256').update(bytes).digest('hex'), row.artifact_sha256);
  const module = new WebAssembly.Module(bytes);
  for (const entropy of row.language === 'tinygo' ? [false, true] : [false]) {
    let instance, printed = '', abiCalls = 0, entropyCalls = 0, exit = 0, trapped = false;
    const view = () => new DataView(instance.exports.memory.buffer);
    const memory = () => new Uint8Array(instance.exports.memory.buffer);
    const calls = [];
    const basic = {
      fd_write(fd, vectors, count, written) {
        assert.ok(fd === 1 || fd === 2); assert.ok(count <= 64);
        let total = 0;
        for(let n = 0; n < count; ++n) {
          const at = view().getUint32(vectors + n * 8, true), length = view().getUint32(vectors + n * 8 + 4, true);
          assert.ok(at + length <= memory().length && total + length <= 8192);
          printed += Buffer.from(memory().subarray(at, at + length)).toString('utf8'); total += length;
        }
        view().setUint32(written, total, true); return 0;
      },
      fd_read(fd, vectors, count, read) { assert.equal(fd, 0); view().setUint32(read, 0, true); return 0; },
      fd_seek() { return 70; }, // basic stdio is nonseekable; no file is supplied
      fd_close(fd) { assert.ok(fd >= 0 && fd <= 2); return 0; },
      environ_sizes_get(count, size) { view().setUint32(count, 0, true); view().setUint32(size, 0, true); return 0; },
      environ_get(vector) { view().setUint32(vector, 0, true); return 0; },
      proc_exit(code) { throw new Exit(code); },
      random_get(at, length) {
        assert.equal(row.language, 'tinygo'); ++entropyCalls;
        assert.ok(at + length <= memory().length && length <= 4096);
        if(!entropy) return 76; // WASIp1 ENOTCAPABLE; guest bytes remain untouched
        for(let n = 0; n < length; ++n) memory()[at + n] = (n * 17 + 3) & 255;
        return 0;
      },
    };
    const imports = {};
    for(const item of WebAssembly.Module.imports(module)) {
      assert.equal(item.kind, 'function');
      imports[item.module] ??= {};
      if(item.module.startsWith('uwvm_ros')) {
        imports[item.module][item.name] = () => { calls.push(item.name); if(item.name === 'abi_version') { ++abiCalls; return 1; } return 13; };
      } else {
        assert.equal(item.module, 'wasi_snapshot_preview1');
        assert.ok(Object.hasOwn(basic, item.name), 'Unexpected scheduling import: ' + item.name);
        imports[item.module][item.name] = basic[item.name];
      }
    }
    instance = new WebAssembly.Instance(module, imports);
    try { instance.exports._start(); }
    catch(error) { if(error instanceof Exit) exit = error.code; else if(error instanceof WebAssembly.RuntimeError) trapped = true; else throw error; }
    assert.ok(calls.every(name => name === 'abi_version'));
    if(row.language === 'assemblyscript') {
      assert.equal(instance.exports.lastStatus(), 0); assert.equal(instance.exports.completedTicks(), 1);
    } else if(row.language === 'tinygo' && !entropy) {
      assert.ok(entropyCalls > 0); // observe refusal, including toolchain abort before main
    } else {
      assert.equal(exit, 0); assert.equal(trapped, false);
      if(row.language === 'rust') assert.ok(printed.includes('deferred result 42') && printed.includes('one local timer tick'));
      if(row.language === 'tinygo') assert.ok(printed.includes('owned deferred result 42') && printed.includes('one local tick'));
      if(row.language === 'c' || row.language === 'cpp') assert.ok(abiCalls > 0);
    }
    results.push({ id: row.id, synthetic_entropy_selected: entropy, entropy_calls: entropyCalls,
      exit_code: exit, trapped, abi_calls: abiCalls, ROS_resource_calls: 0, output: printed, status: 'PASS' });
  }
}
assert.equal(results.length, 12);
const report = { status: 'COMPILED_LOCAL_SCHEDULING_MODELS_PASS', cases: results.length,
  scope: 'Real Wasm with bounded mathematical imports; no native ROS, host entropy, Runtime authorization or delivery',
  actual_ROS_delivery: 'NOT_EXECUTED', results };
fs.writeFileSync(temporary(process.argv[3]), JSON.stringify(report, null, 2) + '\n', { flag: 'wx' });
console.log(JSON.stringify({ status: report.status, cases: report.cases }));
