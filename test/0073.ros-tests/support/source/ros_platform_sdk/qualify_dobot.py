#!/usr/bin/env python3
"""Serial actual DOBOT SDK builds, data contracts and default-denied Wasm runs.

An independent run_dobot owner must bound this process and every descendant in
the original Linux hard64GiB group. No native driver, network, motion, policy,
debugger or updater is used. Original official-suite/performance credit is zero.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time


def pin(path):
    path = Path(path)
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path), bytes=path.stat().st_size, sha256=digest)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    tools = Path(inputs['toolchains'])
    wasi = tools / 'wasi-sdk-33.0-x86_64-linux'
    kit = source / 'sdk/origin/robots/arms/dobot'
    out = args.out
    assert out.is_absolute() and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    report = dict(status='RUNNING', commands=[], artifacts=[], profiles=[], default_authority=[],
                  original_official_cases_run=0, native_delivery_qualified=False,
                  scope='Actual selected SDK/data/default-refusal qualification only; no native driver/controller/performance or cross-host Runtime claim')
    def save():
        (out / 'qualification.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, argv, expected=0, data=None, env=None, deadline=180, cwd=None):
        directory = out / 'commands' / label
        directory.mkdir()
        actual_env = dict(os.environ, **(env or {}))
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), input=data, cwd=cwd or source,
                env=actual_env, stdout=stdout, stderr=stderr, timeout=deadline, check=False)
        row = dict(label=label, argv=list(map(str, argv)), cwd=str(cwd or source),
            exit=result.returncode, expected_exit=expected, stdin_bytes=len(data) if data is not None else None,
            stdin_sha256=hashlib.sha256(data).hexdigest() if data is not None else None,
            stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr'))
        report['commands'].append(row)
        save()
        assert all(row[n]['bytes'] <= 8 << 20 for n in ('stdout', 'stderr'))
        assert result.returncode == expected, (label, result.returncode, (directory / 'stderr').read_text()[-2400:])
        return (directory / 'stdout').read_bytes()
    validator = Path(inputs['validator'])
    runtime = Path(inputs['runtime'])
    def wasm(label, artifact, backend=None):
        run(label + '-validate', [validator, 'validate', artifact])
        if backend:
            run(label + '-imports', [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/tools/check_wasm_imports.py',
                artifact, '--backend', str(backend), '--allow-module', 'wasi_snapshot_preview1',
                '--report', out / (label + '-imports.json')])
        report['artifacts'].append(dict(pin(artifact), label=label))
        save()
    try:
        alias = b'dobot.observation'
        for number, profile in enumerate(('ros1_v3', 'ros1_v4', 'ros2_v4'), 1):
            version = 1 if number < 3 else 2
            family = source / 'sdk/origin/custom-types' / ('dobot-' + profile.replace('_', '-') + '-complete')
            generation = json.loads((family / 'generation.vendor.json').read_bytes())
            include = ['-I' + str(source / 'sdk/origin/include'), '-I' + str(family), '-I' + str(kit / 'include')]
            selected = ['-DUWVM_DOBOT_PROFILE=' + str(number)]
            cpp = ['-std=c++20', '-fno-exceptions', '-fno-rtti']
            wasm_memory = ['-Wl,--threads=1', '-Wl,-z,stack-size=1048576',
                           '-Wl,--initial-memory=16777216', '-Wl,--max-memory=67108864']
            native_dir = out / ('native-' + profile)
            native_dir.mkdir()
            for language, extension, compiler in [('c', 'c', '/usr/bin/clang-21'), ('cpp', 'cc', '/usr/bin/clang++-21')]:
                flags = cpp if language == 'cpp' else ['-std=c11']
                artifact = native_dir / language
                run(profile + '-' + language + '-native-build', [compiler, *flags, '-O1', *include, *selected,
                    kit / 'tests' / ('contract_' + profile + '.' + extension), '-o', artifact])
                run(profile + '-' + language + '-native-contract', [artifact])
                artifact = out / (profile + '-' + language + '-contract.wasm')
                run(profile + '-' + language + '-wasm-build', [wasi / 'bin' / ('clang++' if language == 'cpp' else 'clang'),
                    *flags, '-O1', *wasm_memory, *include, *selected,
                    kit / 'tests' / ('contract_' + profile + '.' + extension), '-o', artifact])
                wasm(profile + '-' + language + '-contract', artifact)
                run(profile + '-' + language + '-wasm-contract', [runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', artifact])
                app = out / (profile + '-' + language + '-app.wasm')
                run(profile + '-' + language + '-app-build', [wasi / 'bin' / ('clang++' if language == 'cpp' else 'clang'),
                    *flags, '-Os', *wasm_memory, *include, *selected, kit / 'examples' / ('guest.' + extension), '-o', app])
                wasm(profile + '-' + language + '-app', app, version)
                for mode in ('raw', 'state', 'query'):
                    assert run(profile + '-' + language + '-denied-' + mode,
                        [runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', app, mode, alias.decode()], expected=13) == b''
            vectors = out / ('golden-vectors-' + profile)
            run(profile + '-independent-vectors', [sys.executable, '-I', '-S', '-B', kit / 'tests/generate_golden_vectors.py',
                '--source', source, '--profile', profile, '--out', vectors])
            description = json.loads((vectors / 'vectors.json').read_bytes())
            assert description['records'] == len(generation['records'])
            for index, batch in enumerate(description['products']):
                for language, compiler in [('c', '/usr/bin/clang-21'), ('cpp', '/usr/bin/clang++-21')]:
                    name = profile + '-golden-' + language + '-' + str(index)
                    flags = cpp + ['-x', 'c++'] if language == 'cpp' else ['-std=c11']
                    artifact = out / name
                    run(name + '-native-build', [compiler, *flags, '-O1', *include, vectors / batch['file'], '-o', artifact])
                    run(name + '-native-run', [artifact])
                    artifact = out / (name + '.wasm')
                    run(name + '-wasm-build', [wasi / 'bin' / ('clang++' if language == 'cpp' else 'clang'), *flags,
                        '-O1', *wasm_memory, *include, vectors / batch['file'], '-o', artifact])
                    wasm(name, artifact)
                    run(name + '-wasm-run', [runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', artifact])
            report['profiles'].append(dict(profile=profile, complete_records=len(generation['records']),
                independent_record_vectors=description['records'], scalar_or_string_field_instances=description['scalar_or_string_field_instances']))
            save()
        report['status'] = 'DOBOT_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
