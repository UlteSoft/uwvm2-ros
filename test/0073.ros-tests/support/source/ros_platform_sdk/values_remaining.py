#!/usr/bin/env python3
"""Build and execute all managed record values against native-checked vectors.

The independent outer owner enforces the original 64GiB/swap0 group and finite
process-tree budgets. Each selected profile builds real Rust, AssemblyScript,
TinyGo and QuickJS/Wasm products serially. Field values, complete re-encoding,
truncation and trailing-data refusal are exercised without a ROS capability.
These authored representation cases are not vendor official-suite/controller
or performance acceptance.
"""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def pin(path):
    path = Path(path)
    with path.open('rb') as stream:
        h = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path), bytes=path.stat().st_size, sha256=h)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    root, out = a.root.resolve(strict=True), a.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source, tools = Path(inputs['source_root']), Path(inputs['toolchains'])
    vendor = inputs['selected_vendor']
    kit = source / 'sdk/origin/robots/platforms' / vendor
    direct = Path(inputs['selected_direct_result'])
    qualification = json.loads((direct / 'qualification.json').read_bytes())
    assert qualification['status'] == 'SELECTED_VENDOR_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS' and qualification['vendor'] == vendor
    runtime, validator = Path(inputs['runtime']), Path(inputs['validator'])
    wasi = tools / 'wasi-sdk-33.0-x86_64-linux'
    rust = tools / 'rust-1.90.0'
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    report = dict(status='RUNNING', vendor=vendor, commands=[], profiles=[], artifacts=[],
                  default_authority=[], original_official_cases_run=0, native_sdk_used=False)
    def save():
        (out / 'values.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, argv, *, cwd=None, env=None, deadline=360):
        directory = out / 'commands' / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=cwd or source, env=dict(os.environ, **(env or {})),
                                    stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, timeout=deadline, check=False)
        row = dict(label=label, argv=list(map(str, argv)), cwd=str(cwd or source), environment_overrides=env or {},
                   exit=result.returncode, stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr'))
        report['commands'].append(row)
        save()
        assert row['stdout']['bytes'] + row['stderr']['bytes'] <= 8 << 20
        assert result.returncode == 0, (label, (directory / 'stderr').read_text()[-2600:])
        return (directory / 'stdout').read_bytes()
    def execute(label, artifact):
        run(label + '-validate', [validator, 'validate', artifact])
        assert run(label + '-actual-wasm-values', [runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', artifact]) == b''
        report['artifacts'].append(dict(label=label, **pin(artifact)))
        save()
    try:
        for profile in json.loads((kit / 'profiles.json').read_bytes())['profiles']:
            key, version = profile['id'], profile['ros_version']
            family = source / 'sdk/origin/custom-types' / profile['family']
            generated = out / ('vectors-' + key)
            run(key + '-independent-managed-value-sources', [sys.executable, '-I', '-S', '-B',
                source / 'test/0073.ros-tests/support/source/ros_platform_sdk/managed_vectors_remaining.py', '--source', source, '--vendor', vendor,
                '--profile', key, '--vectors', direct / ('golden-vectors-' + key) / 'vectors.json', '--out', generated])
            proof = json.loads((generated / 'managed-vectors.json').read_bytes())
            crate = out / ('rust-' + key)
            crate.mkdir()
            shutil.copyfile(generated / 'main.rs', crate / 'main.rs')
            (crate / 'Cargo.toml').write_text('[package]\nname="managed-values"\nversion="0.1.0"\nedition="2021"\npublish=false\n'
                '[[bin]]\nname="managed-values"\npath="main.rs"\n[dependencies]\nroswasm={path=' +
                json.dumps(str(source / 'sdk/origin/rust')) + ',default-features=false,features=["alloc","ros' + str(version) + '"]}\n')
            target = out / 'rust-target'
            run(key + '-rust-values-build', [rust / 'bin/cargo', 'build', '--offline', '--release', '-j1', '--target', 'wasm32-wasip1',
                '--manifest-path', crate / 'Cargo.toml', '--target-dir', target], env=dict(RUSTC=str(rust / 'bin/rustc'),
                RUSTDOC=str(rust / 'bin/rustdoc'), CARGO_HOME=str(out / 'cargo-home'), CARGO_BUILD_JOBS='1',
                RUSTFLAGS='-C opt-level=z -C codegen-units=1 -C debuginfo=0 -C link-arg=--threads=1 -C link-arg=--initial-memory=16777216 -C link-arg=--max-memory=67108864 -C link-arg=-z -C link-arg=stack-size=1048576'))
            artifact = out / (key + '-rust-values.wasm')
            shutil.copyfile(target / 'wasm32-wasip1/release/managed-values.wasm', artifact)
            execute(key + '-rust', artifact)
            artifact = out / (key + '-assemblyscript-values.wasm')
            ts_source = kit / 'tests/managed_values' / (key + '.ts')
            ts_source.parent.mkdir(exist_ok=True)
            body = (generated / 'main.ts').read_bytes()
            if ts_source.exists():
                assert ts_source.read_bytes() == body
            else:
                ts_source.write_bytes(body)
            run(key + '-assemblyscript-values-build', ['/usr/bin/node', '--max-old-space-size=512', '--disable-wasm-trap-handler',
                '--v8-pool-size=1', tools / 'node_modules/assemblyscript/bin/asc.js', ts_source,
                '--runtime', 'incremental', '--use', 'abort=sdk/origin/assemblyscript/api/abortGuest', '--optimizeLevel', '1',
                '--initialMemory', '256', '--maximumMemory', '1024', '--outFile', artifact])
            execute(key + '-assemblyscript', artifact)
            go = out / ('go-' + key)
            (go / 'records').mkdir(parents=True)
            shutil.copyfile(generated / 'main.go', go / 'main.go')
            shutil.copyfile(family / 'bounded_records.go', go / 'records/records.go')
            (go / 'go.mod').write_text('module uwvm2.local/managed-values\n\ngo 1.23\n\nrequire uwvm2.local/roswasm v0.0.0\n'
                'replace uwvm2.local/roswasm => ' + str(source / 'sdk/origin/tinygo') + '\n')
            artifact = out / (key + '-tinygo-values.wasm')
            run(key + '-tinygo-values-build', [tools / 'tinygo/bin/tinygo', 'build', '-target=' + str(kit / 'examples/go/wasmros-wasi.json'),
                '-scheduler=none', '-gc=leaking', '-panic=trap', '-p=1', '-opt=1', '-no-debug', '-tags=ros' + str(version), '-o', artifact, '.'],
                cwd=go, env=dict(PATH=str(tools / 'go/bin') + ':' + str(tools / 'tinygo/bin') + ':/usr/bin:/bin',
                GOROOT=str(tools / 'go'), TINYGOROOT=str(tools / 'tinygo'), GOTOOLCHAIN='local', GOPROXY='off', GOSUMDB='off',
                GOMAXPROCS='1', GOMEMLIMIT='384MiB', GOCACHE=str(out / 'go-cache'), GOMODCACHE=str(out / 'go-modules'), GOFLAGS='-mod=mod'))
            execute(key + '-tinygo', artifact)
            js_source = kit / 'tests/managed_values' / (key + '.mjs')
            js_source.parent.mkdir(exist_ok=True)
            body = (generated / 'main.mjs').read_bytes()
            if js_source.exists() and js_source.read_bytes() != body:
                history = out / ('retained-prior-javascript-source-' + key + '.mjs')
                js_source.rename(history)
                report.setdefault('retained_prior_authored_test_sources', []).append(pin(history))
            if not js_source.exists():
                js_source.write_bytes(body)
            recipe = json.loads((kit / ('examples/javascript-' + key + '.json')).read_bytes())
            common = recipe['common']
            entry = str(js_source.relative_to(source))
            record_module = str((family / 'bounded_records.mjs').relative_to(source))
            common['entry'] = entry
            common['modules'] = [row for row in common['modules'] if row['source'].startswith('sdk/origin/javascript/') or row['source'] == record_module]
            common['modules'].append(dict(name=entry, source=entry))
            common['limits']['input_bytes'], common['limits']['output_bytes'] = 1, 32
            manifest = out / (key + '-javascript-values.json')
            manifest.write_text(json.dumps(recipe, indent=2) + '\n')
            prepared = out / ('quickjs-' + key)
            resources = wasi / 'lib/clang/22'
            archives = wasi / 'share/wasi-sysroot/lib/wasm32-wasip1'
            run(key + '-javascript-values-prepare', [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/javascript/guest/prepare.py',
                '--source-root', source, '--quickjs', inputs['quickjs'], '--manifest', manifest, '--variant', 'selection', '--out', prepared,
                '--clang', wasi / 'bin/clang', '--linker', wasi / 'bin/wasm-ld', '--sysroot', wasi / 'share/wasi-sysroot',
                '--resource-dir', resources, '--libc', archives / 'libc.a', '--libm', archives / 'libm.a',
                '--printscan', archives / 'libc-printscan-long-double.a', '--builtins', resources / 'lib/wasm32-unknown-wasip1/libclang_rt.builtins.a',
                '--validator', validator])
            preparation = json.loads((prepared / 'report.json').read_bytes())
            assert preparation['status'] == 'PREPARED_NOT_COMPILED' and preparation['implicit_permissions'] == []
            for job in preparation['jobs']:
                run(key + '-javascript-values-job-' + job['id'], job['argv'], cwd=job['cwd'])
            execute(key + '-javascript', Path(preparation['artifact']))
            report['profiles'].append(dict(profile=key, records=proof['records'],
                original_scalar_or_string_instances=proof['scalar_or_string_instances'], managed_languages=4))
            save()
        report['status'] = 'ALL_SELECTED_FOUR_MANAGED_LANGUAGE_COMPLETE_VALUES_AND_MALFORMED_REFUSALS_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
