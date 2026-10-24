#!/usr/bin/env python3
"""Qualify public SDK source views in the original bounded Linux cgroup.

Restore only content-checked inert codec sources from the existing independently
read-back checkpoint, then build real applications through the new public paths.
Source/navigation tests do not grant official-example, native-driver or robot
acceptance. No native robot or middleware process is started by this lane.
"""
import argparse
import gzip
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def restore(root, source, out):
    checkpoint = root.parent.parent / 'recovery/ros-remaining-r60/current-source-audit-r1.json'
    proof = json.loads(checkpoint.read_bytes())
    client = json.loads((root / 'control/client-readbacks/current-source-audit-r1-client-readback.json').read_bytes())
    assert proof['status'] == 'TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
    assert client['status'] == 'CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
    assert all(proof[k] == client[k] for k in ('bytes', 'sha256', 'manifest_sha256'))
    archive_path = Path(proof['primary'])
    assert archive_path.stat().st_size == proof['bytes'] and sha(archive_path) == proof['sha256']
    prefix = 'out/current-source-audit-inputs-r1/'
    found, pending, checked = set(), {}, []
    with archive_path.open('rb') as raw, gzip.GzipFile(fileobj=raw) as zipped:
        with tarfile.open(fileobj=zipped, mode='r|') as archive:
            iterator = iter(archive)
            first = next(iterator)
            assert first.name == 'manifest.json' and first.isfile() and first.size <= 16 << 20
            data = archive.extractfile(first).read()
            assert hashlib.sha256(data).hexdigest() == proof['manifest_sha256']
            manifest = json.loads(data)
            assert manifest['original_root'] == str(root)
            for row in manifest['roles']:
                if row['type'] != 'regular' or not row['path'].startswith(prefix + 'sdk/origin/custom-types/'):
                    continue
                relative = Path(row['path'][len(prefix):])
                assert not relative.is_absolute() and '..' not in relative.parts
                dst = source / relative
                checked.append(dict(path=str(dst), bytes=row['bytes'], sha256=row['sha256']))
                if dst.exists():
                    assert not dst.is_symlink() and dst.stat().st_size == row['bytes'] and sha(dst) == row['sha256']
                else:
                    pending.setdefault(row['object'], []).append((dst, row))
            for member in iterator:
                if member.name not in pending:
                    continue
                assert member.isfile() and member.size <= 16 << 20
                data = archive.extractfile(member).read((16 << 20) + 1)
                for dst, row in pending[member.name]:
                    assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
                    dst.parent.mkdir(parents=True, exist_ok=True)
                    assert dst.parent.resolve().is_relative_to(source)
                    with dst.open('xb') as stream:
                        stream.write(data)
                found.add(member.name)
            assert found == set(pending)
        while zipped.read(1 << 20):
            pass
    assert sha(archive_path) == proof['sha256']
    (out / 'restored-source.json').write_text(json.dumps(dict(
        status='INERT_SOURCE_CONTENT_RESTORED_NOT_PRIOR_EXECUTION_STATUS', files=checked,
        checkpoint_sha256=proof['sha256']), indent=2) + '\n')
    return checked


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source, tools = Path(inputs['source_root']), Path(inputs['toolchains'])
    wasi = tools / 'wasi-sdk-33.0-x86_64-linux'
    runtime, validator = Path(inputs['runtime']), Path(inputs['validator'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    report = dict(status='RUNNING', commands=[], products=[], default_authority=[],
                  official_examples_ported=0, original_official_cases_run=0,
                  scope='Real public-path source, producer, Wasm and default-denial acceptance only')

    def save():
        (out / 'layout.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(label, argv, *, expected=0, env=None, data=None, cwd=None, timeout=240):
        directory = out / 'commands' / label
        directory.mkdir()
        row = dict(label=label, argv=list(map(str, argv)), cwd=str(cwd or source),
                   expected=expected, exit=None, environment_overrides=env or {})
        report['commands'].append(row)
        save()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(row['argv'], cwd=cwd or source, env=dict(os.environ, **(env or {})),
                                    input=data, stdout=stdout, stderr=stderr, timeout=timeout, check=False)
        row.update(exit=result.returncode, stdout_sha256=sha(directory / 'stdout'),
                   stderr_sha256=sha(directory / 'stderr'))
        save()
        assert result.returncode == expected, (label, result.returncode, (directory / 'stderr').read_text()[-2600:])
        return (directory / 'stdout').read_bytes()

    def wasm(label, artifact, version, *, argv=(), data=None, expected_exit=13, expected_stdout=b''):
        run(label + '-validate', [validator, 'validate', artifact])
        run(label + '-imports', [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/tools/check_wasm_imports.py',
                                artifact, '--backend', str(version), '--allow-module', 'wasi_snapshot_preview1',
                                '--report', out / (label + '-imports.json')])
        assert run(label + '-default-denied', [runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                                             '--run', artifact, *argv], expected=expected_exit, data=data) == expected_stdout
        report['products'].append(dict(label=label, path=str(artifact), bytes=artifact.stat().st_size, sha256=sha(artifact)))
        save()

    try:
        restored = restore(root, source, out)
        tool = source / 'sdk/origin/tools/sdk_layout.py'
        run('create', [sys.executable, '-I', '-S', '-B', tool, 'create'])
        layout = json.loads(run('check', [sys.executable, '-I', '-S', '-B', tool, 'check']))
        assert layout['languages'] == 15 and layout['platforms'] == 31
        run('idempotent-create', [sys.executable, '-I', '-S', '-B', tool, 'create'])
        run('negative-and-source-models', [sys.executable, '-I', '-S', '-B',
             source / 'test/0073.ros-tests/support/source/ros_platform_sdk/test_sdk_layout.py', '--source', source, '--out', out / 'models'])
        report['layout_source_check'] = layout
        flags = ['-Os', '-Wl,--threads=1', '-Wl,-z,stack-size=1048576',
                 '-Wl,--initial-memory=16777216', '-Wl,--max-memory=67108864']
        for lang, suffix in [('c', '.c'), ('cpp', '.cpp')]:
            base = source / 'sdk/origin/language' / lang
            cpp = ['-std=c++20', '-fno-exceptions', '-fno-rtti'] if lang == 'cpp' else ['-std=c11']
            compiler = wasi / 'bin' / ('clang++' if lang == 'cpp' else 'clang')
            for version in (1, 2):
                label = 'base-' + lang + '-ros' + str(version)
                artifact = out / (label + '.wasm')
                run(label + '-build', [compiler, *cpp, *flags, '-I'+str(base / 'include'),
                    '-DROSWASM_EXAMPLE_ROS_VERSION='+str(version), base / ('example/01_raw_imports'+suffix), '-o', artifact])
                wasm(label, artifact, version)
            base = source / 'sdk/origin/platform/ubtech' / lang
            label, artifact = 'vendor-'+lang, out / ('vendor-'+lang+'.wasm')
            run(label + '-build', [compiler, *cpp, *flags,
                '-I'+str(base / 'include/base'), '-I'+str(base / 'include/vendor'),
                '-I'+str(base / 'include/ros2/ubtech-ros2-main-complete'), '-DUWVM_UBTECH_PROFILE=1',
                base / ('example/vendor/guest.'+('cc' if lang == 'cpp' else 'c')), '-o', artifact])
            wasm(label, artifact, 2, argv=('state', 'ubtech.observation'))
        # Cargo normalizes relative path dependencies before the OS follows
        # directory links. Resolve the selected public project first; never
        # patch the pinned Cargo.toml or add misleading compatibility crates.
        base = Path(run('vendor-rust-resolve', [sys.executable, '-I', '-S', '-B', tool,
            'resolve', '--path', 'platform/ubtech/rust/example/vendor/rust']).decode().strip())
        rust = tools / 'rust-1.90.0'
        run('vendor-rust-build', [rust / 'bin/cargo', 'build', '--offline', '--release', '-j1',
            '--target', 'wasm32-wasip1', '--no-default-features', '--features', 'ros2_main',
            '--manifest-path', base / 'Cargo.toml', '--target-dir', out / 'rust'],
            env=dict(RUSTC=str(rust / 'bin/rustc'), RUSTDOC=str(rust / 'bin/rustdoc'),
                     CARGO_HOME=str(out / 'cargo-home'), CARGO_BUILD_JOBS='1',
                     RUSTFLAGS='-C opt-level=z -C codegen-units=1 -C debuginfo=0 -C link-arg=--threads=1'))
        wasm('vendor-rust', out / 'rust/wasm32-wasip1/release/ubtech-observe.wasm', 2,
             argv=('state', 'ubtech.observation'))
        base = Path(run('vendor-assemblyscript-resolve', [sys.executable, '-I', '-S', '-B', tool,
            'resolve', '--path', 'platform/ubtech/assemblyscript/example/vendor']).decode().strip())
        artifact = out / 'vendor-assemblyscript.wasm'
        run('vendor-assemblyscript-build', ['/usr/bin/node', '--max-old-space-size=256',
            '--disable-wasm-trap-handler', '--v8-pool-size=1', tools / 'node_modules/assemblyscript/bin/asc.js',
            base / 'main_ros2_main.ts', '--runtime', 'incremental', '--use', 'abort=sdk/origin/assemblyscript/api/abortGuest',
            '--optimizeLevel', '1', '--initialMemory', '256', '--maximumMemory', '1024', '--outFile', artifact])
        alias = b'ubtech.observation'
        wasm('vendor-assemblyscript', artifact, 2, data=bytes([0, len(alias)]) + alias)
        base = source / 'sdk/origin/platform/ubtech/tinygo/example/vendor/go'
        artifact = out / 'vendor-tinygo.wasm'
        run('vendor-tinygo-build', [tools / 'tinygo/bin/tinygo', 'build', '-target='+str(base / 'wasmros-wasi.json'),
            '-scheduler=none', '-gc=leaking', '-panic=trap', '-p=1', '-opt=1', '-no-debug',
            '-tags=ros2 ubtech_ros2_main', '-o', artifact, '.'], cwd=base,
            env=dict(PATH=str(tools/'go/bin')+':'+str(tools/'tinygo/bin')+':/usr/bin:/bin',
                GOROOT=str(tools/'go'), TINYGOROOT=str(tools/'tinygo'), GOTOOLCHAIN='local',
                GOPROXY='off', GOSUMDB='off', GOMAXPROCS='1', GOMEMLIMIT='384MiB',
                GOCACHE=str(out/'go-cache'), GOMODCACHE=str(out/'go-modules'), GOFLAGS='-mod=mod'))
        wasm('vendor-tinygo', artifact, 2, argv=('state', 'ubtech.observation'))
        base = source / 'sdk/origin/platform/ubtech/javascript/example/vendor'
        prepared = out / 'quickjs'
        resources = wasi / 'lib/clang/22'
        archives = wasi / 'share/wasi-sysroot/lib/wasm32-wasip1'
        run('vendor-javascript-prepare', [sys.executable, '-I', '-S', '-B',
            source/'sdk/origin/language/javascript/include/guest/prepare.py', '--source-root', source,
            '--quickjs', inputs['quickjs'], '--manifest', base/'javascript-ros2_main.json',
            '--variant', 'selection', '--out', prepared, '--clang', wasi/'bin/clang',
            '--linker', wasi/'bin/wasm-ld', '--sysroot', wasi/'share/wasi-sysroot',
            '--resource-dir', resources, '--libc', archives/'libc.a', '--libm', archives/'libm.a',
            '--printscan', archives/'libc-printscan-long-double.a',
            '--builtins', resources/'lib/wasm32-unknown-wasip1/libclang_rt.builtins.a', '--validator', validator])
        preparation = json.loads((prepared / 'report.json').read_bytes())
        assert preparation['status'] == 'PREPARED_NOT_COMPILED' and preparation['implicit_permissions'] == []
        for job in preparation['jobs']:
            run('vendor-javascript-job-'+job['id'], job['argv'], cwd=job['cwd'])
        # The embedded-JS command completes with exit0 and writes its exact
        # primary/cleanup u32LE pair. Default refusal is primary13, not an engine
        # exit13; no exception, empty stdin or nonzero engine exit is accepted.
        wasm('vendor-javascript', Path(preparation['artifact']), 2,
             data=bytes([0, len(alias)]) + alias, expected_exit=0,
             expected_stdout=(13).to_bytes(4, 'little') + bytes(4))
        for row in restored:
            path = Path(row['path'])
            assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
        report.update(status='PUBLIC_SDK_LAYOUT_AND_TEN_REAL_WASM_PRODUCTS_PASS',
                      source_layout=json.loads(run('final-layout-check', [sys.executable, '-I', '-S', '-B', tool, 'check'])))
        print(report['status'], len(report['commands']), len(report['products']), flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
