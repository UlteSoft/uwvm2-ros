#!/usr/bin/env python3
"""Exercise real kernel launch failures and cleanup from immutable source snapshots.

Run in the existing 64 GiB cgroup. Namespace identity failure and cancellation around fork distinguish
launcher reaping from process-tree cleanup, without a simulated process provider.
The reference must fail the new assertions; corrected Clang ASan/UBSan and GCC
builds must pass. This qualifies the closed Linux component, not vendor drivers.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shutil
import signal
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--reference-root', type=Path, required=True)
    parser.add_argument('--component-receipt', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    inode = GATE.stat().st_ino

    def gate():
        assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
        assert GATE.stat().st_ino == inode
        assert (GATE/'memory.max').read_text().strip() == '68719476736'
        assert (GATE/'memory.swap.max').read_text().strip() == '0'

    gate(); resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    roots = {'reference': args.reference_root.resolve(), 'corrected': args.source_root.resolve()}
    manifests = {key: json.loads((root/'source-manifest.json').read_text()) for key, root in roots.items()}
    assert all(sha(roots[key]/path) == value for key, rows in manifests.items() for path, value in rows.items())
    fixtures = out/'fixtures'; fixtures.mkdir()
    inputs = {str(Path(__file__).resolve()): sha(__file__)}
    for source in (ROOT/'test/0084.ros-runtime').iterdir():
        if source.suffix in ('.cpp', '.h', '.inc'):
            target = fixtures/source.name
            shutil.copyfile(source, target)
            inputs[str(source)] = inputs[str(target)] = sha(source)
    receipt = args.component_receipt.resolve()
    original = json.loads(receipt.read_text()); assert original['complete']
    names = ('bootstrap', 'backend.so', 'matrix.weights', 'ld-linux-x86-64.so.2',
             'libc.so.6', 'libstdc++.so.6', 'libgcc_s.so.1', 'libm.so.6', 'bwrap')
    images = [receipt.parent/'images'/name for name in names]
    inputs.update({str(path): sha(path) for path in [receipt, *images, *[root/'source-manifest.json' for root in roots.values()]]})
    report = dict(complete=False, scope=__doc__, invocation=list(os.sys.argv), cgroup=str(GATE), cgroup_inode=inode,
        memory_max=68719476736, swap_max=0, events_before=(GATE/'memory.events').read_text(),
        source_roots={key: str(value) for key, value in roots.items()}, inputs=inputs, commands=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, argv, timeout=300, negative=False):
        gate(); argv = list(map(str, argv)); started = time.monotonic()
        process = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, start_new_session=True,
            env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        timed_out = False
        try:
            stdout, stderr = process.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            timed_out = True
            os.killpg(process.pid, signal.SIGKILL)
            stdout, stderr = process.communicate()
        (out/(label+'.stdout')).write_bytes(stdout); (out/(label+'.stderr')).write_bytes(stderr)
        report['commands'].append(dict(label=label, argv=argv, exit=process.returncode, timeout=timed_out,
            expected_failure=negative, seconds=time.monotonic()-started)); save()
        if negative:
            assert process.returncode != 0 and (timed_out or b'Assertion' in stderr), (label, stderr[-5000:])
        else:
            assert process.returncode == 0 and not timed_out, (label, stderr[-5000:])
        print(label+': '+str(process.returncode)+(' (expected failure)' if negative else ''), flush=True)
        return stdout

    success = False
    try:
        for variant, compiler, root, flags in [
            ('reference', '/usr/bin/clang++-22', roots['reference'], []),
            ('sanitized', '/usr/bin/clang++-22', roots['corrected'], ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']),
            ('gcc', '/usr/bin/g++-13', roots['corrected'], [])]:
            binary = out/variant
            run('compile-'+variant, [compiler, '-std=c++23', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror', '-pthread',
                '-I'+str(root/'src'), '-isystem', root/'third-parties/fast_io/include', *flags,
                fixtures/'component_identity_failure.cpp', '-Wl,--wrap=read,--wrap=fork', '-ldl', '-o', binary])
            model_binary = out/(variant+'-model')
            run('compile-'+variant+'-model', [compiler, '-std=c++23', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror', '-pthread',
                '-I'+str(root/'src'), '-isystem', root/'third-parties/fast_io/include', *flags,
                fixtures/'model_identity_failure.cpp', '-Wl,--wrap=read,--wrap=fork', '-ldl', '-o', model_binary])
            report.setdefault('binaries', {})[str(binary)] = sha(binary)
            report['binaries'][str(model_binary)] = sha(model_binary)
            stdout = run(variant+'-model-identity', [model_binary, *images], 15, negative=variant=='reference')
            if variant != 'reference': assert b'PASS model identity refusal' in stdout
            for mode in ('identity-refusal', 'cancel-before', 'cancel-fork'):
                stdout = run(variant+'-'+mode, [binary, *images, mode], 15, negative=variant=='reference')
                if variant != 'reference': assert b'PASS identity safety '+mode.encode() in stdout
        success = True
    finally:
        gate()
        report['inputs_unchanged'] = all(sha(path) == value for path, value in inputs.items())
        report['sources_unchanged'] = all(sha(roots[key]/path) == value for key, rows in manifests.items() for path, value in rows.items())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] = success and report['inputs_unchanged'] and report['sources_unchanged']; save()
    assert report['complete']


if __name__ == '__main__':
    main()
