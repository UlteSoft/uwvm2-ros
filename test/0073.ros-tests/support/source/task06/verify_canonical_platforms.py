#!/usr/bin/env python3
"""Install/relocate actual canonical Task06 SDK sources and run real toolchains.

The supplied historical provider descriptors contribute immutable tool/target
library identities only. Every SDK source comes from the current canonical
input, with fresh hashes. No source overlay, origin fallback, driver, hardware,
native ROS peer or substituted compiler is used.
"""
import argparse
import ast
import copy
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


VENDORS = ('elephant', 'ufactory', 'dobot', 'flexiv', 'realman')
RUNTIME = '719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19'


def pin(path, root=None):
    raw = path.read_bytes()
    return dict(path=path.relative_to(root).as_posix() if root else str(path),
                bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def save(path, value):
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def main():
    p = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('source', 'c-provider', 'rust-provider', 'target-libraries', 'wasi-sdk', 'rust-toolchain',
                 'runtime', 'xmake', 'clang', 'fixture', 'rust-verifier', 'profile-verifier', 'out'):
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('original finite Linux owner/common lock required')
    if pin(a.runtime)['sha256'] != RUNTIME:
        raise RuntimeError('exact frozen Runtime required')
    a.out.mkdir()
    sdk = a.out / 'Relocated canonical SDK λ'
    stage = a.out / 'install-stage'
    assembly = []
    source_pins = []
    for source in sorted(a.source.rglob('*')):
        if source.is_symlink():
            raise RuntimeError('canonical source cannot contain links')
        if not source.is_file():
            continue
        relative = source.relative_to(a.source)
        if set(relative.parts) & {'origin', 'test', 'tests', '.xmake'}:
            raise RuntimeError('maintenance/test/cache directory entered canonical installation')
        target = stage / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        source_pins.append(pin(source))
        assembly.append(dict(source=pin(source), installed=relative.as_posix()))
    for path in (Path(__file__), a.c_provider, a.rust_provider, a.runtime, a.xmake,
                 a.clang, a.fixture, a.rust_verifier, a.profile_verifier):
        source_pins.append(pin(path))
    for path in (a.rust_verifier, a.profile_verifier):
        ast.parse(path.read_bytes(), filename=str(path))
    c = copy.deepcopy(next(row for row in json.loads(a.c_provider.read_bytes())['profiles']
                           if row['profile']['startup'] == 'command' and not row['profile']['threads']
                           and not row['profile']['exceptions']))
    c['id'] = 'task06-canonical-c-cpp-wasi33-command'
    # Keep genuine admitted libc/libc++/CRT/builtins, with the original exact
    # hashes. They are target library artifacts rather than authored SDK copies.
    libraries = []
    for fact in c['files']:
        relative = fact['path']
        if relative.startswith(('include/wasm32-wasip1/', 'include/c++/', 'lib/wasm32-wasip1/', 'lib/clang/22/')):
            source = a.target_libraries / relative
        else:
            continue
        if pin(source, a.target_libraries) != fact:
            raise RuntimeError('real pinned WASI target library/header differs: ' + relative)
        target = stage / relative
        if target.exists():
            if target.read_bytes() != source.read_bytes():
                raise RuntimeError('canonical SDK conflicts with genuine target library')
        else:
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, target)
        libraries.append(pin(source))
    rust = copy.deepcopy(next(row for row in json.loads(a.rust_provider.read_bytes())['profiles']
                              if 'rust' in row['languages'] and row['profile']['startup'] == 'command'))
    rust['id'] = 'task06-canonical-rust185-std-command'
    rust['library_abi']['required_files'] = [path.relative_to(stage).as_posix() for path in
        sorted((stage / 'include/platform/ros/rs').rglob('*')) if path.is_file()]
    old_manifest = stage / 'share/wasm-profiles.json'
    if old_manifest.exists():
        old_manifest.unlink()
    current = [pin(path, stage) for path in sorted(stage.rglob('*')) if path.is_file()]
    for row in (c, rust):
        row['files'] = current
    save(old_manifest, dict(format='uwvm-sdk-wasm-profiles-v1', defaults=c['profile'],
                            profiles=[c, rust], default_authority=[]))
    stage.rename(sdk)
    before = [pin(path, sdk) for path in sorted(sdk.rglob('*')) if path.is_file()]
    save(a.out / 'installation.json', dict(canonical_source_files=assembly, relocated_sdk=str(sdk),
        source_overrides=[], origin_fallback=False, target_library_inputs=libraries,
        provider_descriptor_inputs=[pin(a.c_provider), pin(a.rust_provider)],
        provider_scope='Genuine unchanged toolchain/target-library identities with fresh current canonical SDK file pins'))
    save(a.out / 'installed-source-pins.json', before)
    for name in ('configuration', 'global', 'temporary', 'commands'):
        (a.out / name).mkdir()
    environment = dict(os.environ, XMAKE_PROGRAM_DIR='/home/macromodel/.local/share/xmake',
        XMAKE_STATS='false', XMAKE_SKIP_HISTORY='y', XMAKE_CONFIGDIR=str(a.out / 'configuration'),
        XMAKE_GLOBALDIR=str(a.out / 'global'), XMAKE_TMPDIR=str(a.out / 'temporary'),
        TMPDIR=str(a.out / 'temporary'), PYTHONDONTWRITEBYTECODE='1')
    cases, artifacts = [], []

    def run(name, command, layer, expected=0, data=None, wanted=None):
        directory = a.out / 'commands' / name
        directory.mkdir()
        with (directory / 'stdout').open('xb') as out, (directory / 'stderr').open('xb') as err:
            result = subprocess.run(list(map(str, command)), cwd=sdk / 'example', env=environment,
                input=data, stdout=out, stderr=err, timeout=1000, check=False)
        row = dict(name=name, layer=layer, argv=list(map(str, command)), exit=result.returncode,
            expected_exit=expected, stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr'))
        row['status'] = 'PASS' if result.returncode == expected and (wanted is None or
            (directory / 'stdout').read_bytes() == wanted) else 'FAIL'
        cases.append(row)
        save(a.out / 'cases.json', cases)
        print(json.dumps({key: row[key] for key in ('name', 'layer', 'exit', 'status')}), flush=True)
        if row['status'] != 'PASS':
            raise RuntimeError('actual canonical SDK check failed: ' + name)

    def compile_example(label, platform, language, selection):
        run(label + '-configure', [a.xmake, 'f', '-c', '--yes', '--ros-sdk-platforms=ros',
            '--ros-sdk-languages=c,cpp', '--sdk-toolchain=' + str(a.wasi_sdk),
            '--sdk-target=wasm32-wasip1', '--sdk-startup=command', '--sdk-exceptions=n',
            '--sdk-threads=n', '--sdk-offline=y', '--sdk-python=/usr/bin/python3',
            '--sdk-example-platform=' + platform, '--sdk-example-language=' + language,
            '--sdk-example=' + selection, '--ros-sdk-cpp-standard=20'], 'ACTUAL_XMAKE_CONFIGURATION')
        output = a.out / (label + '-product')
        run(label + '-build', [a.xmake, 'sdk-example-build', '--output=' + str(output)],
            'ACTUAL_CANONICAL_XMAKE_WASM_BUILD')
        receipt = json.loads((output / 'build-receipt.json').read_bytes())
        if receipt['status'] != 'BUILT_NOT_EXECUTED' or len(receipt['artifacts']) != 1:
            raise RuntimeError('one genuine Wasm artifact required')
        artifact = Path(receipt['artifacts'][0]['path'])
        if pin(artifact)['sha256'] != receipt['artifacts'][0]['sha256']:
            raise RuntimeError('exact compiler-produced artifact required')
        run(label + '-validate', ['/usr/bin/wasm-validate', '--enable-all', artifact], 'ACTUAL_WASM_VALIDATION')
        run(label + '-imports', ['/usr/bin/wasm-objdump', '-x', artifact], 'ACTUAL_WASM_IMPORT_INVENTORY')
        artifacts.append(dict(platform=platform, language=language, selection=selection, artifact=pin(artifact)))
        return [a.runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                '--ros1-global-disable', '--ros2-global-disable', '--run', artifact]

    status = 'FAILED_RETAINED'
    failure = None
    try:
        for vendor in VENDORS:
            value = json.loads((sdk / f'example/platform/{vendor}/state-reader-selections.json').read_bytes())
            rows = value if isinstance(value, list) else value.get('selections', value.get('profiles'))
            selected = rows[0]
            entry = 'state-reader.' + selected.get('selection', selected.get('id'))
            for language in ('c', 'cpp'):
                label = vendor + '-' + language
                guest = compile_example(label, vendor, language, entry)
                run(label + '-default-denial', [*guest, 'raw', 'task06-ungranted-canonical-state'],
                    'ACTUAL_GUEST_DEFAULT_DENIAL', 13)
        run('seven-current-rust-profiles', [sys.executable, '-I', '-S', '-B', a.profile_verifier,
            '--sdk', sdk, '--c-sdk', sdk, '--toolchain', a.rust_toolchain, '--runtime', a.runtime,
            '--xmake', a.xmake, '--clang', a.clang, '--fixture', a.fixture,
            '--verifier', a.rust_verifier, '--profiles',
            'ros2_humble_v1,ros2_jazzy_v1,ros2_lyrical_v1,ros2_humble_v2,ros2_jazzy_v2,ros2_humble_v2_2,ros2_jazzy_v2_2',
            '--out', a.out / 'rust-profiles'], 'ACTUAL_SEVEN_PROFILE_XMAKE_CARGO_GUEST_BATCH')
        status = 'PASS_SCOPED_CANONICAL_INSTALL_RELOCATION_AND_EXECUTION'
    except Exception as error:
        failure = str(error)
    after = [pin(path, sdk) for path in sorted(sdk.rglob('*')) if path.is_file()]
    immutable = before == after and all(pin(Path(row['path'])) == row for row in source_pins)
    if not immutable:
        status, failure = 'FAILED_RETAINED', 'canonical SDK or source/tool input changed during verification'
    save(a.out / 'summary.json', dict(status=status, error=failure, cases=len(cases),
        actual_c_cpp_xmake_builds=len(artifacts), c_cpp_artifacts=artifacts,
        installed_sdk_immutable=immutable, source_override_count=0, origin_fallback=False,
        native_ros_cases=0, gateway_cases=0, hardware_cases=0, complete_platform_support=False,
        default_authority=[]))
    if status != 'PASS_SCOPED_CANONICAL_INSTALL_RELOCATION_AND_EXECUTION':
        raise RuntimeError(failure)


if __name__ == '__main__':
    main()
