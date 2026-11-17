#!/usr/bin/env python3
"""Exercise actual rosv2 host memory normalization with real tagged Wasm memory.

Matching-tag copies run with normal enforcement. Separately, the trusted
silent-log diagnostic mode allows different pointer encodings to resolve to
the same physical span, so normalized disjointness must still reject aliases
without overwriting canaries or consuming a guard event. Default mismatch cases
must terminate at the real host boundary with the default crash signal and
memory-protection diagnostic code 122; the diagnostic is not a numeric exit.
No old implementation, SDK or previous test is changed. Every process belongs
to the existing 64 GiB zero-swap cgroup.
"""
import argparse
import json
from pathlib import Path
import re
import resource
import signal
import subprocess
import sys
import time
sys.dont_write_bytecode = True
from run_scene_vm import Guest, GATE, ROOT, grant, policy, sha, verify_gate


def uleb(value):
    result = bytearray()
    while True:
        byte = value & 127; value >>= 7
        result.append(byte | (128 if value else 0))
        if not value:
            return bytes(result)


def read_uleb(data, offset):
    value = 0
    for shift in range(0, 35, 7):
        byte = data[offset]; offset += 1; value |= (byte & 127) << shift
        if not byte & 128:
            return value, offset
    raise ValueError('malformed uleb')


def add_private_instructions(image, checks, memory):
    """Replace two unreachable helper bodies, preserving valid Wasm section sizes.

    Standard wat2wasm cannot spell this VM's documented private FF operations.
    Opcode bytes are inserted only into the two named typed helpers; no global
    search/replacement can corrupt an unrelated instruction or byte constant.
    """
    assert image[:8] == b'\0asm\1\0\0\0'
    result = bytearray(image[:8]); cursor = 8; patched = False
    while cursor < len(image):
        kind = image[cursor]; length, begin = read_uleb(image, cursor+1)
        payload = image[begin:begin+length]; cursor = begin+length
        if kind == 10:
            count, position = read_uleb(payload, 0); bodies = []
            for _ in range(count):
                size, start = read_uleb(payload, position)
                bodies.append(payload[start:start+size]); position = start+size
            assert position == len(payload) and count == checks+3
            assert bodies[checks] == bodies[checks+1] == b'\x00\x00\x0b'
            # memtag.store(address, byte_count); memtag.status() -> address type.
            bodies[checks] = b'\x00\x20\x00\x20\x01\xff'+uleb(0x0101002b)+uleb(memory)+b'\x0b'
            bodies[checks+1] = b'\x00\xff'+uleb(0x01010020)+uleb(memory)+b'\x0b'
            payload = uleb(count)+b''.join(uleb(len(body))+body for body in bodies)
            patched = True
        result += bytes([kind])+uleb(len(payload))+payload
    assert patched and cursor == len(image)
    return bytes(result)


class TaggedGuest(Guest):
    tag = 3
    def pointer(self, address, tag=None):
        return address | ((self.tag if tag is None else tag) << (28 if self.width == 32 else 56))

    def word(self, address):
        return f'i{self.width}.const {self.pointer(address)} i64.load $work'

    def write(self, address, value):
        if isinstance(value, int):
            value = 'i64.const '+str(value)
        self.body.append(f'i{self.width}.const {self.pointer(address)} {value} i64.store $work')

    def open(self, alias, expected=0, output=512):
        address = self.cursor; self.cursor += len(alias)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "{alias}")')
        self.check(f'i32.const {self.memory} i64.const {self.pointer(address)} i64.const {len(alias)} '
                   f'i64.const {self.pointer(output)} call $open', expected)
        return self.save(alias, output)

    def call(self, name, cap, arguments=(), expected=0, *, output=512, length=1024,
             request_tag=None, output_tag=None, length_tag=None):
        descriptor = next(row for row in self.operations if row['name'] == name)
        for index, value in enumerate(arguments):
            self.write(128+8*index, value)
        self.check(f'{cap} i32.const {self.memory} i64.const {self.pointer(128, request_tag)} '
                   f'i64.const {descriptor["words"]*8} i64.const {self.pointer(output, output_tag)} '
                   f'i64.const 32 i64.const {self.pointer(length, length_tag)} call ${name}', expected)

    def render(self):
        source = super().render()
        helpers = f'\n(func $tag.store (param i{self.width} i{self.width}) unreachable)\n' \
                  f'(func $tag.status (result i{self.width}) unreachable)\n'
        return source.replace('\n(func (export "_start")', helpers+'(func (export "_start")', 1)


