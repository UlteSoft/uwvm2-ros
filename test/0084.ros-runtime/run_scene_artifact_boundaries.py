#!/usr/bin/env python3
"""Exercise artifact FP restoration and real tagged-memory host boundaries.

The existing artifact semantic guest receives dynamic Wasm FP checks after
every host return. Tagged guests independently exercise all three new imports,
including physically aliased result-length slots under trusted diagnostic
mode, checked canaries, and default mismatch crashes. Run only in the existing
64 GiB, zero-swap cgroup; every output stays under the SDK .tmp directory.
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
import run_scene_artifacts as artifact
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest, add_private_instructions

base = artifact.base
ROOT, GATE = base.ROOT, base.GATE
OPERATIONS = artifact.OPERATIONS[:4]


class TaggedArtifactGuest(TaggedGuest):
    def call(self, name, cap, arguments=(), expected=0, *, output=512, length=1024,
             request_tag=None, output_tag=None, length_tag=None, capacity=None):
        descriptor = next(row for row in self.operations if row['name'] == name)
        for index, value in enumerate(arguments):
            self.write(128+8*index, value)
        if capacity is None:
            capacity = max(descriptor['bytes'], 32)
        self.check(f'{cap} i32.const {self.memory} i64.const {self.pointer(128, request_tag)} '
                   f'i64.const {descriptor["words"]*8} i64.const {self.pointer(output, output_tag)} '
                   f'i64.const {capacity} i64.const {self.pointer(length, length_tag)} call ${name}', expected)


def tagged_guest(width, memory, scenario):
    g = TaggedArtifactGuest(OPERATIONS, width, memory)
    g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64' if width == 64 else ''), 2)
    cap = g.open('config')
    if scenario == 'mismatch-open':
        g.call('artifact.open', cap, artifact.opening(), expected=22, length_tag=5)
        return g

    def aliases(name, arguments):
        for slot in (512, 128):
            g.write(512, 0x123456789abcdef); g.write(1024, 0x3456789)
            g.call(name, cap, arguments, expected=22, length=slot, length_tag=0)
            g.equal(g.word(512), 0x123456789abcdef); g.equal(g.word(1024), 0x3456789)
            first = 'i64.const '+str(arguments[0]) if isinstance(arguments[0], int) else arguments[0]
            g.check(g.word(128)+' '+first+' i64.ne')

    if scenario == 'aliases':
        aliases('artifact.open', artifact.opening())
    # max-resources=1 proves rejected aliased opens did not publish handles.
    g.call('artifact.open', cap, artifact.opening()); handle = g.save('artifact')
    if scenario in ('mismatch-read', 'mismatch-describe'):
        operation = 'artifact.'+scenario.removeprefix('mismatch-')
        g.call(operation, cap, [handle, 0, 8] if operation.endswith('read') else [handle], expected=22, length_tag=5)
        return g
    if scenario == 'aliases':
        aliases('artifact.describe', [handle])
        aliases('artifact.read', [handle, 0, 8])
    g.call('artifact.describe', cap, [handle]); g.equal(g.word(1024), 64)
    for index, value in enumerate([artifact.GENERATION, len(artifact.PAYLOAD), 2, 32, *artifact.opening()[1:]]):
        g.equal(g.word(512+index*8), value)
    g.call('artifact.read', cap, [handle, 0, 8]); g.equal(g.word(1024), 8)
    g.equal(g.word(512), int.from_bytes(artifact.PAYLOAD[:8], 'little'))
    if scenario == 'aliases':
        # Different encodings with disjoint spans must still succeed in the
        # selected diagnostic mode, proving alias rejection was not tag denial.
        g.call('artifact.read', cap, [handle, 0, 8], length_tag=0)
        g.equal(g.word(1024), 8); g.equal(g.word(512), int.from_bytes(artifact.PAYLOAD[:8], 'little'))
    g.call('resource.close', cap, [handle]); g.check(cap+' call $close')
    return g


def tagged_policy(source):
    return [*base.policy(OPERATIONS), '--ros-scene-grant', 'robot', 'config', 'configuration', 'config', 'sealed-input',
            'artifact.open,artifact.read,artifact.describe,resource.close',
            '--ros-scene-input', 'robot', 'config', str(source), artifact.DIGEST.hex(),
            str(artifact.GENERATION), str(len(artifact.PAYLOAD)), '32',
            '--ros-scene-input-budget', 'robot', str(len(artifact.PAYLOAD)), '1',
            '--ros-scene-limit', 'robot', 'config', 'max-resources', '1']


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
    source = out/'artifact-input.bin'; source.write_bytes(artifact.PAYLOAD)
    paths = [Path(__file__), *[ROOT/'test/0084.ros-runtime'/name for name in
             ('run_scene_artifacts.py', 'run_scene_fp.py', 'run_scene_memtag.py', 'run_scene_vm.py')],
             ROOT/'src/uwvm2/uwvm/imported/scene/host.h', ROOT/'src/uwvm2/imported/ros/scene/artifact_contract.h',
             ROOT/'src/uwvm2/imported/ros/scene/artifacts.h', ROOT/'src/uwvm2/imported/ros/scene/broker.h',
             ROOT/'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp', source]
    runtime = args.runtime.resolve() if args.runtime else None
    if runtime:
        paths.append(runtime)
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path):base.sha(path) for path in paths}
    report = dict(complete=False, prepare_only=args.prepare_only, inputs=pins(),
                  scope='artifact FP arithmetic and software MTE host-boundary execution; no native ROS parity claim',
                  cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  core_limit=0, events_before=(GATE/'memory.events').read_text(), guests=[], cases=[])

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)

    def save():
        report['events_after'] = (GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def compile_guest(name, guest, tagged=False):
        wat, wasm = out/(name+'.wat'), out/(name+'.wasm'); wat.write_text(guest.render())
        argv = ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)]
        result = subprocess.run(argv, capture_output=True, timeout=30)
        (out/(name+'-compile.stderr')).write_bytes(result.stderr)
        assert result.returncode == 0, result.stderr.decode(errors='replace')
        if tagged:
            wasm.write_bytes(add_private_instructions(wasm.read_bytes(), len(guest.checks), guest.memory))
        report['guests'].append(dict(name=name, argv=argv, wasm_sha256=base.sha(wasm), assertions=len(guest.checks),
                                    fp_sites=getattr(guest, 'fp_sites', 0), arithmetic_checks_per_site=0 if tagged else 12))
        save(); return wasm

    def execute(name, guest, wasm, flags, engine, scenario=None):
        base.verify_gate()
        argv = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                '--runtime-compile-threads', '1', '--log-color', 'disable', *flags]
        if engine == 'aot':
            argv += ['--wasm-policy-global', 'runtime.llvm.policy', 'max', '--runtime-llvm-jit-cache-path', 'disable']
        if scenario:
            argv += ['--wasm-policy-global', 'runtime.memtag-backend', 'software', '--runtime-memtag-mode', 'sync']
            if scenario == 'aliases':
                argv += ['--runtime-memtag-action', 'silent-log', 'err']
        argv += ['--run', str(wasm)]
        begin = time.monotonic(); result = subprocess.run(argv, capture_output=True, timeout=120)
        (out/(name+'.stdout')).write_bytes(result.stdout); (out/(name+'.stderr')).write_bytes(result.stderr)
        errors = result.stderr.decode(errors='replace')
        crash = bool(scenario and scenario.startswith('mismatch-'))
        expected = -signal.SIGILL if crash else 0
        passed = result.returncode == expected; violations = []
        if crash:
            reason = 'Reason: host interface memory tag mismatch' if engine == 'int' else 'Reason: memory tag mismatch'
            passed = passed and all(token in errors for token in
                ('[crash] memory-protection', reason, 'Exit status: 122', 'Fault action: crash (OS core-dump policy applies)',
                 f'Memory index: {guest.memory}', f'Pointer: {guest.pointer(1024, 5)}', 'Pointer tag: 5', 'Allocation tag: 3'))
        if scenario == 'aliases':
            violations = [json.loads(line) for line in errors.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.')]
            passed = passed and len(violations) >= 7 and all(row['pointer_tag']==0 and row['allocation_tag']==3 and
                row['memory']==guest.memory and row['pointer'] in (128,512,1024) for row in violations)
        report['cases'].append(dict(name=name, argv=argv, exit=result.returncode, expected=expected, passed=passed,
                                    seconds=time.monotonic()-begin, violations=violations))
        save(); assert passed, (name, result.returncode, errors[-5000:])
        print('PASS', name, flush=True)

    try:
        for width, memory in ((32,0), (64,0), (32,1), (64,1)):
            original = artifact.base.Guest
            artifact.base.Guest = FPGuest
            try:
                guest = artifact.successful_guest(width, memory)
            finally:
                artifact.base.Guest = original
            name = f'fp-memory{width}-index{memory}'; wasm = compile_guest(name, guest)
            if not args.prepare_only:
                for engine in ('int', 'aot'):
                    execute(name+'-'+engine, guest, wasm, artifact.configured(source), engine)
            for scenario in ('valid', 'aliases', 'mismatch-open', 'mismatch-read', 'mismatch-describe'):
                guest = tagged_guest(width, memory, scenario)
                name = f'memtag-{scenario}-memory{width}-index{memory}'
                wasm = compile_guest(name, guest, True)
                if not args.prepare_only:
                    for engine in ('int', 'aot'):
                        execute(name+'-'+engine, guest, wasm, tagged_policy(source), engine, scenario)
        report['inputs_after'] = pins()
        assert report['inputs_after'] == report['inputs'], 'qualification inputs changed'
        report['complete'] = True
    finally:
        save()


if __name__ == '__main__':
    main()
