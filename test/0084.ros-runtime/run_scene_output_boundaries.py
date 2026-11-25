#!/usr/bin/env python3
"""Qualify volatile output FP restoration and real tagged-memory boundaries.

All seven output wrappers and published artifact.read run in real Wasm on the
interpreter and LLVM, with memory32/64 and memory index zero/one. Normal tags,
physical aliases in trusted diagnostic mode, and default mismatch crashes are
separate cases. This driver neither grants WASI nor exposes native paths. Every
compiler and runtime inherits the existing 64 GiB, zero-swap cgroup.
"""
import argparse
import json
from pathlib import Path
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
OPERATIONS = [dict(name='artifact.'+name, words=words, bytes=72) for name, words in
              [('begin', 2), ('append', 3), ('commit', 3), ('abort', 2),
               ('remove', 2), ('status', 1), ('receipt', 1)]]
OPERATIONS += [dict(name='artifact.read', words=3, bytes=8)]
PAYLOAD = 0x3139383654554f53


class OutputCalls:
    def address(self, value, tag=None):
        return self.pointer(value, tag) if isinstance(self, TaggedGuest) else value

    def call(self, name, cap, arguments=(), expected=0, *, output=512, length=1024,
             request_tag=None, output_tag=None, length_tag=None, capacity=None):
        descriptor = next(row for row in self.operations if row['name'] == name)
        for index, value in enumerate(arguments):
            self.write(128+8*index, value)
        if capacity is None:
            capacity = descriptor['bytes']
        self.check(f'{cap} i32.const {self.memory} i64.const {self.address(128, request_tag)} '
                   f'i64.const {len(arguments)*8} i64.const {self.address(output, output_tag)} '
                   f'i64.const {capacity} i64.const {self.address(length, length_tag)} call ${name}', expected)


class OutputFP(OutputCalls, FPGuest):
    pass


class OutputTagged(OutputCalls, TaggedGuest):
    pass


def configured():
    return [*base.policy(OPERATIONS), '--ros-scene-grant', 'robot', 'cache', 'cache', 'cache-data',
            'volatile-output', ','.join(row['name'] for row in OPERATIONS),
            '--ros-scene-output-budget', 'robot', '64', '1', '160',
            '--ros-scene-output', 'robot', 'cache', '64', '1', '64', '64']


