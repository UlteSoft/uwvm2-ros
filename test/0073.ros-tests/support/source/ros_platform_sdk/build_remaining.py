#!/usr/bin/env python3
"""Build and measure four current compact interpreter profiles, serially.

The input is a separate frozen ordinary-source snapshot, never the live shared
checkout. These direct compiler products explicitly exclude LLVM execution,
WSEA verification, custom vendor registries and native debugging. Their sizes
do not qualify xmake's default product or a native-bound multi-vendor build.
The independent owner supplies the original hard64GiB/swap0 containment and a
finite dedicated compiler allowance; the hard cgroup limit is never changed.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys


def pin(path):
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path), bytes=path.stat().st_size, sha256=digest)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['runtime_source_root'])
    manifest = json.loads((source / 'source-io.json').read_bytes())
    assert source.is_relative_to(root) and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    compiler = Path('/usr/bin/clang++-21')
    report = dict(status='RUNNING', profiles=[], commands=[], default_authority=[],
                  compiler=pin(compiler), source_manifest=pin(source / 'source-io.json'),
                  source_files=len(manifest), source_bytes=sum(row['bytes'] for row in manifest),
                  file_and_load_budget_bytes=32 << 20,
                  exclusions=['LLVM execution', 'WSEA verifier', 'custom vendor schemas',
                              'native debugger', 'native ROS delivery', 'xmake default product'])

    def stable():
        for row in manifest:
            path = source / row['path']
            assert path.is_file() and not path.is_symlink()
            actual = pin(path)
            assert all(actual[k] == row[k] for k in ('bytes', 'sha256'))

    def save():
        (out / 'build.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(label, argv, timeout=600):
        directory = out / 'commands' / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=source, stdin=subprocess.DEVNULL,
                                    stdout=stdout, stderr=stderr, check=False, timeout=timeout)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode,
                                      stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr')))
        save()
        assert result.returncode == 0, (label, (directory / 'stderr').read_text()[-3000:])

    try:
        stable()
        module_path = Path(inputs['source_root']) / 'sdk/origin/tools/ros_binary_size.py'
        spec = importlib.util.spec_from_file_location('current_compact_elf_reader', module_path)
        size = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(size)
        flags = [compiler, '-std=c++26', '-stdlib=libc++', '-Os', '-g0', '-pthread',
            '-fconstexpr-steps=8388608', '-Wno-undefined-inline', '-ffunction-sections', '-fdata-sections',
            '-DUWVM=2', '-DNDEBUG', '-DUWVM_USE_UWVM_INT', '-DUWVM_DISABLE_JIT', '-DUWVM_USE_THREAD_LOCAL',
            '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64', '-DUWVM_ENABLE_WASIP1_SOCKET',
            '-DUWVM_VERSION_X=2', '-DUWVM_VERSION_Y=0', '-DUWVM_VERSION_Z=3', '-DUWVM_VERSION_S=0']
        for name in ['src', 'third-parties/fast_io/include', 'third-parties/bizwen/include',
                     'third-parties/boost_unordered/include']:
            flags += ['-I', source / name]
        for profile in ['none', 'ros1', 'ros2', 'dual']:
            destination = out / profile
            destination.mkdir()
            selected = flags + (['-DUWVM_IMPORT_ROS1'] if profile in ('ros1', 'dual') else [])
            if profile in ('ros2', 'dual'):
                selected += ['-DUWVM_IMPORT_ROS2']
            for name, relative in [('runtime', 'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'),
                                   ('main', 'src/uwvm2/uwvm/main.default.cpp'),
                                   ('host_api', 'src/uwvm2/uwvm/host_api.default.cpp')]:
                run(profile + '-' + name, [*selected, '-MD', '-MF', destination / (name + '.d'),
                    '-c', source / relative, '-o', destination / (name + '.o')])
            binary = destination / 'uwvm2-ros'
            run(profile + '-link', [compiler, '-stdlib=libc++', '-fuse-ld=/usr/bin/ld.lld-21', '-pthread',
                *[destination / (name + '.o') for name in ['main', 'runtime', 'host_api']],
                '-Wl,--threads=1', '-Wl,--gc-sections', '-ldl', '-o', binary], timeout=90)
            stripped = destination / 'uwvm.stripped'
            run(profile + '-strip', ['/usr/bin/strip', '--strip-all', '-o', stripped, binary], timeout=30)
            run(profile + '-version', [stripped, '--version'], timeout=15)
            observed = size.measure(stripped)
            assert observed['bytes'] <= 32 << 20 and observed['load_file_bytes'] <= 32 << 20
            report['profiles'].append(dict(profile=profile, original=size.measure(binary), stripped=observed))
            save()
        stable()
        assert pin(compiler) == report['compiler']
        report['status'] = 'FROZEN_CURRENT_SOURCE_FOUR_COMPACT_INTERPRETER_SIZE_BUILDS_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
