"""Compile the upstream camera synchronizer with the installed WR C++ SDK."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import sys

from check_isaac_ros_message_sync import REVISION, SOURCES
from run_scene_vm import sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--upstream', type=Path, required=True)
    parser.add_argument('--sdk', type=Path, required=True, help='Installed signed C++ development prefix')
    parser.add_argument('--vm', type=Path, required=True, help='VM from the signed toolchain qualification')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if not __debug__:
        raise RuntimeError('Qualification requires Python assertions')
    verify_gate()
    upstream = args.upstream.resolve() / 'isaac_common'
    assert {name: sha(upstream / name) for name in SOURCES} == SOURCES
    vm, sdk, output = (path.resolve() for path in (args.vm, args.sdk, args.output))
    output.mkdir(parents=True, exist_ok=False)
    os.environ['TMPDIR'] = str(output)
    sys.path.insert(0, str(sdk / 'install'))
    from guest_profile import Profile
    from imports import inspect

    source = Path(__file__).with_name('isaac_camera_sync.cpp').resolve()
    include = upstream / 'include'
    result = dict(passed=False, upstream=REVISION, source_sha256=sha(source), vm_sha256=sha(vm),
                  scope='CPU timestamp matching in an import-free C++ Wasm guest',
                  input_frames=11, expected_pairs=5, builds={}, commands=[],
                  cgroup=Path('/proc/self/cgroup').read_text().strip(), cpus=sorted(os.sched_getaffinity(0)))
    report = output / 'results.json'

    def run(label, command, expected=0):
        with (output / (label + '.log')).open('w') as log:
            status = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=180).returncode
        result['commands'].append(dict(label=label, argv=list(map(str, command)), exit=status))
        report.write_text(json.dumps(result, indent=2) + '\n')
        if status != expected:
            raise RuntimeError(f'{label} exited {status}, expected {expected}. See {output}/{label}.log')
        if expected == 120:
            assert 'unreachable' in (output / (label + '.log')).read_text().lower()

    native = output / 'native'
    run('native-build', ['/usr/bin/c++', '-std=c++17', '-O3', '-g', '-I', include, source, '-o', native])
    run('native', [native])
    for name, defines, expected in [('reference', [], 0), ('wrong-pair-count', ['-D', 'WR_SYNC_WRONG_PAIR_COUNT'], 120)]:
        guest = output / (name + '.wasm')
        run(name + '-build', [sys.executable, '-B', sdk / 'bin/wrcc', '--build-receipt', '--target',
                              'wasm32', '--std', 'c++17', '-O3', '-g', '--reactor', '--export', '_start',
                              '-I', include, *defines, source, '-o', guest])
        receipt = Path(str(guest) + '.wros-build.json')
        built = json.loads(receipt.read_bytes())
        profile = built['profile']
        assert built['mode'] == 'strict' and built['artifact_sha256'] == sha(guest)
        assert (profile['address_bits'], profile['wasm_version'], profile['threads'], profile['mte'],
                profile['exceptions']) == (32, '3.0', False, True, 'eh')
        assert inspect(guest.read_bytes()) == []
        run(name + '-verify', [sys.executable, '-B', sdk / 'install/qualification.py', '--vm', vm,
                               '--artifact', guest, '--receipt', receipt, '--approved-receipt-sha256', sha(receipt)])
        result['builds'][name] = dict(sha256=sha(guest), imports=[], profile=profile, receipt_sha256=sha(receipt))
        for engine in ('int', 'aot'):
            run(name + '-' + engine, [vm, *Profile().uwvm_feature_flags(), '--debug-fault-action', 'exit',
                                      '--runtime-' + engine, '--runtime-compile-threads', '1',
                                      '--runtime-llvm-jit-cache-path', 'disable', '--run', guest], expected)
    result['passed'] = True
    report.write_text(json.dumps(result, indent=2) + '\n')


if __name__ == '__main__':
    main()
