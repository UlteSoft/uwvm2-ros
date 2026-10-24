#!/usr/bin/env python3
"""Build the current shared ROS Runtime and exercise every registered import.

This is an actual Wasm basic-I/O/default-denial census, not positive native ROS,
Gateway transport, complete policy mutation coverage, vendor or upstream credit.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import time


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-archive', type=Path, required=True)
    parser.add_argument('--archive-sha256', required=True)
    parser.add_argument('--wasm-tools', type=Path, required=True)
    parser.add_argument('--compiler', type=Path, default=Path('/usr/bin/clang++-22'))
    args = parser.parse_args()
    root = Path.cwd()
    report = dict(status='RUNNING', source_archive_sha256=args.archive_sha256, commands=[], cases=[],
        default_authority=[], native_ros=0, gateway=0, upstream=0, qemu=0, vtune_hardware=0,
        selected_vendor_registries=[], full_permission_matrix_complete=False)
    result_path = root / 'runtime-denial.json'

    def save():
        result_path.write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')

    def run(label, argv, timeout=300):
        stdout, stderr = root / (label + '.stdout'), root / (label + '.stderr')
        started = time.monotonic()
        with stdout.open('xb') as out, stderr.open('xb') as err:
            value = subprocess.run(list(map(str, argv)), stdin=subprocess.DEVNULL, stdout=out, stderr=err, timeout=timeout)
        row = dict(label=label, argv=list(map(str, argv)), exit=value.returncode,
            elapsed_seconds=time.monotonic() - started, stdout_sha256=sha(stdout), stderr_sha256=sha(stderr))
        report['commands'].append(row)
        save()
        if value.returncode:
            raise RuntimeError((row, stderr.read_text(errors='replace')[-5000:]))
        return stdout.read_bytes()

    try:
        if sha(args.source_archive) != args.archive_sha256:
            raise ValueError('exact transferred ordinary-source archive required')
        source = root / 'source'
        source.mkdir()
        with tarfile.open(args.source_archive, 'r:') as archive:
            archive.extractall(source, filter='data')
        pins = []
        for path in sorted(source.rglob('*')):
            if path.is_file():
                if path.is_symlink() or not path.resolve().is_relative_to(source):
                    raise ValueError('ordinary confined current source snapshot required')
                pins.append(dict(path=path.relative_to(source).as_posix(), bytes=path.stat().st_size, sha256=sha(path)))
        (root / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
        report['source_files'] = len(pins)
        run('compiler-version', [args.compiler, '--version'], 20)
        flags = ['-std=c++26', '-stdlib=libc++', '-Os', '-g0', '-pthread', '-Wno-undefined-inline',
            '-fconstexpr-steps=8388608', '-DUWVM=2', '-DUWVM_USE_UWVM_INT', '-DUWVM_DISABLE_JIT',
            '-DUWVM_USE_THREAD_LOCAL', '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64', '-DUWVM_ENABLE_WASIP1_SOCKET',
            '-DUWVM_VERSION_X=2', '-DUWVM_VERSION_Y=0', '-DUWVM_VERSION_Z=3', '-DUWVM_VERSION_S=0',
            '-DUWVM_IMPORT_ROS1', '-DUWVM_IMPORT_ROS2']
        for directory in ('src', 'third-parties/fast_io/include', 'third-parties/bizwen/include', 'third-parties/boost_unordered/include'):
            flags += ['-I', str(source / directory)]
        objects = []
        for name, relative in (('runtime','src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'),
                ('main','src/uwvm2/uwvm/main.default.cpp'), ('host-api','src/uwvm2/uwvm/host_api.default.cpp')):
            obj = root / (name + '.o')
            run('compile-' + name, [args.compiler, *flags, '-c', source / relative, '-o', obj], 600)
            objects.append(obj)
        runtime = root / 'uwvm-dual'
        run('link-runtime', [args.compiler, '-stdlib=libc++', '-pthread', *objects, '-ldl', '-o', runtime], 120)
        run('runtime-version', [runtime, '--version'], 20)
        report['runtime'] = dict(path=str(runtime), bytes=runtime.stat().st_size, sha256=sha(runtime),
            optimization='-Os -g0', stripped=False, scope='dual builtins, no vendor registry; not xmake option-size acceptance')
        host = (source / 'src/uwvm2/uwvm/imported/ros/host.h').read_text()
        name_map = dict(re.findall(r'inline constexpr char8_t\s+([A-Za-z0-9_]+)\[\]\s*=\s*u8"([^"]+)";', host))
        registered = {}
        for match in re.finditer(r'struct\s+([A-Za-z0-9_]+_local_imported_module_t)\s*\{([\s\S]*?)\n    \};', host):
            body = match[2]
            module = re.search(r'module_name\{u8"([^"]+)"\}', body)
            if not module:
                continue
            wrappers = re.findall(r'wrapper::native_function<[\s\S]*?,\s*wrapper::([A-Za-z0-9_]+)>', body)
            registered[module[1]] = [name_map[name] for name in wrappers]
        metadata = {}
        for name in ('abi-v1.json','service-client-v1.json','rmw-publisher-v1.json','rmw-subscription-v1.json','rmw-service-v1.json'):
            value = json.loads((source / 'sdk/origin/abi' / name).read_bytes())
            for module, row in value['modules'].items():
                if module in metadata:
                    raise ValueError('duplicate ABI metadata module')
                metadata[module] = row
        if set(metadata) != set(registered):
            raise ValueError(('actual registered/metadata module mismatch', set(metadata), set(registered)))
        for module, row in metadata.items():
            if row['function_count'] != len(row['functions']) or [f['name'] for f in row['functions']] != registered[module]:
                raise ValueError(('actual registered/metadata function order mismatch', module))
        report['registered_modules'] = {name: len(functions) for name, functions in registered.items()}
        report['denominator'] = sum(len(row['functions']) for row in metadata.values())
        for module, description in metadata.items():
            for function in description['functions']:
                name = function['name']
                params = [value['type'] if isinstance(value, dict) else value for value in function['params']]
                if function['results'] != ['i32'] or any(value not in ('i32','i64') for value in params):
                    raise ValueError('unreviewed actual import signature')
                arguments = [0] * len(params)
                if name == 'capability_open':
                    arguments = [0, 16, 8, 48]
                label = module + '--' + name
                wat, wasm = root / (label + '.wat'), root / (label + '.wasm')
                signature = '(param ' + ' '.join(params) + ')' if params else ''
                call = ' '.join('(' + kind + '.const ' + str(value) + ')' for kind, value in zip(params, arguments))
                wat.write_text('(module\n(import "' + module + '" "' + name + '" (func $f ' + signature + ' (result i32)))\n'
                    '(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32)(result i32)))\n'
                    '(memory (export "memory") 1)(data (i32.const 16) "no-grant")\n'
                    '(func (export "_start") (local $error i32)\n'
                    '(local.set $error (call $f ' + call + '))\n'
                    '(i32.store (i32.const 128)(local.get $error))\n'
                    '(i32.store (i32.const 64)(i32.const 128))(i32.store (i32.const 68)(i32.const 4))\n'
                    '(if (call $write (i32.const 1)(i32.const 64)(i32.const 1)(i32.const 72))(then unreachable))\n'
                    '(if (i32.ne (i32.load (i32.const 72))(i32.const 4))(then unreachable))))\n')
                run(label + '-parse', [args.wasm_tools, 'parse', wat, '-o', wasm], 20)
                run(label + '-validate', [args.wasm_tools, 'validate', wasm], 20)
                output = run(label + '-guest', [runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', wasm], 20)
                if len(output) != 4:
                    raise RuntimeError((label, 'exact four-byte original return value required', output))
                error = int.from_bytes(output, 'little')
                expected = {1} if name == 'abi_version' else {9,13,22,95,125}
                if error not in expected:
                    raise RuntimeError((label, 'default authority refusal differs', error))
                report['cases'].append(dict(module=module, function=name, status='PASS',
                    result=error, classification='ABI_VERSION_ONLY' if name == 'abi_version' else 'NO_GRANT_REFUSAL',
                    wasm_sha256=sha(wasm), native_ros_credit=0))
                save()
        report.update(status='PASS', passed=len(report['cases']),
            actual_wasm_cases=len(report['cases']), gateway_cases=0, original_official_suite_cases=0)
        if report['passed'] != report['denominator']:
            raise RuntimeError('incomplete actual import denial census')
        save()
        print(json.dumps(dict(status='PASS', passed=report['passed'], denominator=report['denominator'],
            registered_modules=report['registered_modules'], runtime=report['runtime'])), flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        save()
        raise


if __name__ == '__main__':
    main()
