#!/usr/bin/env python3
"""Build frozen ARM64 interpreter profiles, then execute them with QEMU in the inherited cgroup."""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import time

sys.dont_write_bytecode = True
from runtime import cgroup_incarnation, cgroup_oom_deltas, require_arm
from gateway_qualification import pins_preserved, read_file_binding


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def arm_elf(path):
    """Use the common complete opened-image fact, including device and inode.

    A fresh build report must identify the compiled image itself. A later
    identical-byte replacement cannot reuse that image's qualification.
    """
    return require_arm(path)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--config', type=Path, required=True,
                        help='JSON compiler argv, compile_flags, link_flags, qemu_argv and optional environment')
    parser.add_argument('--profiles', nargs='+', choices=['none', 'ros1', 'ros2', 'dual'],
                        default=['none', 'ros1', 'ros2', 'dual'])
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    out = args.out.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    manifest = json.loads(args.manifest.read_text())
    config = json.loads(args.config.read_text())
    # Reviewed custom registries may be selected for ROS profiles without
    # injecting a provider into the ordinary no-ROS profile. These are explicit
    # compiler inputs, preserved through the configuration and source pins.
    profile_flags = config.get('profile_compile_flags', {})
    if (not isinstance(profile_flags, dict) or set(profile_flags) - {'none', 'ros1', 'ros2', 'dual'}
            or any(not isinstance(flags, list) or any(not isinstance(flag, str) or not flag or '\x00' in flag
                   for flag in flags) for flags in profile_flags.values())):
        raise RuntimeError('profile_compile_flags must map closed profile names to explicit compiler argv strings')
    spec = importlib.util.spec_from_file_location('ros_cgroup', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    before = module.probe()
    report = {'status': 'FAIL', 'source': str(source), 'source_manifest_sha256': digest(args.manifest),
              'config_sha256': digest(args.config), 'builder_sha256': digest(__file__),
              'artifact_helper_sha256': digest(Path(__file__).with_name('runtime.py')),
              'binding_helper_sha256': digest(Path(__file__).with_name('gateway_qualification.py')),
              'parallelism': 1, 'cgroup_before': before, 'profiles': []}
    active = None
    accepted_incarnation = None

    def checked_source(name):
        expected = manifest[name]
        expected = expected['sha256'] if isinstance(expected, dict) else expected
        if digest(source/name) != expected:
            raise RuntimeError('frozen source changed: ' + name)
        return expected

    def terminate(signum, frame):
        if active is not None and active.poll() is None:
            os.killpg(active.pid, signal.SIGKILL)
            active.wait(timeout=5)
        raise SystemExit(128+signum)

    signal.signal(signal.SIGTERM, terminate)
    signal.signal(signal.SIGINT, terminate)
    environment = dict(os.environ)
    environment.update(config.get('environment', {}))

    def run(command, log, timeout):
        nonlocal active
        start = time.monotonic()
        with log.open('wb') as stream:
            active = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT,
                                      env=environment, start_new_session=True)
        record = {'command': command, 'pid': active.pid,
                  'membership': Path(f'/proc/{active.pid}/cgroup').read_text().splitlines()}
        if record['membership'] != before['membership']:
            os.killpg(active.pid, signal.SIGKILL)
            active.wait(timeout=5)
            raise RuntimeError('child cgroup mismatch')
        try:
            record['returncode'] = active.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            os.killpg(active.pid, signal.SIGKILL)
            active.wait(timeout=5)
            record['timeout'] = timeout
            raise
        finally:
            active = None
        record['seconds'] = round(time.monotonic()-start, 3)
        return record

    try:
        if Path(__file__).resolve().parent != source/'test/0073.ros-tests/support/source/ros_aarch64':
            raise RuntimeError('fresh Worker builder/helpers must execute from the complete frozen source tree')
        if before['status'] != 'PASS':
            raise RuntimeError('existing 64 GiB cgroup not proven')
        accepted_incarnation = cgroup_incarnation(Path(before['cgroup_ancestors'][0]['path']))
        before = module.probe()
        if (before['status'] != 'PASS' or
                cgroup_incarnation(Path(before['cgroup_ancestors'][0]['path'])) != accepted_incarnation):
            raise RuntimeError('Worker build cgroup incarnation changed while accepting the counter baseline')
        report['cgroup_before'] = before
        report['cgroup_incarnation_before'] = accepted_incarnation
        report['memory_swap_max_before'] = (Path(accepted_incarnation['path'])/'memory.swap.max').read_text().strip()
        if report['memory_swap_max_before'] != '0':
            raise RuntimeError('Worker build requires zero swap in the existing 64 GiB cgroup')
        limit = before['effective_memory_max']
        used = int(before['cgroup_ancestors'][0]['memory.current'])
        if limit-used < 16*1024**3:
            raise RuntimeError('insufficient inherited cgroup headroom for one compiler plus 8 GiB reserve')
        for name in manifest:
            checked_source(name)
        for label, argv in (('compiler',config['compiler']),('qemu',config['qemu_argv'])):
            pin = read_file_binding(argv[0])
            report[label] = {'argv':argv,'sha256':pin['sha256'],'input':pin}
        base = ['-std=c++26', '-O1', '-g0', '-pthread', '-Wno-undefined-inline',
                '-fconstexpr-steps=8388608', '-DUWVM=2', '-DUWVM_USE_UWVM_INT',
                '-DUWVM_DISABLE_JIT', '-DUWVM_USE_THREAD_LOCAL',
                '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64', '-DUWVM_ENABLE_WASIP1_SOCKET',
                '-DUWVM_VERSION_X=2', '-DUWVM_VERSION_Y=0', '-DUWVM_VERSION_Z=3', '-DUWVM_VERSION_S=0']
        for directory in ['src', 'third-parties/fast_io/include', 'third-parties/bizwen/include',
                          'third-parties/boost_unordered/include']:
            base.extend(['-I', str(source/directory)])
        for profile in args.profiles:
            dest = out/profile
            dest.mkdir(mode=0o700)
            row = {'profile': profile, 'status': 'FAIL', 'commands': [], 'dependencies': {}, 'objects': {}}
            report['profiles'].append(row)
            flags = [*config['compile_flags'], *base, *profile_flags.get(profile, [])]
            if profile in ('ros1', 'dual'):
                flags.append('-DUWVM_IMPORT_ROS1')
            if profile in ('ros2', 'dual'):
                flags.append('-DUWVM_IMPORT_ROS2')
            for unit, relative in [('runtime', 'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'),
                                   ('main', 'src/uwvm2/uwvm/main.default.cpp'),
                                   ('host_api', 'src/uwvm2/uwvm/host_api.default.cpp')]:
                dep = dest/(unit+'.d')
                command = [*config['compiler'], *flags, '-MMD', '-MF', str(dep),
                           '-c', str(source/relative), '-o', str(dest/(unit+'.o'))]
                record = run(command, dest/(unit+'.log'), 600)
                row['commands'].append(record)
                if record['returncode']:
                    raise RuntimeError(profile+' '+unit+' compile failed')
                text = dep.read_text().replace('\\\n', ' ')
                names = sorted(set(os.path.normpath(name) for name in
                    re.findall(re.escape(str(source)) + r'/[^\s\\]+', text)))
                if not names:
                    raise RuntimeError('compiler dependency record is empty')
                row['dependencies'][unit] = {str(Path(name).relative_to(source)):
                    checked_source(str(Path(name).relative_to(source))) for name in names}
                row['objects'][unit] = arm_elf(dest/(unit+'.o'))
            binary = dest/'uwvm2-ros'
            command = [*config['compiler'], *config['link_flags'], '-pthread',
                       *[str(dest/(unit+'.o')) for unit in ('main', 'runtime', 'host_api')],
                       '-ldl', '-o', str(binary)]
            record = run(command, dest/'link.log', 120)
            row['commands'].append(record)
            if record['returncode']:
                raise RuntimeError(profile+' link failed')
            row['binary'] = arm_elf(binary)
            record = run([*config['qemu_argv'], str(binary), '--version'], dest/'version.log', 30)
            row['commands'].append(record)
            if record['returncode']:
                raise RuntimeError(profile+' ARM QEMU version execution failed')
            row['status'] = 'PASS'
            print('PASS', profile, 'AArch64 ELF and QEMU execution', flush=True)
        for name in manifest:
            checked_source(name)
        if (digest(__file__) != report['builder_sha256'] or
                digest(Path(__file__).with_name('runtime.py')) != report['artifact_helper_sha256'] or
                digest(Path(__file__).with_name('gateway_qualification.py')) != report['binding_helper_sha256'] or
                digest(args.config) != report['config_sha256'] or digest(args.manifest) != report['source_manifest_sha256']):
            raise RuntimeError('executed Worker builder/artifact helper changed during compilation')
        report['driver_inputs_preserved'] = True
        if not pins_preserved([report[label]['input'] for label in ('compiler','qemu')]):
            raise RuntimeError('Worker compiler/QEMU opened input identity changed during compilation/execution')
        report['tools_preserved'] = True
        report['status'] = 'PASS'
    except Exception as error:
        report['error'] = str(error)
    finally:
        try:
            final_before = cgroup_incarnation(Path(before['cgroup_ancestors'][0]['path']))
            after = module.probe()
            report['cgroup_after'] = after
            final_incarnation = cgroup_incarnation(Path(before['cgroup_ancestors'][0]['path']))
            report['memory_swap_max_after'] = (Path(final_incarnation['path'])/'memory.swap.max').read_text().strip()
            same = (accepted_incarnation is not None and accepted_incarnation == final_before == final_incarnation
                    and before.get('membership') == after.get('membership')
                    and after['cgroup_ancestors'][0]['path'] == accepted_incarnation['path'])
            valid, changes = cgroup_oom_deltas(before, after, same)
            report.update(cgroup_incarnation_after_before_probe=final_before,
                          cgroup_incarnation_after=final_incarnation, same_cgroup_incarnation=same,
                          memory_events=changes)
            if not valid or report['memory_swap_max_after'] != '0' or after['status'] != 'PASS':
                report.update(status='FAIL', error='Worker build same-incarnation zero-OOM evidence failed')
        except (OSError, ValueError, KeyError, RuntimeError) as error:
            report.update(status='FAIL', cgroup_final_probe_error=str(error))
        (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(report['status'], out/'report.json', flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