def guest(width, memory, scenario):
    tagged = scenario not in ('fp', 'denied', 'no-grants')
    g = (OutputTagged if tagged else OutputFP)(OPERATIONS, width, memory)
    if tagged:
        g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
        g.check('call $tag.status'+(' i32.wrap_i64' if width == 64 else ''), 2)
    if scenario in ('denied', 'no-grants'):
        for descriptor in OPERATIONS:
            g.call(descriptor['name'], 'i64.const 1', [1]*descriptor['words'],
                   expected=13 if scenario == 'denied' else 9)
        return g
    cap = g.open('cache')
    mismatch = scenario.split('-')[1:] if scenario.startswith('mismatch-') else []

    def invoke(action, arguments):
        name = 'artifact.'+action
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
        g.call(name, cap, arguments)
        g.equal(g.word(1024), 8 if action == 'read' else 72)
        if action != 'read':
            g.equal(g.word(536), 0)  # Receipt effect_status, separate from carrier status.
        if scenario == 'fp' and action in ('begin', 'append', 'commit', 'abort', 'remove'):
            # Every mutation's retained exact replay also passes through the
            # actual wrapper, with arithmetic immediately after its return.
            saved = [g.save('replay-'+action+'-'+str(index), 512+8*index) for index in range(9)]
            g.call(name, cap, arguments)
            for index, value in enumerate(saved):
                g.check(g.word(512+8*index)+' '+value+' i64.ne')
        return True

    if not invoke('begin', [1, 64]): return g
    obj = g.save('object', 528)
    g.equal(g.word(560), 64); g.equal(g.word(568), 1); g.equal(g.word(576), 0)
    # An undersized receipt cannot copy/allocate the append request or advance
    # its key. The valid same-key append immediately afterwards must succeed.
    g.write(512, 0x123456789abcdef)
    g.call('artifact.append', cap, [2, obj, 0, PAYLOAD], expected=90, capacity=71)
    g.equal(g.word(1024), 72); g.equal(g.word(512), 0x123456789abcdef)
    if not invoke('append', [2, obj, 0, PAYLOAD]): return g
    g.equal(g.word(544), 8); g.equal(g.word(552), 8)
    if not invoke('commit', [3, obj, 8]): return g
    g.equal(g.word(560), 64); g.equal(g.word(568), 2); g.equal(g.word(576), 1)
    if not invoke('status', [obj]): return g
    g.equal(g.word(568), 2)
    if not invoke('receipt', [3]): return g
    g.equal(g.word(512), 3); g.equal(g.word(568), 2)
    if not invoke('read', [obj, 0, 8]): return g
    g.equal(g.word(512), PAYLOAD)
    if scenario == 'aliases':
        # Positive diagnostic control: a non-overlapping mismatched encoding
        # still succeeds, so earlier rejections cannot be credited to tag denial.
        g.call('artifact.read', cap, [obj, 0, 8], length_tag=0)
        g.equal(g.word(512), PAYLOAD); g.equal(g.word(1024), 8)
    if not invoke('remove', [4, obj]): return g
    g.equal(g.word(560), 0); g.equal(g.word(568), 4)
    g.call('artifact.begin', cap, [5, 64]); obj = g.save('second', 528)
    if not invoke('abort', [6, obj]): return g
    g.equal(g.word(560), 0); g.equal(g.word(568), 3)
    g.call('artifact.receipt', cap, [6]); g.equal(g.word(568), 3)
    g.check(cap+' call $close')
    return g


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--prepare-only', action='store_true')
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); base.verify_gate()
    if not args.prepare_only and args.runtime is None:
        parser.error('--runtime is required without --prepare-only')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    paths = [Path(__file__), *[ROOT/'test/0084.ros-runtime'/name for name in
             ('run_scene_fp.py', 'run_scene_memtag.py', 'run_scene_vm.py')]]
    if not args.prepare_only:
        paths += [ROOT/'src/uwvm2/uwvm/imported/scene/host.h',
                  *[ROOT/'src/uwvm2/imported/ros/scene'/name for name in
                    ('output_contract.h', 'outputs.h', 'broker.h', 'deployment.h')]]
    runtime = args.runtime.resolve() if args.runtime else None
    if runtime: paths.append(runtime)
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path): base.sha(path) for path in paths}
    report = dict(complete=False, prepare_only=args.prepare_only, inputs=pins(),
                  scope='actual volatile output FP and software MTE boundaries; no native ROS parity claim',
                  cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  core_limit=0, events_before=(GATE/'memory.events').read_text(), guests=[], cases=[])

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)

    def save():
        report['events_after'] = (GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    scenarios = ['fp', 'denied', 'no-grants', 'valid', 'aliases']
    scenarios += ['mismatch-'+name+'-length' for name in ('begin', 'append', 'commit', 'abort', 'remove', 'status', 'receipt', 'read')]
    scenarios += ['mismatch-append-request', 'mismatch-append-output']
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
                    if engine == 'aot':
                        argv += ['--wasm-policy-global', 'runtime.llvm.policy', 'max', '--runtime-llvm-jit-cache-path', 'disable']
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
                        passed = passed and len(violations) >= 17 and all(row['pointer_tag'] == 0 and row['allocation_tag'] == 3 and
                            row['memory'] == memory and row['pointer'] in (128, 512, 1024) for row in violations)
                    report['cases'].append(dict(name=run_name, argv=argv, exit=result.returncode, expected=expected, passed=passed,
                                                seconds=time.monotonic()-start, violations=violations))
                    save(); assert passed, (run_name, result.returncode, errors[-5000:])
                    print('PASS', run_name, flush=True)
        report['inputs_after'] = pins()
        assert report['inputs_after'] == report['inputs'], 'qualification inputs changed'
        report['complete'] = True
    finally:
        save()


if __name__ == '__main__':
    main()
