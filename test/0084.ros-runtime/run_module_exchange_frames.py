#!/usr/bin/env python3
"""Exercise complete ROSv2 frame bytes and erasure in actual tagged Wasm.

Both endpoints own unexported shared memories and import only ROSv2 plus the
peer's closed Wasm entry points. No WASIp1 imports are present. Payload sizes
cross byte, word, granule and former scratch-buffer boundaries. Each response
has a different deterministic byte pattern; all payload bytes are checked and
the full delivery capacity and length word must be zero after consumption.
Software tags, wasm32/64, memory indices zero/one and INT/AOT are distinct cases.
This is a carrier regression, not native driver or physical safety evidence.
"""
import argparse
import json
from pathlib import Path
import subprocess
import time

from run_module_security_vm import Fixture, CALLS
from run_scene_memtag import read_uleb, uleb
from run_scene_vm import ROOT, GATE, sha, verify_gate


class Endpoint(Fixture):
    def __init__(self, width, memory, tagged):
        super().__init__(CALLS, width, memory, True)
        self.tagged = tagged
        self.helpers = []

    def pointer(self, address):
        return address | ((7 << (28 if self.width == 32 else 56)) if self.tagged else 0)

    def word(self, address):
        return f'i{self.width}.const {self.pointer(address)} i64.load $work'

    def write(self, address, value):
        value = f'i64.const {value}' if isinstance(value, int) else value
        self.body.append(f'i{self.width}.const {self.pointer(address)} {value} i64.store $work')

    def open(self, alias, expected=0, output=512):
        self.open_import = True
        address = self.blob(alias.encode())
        self.check(f'i32.const {self.memory} i64.const {self.pointer(address)} i64.const {len(alias)} '
                   f'i64.const {self.pointer(output)} call $open', expected)
        return self.save(alias, output)

    def invoke(self, name, cap, token=0, payload=0, expected=0, output=12288, capacity=8192, length=30000):
        self.write(128, token)
        self.write(136, payload)
        self.check(f'{cap} i32.const {self.memory} i64.const {self.pointer(128)} i64.const {16+payload} '
                   f'i64.const {self.pointer(output)} i64.const {capacity} i64.const {self.pointer(length)} call ${name}', expected)

    def pattern(self, address, length, seed, write=False):
        self.body.append(f'i{self.width}.const {self.pointer(address)} i32.const {length} '
                         f'i32.const {seed} call ${"fill" if write else "expect"}')

    def input_after_call(self, token, payload, seed, clear):
        if clear:
            self.pattern(128, payload+16, -1)
        else:
            expression = f'i64.const {token}' if isinstance(token, int) else token
            self.check(f'{self.word(128)} {expression} i64.ne')
            self.equal(self.word(136), payload)
            self.pattern(144, payload, seed)

    def render(self, export='_start'):
        extend = 'i64.extend_i32_u' if self.width == 64 else ''
        for write in (False, True):
            name = 'fill' if write else 'expect'
            # A seed of -1 checks zero; every other seed has a byte-varying
            # pattern so corrupt/missing tails cannot pass on the first word.
            action = ('local.get $value i32.store8 $work' if write else
                      'i32.load8_u $work local.get $value i32.ne if unreachable end')
            self.helpers.append(f'''(func ${name} (param $base i{self.width}) (param $size i32) (param $seed i32)
                (local $i i32) (local $value i32)
                block $done loop $next
                  local.get $i local.get $size i32.eq br_if $done
                  local.get $seed i32.const -1 i32.eq if i32.const 0 local.set $value
                  else local.get $i i32.const 17 i32.mul local.get $seed i32.add i32.const 255 i32.and local.set $value end
                  local.get $base local.get $i {extend} i{self.width}.add {action}
                  local.get $i i32.const 1 i32.add local.set $i br $next
                end end)''')
        extras = list(self.extra_functions)
        if self.tagged:
            # Named helper's defined-function ordinal is known: contains and
            # generated check functions precede extra_functions in Fixture.
            self.tag_function = 1+len(self.checks)
            self.extra_functions.insert(0, f'(func $tag.store (param i{self.width} i{self.width}) unreachable)')
            self.extra_functions.append(f'(func $tag.init i{self.width}.const {self.pointer(0)} '
                                        f'i{self.width}.const 65536 call $tag.store) (start $tag.init)')
        self.extra_functions.extend(self.helpers)
        result = super().render(export)
        self.extra_functions = extras
        return result


