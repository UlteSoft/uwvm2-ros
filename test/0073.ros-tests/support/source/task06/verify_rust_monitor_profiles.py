#!/usr/bin/env python3
"""Run the six remaining real Rust profile builds and whole-message Guests.

The accepted unchanged H1 r5 receipt is separate; it is not rerun by default.
Every selected profile gets an actual native C data encoder over its full
versioned generated header, then the genuine relocated-SDK xmake/Cargo verifier.
Data encoders are not ROS peers or transport substitutes. No generator runs.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


PROFILES = ('ros2_humble_v1', 'ros2_jazzy_v1', 'ros2_lyrical_v1', 'ros2_humble_v2',
            'ros2_jazzy_v2', 'ros2_humble_v2_2', 'ros2_jazzy_v2_2')


def pin(path):
    raw = path.read_bytes()
    return dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('sdk', 'c-sdk', 'toolchain', 'runtime', 'xmake', 'clang', 'fixture', 'verifier', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--profiles', default=','.join(PROFILES[1:]))
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same finite Linux owner/common lock required')
    profiles = args.profiles.split(',')
    if not profiles or len(set(profiles)) != len(profiles) or any(value not in PROFILES for value in profiles):
        raise RuntimeError('explicit distinct original source profiles required')
    args.out.mkdir()
    pins = [pin(path) for path in (Path(__file__), args.clang, args.fixture, args.verifier)]
    sdk_sources = sorted(path for path in args.sdk.rglob('*') if path.is_file())
    c_sources = sorted(path for path in (args.c_sdk / 'include').rglob('*') if path.is_file())
    if not sdk_sources or not c_sources:
        raise RuntimeError('actual complete installed Rust and C data-source SDKs required')
    pins.extend(pin(path) for path in [*sdk_sources, *c_sources])
    ast.parse(Path(__file__).read_bytes())
    ast.parse(args.verifier.read_bytes())
    manifest_path = args.c_sdk / 'example/platform/flexiv/manifest.json'
    pins.append(pin(manifest_path))
    manifest = json.loads(manifest_path.read_bytes())
    expected = ("Joint positions: [" + ', '.join("'" + format(value, '.3f') + "'" for value in
                (0.125, -0.0, 1.0 / 3.0, -1.25, 100.0, -100.0, 2.345)) + "]\n"
                "TCP Position: x=1.250, y=-2.500, z=0.125\n"
                "TCP Orientation: w=0.500, x=0.100, y=-0.200, z=0.300\n"
                "External Force Magnitude: 13.000 N\n---\n").encode()
    empty = b'Joint positions: []\n' + expected.split(b'\n', 1)[1]
    commands, results = [], []

    def run(label, command, folder, data_output=None):
        argv = list(map(str, command))
        result = subprocess.run(argv, capture_output=True, timeout=900, check=False)
        stdout = folder / (label + '.stdout')
        stdout.write_bytes(result.stdout)
        (folder / (label + '.stderr')).write_bytes(result.stderr)
        commands.append(dict(name=label, argv=argv, exit=result.returncode,
                             stdout=pin(stdout), layer='NATIVE_COMPLETE_VALUE_DATA_ONLY' if '-fixture-' in label else 'ACTUAL_SDK_TOOL_OR_WASM_VERIFIER'))
        (args.out / 'commands.json').write_text(json.dumps(commands, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError('actual tool/fixture/verifier failure: ' + label)
        if data_output is not None:
            data_output.write_bytes(result.stdout)

    for selected in profiles:
        profile = next(row for row in manifest['profiles'] if row['id'] == selected)
        if profile['ros_version'] != 2 or profile['monitor_force_member'] not in ('ext_wrench_in_tcp', 'tcp_wrench_local'):
            raise RuntimeError('genuine selected monitor profile differs')
        include = args.c_sdk / 'include/platform/flexiv/c/ros2' / profile['family']
        if not (include / 'bounded_records.h').is_file():
            raise RuntimeError('selected complete native data codec header absent')
        folder = args.out / selected
        folder.mkdir()
        fixtures = folder / 'fixtures'
        fixtures.mkdir()
        executable = folder / 'native-complete-value-encoder'
        run(selected + '-fixture-build', [args.clang, '-std=c11', '-O2',
            '-I' + str(args.c_sdk / 'include'), '-I' + str(include),
            '-DTASK06_MONITOR_FORCE_MEMBER=' + profile['monitor_force_member'], args.fixture,
            '-o', executable], folder)
        for mode in ('valid', 'empty-q', 'nan-unobserved', 'trailing', 'square-overflow'):
            run(selected + '-fixture-' + mode, [executable, mode], folder, fixtures / (mode + '.bin'))
        (fixtures / 'valid.stdout').write_bytes(expected)
        (fixtures / 'empty-q.stdout').write_bytes(empty)
        output = folder / 'actual-sdk-checks'
        run(selected + '-sdk-verifier', ['/usr/bin/python3', '-I', '-S', '-B', args.verifier,
            '--sdk', args.sdk, '--toolchain', args.toolchain, '--runtime', args.runtime,
            '--xmake', args.xmake, '--fixtures', fixtures, '--profile', selected, '--out', output], folder)
        result = json.loads((output / 'summary.json').read_bytes())
        if result['status'] != 'PASS_SCOPED_RUST_CALLBACK':
            raise RuntimeError('retained profile verifier did not pass: ' + selected)
        cases = json.loads((output / 'cases.json').read_bytes())
        results.append(dict(profile=selected, status=result['status'], cases=len(cases),
                            summary=pin(output / 'summary.json'), native_ros=0))
    for row in pins:
        if pin(Path(row['path'])) != row:
            raise RuntimeError('source/tool/full codec input changed')
    if sorted(path for path in args.sdk.rglob('*') if path.is_file()) != sdk_sources or \
            sorted(path for path in (args.c_sdk / 'include').rglob('*') if path.is_file()) != c_sources:
        raise RuntimeError('installed SDK input file set changed')
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    (args.out / 'profiles.json').write_text(json.dumps(results, indent=2) + '\n')
    summary = dict(status='PASS_SELECTED_REAL_RUST_MONITOR_PROFILES', profiles=len(results),
                   exact_profiles=profiles, unchanged_h1_r5_rerun='ros2_humble_v1' in profiles,
                   actual_xmake_cargo_builds=len(results), actual_wasm_validation=len(results),
                   native_ros=0, gateway=0, original_processes=0, hardware=0,
                   scope='Version-specific complete C data fixtures plus actual Rust SDK builds/Guests; no native ROS positive')
    # Counts come from the actual subordinate case receipts, never a planned7.
    summary['guest_cases'] = sum(sum(case['layer'].startswith('ACTUAL_GUEST_')
                                    for case in json.loads((args.out / row['profile'] / 'actual-sdk-checks/cases.json').read_bytes()))
                                 for row in results)
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
