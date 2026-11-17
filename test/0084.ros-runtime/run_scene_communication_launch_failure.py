#!/usr/bin/env python3
"""Exercise real Wasm creation under inherited kernel launch refusal.

The production VM, exact ROS image closure and native syscalls are real. Only
the kernel refusal is test-injected. Failed take must publish no node, retained
descriptions must remain conservative, and the VM must exit within its bound.
This does not qualify arbitrary drivers, physical stop or crash durability.
"""
import argparse
import ctypes
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import time

from run_scene_communication import (
    ROOT, GATE, verify_gate, sha, CommunicationGuest, OPERATIONS, selected,
    configured, alias, profile, definition, worker_definition, u64, text,
)


def guest(documents, width, memory, expected):
    g = CommunicationGuest(OPERATIONS, width, memory)
    g.loop_id = 0
    g.globals.update(['status', 'overall', 'launch-deadline'])
    g.check('call $abi', 2)
    g.steady = g.open('steady')
    g.call('clock.create', g.steady, [0])
    g.clock = g.save('steady-clock')
    g.body.append(g.refresh(30000000000)+' global.set $overall')
    rows = {row['identity']: row for row in selected(documents[0], 0)}
    context_cap = g.open(alias(0, 'context'))
    node_cap = g.open(alias(0, 'node'))
    profiles = {name: definition.compile_definition(rows[name])[1] for name in ('context', 'node')}
    g.raw('context.open', context_cap, profile(profiles['context']))
    context = g.save('context')
    g.body.append(g.refresh(14000000000)+' global.set $launch-deadline')
    body = bytes(16)+profile(profiles['node'])+u64(1)+bytes(8)
    patches = [(0, context_cap), (8, context), (len(body)-8, 'global.get $launch-deadline')]
    g.raw('node.open', node_cap, body, patches, capacity=48)
    g.equal(g.word(512), 1)
    request = g.save('creation-request', 520)
    patch = [(0, request)]
    g.body.append('block $done loop $pending')
    g.dynamic('communication.open.take', node_cap, bytes(8), patch, capacity=64)
    g.body.append('global.get $status i32.const 11 i32.ne br_if $done')
    g.bound()
    g.body.append('br $pending end end')
    g.check('global.get $status', expected)
    # Failure is retained, not turned into a usable endpoint by another take.
    for _ in range(3):
        g.raw('communication.open.take', node_cap, bytes(8), patch, capacity=64, expected=expected)
        g.raw('request.describe', node_cap, bytes(8), patch)
        g.check(g.word(512)+' '+request+' i64.ne')
        g.exact(520, text('outcome_unknown')+text('unknown'))
    return g


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--pair-receipt', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    assert os.sched_getaffinity(0).issubset({4, 5, 6, 7})
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    # Adopt orphaned descendants so a leaked worker cannot be hidden by VM exit.
    libc = ctypes.CDLL(None, use_errno=True)
    assert libc.prctl(36, 1, 0, 0, 0) == 0  # PR_SET_CHILD_SUBREAPER
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    runtime = args.runtime.resolve(); source = args.source_root.resolve()
    build = runtime.parent/'full-build-results.json'
    built = json.loads(build.read_text())
    assert built['complete'] and built['binary_sha256'] == sha(runtime) and built['source_root'] == str(source)
    pair = args.pair_receipt.resolve(); evidence = json.loads(pair.read_text()); assert evidence['complete']
    wrapper = Path(__file__).with_name('component_launch_failure_wrapper.cpp')
    inputs = {str(p.resolve()): sha(p) for p in [Path(__file__), wrapper, build, runtime, pair, source/'source-manifest.json',
        Path(__file__).with_name('run_scene_communication.py'), Path(__file__).with_name('run_scene_recording.py'),
        Path(__file__).with_name('run_scene_vm.py'), Path(definition.__file__), Path(worker_definition.__file__)]}
    packs, documents = [], []
    for side in range(2):
        path = pair.parent/f'pack-{side}/manifest.json'; assert evidence['pack_sha256'][str(path)] == sha(path)
        pack = json.loads(path.read_text()); assert pack['complete']; packs.append(pack); inputs[str(path)] = sha(path)
        for image in [pack['launcher'], *pack['files']]:
            path = Path(image['path']); assert sha(path) == image['sha256'] and path.stat().st_size == image['bytes']
            inputs[str(path)] = sha(path)
        path = pair.parent/f'peer-{side}.json'; document = json.loads(path.read_text()); documents.append(document); inputs[str(path)] = sha(path)
        encoded, _ = worker_definition.compile_definition(document)
        assert encoded == Path(next(row['path'] for row in pack['files'] if row['destination'] == '/worker/profile')).read_bytes()
    flags = configured(packs, documents)
    record = dict(complete=False, scope=__doc__, runtime_sha256=sha(runtime), inputs=inputs, cases=[], commands=[],
        cgroup=str(GATE), memory_max=68719476736, swap_max=0, events_before=(GATE/'memory.events').read_text())

    def save():
        (out/'results.json').write_text(json.dumps(record, indent=2)+'\n')

    def run(label, argv, timeout=120):
        verify_gate(); argv = list(map(str, argv)); start = time.monotonic()
        process = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, start_new_session=True)
        timed_out = False
        try:
            stdout, stderr = process.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            timed_out = True; os.killpg(process.pid, signal.SIGKILL); stdout, stderr = process.communicate()
        (out/(label+'.stdout')).write_bytes(stdout); (out/(label+'.stderr')).write_bytes(stderr)
        record['commands'].append(dict(label=label, argv=argv, exit=process.returncode, timed_out=timed_out, seconds=time.monotonic()-start)); save()
        assert process.returncode == 0 and not timed_out, (label, stderr[-7000:])

    success = False
    try:
        for fault, expected, defines in [('pidfd', 95, ['PIDFD']), ('gate', 5, ['GATE']), ('both', 95, ['PIDFD', 'GATE'])]:
            binary = out/('launch-'+fault)
            run('compile-'+fault, ['/usr/bin/clang++-22', '-std=c++23', '-O3', '-g', '-Wall', '-Wextra', '-Werror',
                '-DUWVM_TEST_RUNTIME_PATH="'+str(runtime)+'"', *['-DUWVM_TEST_LAUNCH_'+value+'_FAILURE=1' for value in defines], wrapper, '-o', binary])
            record.setdefault('wrappers', {})[str(binary)] = sha(binary)
            for width, memory in [(32, 0), (64, 0), (32, 1), (64, 1)]:
                name = f'{fault}-{width}-{memory}'; g = guest(documents, width, memory, expected)
                wat, wasm = out/(name+'.wat'), out/(name+'.wasm'); wat.write_text(g.render())
                run('compile-'+name, ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', wat, '-o', wasm])
                for engine in ('int', 'aot'):
                    label = name+'-'+engine
                    run(label, [binary, '--wasm-set-main-module-name', 'main', '--runtime-'+engine, '--runtime-compile-threads', '0',
                        '--runtime-llvm-jit-cache-path', 'disable', '--log-color', 'disable', *flags, '--run', wasm])
                    # This runner owns no other children at this point.
                    try:
                        remaining = os.waitpid(-1, os.WNOHANG)
                    except ChildProcessError:
                        remaining = None
                    assert remaining is None, ('orphaned process after VM exit', remaining)
                    record['cases'].append(dict(name=label, passed=True, assertions=len(g.checks), wasm_sha256=sha(wasm), no_descendants=True)); save()
                    print('PASS '+label, flush=True)
        success = True
    finally:
        record['inputs_unchanged'] = all(sha(Path(p)) == digest for p, digest in inputs.items())
        record['events_after'] = (GATE/'memory.events').read_text()
        record['complete'] = success and record['inputs_unchanged'] and record['events_before'] == record['events_after']; save()
    assert record['complete']


if __name__ == '__main__':
    main()