def tag_instruction(image, ordinal, memory):
    """Replace only the asserted typed helper body, never matching arbitrary bytes."""
    assert image[:8] == b'\0asm\1\0\0\0'
    output = bytearray(image[:8])
    cursor, patched = 8, False
    while cursor < len(image):
        section = image[cursor]
        length, begin = read_uleb(image, cursor+1)
        payload = image[begin:begin+length]
        cursor = begin+length
        if section == 10:
            count, position = read_uleb(payload, 0)
            bodies = []
            for _ in range(count):
                size, start = read_uleb(payload, position)
                bodies.append(payload[start:start+size])
                position = start+size
            assert position == len(payload) and ordinal < count and bodies[ordinal] == b'\x00\x00\x0b'
            bodies[ordinal] = b'\x00\x20\x00\x20\x01\xff'+uleb(0x0101002b)+uleb(memory)+b'\x0b'
            payload = uleb(count)+b''.join(uleb(len(body))+body for body in bodies)
            patched = True
        output += bytes([section])+uleb(len(payload))+payload
    assert patched and cursor == len(image)
    return bytes(output)


def pair(width, memory, tagged, payload, clear=True):
    main, worker = (Endpoint(width, memory, tagged) for _ in range(2))
    extent = payload+39  # The unused suffix must also be erased.
    worker.globals.add('exchange')
    cap = 'global.get $exchange'
    worker.invoke('module.call.take', cap, expected=11, capacity=extent)
    worker.extra_functions.append('(func (export "init") (param $cap i64) local.get $cap global.set $exchange '+'\n'.join(worker.body)+')')
    worker.body = []
    worker.pattern(12288, extent, 111, True)
    worker.invoke('module.call.take', cap, capacity=extent)
    token = worker.save('token', 12288)
    worker.equal(worker.word(12296), payload)
    worker.equal(worker.word(30000), payload+16)
    worker.pattern(12304, payload, 3)
    worker.pattern(144, payload, 91, True)
    worker.invoke('module.call.respond', cap, token, payload, output=28000, capacity=16, length=31000)
    worker.input_after_call(token, payload, 91, clear)
    worker.pattern(12288, extent, -1)
    worker.equal(worker.word(30000), 0)
    worker.extra_functions.append('(func (export "pump") '+'\n'.join(worker.body)+')')
    worker.body = []
    main.extra_imports = ['(import "worker" "init" (func $init (param i64)))', '(import "worker" "pump" (func $pump))']
    cap = main.open('exchange')
    main.body.append(cap+' call $init')
    for _ in range(2):
        main.pattern(144, payload, 3, True)
        main.invoke('module.call.submit', cap, payload=payload, output=28000, capacity=16, length=31000)
        token = main.save('token', 28000)
        main.input_after_call(0, payload, 3, clear)
        main.body.append('call $pump')
        main.pattern(12288, extent, 111, True)
        main.invoke('module.call.result', cap, token, capacity=extent)
        main.equal(main.word(12296), payload)
        main.equal(main.word(30000), payload+16)
        main.pattern(12304, payload, 91)
        main.invoke('module.call.acknowledge', cap, token, output=34000, capacity=0, length=35000)
        main.pattern(12288, extent, -1)
        main.input_after_call(token, 0, 3, clear)
        main.equal(main.word(30000), 0)
    return main, worker


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--clear-input', choices=('default', 'true', 'false'), default='default')
    args = parser.parse_args()
    verify_gate()
    runtime, out = args.runtime.resolve(), args.output.resolve()
    assert any(out.is_relative_to(folder.resolve()) for folder in (ROOT/'.tmp', ROOT.parent/'uwvm2-ros-sdk/.tmp'))
    out.mkdir(parents=True, exist_ok=False)
    build_path = runtime.parent/'full-build-results.json'
    build = json.loads(build_path.read_text())
    assert build['complete'] and build['binary_sha256'] == sha(runtime)
    inputs = [Path(__file__).resolve(), *[Path(__file__).with_name(name) for name in
              ('run_module_security_vm.py', 'run_scene_memtag.py', 'run_scene_vm.py')], runtime, build_path]
    pins = {str(path): sha(path) for path in inputs}
    for path in inputs[:4]:
        target = out/'fixture-source'/path.name
        target.parent.mkdir(exist_ok=True)
        target.write_bytes(path.read_bytes())
        pins[str(target)] = sha(target)
    report = dict(complete=False, scope=__doc__, inputs=pins, runtime_sha256=sha(runtime), clear_input=args.clear_input,
        cgroup=str(GATE), memory_max=68719476736, swap_max=0,
        events_before=(GATE/'memory.events').read_text(), cases=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    try:
        for width, memory in ((32, 0), (32, 1), (64, 0), (64, 1)):
            for tagged in (False, True):
                for size in (0, 1, 7, 8, 15, 16, 17, 255, 496, 497, 4096):
                    label = f'w{width}-m{memory}-tag{int(tagged)}-bytes{size}'
                    images = []
                    for role, endpoint in zip(('main', 'worker'), pair(width, memory, tagged, size, args.clear_input != 'false')):
                        wat, wasm = (out/(label+'-'+role+suffix) for suffix in ('.wat', '.wasm'))
                        wat.write_text(endpoint.render('_start' if role == 'main' else None))
                        subprocess.run(['wat2wasm', '--enable-threads', '--enable-memory64', '--enable-multi-memory',
                                        '--debug-names', str(wat), '-o', str(wasm)], check=True, capture_output=True)
                        if tagged:
                            wasm.write_bytes(tag_instruction(wasm.read_bytes(), endpoint.tag_function, memory))
                        images.append(wasm)
                    flags = ['--ros-scene-create', 'exchange', '--ros-scene-bind', 'exchange', 'main',
                             '--ros-scene-bind', 'exchange', 'worker']
                    for name in ['capability.open', *CALLS]:
                        flags += ['--ros-scene-interface', 'exchange', name, 'allow']
                    flags += ['--ros-scene-call-budget', 'exchange', '65536', '1', '--ros-scene-call', 'exchange', 'echo',
                              'main', 'worker', 'echo', 'octets', 'octets', str(size), str(size), '1', '10000000000',
                              '--ros-scene-grant', 'exchange', 'exchange', 'module.call', 'echo', 'registered-message', ','.join(CALLS)]
                    if args.clear_input != 'default':
                        flags += ['--ros-scene-call-clear-input', 'exchange', 'echo', args.clear_input]
                    for engine in ('int', 'aot'):
                        verify_gate()
                        command = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                                   '--runtime-compile-threads', '0', '--runtime-llvm-jit-cache-path', 'disable',
                                   '--runtime-memtag-backend', 'software', *flags,
                                   '--wasm-preload-library', str(images[1]), 'worker', '--run', str(images[0])]
                        start = time.monotonic()
                        result = subprocess.run(command, capture_output=True, timeout=60)
                        (out/(label+'-'+engine+'.stderr')).write_bytes(result.stderr)
                        (out/(label+'-'+engine+'.stdout')).write_bytes(result.stdout)
                        report['cases'].append(dict(name=label+'-'+engine, argv=command, passed=result.returncode == 0,
                            returncode=result.returncode, seconds=time.monotonic()-start,
                            main_sha256=sha(images[0]), worker_sha256=sha(images[1])))
                        save()
                        assert result.returncode == 0, (label, engine, result.stderr[-5000:])
                print(f'PASS width={width} memory={memory} tagged={tagged}: 22 frame cases', flush=True)
        report['complete'] = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(path) == digest for path, digest in pins.items())
        report['complete'] &= report['inputs_unchanged']
        report['events_after'] = (GATE/'memory.events').read_text()
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
