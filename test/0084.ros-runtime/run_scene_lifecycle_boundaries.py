#!/usr/bin/env python3
"""Exercise lifecycle FP restoration and real software MTE carrier boundaries.

All six imports receive dynamic arithmetic after normal/short/replay/denied
returns, and tagged-memory request/output/length faults independently. Physical
alias tests use trusted diagnostic mode to prove rejection before effects;
successful same-key submit/take/respond afterwards preserve the lifecycle.
Both engines, both memory widths and index zero/one execute in the existing
64 GiB zero-swap cgroup. This does not qualify native ROS callbacks or hardware
MTE and creates no WASI or ROS-v1 grants.
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
import run_scene_vm as base
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest, add_private_instructions

ROOT, GATE = base.ROOT, base.GATE
CONTRACT = ROOT/'src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h'
LIFECYCLE = [dict(name=n, words=int(w), bytes=int(b)) for n, w, b in
             re.findall(r' X\(\w+,\w+,"([^"]+)",(\d+),(\d+)\)', CONTRACT.read_text())]
assert len(LIFECYCLE) == 6
OPERATIONS = LIFECYCLE + [dict(name='clock.create', words=1, bytes=8), dict(name='clock.read', words=1, bytes=32)]


class LifecycleCalls:
    def address(self, value, tag=None):
        return self.pointer(value, tag) if isinstance(self, TaggedGuest) else value

    def call(self, name, cap, arguments=(), expected=0, *, output=512, length=1024,
             request_tag=None, output_tag=None, length_tag=None, capacity=None):
        descriptor = next(row for row in self.operations if row['name'] == name)
        for index, value in enumerate(arguments): self.write(128+8*index, value)
        if capacity is None: capacity = descriptor['bytes']
        self.check(f'{cap} i32.const {self.memory} i64.const {self.address(128, request_tag)} '
                   f'i64.const {len(arguments)*8} i64.const {self.address(output, output_tag)} '
                   f'i64.const {capacity} i64.const {self.address(length, length_tag)} call ${name}', expected)


class LifecycleFP(LifecycleCalls, FPGuest):
    pass


class LifecycleTagged(LifecycleCalls, TaggedGuest):
    pass


def configured():
    return [*base.policy(OPERATIONS), '--ros-scene-grant', 'robot', 'node', 'lifecycle', 'node-data',
            'local-lifecycle', ','.join(row['name'] for row in LIFECYCLE),
            '--ros-scene-lifecycle', 'robot', 'node', '2', '120000000000', '254', '3',
            '--ros-scene-limit', 'robot', 'node', 'max-resources', '2',
            '--ros-scene-grant', 'robot', 'steady', 'clock.steady', 'steady', 'local', 'clock.create,clock.read']


def guest(width, memory, scenario):
    tagged = scenario not in ('fp', 'denied', 'no-grants')
    g = (LifecycleTagged if tagged else LifecycleFP)(OPERATIONS, width, memory)
    if tagged:
        g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
        g.check('call $tag.status'+(' i32.wrap_i64' if width == 64 else ''), 2)
    if scenario in ('denied', 'no-grants'):
        for descriptor in LIFECYCLE:
            g.call(descriptor['name'], 'i64.const 1', [1]*descriptor['words'], expected=13 if scenario == 'denied' else 9)
        return g
    cap = g.open('node'); mismatch = scenario.split('-')[1:] if scenario.startswith('mismatch-') else []

    def invoke(action, arguments):
        name = 'lifecycle.'+action; descriptor = next(row for row in LIFECYCLE if row['name'] == name)
        if mismatch and action == mismatch[0]:
            g.call(name, cap, arguments, expected=22, **{mismatch[1]+'_tag': 5})
            return False
        if scenario == 'aliases':
            for slot in (512, 128):
                g.write(512, 0x123456789abcdef); g.write(1024, 0x3456789)
                g.call(name, cap, arguments, expected=22, length=slot, length_tag=0)
                g.equal(g.word(512), 0x123456789abcdef); g.equal(g.word(1024), 0x3456789)
                first = 'i64.const '+str(arguments[0]) if isinstance(arguments[0], int) else arguments[0]
                g.check(g.word(128)+' '+first+' i64.ne')
        g.write(512, 0x123456789abcdef)
        g.call(name, cap, arguments, expected=90, capacity=descriptor['bytes']-1)
        g.equal(g.word(1024), descriptor['bytes']); g.equal(g.word(512), 0x123456789abcdef)
        g.call(name, cap, arguments); g.equal(g.word(1024), descriptor['bytes'])
        if scenario == 'fp' and action in ('submit', 'respond'):
            saved = [g.save('replay-'+action+'-'+str(index), 512+8*index) for index in range(5)]
            g.call(name, cap, arguments)
            for index, value in enumerate(saved): g.check(g.word(512+8*index)+' '+value+' i64.ne')
        return True

    if not invoke('open', [1]): return g
    client = g.save('client')
    g.call('lifecycle.open', cap, [2]); server = g.save('server')
    g.call('lifecycle.open', cap, [1], expected=11)  # Aliased/short opens cannot retain a handle.
    if not invoke('state', [client]): return g
    g.equal(g.word(512), 1); g.equal(g.word(520), 1)
    if not invoke('transitions', [client]): return g
    g.equal(g.word(512), 34)
    steady = g.open('steady'); g.call('clock.create', steady, [0]); clock = g.save('clock')
    g.call('clock.read', steady, [clock], output=768, length=1104)
    g.globals.add('deadline'); g.body.append(g.word(768)+' i64.const 60000000000 i64.add global.set $deadline')
    if not invoke('submit', [client, 1, 1, 1, 'global.get $deadline']): return g
    request = g.save('request', 520); g.equal(g.word(528), 2); g.equal(g.word(536), 10)
    if not invoke('take', [server]): return g
    g.check(g.word(512)+' '+request+' i64.ne'); g.equal(g.word(520), 1); g.equal(g.word(528), 2)
    g.equal(g.word(536), 10); g.check(g.word(544)+' global.get $deadline i64.ne')
    if not invoke('respond', [server, request, 2, 97]): return g
    g.equal(g.word(512), 1); g.equal(g.word(528), 3); g.equal(g.word(536), 2); g.equal(g.word(544), 0)
    g.call('lifecycle.state', cap, [client]); g.equal(g.word(512), 2); g.equal(g.word(520), 3)
    for offset in (528, 536, 544, 552, 560): g.equal(g.word(offset), 0)
    if scenario == 'aliases':
        # A genuinely disjoint mismatched encoding succeeds in the same
        # diagnostic mode: earlier rejections were due to physical overlap.
        g.call('lifecycle.state', cap, [client], length_tag=0)
        g.equal(g.word(512), 2); g.equal(g.word(520), 3); g.equal(g.word(1024), 56)
    g.check(cap+' call $close')
    return g


def main():
    global LIFECYCLE, OPERATIONS
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--prepare-only', action='store_true')
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); base.verify_gate(); resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    if not args.prepare_only and args.runtime is None: parser.error('--runtime is required')
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    contract = production_root/CONTRACT.relative_to(ROOT)
    LIFECYCLE = [dict(name=n, words=int(w), bytes=int(b)) for n, w, b in
                 re.findall(r' X\(\w+,\w+,"([^"]+)",(\d+),(\d+)\)', contract.read_text())]
    assert len(LIFECYCLE) == 6
    OPERATIONS = LIFECYCLE + [dict(name='clock.create', words=1, bytes=8), dict(name='clock.read', words=1, bytes=32)]
    out = args.output.resolve(); assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    paths = [Path(__file__).resolve(), CONTRACT, *[ROOT/'test/0084.ros-runtime'/name for name in
             ('run_scene_vm.py', 'run_scene_fp.py', 'run_scene_memtag.py')], ROOT/'src/uwvm2/uwvm/imported/scene/host.h',
             *[ROOT/'src/uwvm2/imported/ros/scene'/name for name in ('managed_lifecycle.h', 'broker.h', 'deployment.h')]]
    runtime = args.runtime.resolve() if args.runtime else None
    if runtime: paths.append(runtime)
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path): base.sha(path) for path in paths}
    report = dict(complete=False, prepare_only=args.prepare_only, inputs=pins(), scope=__doc__,
                  cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  core_limit=0, events_before=(GATE/'memory.events').read_text(), guests=[], cases=[])

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)
    if runtime:
        report['runtime'] = str(runtime)
        report['runtime_sha256'] = base.sha(runtime)

    def save():
        report['events_after'] = (GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    scenarios = ['fp', 'denied', 'no-grants', 'valid', 'aliases']
    scenarios += ['mismatch-'+row['name'].split('.')[1]+'-'+slot for row in LIFECYCLE for slot in ('request', 'output', 'length')]
    try:
        for width, memory in ((32, 0), (64, 0), (32, 1), (64, 1)):
            for scenario in scenarios:
                tagged = scenario not in ('fp', 'denied', 'no-grants')
                g = guest(width, memory, scenario); name = f'{scenario}-memory{width}-index{memory}'
                wat, wasm = out/(name+'.wat'), out/(name+'.wasm'); wat.write_text(g.render())
                argv = ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)]
                result = subprocess.run(argv, capture_output=True, timeout=30)
                (out/(name+'-compile.stderr')).write_bytes(result.stderr)
                assert result.returncode == 0, result.stderr.decode(errors='replace')
                if tagged: wasm.write_bytes(add_private_instructions(wasm.read_bytes(), len(g.checks), memory))
                report['guests'].append(dict(name=name, argv=argv, sha256=base.sha(wasm), assertions=len(g.checks),
                                            fp_sites=getattr(g, 'fp_sites', 0), arithmetic_checks_per_site=0 if tagged else 12))
                save()
                if args.prepare_only: continue
                flags = (base.policy(OPERATIONS, 'deny' if scenario == 'denied' else 'allow')
                         if scenario in ('denied', 'no-grants') else configured())
                for engine in ('int', 'aot'):
                    base.verify_gate(); run_name = name+'-'+engine
                    argv = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                            '--runtime-compile-threads', '1', '--log-color', 'disable', *flags]
                    if engine == 'aot': argv += ['--wasm-policy-global', 'runtime.llvm.policy', 'max', '--runtime-llvm-jit-cache-path', 'disable']
                    if tagged:
                        argv += ['--wasm-policy-global', 'runtime.memtag-backend', 'software', '--runtime-memtag-mode', 'sync']
                        if scenario == 'aliases': argv += ['--runtime-memtag-action', 'silent-log', 'err']
                    argv += ['--run', str(wasm)]
                    start = time.monotonic(); result = subprocess.run(argv, capture_output=True, timeout=120)
                    (out/(run_name+'.stdout')).write_bytes(result.stdout); (out/(run_name+'.stderr')).write_bytes(result.stderr)
                    errors = result.stderr.decode(errors='replace'); crash = scenario.startswith('mismatch-')
                    expected = -signal.SIGILL if crash else 0; passed = result.returncode == expected; violations = []
                    if crash:
                        slot = {'request': 128, 'output': 512, 'length': 1024}[scenario.split('-')[2]]
                        reason = 'Reason: host interface memory tag mismatch' if engine == 'int' else 'Reason: memory tag mismatch'
                        passed = passed and all(token in errors for token in
                            ('[crash] memory-protection', reason, 'Exit status: 122', 'Fault action: crash (OS core-dump policy applies)',
                             f'Memory index: {memory}', f'Pointer: {g.pointer(slot, 5)}', 'Pointer tag: 5', 'Allocation tag: 3'))
                    if scenario == 'aliases':
                        violations = [json.loads(line) for line in errors.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.')]
                        passed = passed and len(violations) >= 13 and all(row['pointer_tag'] == 0 and row['allocation_tag'] == 3 and
                            row['memory'] == memory and row['pointer'] in (128, 512, 1024) for row in violations)
                    report['cases'].append(dict(name=run_name, argv=argv, exit=result.returncode, expected=expected, passed=passed,
                                                seconds=time.monotonic()-start, violations=violations))
                    save(); assert passed, (run_name, result.returncode, errors[-5000:])
                    print('PASS', run_name, flush=True)
        report['inputs_after'] = pins(); assert report['inputs_after'] == report['inputs'], 'lifecycle qualification inputs changed'
        report['complete'] = True
    finally: save()


if __name__ == '__main__':
    main()
