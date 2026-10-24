#!/usr/bin/env python3
"""Execute the original native ROS witness suites, proving every ARM probe and spawned Gateway under QEMU."""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import time
import traceback

from runtime import Evidence, digest
from artifacts import Pool
from run import StartupEnvelope

sys.dont_write_bytecode = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--workspace', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--qemu', type=Path, required=True)
    parser.add_argument('--binfmt-evidence', type=Path, required=True)
    parser.add_argument('--artifact-pool', type=Path)
    parser.add_argument('--suite', default='all', choices=['all', 'topics', 'resources', 'tf', 'lifecycle', 'secure', 'action', 'performance'])
    args = parser.parse_args()
    source, workspace = args.source.resolve(strict=True), args.workspace.resolve(strict=True)
    lease = json.loads(args.binfmt_evidence.read_text())
    name = lease['name']
    registered = (Path('/proc/sys/fs/binfmt_misc')/name).read_text()
    if registered != lease['entry'] or lease['qemu_sha256'] != digest(args.qemu) or os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != name:
        raise RuntimeError('verified active temporary binfmt lease required')
    spec = importlib.util.spec_from_file_location('ros_cgroup', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
    probe = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(probe)
    spec = importlib.util.spec_from_file_location('original_ros_native', source/'test/0073.ros-tests/support/source/ros/run_native.py')
    original = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(original)
    startup = StartupEnvelope(original, {original.main.__code__: ('ros1-master', 30, .025)})
    startup.install()
    evidence = Evidence(args.out.resolve(), probe.probe)
    evidence.emulator(args.qemu)
    pool = Pool(args.artifact_pool) if args.artifact_pool else None
    if pool:
        pool.install()
    roots = {workspace/'gateway/noetic-rootfs': 1, workspace/'gateway/jazzy-rootfs': 2}
    old_popen = subprocess.Popen
    launches = []

    def start(command, *positional, **keyword):
        required = []
        if isinstance(command, (list, tuple)) and len(command) > 2 and command[0] == 'chroot':
            root = Path(command[1]).resolve()
            version = roots.get(root)
            if version is None:
                raise RuntimeError('unexpected rootfs in frozen native runner')
            for token in command[2:]:
                if isinstance(token, str) and token.startswith('/tmp/uwvm-native-snapshot-'):
                    path = root/token.lstrip('/')
                    label = f'ros{version}:'+str(path)
                    evidence.target(label, path)
                    required.append(label)
            if not required:
                # roscore and ros2 CLI are ARM Python programs, reached via the
                # SDK's ARM shell and interpreter rather than the probe binaries.
                interpreter = (root/'usr/bin/python3').resolve(strict=True)
                label = f'ros{version}:python'
                evidence.target(label, interpreter)
                required.append(label)
        started = time.monotonic()
        process = old_popen(command, *positional, **keyword)
        process._uwvm_arm_launch_time = started
        if required:
            name = 'native-'+str(len(launches))+'/'
            launches.append({'name': name, 'pid': process.pid, 'required': required, 'command': list(command)})
            evidence.start(process, name, list(command))
        return process

    code, status, error = 1, 'FAIL', None
    try:
        subprocess.Popen = start
        sys.argv = [str(source/'test/0073.ros-tests/support/source/ros/run_native.py'), '--workspace', str(workspace),
                    '--suite', args.suite, '--out', str(evidence.out/'native-results')]
        code = original.main()
        status = 'PASS' if code == 0 else 'BLOCKED' if code == 77 else 'FAIL'
        for launch in launches:
            evidence.check(launch['required'], launch['name'])
            observed = {label for sample in evidence.samples.values()
                        if sample['name'] == launch['name'] and sample['ancestry'][0] == launch['pid']
                        for label in sample['mapped_arm_targets']}
            if set(launch['required'])-observed:
                raise RuntimeError('missing ARM target mapping in the actual launch process tree: '+launch['name'])
    except BaseException:
        error = traceback.format_exc()
    finally:
        subprocess.Popen = old_popen
        startup.restore()
        if pool:
            pool.restore()
        result = evidence.finish(status if error is None else 'FAIL', {'suite': 'original-native-'+args.suite,
            'original_driver_sha256': digest(source/'test/0073.ros-tests/support/source/ros/run_native.py'), 'driver_sha256': digest(__file__),
            'launches': launches, 'binfmt_lease': lease, 'active_binfmt_content': registered,
            'artifact_pool': pool.evidence() if pool else None,
            'qemu_startup_envelopes': startup.rows,
            'error': error, 'raw_native_report': str(evidence.out/'native-results/report.json')})
        if result['status'] != 'PASS':
            code = 1
    print(result['status'], evidence.out/'architecture-evidence.json', flush=True)
    return code


if __name__ == '__main__':
    raise SystemExit(main())
