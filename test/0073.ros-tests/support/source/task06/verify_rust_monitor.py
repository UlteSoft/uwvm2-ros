#!/usr/bin/env python3
"""Build the genuine Rust monitor through a relocated SDK's xmake/Cargo route.

Canonical complete native-fixture recordings exercise application behavior;
they do not supply a ROS transport or replace a Guest executable. All SDK and
tool inputs stay read-only; only this run's output/cache directories are used.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

RUNTIME_SHA256 = '719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path, value):
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('sdk', 'toolchain', 'runtime', 'xmake', 'fixtures', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--profile', default='ros2_humble_v1')
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1' or sha(args.runtime) != RUNTIME_SHA256:
        raise RuntimeError('same finite owner and exact frozen Runtime required')
    if any(set(path.relative_to(args.sdk).parts) & {'origin', 'test', 'tests', '.xmake'}
           for path in args.sdk.rglob('*')):
        raise RuntimeError('relocated SDK cannot contain any origin/test tree or inherited xmake cache')
    args.out.mkdir()
    for name in ('temporary', 'configuration', 'global', 'cargo-home'):
        (args.out / name).mkdir()
    environment = dict(os.environ, XMAKE_PROGRAM_DIR='/home/macromodel/.local/share/xmake',
                       XMAKE_STATS='false', XMAKE_SKIP_HISTORY='y', XMAKE_CONFIGDIR=str(args.out / 'configuration'),
                       XMAKE_GLOBALDIR=str(args.out / 'global'), XMAKE_TMPDIR=str(args.out / 'temporary'),
                       TMPDIR=str(args.out / 'temporary'), CARGO_HOME=str(args.out / 'cargo-home'))
    sources = sorted(p for p in args.sdk.rglob('*') if p.is_file())
    if any(p.is_symlink() for p in args.sdk.rglob('*')):
        raise RuntimeError('ordinary SDK inputs required')
    pins = [dict(path=str(p), bytes=p.stat().st_size, sha256=sha(p)) for p in sources]
    pins += [dict(path=str(p), bytes=p.stat().st_size, sha256=sha(p)) for p in
             (Path(__file__), args.runtime, args.xmake)]
    pins += [dict(path=str(p), bytes=p.stat().st_size, sha256=sha(p))
             for p in sorted(args.fixtures.rglob('*')) if p.is_file()]
    save(args.out / 'source-pins.json', pins)
    cases = []

    def run(name, command, layer, expected=0, data=None, wanted=None):
        folder = args.out / name
        folder.mkdir()
        with (folder / 'stdout').open('xb') as out, (folder / 'stderr').open('xb') as err:
            result = subprocess.run(list(map(str, command)), cwd=args.sdk / 'example', env=environment,
                                    input=data, stdout=out, stderr=err, timeout=240)
        output = (folder / 'stdout').read_bytes()
        row = dict(name=name, argv=list(map(str, command)), layer=layer,
                   exit=result.returncode, expected_exit=expected,
                   oracle_match=wanted is None or output == wanted,
                   stdout_sha256=sha(folder / 'stdout'), stderr_sha256=sha(folder / 'stderr'))
        row['status'] = 'PASS' if result.returncode == expected and row['oracle_match'] else 'FAIL'
        cases.append(row)
        save(args.out / 'cases.json', cases)
        print(json.dumps({k: row[k] for k in ('name', 'layer', 'status', 'exit')}), flush=True)
        return row['status'] == 'PASS'

    for name, program in (('xmake', args.xmake), ('rustc', args.toolchain / 'bin/rustc'),
                          ('cargo', args.toolchain / 'bin/cargo')):
        run(name + '-version', [program, '--version'], 'ACTUAL_TOOL_IDENTITY')
    entry = 'robot-states-monitor-process.' + args.profile
    configuration = [args.xmake, 'f', '-c', '-P', args.sdk / 'example', '--ros-sdk-platforms=ros',
                     '--ros-sdk-languages=rs', '--sdk-toolchain=' + str(args.toolchain),
                     '--sdk-target=wasm32-wasip1', '--sdk-startup=command', '--sdk-exceptions=n',
                     '--sdk-threads=n', '--sdk-offline=y', '--sdk-python=/usr/bin/python3',
                     '--sdk-rust-version=1.85.0', '--sdk-rust-library=std', '--sdk-rust-edition=2021',
                     '--sdk-rust-panic=abort', '--sdk-ros-backend=ros2', '--sdk-example-platform=flexiv',
                     '--sdk-example-language=rs', '--sdk-example=' + entry]
    good = run('configure', configuration, 'ACTUAL_XMAKE_CONFIGURATION')
    output = args.out / 'rust-output'
    if good:
        good = run('build', [args.xmake, 'sdk-example-build', '-P', args.sdk / 'example',
                            '--output=' + str(output)], 'ACTUAL_XMAKE_CARGO_WASM_BUILD')
    if good:
        receipt = json.loads((output / 'build-receipt.json').read_bytes())
        if receipt['status'] != 'BUILT_NOT_EXECUTED' or len(receipt['artifacts']) != 1:
            raise RuntimeError('actual single Rust artifact receipt required')
        artifact = Path(receipt['artifacts'][0]['path'])
        frozen = sha(artifact)
        if frozen != receipt['artifacts'][0]['sha256']:
            raise RuntimeError('exact Rust Wasm receipt hash required')
        run('validate', ['/usr/bin/wasm-validate', '--enable-all', artifact], 'WASM_VALIDATION')
        run('imports', ['/usr/bin/wasm-objdump', '-x', artifact], 'ACTUAL_WASM_IMPORT_INVENTORY')
        runtime = [args.runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                   '--ros1-global-disable', '--ros2-global-disable', '--run', artifact]
        for mode, expected in (('valid', 0), ('empty-q', 0), ('nan-unobserved', 22),
                               ('trailing', 22), ('square-overflow', 34)):
            data = (args.fixtures / (mode + '.bin')).read_bytes()
            wanted = (args.fixtures / (mode + '.stdout')).read_bytes() if expected == 0 else None
            run('guest-' + mode, runtime + ['process', '1'], 'ACTUAL_GUEST_COMPLETE_CALLBACK',
                expected, data, wanted)
        run('live-denial', runtime + ['live', 'task06-ungranted-monitor', '1'],
            'ACTUAL_GUEST_DEFAULT_DENIAL', 13)
        data = (args.fixtures / 'valid.bin').read_bytes()
        prefix = b'Finite virtual-address prerequisite: 32768 MiB; physical/RSS limits are separate\n'
        run('xmake-process', [args.xmake, 'sdk-example-run', '-P', args.sdk / 'example',
                             '--receipt=' + str(output / 'build-receipt.json'), '--runtime=' + str(args.runtime)],
            'ACTUAL_GUEST_XMAKE_CALLBACK', 0, data, prefix + (args.fixtures / 'valid.stdout').read_bytes())
        if sha(artifact) != frozen:
            raise RuntimeError('Rust artifact changed')
    for pin in pins:
        if sha(Path(pin['path'])) != pin['sha256']:
            raise RuntimeError('frozen SDK/source/tool changed')
    if {str(p) for p in args.sdk.rglob('*') if p.is_file()} != {str(p) for p in sources}:
        raise RuntimeError('SDK input file set changed')
    if any(set(path.relative_to(args.sdk).parts) & {'origin', 'test', 'tests', '.xmake'}
           for path in args.sdk.rglob('*')):
        raise RuntimeError('a forbidden maintenance/test/cache directory appeared inside the SDK')
    save(args.out / 'summary.json', dict(
        status='PASS_SCOPED_RUST_CALLBACK' if cases and all(c['status'] == 'PASS' for c in cases) else 'FAILED_RETAINED',
        layers={layer: dict(passed=sum(c['layer'] == layer and c['status'] == 'PASS' for c in cases),
                            denominator=sum(c['layer'] == layer for c in cases))
                for layer in sorted({c['layer'] for c in cases})},
        native_ros=0, gateway=0, hardware=0, original_processes=0, default_authority=[],
        sdk_has_no_origin_test_or_xmake_cache=True,
        scope='Real Rust/Cargo/Wasm complete callback; recorded native fixture bytes are not ROS delivery'))
    if not cases or any(c['status'] != 'PASS' for c in cases):
        raise SystemExit(1)


if __name__ == '__main__':
    main()
