#!/usr/bin/env python3
"""Qualify real model and DDS process-tree exit after their owner dies.

Independent observer pidfds check workers before release, at readiness and
after attachment. Rescue never counts as product cleanup. Optimized Clang,
GCC and ASan/UBSan use frozen sources under the existing 64 GiB gate.
This is Linux x86-64 lifecycle evidence, not physical-stop qualification.
"""
import argparse
import json
import os
from pathlib import Path
import resource
import shutil
import subprocess
import time
from module_exchange_performance import ROOT, GATE, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--model-images', type=Path, required=True)
    parser.add_argument('--communication-images', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    root = args.source_root.resolve()
    manifest = json.loads((root/'source-manifest.json').read_text())
    assert all(sha(root/name) == digest for name, digest in manifest.items())
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    fixture = out/'fixtures'
    fixture.mkdir()
    pins = {str(Path(__file__).resolve()): sha(__file__)}
    for name in ['worker_owner_death.cpp', 'model_worker_fixture.h', 'communication_worker_sandbox.cpp']:
        original = ROOT/'test/0084.ros-runtime'/name
        shutil.copyfile(original, fixture/name)
        pins[str(original)] = pins[str(fixture/name)] = sha(original)
    common = ['ld-linux-x86-64.so.2', 'libc.so.6', 'libstdc++.so.6', 'libgcc_s.so.1', 'libm.so.6', 'bwrap']
    images = dict(model=[args.model_images.resolve()/n for n in ['bootstrap', 'backend.so', 'matrix.weights', *common]],
                  communication=[args.communication_images.resolve()/n for n in ['bootstrap', 'backend.so', *common]])
    for paths in images.values():
        for path in paths:
            pins[str(path)] = sha(path)
    report = dict(complete=False, scope=__doc__, source_root=str(root), inputs=pins,
                  cgroup=str(GATE), memory_max=68719476736, swap_max=0,
                  events_before=(GATE/'memory.events').read_text(), commands=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, command):
        verify_gate()
        command = list(map(str, command))
        start = time.monotonic()
        result = subprocess.run(command, capture_output=True, timeout=180,
            env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (out/(label+'.stdout')).write_bytes(result.stdout)
        (out/(label+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(label=label, argv=command, exit=result.returncode, seconds=time.monotonic()-start))
        save()
        assert result.returncode == 0, (label, result.stderr[-4000:])
        print(label+': 0', flush=True)
        return result

    success = False
    try:
        for variant, compiler, flags in [
            ('clang', '/usr/bin/clang++-22', []),
            ('sanitized', '/usr/bin/clang++-22', ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']),
            ('gcc', '/usr/bin/g++-13', [])]:
            for kind in images:
                binary = out/(variant+'-'+kind)
                defines = ['-DUWVM_RELEASE_COMMUNICATION'] if kind == 'communication' else []
                run('compile-'+variant+'-'+kind, [compiler, '-std=c++23', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror', '-pthread',
                    '-I'+str(root/'src'), '-isystem', root/'third-parties/fast_io/include', *flags, *defines,
                    fixture/'worker_owner_death.cpp', '-Wl,--wrap=read', '-o', binary])
                pins[str(binary)] = sha(binary)
                for stage in ['release', 'ready', 'attached']:
                    result = run(variant+'-'+kind+'-'+stage, [binary, *images[kind], stage])
                    assert b'root-exited=1 worker-exited=1 before-rescue' in result.stdout
        success = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(name) == digest for name, digest in pins.items())
        report['sources_unchanged'] = all(sha(root/name) == digest for name, digest in manifest.items())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] = success and report['inputs_unchanged'] and report['sources_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
