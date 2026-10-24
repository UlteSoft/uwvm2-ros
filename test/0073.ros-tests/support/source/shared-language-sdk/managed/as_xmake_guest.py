#!/usr/bin/env python3
"""Qualify two selected real AS commands through isolated installed xmake rules.

The private AS-only provider pins real SDK/AssemblyScript source, its actual
standard library, the build-driver executable and selected offline input
closure. It supplies no C libc/sysroot and makes no C/C++ provider claim. The
current integrated shared sources are copied without applying superseded
patches. Public shared files/manifests remain untouched.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def pin(path, root=None):
    real = path.resolve(strict=True)
    with real.open('rb') as stream:
        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(real.relative_to(root)) if root else str(real), bytes=real.stat().st_size, sha256=digest)


def save(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + '\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'node-modules', 'node', 'validator', 'runtime', 'xmake', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--one-backend', choices=('1', '2'),
                        help='Qualify one complete selected build/run when a smaller real scope is needed')
    parser.add_argument('--api-source', type=Path,
                        help='Explicit owned API postimages from the real native parser/formatter receipt')
    parser.add_argument('--application-source', type=Path,
                        help='Explicit unchanged examples root when using a separate SDK source capsule')
    parser.add_argument('--stdlib-from-compiler', action='store_true',
                        help='Retain actual asc standard-library resolution in the pinned complete compiler closure')
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('shared admitted Linux finite owner required')
    source = args.source.resolve(strict=True)
    origin = source / 'sdk/origin'
    args.out.mkdir(parents=True, exist_ok=False)
    basis = source
    if args.api_source is not None or args.application_source is not None:
        # Preserve Distribution's original-source confinement and path rebinding
        # contract. A fresh maintenance snapshot owns every explicit overlay;
        # installed files never reference these maintenance inputs afterwards.
        composite = args.out / 'source-snapshot'
        composite.mkdir()
        for relative in ('sdk/tools', 'sdk/origin/tools/xmake', 'sdk/origin/tools/shared_managed04'):
            shutil.copytree(source / relative, composite / relative,
                            ignore=shutil.ignore_patterns('._*', '__pycache__', '*.pyc'))
        for relative in ('sdk/xmake.lua', 'sdk/example/xmake.lua', 'sdk/origin/tools/wasm_profiles.py',
                         'sdk/origin/tools/check_wasm_imports.py', 'sdk/origin/abi/abi-v1.json'):
            target = composite / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source / relative, target)
        if args.api_source is not None:
            shutil.copytree(args.api_source.resolve(strict=True), composite / 'sdk/origin/assemblyscript', dirs_exist_ok=True)
        if args.application_source is not None:
            shutil.copytree(args.application_source.resolve(strict=True), composite / 'sdk/origin/examples', dirs_exist_ok=True)
        source, origin = composite, composite / 'sdk/origin'
    commands = args.out / 'commands'
    commands.mkdir()
    report = dict(format='uwvm-as-isolated-xmake-guest-v1', status='STARTED', commands=[],
        compiled=0, validated=0, binary_imports=0, actual_guest_denials=0,
        denominator=1 if args.one_backend else 2, native_ros=0, official_upstream=0, default_authority=[],
        public_generator_or_manifest_modified=False, source_basis=str(basis),
        explicit_api_overlay=str(args.api_source) if args.api_source else None,
        explicit_application_overlay=str(args.application_source) if args.application_source else None)

    def record():
        save(args.out / 'acceptance.json', report)

    def run(name, argv, cwd):
        folder = commands / name
        folder.mkdir()
        env = dict(os.environ, XMAKE_STATS='false', XMAKE_SKIP_HISTORY='y', PYTHONDONTWRITEBYTECODE='1')
        with (folder / 'stdout').open('xb') as stdout, (folder / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=cwd, env=env, stdin=subprocess.DEVNULL,
                stdout=stdout, stderr=stderr, check=False)
        report['commands'].append(dict(name=name, argv=list(map(str, argv)), cwd=str(cwd),
            exit=result.returncode, stdout=pin(folder / 'stdout'), stderr=pin(folder / 'stderr')))
        record()
        if result.returncode:
            raise RuntimeError('failed ' + name + '; retained original command output')
        return (folder / 'stdout').read_text()

    try:
        report['tools'] = [pin(path) for path in (args.node, args.validator, args.runtime, args.xmake)]
        for name, tool in (('node', args.node), ('validator', args.validator), ('xmake', args.xmake)):
            report.setdefault('tool_versions', {})[name] = run('version-' + name, [tool, '--version'], args.out).strip()
        sys.path.insert(0, str(source / 'sdk/tools'))
        spec = importlib.util.spec_from_file_location('_as_actual_distribution', source / 'sdk/tools/prepare_distribution.py')
        distribution = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(distribution)
        assembled = args.out / 'sdk-original'
        assembled.mkdir()
        delivery = distribution.Distribution(origin, assembled)
        api, applications = origin / 'assemblyscript', origin / 'examples'
        for path in sorted(api.rglob('*.ts')):
            if any(p in ('test', 'tests', 'contracts', 'examples') for p in path.relative_to(api).parts):
                continue
            delivery.add(path, 'include/platform/ros/as/' + path.relative_to(api).as_posix())
        for path in sorted((applications / 'assemblyscript').rglob('*.ts')):
            delivery.add(path, 'example/platform/ros/as/assemblyscript/' + path.relative_to(applications / 'assemblyscript').as_posix())
        for path in sorted((applications / 'portable_telemetry/assemblyscript').glob('*.ts')):
            delivery.add(path, 'example/platform/ros/as/portable_telemetry/assemblyscript/' + path.name)
        for path in sorted((origin / 'tools/xmake').rglob('*')):
            if path.suffix in ('.lua', '.py'):
                delivery.add(path, 'tools/xmake/' + path.relative_to(origin / 'tools/xmake').as_posix())
        for name in ('wasm_profiles.py', 'check_wasm_imports.py'):
            delivery.add(origin / 'tools' / name, 'tools/' + name)
        delivery.add(origin / 'abi/abi-v1.json', 'share/abi/abi-v1.json')
        delivery.add(origin / 'tools/shared_managed04/profile.py', 'tools/shared_managed04/profile.py')
        delivery.add(origin / 'tools/xmake/examples/managed_rules.lua', 'example/platform/ros/as/xmake.lua')
        delivery.copy_sources()
        shutil.copy2(source / 'sdk/xmake.lua', assembled / 'xmake.lua')
        (assembled / 'example').mkdir(exist_ok=True)
        shutil.copy2(source / 'sdk/example/xmake.lua', assembled / 'example/xmake.lua')
        report['integrated_shared_sources'] = [pin(origin / name) for name in (
            'tools/xmake/common/execution.lua', 'tools/wasm_profiles.py', 'tools/shared_managed04/profile.py')]
        report['private_source_patches_applied'] = []
        # The actual AS stdlib is a source sysroot for this AS provider. It is
        # byte-identical to the selected compiler's stdlib and provides no libc.
        if args.stdlib_from_compiler:
            # asc actually resolves its builtin stdlib from its compiler package.
            # The complete original package remains an individually hashed input;
            # no alternate implementation or host application replaces it.
            report['standard_library'] = dict(resolution='actual asc compiler package default',
                root=str(args.node_modules / 'assemblyscript/std'), complete_compiler_closure_pinned=True,
                installed_api_sysroot='include/platform/ros/as')
        else:
            shutil.copytree(args.node_modules / 'assemblyscript/std', assembled / 'lib/as-standard-library')
        sdk = args.out / 'SDK moved with Unicode-λ'
        assembled.rename(sdk)
        if any((sdk / name).exists() for name in ('origin', 'test', 'tests', 'sdk')):
            raise ValueError('the relocated installed SDK must contain no checkout or repository tests')
        report['isolated_sdk'] = str(sdk)
        report['assembly'] = list(delivery.files.values())
        compiler = Path(sys.executable).resolve(strict=True)
        toolchain = compiler.parent.parent
        runtime = dict(name='AssemblyScript', route='assemblyscript', exact_version_required=True,
            mode='linear-memory', version='0.28.20', language='as', address_bits=32,
            target='wasm32-wasip1', startup='command', exceptions=False, default_authority=[],
            source_ready=True, compiled=False, runtime_tested=False)
        input_files = {str(p.resolve(strict=True)): pin(p) for p in args.node_modules.rglob('*') if p.is_file()}
        for p in (args.node, args.validator):
            input_files[str(p.resolve(strict=True))] = pin(p)
        inputs = sdk / 'share/as-inputs.json'
        save(inputs, dict(format='uwvm-managed-example-inputs-v1', language='as', runtime=runtime,
            paths=dict(node=str(args.node), asc=str(args.node_modules / 'assemblyscript/bin/asc.js'), validator=str(args.validator)),
            directories=[str(args.node_modules)], files=list(input_files.values())))
        contract = dict(target='wasm32-wasip1', address_bits=32, wasi='wasip1', exceptions=False,
            threads=False, startup='command', core_baseline='3.0')
        artifact_pins = [pin(p, sdk) for p in sorted(sdk.rglob('*')) if p.is_file()]
        profile = dict(id='task04-actual-as02820-source-closure', profile=contract, toolchain_id='actual-as02820-offline',
            sysroot='include/platform/ros/as' if args.stdlib_from_compiler else 'lib/as-standard-library',
            resource_dir=None, files=artifact_pins, runtime_requirements=[],
            library_abi=dict(address_bits=32, wasi_pointer_abi='wasip1-memory32', exceptions=False, threads=False,
                exception_encoding='none', thread_abi='none', shared_memory=False,
                required_files=['include/platform/ros/as/api.ts']),
            languages={'as': dict(toolchain_id='actual-as02820-offline', version='0.28.20',
                compiler=pin(compiler, toolchain), compiler_argv=[], linker_argv=[],
                library_modes=['runtime:assemblyscript'], source_ready=True, compiled=False, runtime_tested=False,
                runtime_identity={k: runtime[k] for k in ('route', 'name', 'version', 'mode', 'language', 'target', 'address_bits', 'startup', 'exceptions')},
                driver='uwvm-managed-sdk-consumer-v1', supports_clang_config=False,
                host_runtime_dependency_names=['admitted Linux CPython/Node dynamic loader and standard-library TCB; build tools only'])})
        save(sdk / 'share/wasm-profiles.json', dict(format='uwvm-sdk-wasm-profiles-v1', defaults=contract,
            profiles=[profile], default_authority=[]))
        report['provider_manifest'] = pin(sdk / 'share/wasm-profiles.json')
        for backend in ((int(args.one_backend),) if args.one_backend else (1, 2)):
            tag = 'ros' + str(backend)
            run('configure-' + tag, [args.xmake, 'f', '-c', '--yes', '--ros-sdk-platforms=ros', '--ros-sdk-languages=as',
                '--sdk-target=wasm32-wasip1', '--sdk-startup=command', '--sdk-exceptions=n', '--sdk-threads=n',
                '--sdk-offline=y', '--sdk-python=' + str(compiler), '--sdk-toolchain=' + str(toolchain),
                '--sdk-example-platform=ros', '--sdk-example-language=as', '--sdk-example=telemetry-' + tag,
                '--ros-sdk-as-runtime=assemblyscript', '--ros-sdk-as-runtime-version=0.28.20',
                '--ros-sdk-managed-backend=' + str(backend), '--ros-sdk-managed-inputs=' + str(inputs),
                '--ros-sdk-managed-inputs-sha256=' + pin(inputs)['sha256']], sdk / 'example')
            product = args.out / tag
            run('xmake-build-' + tag, [args.xmake, 'sdk-example-build', '--output=' + str(product)], sdk / 'example')
            receipt_path = product / 'build-receipt.json'
            receipt = json.loads(receipt_path.read_bytes())
            if receipt['status'] != 'BUILT_NOT_EXECUTED' or len(receipt['artifacts']) != 1:
                raise ValueError('one actual core-Wasm command receipt required')
            artifact = Path(receipt['artifacts'][0]['path'])
            if artifact.read_bytes()[:8] != b'\0asm\x01\0\0\0':
                raise ValueError('actual final Core Wasm required')
            report['compiled'] += 1
            run('validate-' + tag, [args.validator, 'validate', artifact], sdk)
            report['validated'] += 1
            run('imports-' + tag, [compiler, '-I', '-S', '-B', sdk / 'tools/check_wasm_imports.py', artifact,
                '--backend', str(backend), '--allow-module', 'wasi_snapshot_preview1', '--report', args.out / (tag + '-imports.json')], sdk)
            report['binary_imports'] += 1
            output = run('xmake-guest-' + tag, [args.xmake, 'sdk-example-run', '--receipt=' + str(receipt_path),
                '--runtime=' + str(args.runtime)], sdk / 'example')
            if output.count('PASS_ROS_DEFAULT_DENIED') != 1:
                raise ValueError('actual Guest must emit exactly one self-checked default-denial marker')
            report['actual_guest_denials'] += 1
            record()
        for row in input_files.values():
            if pin(Path(row['path'])) != row:
                raise ValueError('actual compiler/runtime input changed')
        if any(pin(Path(row['path'])) != row for row in report['tools']):
            raise ValueError('actual build/validator/Runtime tools changed')
        report['status'] = 'ACTUAL_AS_XMAKE_GUEST_DEFAULT_DENIAL_' + str(report['actual_guest_denials']) + '_OF_' + str(report['denominator'])
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        record()


if __name__ == '__main__':
    main()
