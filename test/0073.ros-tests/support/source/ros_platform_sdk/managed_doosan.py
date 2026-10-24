#!/usr/bin/env python3
"""Build actual managed DOOSAN Wasm examples under the independent Linux owner.

Rust, AssemblyScript and TinyGo execute native Wasm; JavaScript executes only
inside QuickJS Wasm. This lane checks real products/imports/default refusals.
It grants no authority and claims no native delivery or original official suite.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


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
    kit = source / 'sdk/origin/robots/arms/doosan'
    out = args.out
    assert out.is_absolute() and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    report = dict(status='RUNNING', commands=[], artifacts=[], default_authority=[],
                  original_official_cases_run=0, native_delivery_qualified=False,
                  scope='Actual four-language selected Wasm builds and exact default refusals; native delivery and full managed record-value testing remain separate')
    def save():
        (out / 'managed.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, argv, expected=0, data=None, env=None, deadline=240, cwd=None):
        directory = out / 'commands' / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), input=data, cwd=cwd or source,
                env=dict(os.environ, **(env or {})), stdout=stdout, stderr=stderr, timeout=deadline, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), cwd=str(cwd or source),
            environment_overrides=env or {},
            exit=result.returncode, expected_exit=expected,
            stdin_bytes=len(data) if data is not None else None,
            stdin_sha256=hashlib.sha256(data).hexdigest() if data is not None else None,
            stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr')))
        save()
        assert (directory / 'stdout').stat().st_size + (directory / 'stderr').stat().st_size <= 8 << 20
        assert result.returncode == expected, (label, result.returncode, (directory / 'stderr').read_text()[-2400:])
        return (directory / 'stdout').read_bytes()
    runtime, validator = Path(inputs['runtime']), Path(inputs['validator'])
    def wasm(label, artifact, version):
        run(label + '-validate', [validator, 'validate', artifact])
        run(label + '-imports', [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/tools/check_wasm_imports.py',
            artifact, '--backend', str(version), '--allow-module', 'wasi_snapshot_preview1', '--report', out / (label + '-imports.json')])
        report['artifacts'].append(dict(pin(artifact), label=label))
        save()
    try:
        alias = b'doosan.observation'
        for profile in tuple(r['id'] for r in json.loads((kit/'profiles.json').read_bytes())['profiles']):
            version = 1 if profile.startswith('ros1_') else 2
            rust = tools / 'rust-1.90.0'
            target = out / ('rust-' + profile)
            run(profile + '-rust-build', [rust / 'bin/cargo', 'build', '--offline', '--release', '-j1', '--target', 'wasm32-wasip1',
                '--no-default-features', '--features', profile, '--manifest-path', kit / 'examples/rust/Cargo.toml', '--target-dir', target],
                env=dict(RUSTC=str(rust / 'bin/rustc'), RUSTDOC=str(rust / 'bin/rustdoc'),
                    CARGO_HOME=str(out / 'cargo-home'), CARGO_BUILD_JOBS='1',
                    RUSTFLAGS='-C opt-level=z -C codegen-units=1 -C debuginfo=0 -C link-arg=--threads=1 -C link-arg=--initial-memory=16777216 -C link-arg=--max-memory=67108864 -C link-arg=-z -C link-arg=stack-size=1048576'))
            artifact = target / 'wasm32-wasip1/release/doosan-observe.wasm'
            wasm(profile + '-rust', artifact, version)
            for mode in ('state', 'command'):
                assert run(profile + '-rust-denied-' + mode, [runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                    '--run', artifact, mode, alias.decode()], expected=13) == b''
            artifact = out / (profile + '-assemblyscript.wasm')
            run(profile + '-assemblyscript-build', ['/usr/bin/node', '--max-old-space-size=256',
                '--disable-wasm-trap-handler', '--v8-pool-size=1',
                tools / 'node_modules/assemblyscript/bin/asc.js', kit / ('examples/main_' + profile + '.ts'),
                '--runtime', 'incremental', '--use', 'abort=sdk/origin/assemblyscript/api/abortGuest', '--optimizeLevel', '1',
                '--initialMemory', '256', '--maximumMemory', '1024', '--outFile', artifact])
            wasm(profile + '-assemblyscript', artifact, version)
            for mode in (0, 1):
                assert run(profile + '-assemblyscript-denied-' + str(mode), [runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                    '--run', artifact], expected=13, data=bytes([mode, len(alias)]) + alias) == b''
            artifact = out / (profile + '-tinygo.wasm')
            run(profile + '-tinygo-build', [tools / 'tinygo/bin/tinygo', 'build', '-target=' + str(kit / 'examples/go/wasmros-wasi.json'), '-scheduler=none',
                '-gc=leaking', '-panic=trap', '-p=1', '-opt=1', '-no-debug',
                '-tags=ros' + str(version) + ' doosan_' + profile, '-o', artifact, '.'], cwd=kit / 'examples/go',
                env=dict(PATH=str(tools / 'go/bin') + ':' + str(tools / 'tinygo/bin') + ':/usr/bin:/bin',
                    GOROOT=str(tools / 'go'), TINYGOROOT=str(tools / 'tinygo'),
                    GOTOOLCHAIN='local', GOPROXY='off', GOSUMDB='off', GOMAXPROCS='1',
                    GOMEMLIMIT='384MiB', GOCACHE=str(out / 'go-cache'),
                    GOMODCACHE=str(out / 'go-modules'), GOFLAGS='-mod=mod'))
            wasm(profile + '-tinygo', artifact, version)
            for mode in ('state', 'command'):
                argv = [mode, alias.decode()]
                assert run(profile + '-tinygo-denied-' + mode, [runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                    '--run', artifact, *argv], expected=13) == b''
            prepared = out / ('quickjs-' + profile)
            resources = wasi / 'lib/clang/22'
            archives = wasi / 'share/wasi-sysroot/lib/wasm32-wasip1'
            run(profile + '-javascript-prepare', [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/javascript/guest/prepare.py',
                '--source-root', source, '--quickjs', inputs['quickjs'], '--manifest', kit / ('examples/javascript-' + profile + '.json'),
                '--variant', 'selection', '--out', prepared, '--clang', wasi / 'bin/clang', '--linker', wasi / 'bin/wasm-ld',
                '--sysroot', wasi / 'share/wasi-sysroot', '--resource-dir', resources,
                '--libc', archives / 'libc.a', '--libm', archives / 'libm.a', '--printscan', archives / 'libc-printscan-long-double.a',
                '--builtins', resources / 'lib/wasm32-unknown-wasip1/libclang_rt.builtins.a', '--validator', validator])
            preparation = json.loads((prepared / 'report.json').read_bytes())
            assert preparation['status'] == 'PREPARED_NOT_COMPILED' and preparation['implicit_permissions'] == []
            for job in preparation['jobs']:
                run(profile + '-javascript-job-' + job['id'], job['argv'], cwd=job['cwd'])
            artifact = Path(preparation['artifact'])
            wasm(profile + '-javascript', artifact, version)
            for mode in (0, 1):
                output = run(profile + '-javascript-denied-' + str(mode), [runtime, '-Rint',
                    '--wasip1-global-noinherit-system-environment', '--run', artifact], data=bytes([mode, len(alias)]) + alias)
                assert output == b'\r\0\0\0\0\0\0\0', output
        report['status'] = 'DOOSAN_FOUR_MANAGED_LANGUAGE_PRODUCTS_AND_EXACT_REFUSALS_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