def guest(operations, width, memory, scenario):
    g = TaggedGuest(operations, width, memory)
    g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64' if width == 64 else ''), 2)
    g.check('call $abi', 2)
    capability = g.open('signal')
    g.call('guard.create', capability)
    resource = g.save('resource')
    g.equal(g.word(1024), 8)
    g.call('guard.trigger', capability, [resource])
    if scenario.startswith('mismatch-'):
        g.call('guard.take', capability, [resource], expected=22, **{scenario[9:]+'_tag': 5})
        # If the host merely returns without delivering the recorded tag fault,
        # this fixture must not look like a successful fault qualification.
        return g
    if scenario == 'aliases':
        for target in ['output', 'request']:
            g.write(512, 0x123456789abcdef)
            g.write(1024, 0x3456789)
            # Encoded output/request carries tag3, length slot tag0: raw
            # integers look disjoint, actual physical bytes overlap exactly.
            g.call('guard.take', capability, [resource], expected=22,
                   length=512 if target == 'output' else 128, length_tag=0)
            g.equal(g.word(512), 0x123456789abcdef)
            g.equal(g.word(1024), 0x3456789)
            g.check(g.word(128)+' '+resource+' i64.ne')
            g.call('guard.take', capability, [resource])
            g.equal(g.word(512), 1)
            g.equal(g.word(1024), 8)
            g.call('guard.take', capability, [resource], expected=11)
            g.call('guard.trigger', capability, [resource])
        # Positive control for diagnostic mode: different encodings with
        # genuinely disjoint spans still succeed and produce a mismatch log.
        g.call('guard.take', capability, [resource], length_tag=0)
        g.equal(g.word(512), 1)
        g.equal(g.word(1024), 8)
    else:
        g.call('guard.take', capability, [resource])
        g.equal(g.word(512), 1)
        g.equal(g.word(1024), 8)
    g.call('guard.take', capability, [resource], expected=11)
    g.check(capability+' call $close')
    return g


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--engines', nargs='+', choices=['int', 'aot'], default=['int', 'aot'])
    parser.add_argument('--source-root', type=Path, default=ROOT,
                        help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    runtime = args.runtime.resolve(); out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    contract = production_root/'src/uwvm2/imported/ros/runtime/contract.h'
    operations = [dict(name=n, words=int(w), bytes=int(b)) for n, w, b in
                  re.findall(r' X\(\w+,"([^"]+)","[^"]+",(\d+),(-?\d+)\)', contract.read_text()) if n.startswith('guard.')]
    assert len(operations) == 3
    sources = [Path(__file__), ROOT/'test/0084.ros-runtime/run_scene_vm.py',
               production_root/'src/uwvm2/uwvm/imported/scene/host.h', production_root/'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp',
               production_root/'src/uwvm2/extension/memtag/instruction.h', production_root/'src/uwvm2/runtime/memtag/host_access.h',
               production_root/'src/uwvm2/runtime/memtag/bridge.h', production_root/'src/uwvm2/runtime/lib/uwvm_runtime_host_memtag_fault.h', contract]
    pins = lambda: {str(path): sha(path) for path in sources}
    report = dict(complete=False, scope='actual rosv2 tagged-memory host boundary; software MTE, memory32/64, indexes0/1, INT/AOT',
                  runtime=str(runtime), runtime_sha256=sha(runtime), cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino,
                  memory_max=68719476736, swap_max=0, core_limit=0, events_before=(GATE/'memory.events').read_text(),
                  production_source_root=str(production_root), fixture_source_root=str(ROOT),
                  inputs=pins(), fixtures=[], cases=[])
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    flags = policy(operations)+grant('signal', 'guard', [row['name'] for row in operations])
    flags += ['--ros-scene-limit', 'robot', 'signal', 'max-handles', '1',
              '--ros-scene-limit', 'robot', 'signal', 'max-resources', '1']
    try:
        for width, memory in [(32, 0), (64, 0), (32, 1), (64, 1)]:
            for scenario in ['valid', 'aliases', 'mismatch-request', 'mismatch-output', 'mismatch-length']:
                stem = f'{scenario}-memory{width}-index{memory}'
                g = guest(operations, width, memory, scenario)
                wat, wasm = out/(stem+'.wat'), out/(stem+'.wasm')
                wat.write_text(g.render())
                command = ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)]
                verify_gate(); result = subprocess.run(command, capture_output=True, timeout=30)
                (out/(stem+'-compile.stderr')).write_bytes(result.stderr)
                assert result.returncode == 0, result.stderr.decode()
                wasm.write_bytes(add_private_instructions(wasm.read_bytes(), len(g.checks), memory))
                report['fixtures'].append(dict(name=stem, argv=command, wasm_sha256=sha(wasm), wat_sha256=sha(wat), assertions=len(g.checks)))
                for engine in args.engines:
                    verify_gate(); name = stem+'-'+engine
                    command = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                               '--runtime-compile-threads', '1', '--runtime-memtag-backend', 'software',
                               '--runtime-memtag-mode', 'sync', '--log-color', 'disable', *flags]
                    if engine == 'aot':
                        command += ['--runtime-llvm-jit-cache-path', 'disable']
                    if scenario == 'aliases':
                        command += ['--runtime-memtag-action', 'silent-log', 'err']
                    command += ['--run', str(wasm)]
                    before = time.monotonic(); result = subprocess.run(command, capture_output=True, timeout=120)
                    (out/(name+'.stdout')).write_bytes(result.stdout); (out/(name+'.stderr')).write_bytes(result.stderr)
                    errors = result.stderr.decode(errors='replace')
                    expected = -signal.SIGILL if scenario.startswith('mismatch-') else 0
                    passed = result.returncode == expected
                    violations = []
                    if expected < 0:
                        expected_reason = 'Reason: host interface memory tag mismatch' if engine == 'int' else 'Reason: memory tag mismatch'
                        failed_slot = {'request': 128, 'output': 512, 'length': 1024}[scenario[9:]]
                        passed = (passed and '[crash] memory-protection' in errors and expected_reason in errors
                                  and 'Exit status: 122' in errors and 'Fault action: crash (OS core-dump policy applies)' in errors
                                  and f'Memory index: {memory}' in errors and f'Pointer: {g.pointer(failed_slot, 5)}' in errors
                                  and 'Pointer tag: 5' in errors and 'Allocation tag: 3' in errors)
                    if scenario == 'aliases':
                        violations = [json.loads(line) for line in errors.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.')]
                        passed = passed and len(violations) >= 3 and all(
                            v['pointer_tag'] == 0 and v['allocation_tag'] == 3 and v['memory'] == memory and v['pointer'] in [128, 512, 1024]
                            for v in violations)
                    report['cases'].append(dict(name=name, argv=command, returncode=result.returncode, expected=expected,
                                                passed=passed, seconds=time.monotonic()-before, violations=violations))
                    save(); assert passed, (name, result.returncode, errors[-5000:])
                    print('PASS', name, flush=True)
        report['inputs_after'] = pins()
        assert report['inputs'] == report['inputs_after'] and sha(runtime) == report['runtime_sha256'], 'qualification input changed'
        report['complete'] = True
    finally:
        report['events_after'] = (GATE/'memory.events').read_text(); save()


if __name__ == '__main__':
    main()
