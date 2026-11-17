#!/usr/bin/env python3
"""Check exact finite ceilings before model/component and DDS sealed execution.

Run under the existing 64 GiB zero-swap gate. Builds use -O3 -g. The frozen
reference must fail readback fault assertions after cleaning its worker; the
candidate must pass Clang, GCC and ASan/UBSan. ABI boundary instantiations test
32/64-bit conversion logic, not native execution on other architectures.
"""
import argparse
import json
import os
from pathlib import Path
import resource
import shutil
import signal
import subprocess
import time
from module_exchange_performance import ROOT, GATE, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--reference-root', type=Path, required=True)
    parser.add_argument('--model-images', type=Path, required=True)
    parser.add_argument('--communication-images', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    roots = dict(reference=args.reference_root.resolve(), corrected=args.source_root.resolve())
    manifests = {key: json.loads((root/'source-manifest.json').read_text()) for key, root in roots.items()}
    assert all(sha(roots[key]/name) == digest for key, rows in manifests.items() for name, digest in rows.items())
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    fixture = out/'fixtures'
    fixture.mkdir()
    pins = {str(Path(__file__).resolve()): sha(__file__)}
    for name in ['linux_resource_limits.cpp', 'worker_resource_limits.cpp',
                 'model_worker_fixture.h', 'communication_worker_sandbox.cpp', 'model_sandbox.cpp']:
        source = ROOT/'test/0084.ros-runtime'/name
        shutil.copyfile(source, fixture/name)
        pins[str(source)] = pins[str(fixture/name)] = sha(source)
    common = ['ld-linux-x86-64.so.2', 'libc.so.6', 'libstdc++.so.6', 'libgcc_s.so.1', 'libm.so.6', 'bwrap']
    images = dict(model=[args.model_images.resolve()/n for n in ['bootstrap', 'backend.so', 'matrix.weights', *common]],
                  communication=[args.communication_images.resolve()/n for n in ['bootstrap', 'backend.so', *common]])
    for paths in images.values():
        for path in paths:
            pins[str(path)] = sha(path)
    report = dict(complete=False, scope=__doc__, invocation=os.sys.argv,
                  source_roots={k: str(v) for k, v in roots.items()}, inputs=pins,
                  cgroup=str(GATE), memory_max=68719476736, swap_max=0,
                  events_before=(GATE/'memory.events').read_text(), commands=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, command, expected=0):
        verify_gate()
        command = list(map(str, command))
        started = time.monotonic()
        result = subprocess.run(command, capture_output=True, timeout=180,
                                env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (out/(label+'.stdout')).write_bytes(result.stdout)
        (out/(label+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(label=label, argv=command, exit=result.returncode,
                                       expected_exit=expected, seconds=time.monotonic()-started))
        save()
        assert result.returncode == expected, (label, result.stderr[-4000:])
        print(label+': '+str(result.returncode), flush=True)
        return result

    success = False
    try:
        for variant, compiler, root, flags in [
            ('reference', '/usr/bin/clang++-22', roots['reference'], []),
            ('clang', '/usr/bin/clang++-22', roots['corrected'], []),
            ('sanitized', '/usr/bin/clang++-22', roots['corrected'], ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']),
            ('gcc', '/usr/bin/g++-13', roots['corrected'], [])]:
            build = [compiler, '-std=c++23', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror', '-pthread',
                     '-I'+str(root/'src'), '-isystem', root/'third-parties/fast_io/include', *flags]
            if variant != 'reference':
                binary = out/(variant+'-boundaries')
                run('compile-'+binary.name, [*build, fixture/'linux_resource_limits.cpp', '-o', binary])
                pins[str(binary)] = sha(binary)
                assert b'PASS' in run(binary.name, [binary]).stdout
            for kind in images:
                binary = out/(variant+'-'+kind)
                defines = ['-DUWVM_RELEASE_COMMUNICATION'] if kind == 'communication' else []
                run('compile-'+binary.name, [*build, *defines, fixture/'worker_resource_limits.cpp',
                    '-Wl,--wrap=setrlimit,--wrap=getrlimit', '-o', binary])
                pins[str(binary)] = sha(binary)
                modes = ['readback-denied', 'soft-mismatch', 'hard-mismatch']
                if variant != 'reference':
                    modes += ['setter-denied', 'success']
                for mode in modes:
                    result = run(binary.name+'-'+mode, [binary, *images[kind], mode],
                                 -signal.SIGABRT if variant == 'reference' else 0)
                    if variant == 'reference':
                        assert b'identity!=error::success' in result.stderr
                    else:
                        assert b'PASS' in result.stdout
                if variant != 'reference':
                    source = 'model_sandbox.cpp' if kind == 'model' else 'communication_worker_sandbox.cpp'
                    regression = out/(variant+'-'+kind+'-regression')
                    run('compile-'+regression.name, [*build, fixture/source, '-o', regression])
                    pins[str(regression)] = sha(regression)
                    run(regression.name, [regression, *images[kind]])
        success = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(name) == digest for name, digest in pins.items())
        report['sources_unchanged'] = all(sha(roots[key]/name) == digest for key, rows in manifests.items() for name, digest in rows.items())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] = success and report['inputs_unchanged'] and report['sources_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
