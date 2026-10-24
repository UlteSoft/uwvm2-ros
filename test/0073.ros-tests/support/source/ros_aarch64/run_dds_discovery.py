#!/usr/bin/env python3
"""Validate a discovery-only SDK profile, then run unchanged ROS2 native assertions."""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import shutil
import signal
import subprocess
import sys
import time
import traceback
import xml.etree.ElementTree as ET

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm
from rebuild_affected import rooted, sdk_inventory, source_state
from run_resources import bind_frozen_work

DOCS = ['https://fast-dds.docs.eprosima.com/en/v2.14.6/fastdds/discovery/general_disc_settings.html',
        'https://fast-dds.docs.eprosima.com/en/v2.14.6/fastdds/xml_configuration/domainparticipant.html',
        'https://fast-dds.docs.eprosima.com/en/2.14.x/fastdds/ros2/ros2_configure.html']


def profile_scope(path):
    """Accept only the declared participant announcement changes."""
    ns = '{http://www.eprosima.com/XMLSchemas/fastRTPS_Profiles}'
    root = ET.fromstring(Path(path).read_bytes())
    expected = {'dds/profiles/participant/rtps/builtin/discovery_config/leaseAnnouncement/sec': '0',
        'dds/profiles/participant/rtps/builtin/discovery_config/leaseAnnouncement/nanosec': '100000000',
        'dds/profiles/participant/rtps/builtin/discovery_config/initialAnnouncements/count': '20',
        'dds/profiles/participant/rtps/builtin/discovery_config/initialAnnouncements/period/sec': '0',
        'dds/profiles/participant/rtps/builtin/discovery_config/initialAnnouncements/period/nanosec': '100000000'}
    leaves, seen = {}, set()
    def visit(node, parent=''):
        if not node.tag.startswith(ns):
            raise RuntimeError('profile uses an unexpected XML namespace')
        name = node.tag[len(ns):]
        current = parent+'/'+name if parent else name
        if current in seen:
            raise RuntimeError('duplicate profile XML element: '+current)
        seen.add(current)
        if name == 'participant':
            if node.attrib != {'profile_name': 'uwvm_qemu_discovery_100ms', 'is_default_profile': 'true'}:
                raise RuntimeError('exact default participant profile required')
        elif node.attrib:
            raise RuntimeError('unexpected profile attributes')
        if len(node):
            for child in node:
                visit(child, current)
        else:
            leaves[current] = (node.text or '').strip()
    visit(root)
    if leaves != expected:
        raise RuntimeError('profile changes settings beyond participant announcements')
    return {'announcement_nanoseconds': 100000000, 'initial_count': 20,
            'initial_period_nanoseconds': 100000000, 'endpoint_qos_changed': False,
            'original_discovery_assertion': '200 iterations sleeping10ms; binary unchanged',
            'primary_documentation': DOCS}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=('prepare', 'execute'), required=True)
    for name in ('source', 'sdk-workspace', 'qemu', 'binfmt-evidence', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--dds-participant-profile', type=Path,
                        default=Path(__file__).with_name('dds_participant_100ms.xml'))
    parser.add_argument('--profile-prepared', type=Path)
    parser.add_argument('--qualified-report', type=Path)
    parser.add_argument('--old-default-reports', type=Path, nargs=2)
    args = parser.parse_args()
    if args.mode == 'execute' and not all((args.profile_prepared, args.qualified_report, args.old_default_reports)):
        parser.error('execute requires --profile-prepared, --qualified-report and two --old-default-reports')
    source = args.source.resolve(strict=True)
    manifest = source.parent/'source-manifest.json'
    sdk_workspace = args.sdk_workspace.resolve(strict=True)
    sdk = sdk_workspace/'gateway/jazzy-rootfs'
    out = args.out.resolve()
    out.mkdir(parents=True, mode=0o700, exist_ok=False)
    root = out/'rootfs'
    temporary = out/'owned-tmp'
    inputs = temporary/'dds-profile-run'
    mounts, evidence, active = [], None, None
    initial, frozen, sdk_before, before = None, None, None, None
    start, status = time.monotonic(), 'FAIL'
    report = {'status': 'RUNNING', 'mode': args.mode, 'suite': 'ros2-native',
        'scope': 'Explicit participant discovery profile; original native assertions and endpoint QoS unchanged',
        'source': str(source), 'source_manifest_sha256': digest(manifest), 'commands': []}

    def save():
        report['seconds'] = round(time.monotonic()-start, 6)
        (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')

    def checked(argv):
        subprocess.run([str(token) for token in argv], check=True, capture_output=True, timeout=30)

    def bind(origin, destination, readonly=False, recursive=False):
        destination.mkdir(parents=True, exist_ok=True)
        checked(['mount', '--rbind' if recursive else '--bind', origin, destination])
        mounts.append(destination)
        checked(['mount', '--make-rprivate', destination])
        if readonly:
            checked(['mount', '-o', 'remount,bind,ro', destination])

    def identity():
        stat = evidence.leaf.stat()
        return {'device': stat.st_dev, 'inode': stat.st_ino, 'path': str(evidence.leaf),
                'membership': Path('/proc/self/cgroup').read_text().splitlines()}

    def stop():
        if active is not None:
            try:
                os.killpg(active.pid, signal.SIGTERM)
            except ProcessLookupError:
                pass
            try:
                active.wait(timeout=5)
            except subprocess.TimeoutExpired:
                try:
                    os.killpg(active.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                active.wait(timeout=5)

    def interrupted(signum, frame):
        raise RuntimeError('profile run interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    env = dict(os.environ)
    for key in ('FASTRTPS_DEFAULT_PROFILES_FILE', 'FASTDDS_DEFAULT_PROFILES_FILE', 'FAST_DDS_PROFILES_FILE',
                'RMW_FASTRTPS_USE_QOS_FROM_XML', 'RMW_FASTRTPS_PUBLICATION_MODE',
                'ROS_DISCOVERY_SERVER', 'FASTDDS_BUILTIN_TRANSPORTS', 'ROS_LOCALHOST_ONLY'):
        if env.get(key):
            raise RuntimeError('default DDS baseline required; inherited setting must be reviewed: '+key)
    for key in ('CC', 'CXX', 'LD_LIBRARY_PATH', 'LD_PRELOAD', 'QEMU_LD_PREFIX',
                'CFLAGS', 'CXXFLAGS', 'LDFLAGS', 'FASTRTPS_DEFAULT_PROFILES_FILE',
                'FASTDDS_DEFAULT_PROFILES_FILE', 'FAST_DDS_PROFILES_FILE',
                'RMW_FASTRTPS_USE_QOS_FROM_XML', 'RMW_FASTRTPS_PUBLICATION_MODE',
                'ROS_DISCOVERY_SERVER', 'FASTDDS_BUILTIN_TRANSPORTS', 'ROS_LOCALHOST_ONLY'):
        env.pop(key, None)
    env.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',
               PYTHONDONTWRITEBYTECODE='1')

    def run(name, arguments, required, timeout):
        nonlocal active
        command = ['/usr/bin/chroot', str(root), '/bin/bash', '-lc',
            'source /opt/ros/jazzy/setup.bash && exec "$@"', 'uwvm-discovery-profile', *arguments]
        row = {'name': name, 'command': command, 'status': 'RUNNING', 'timeout_seconds': timeout}
        report['commands'].append(row)
        begun = time.monotonic()
        try:
            with (out/(name+'.log')).open('wb') as log:
                active = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT,
                                          env=env, start_new_session=True)
            row['pid'] = active.pid
            evidence.start(active, name+'/', command)
            deadline = time.monotonic()+timeout
            while active.poll() is None:
                if args.mode == 'execute':
                    with evidence._lock:
                        samples = list(evidence.samples.values())
                    for sample in samples:
                        if (sample['name'] == name+'/' and sample['ancestry'][0] == active.pid
                                and 'uwvm-ros2-native-probe' in sample['mapped_arm_targets']):
                            try:
                                variables = dict(token.split(b'=', 1) for token in Path(f'/proc/{sample["pid"]}/environ').read_bytes().split(b'\0') if b'=' in token)
                                actual_env = {key.decode(): value.decode(errors='replace') for key, value in variables.items()
                                    if key.startswith((b'ROS_', b'RMW_', b'FAST', b'DDS', b'CYCLONE'))}
                                if actual_env.get('FASTRTPS_DEFAULT_PROFILES_FILE') != report['profile']['inside']:
                                    raise RuntimeError('actual ARM native process lacks the selected profile environment')
                                row['actual_arm_process_environment'] = {'pid': sample['pid'], 'values': actual_env,
                                    'scope': 'Initial exec environment; original CPP subsequently sets Domain72/RMW/security values'}
                            except (FileNotFoundError, ProcessLookupError):
                                pass
                if time.monotonic() >= deadline:
                    raise subprocess.TimeoutExpired(command, timeout)
                time.sleep(.02)
            row['returncode'] = active.returncode
            evidence.check(required, name+'/')
            observed = {label for sample in evidence.samples.values()
                        if sample['name'] == name+'/' and sample['ancestry'][0] == active.pid
                        for label in sample['mapped_arm_targets']}
            if set(required)-observed:
                raise RuntimeError('missing expected ARM mappings in actual profile launch PID tree')
            if row['returncode']:
                raise RuntimeError('original native assertions or profile SDK tool failed: '+name)
            if args.mode == 'execute' and 'actual_arm_process_environment' not in row:
                raise RuntimeError('actual ARM native profile environment was not captured')
            row['status'] = 'PASS'
        except BaseException:
            row['status'] = 'FAIL'
            raise
        finally:
            stop()
            active = None
            row['seconds'] = round(time.monotonic()-begun, 6)
            save()

    try:
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('launch profile run with unshare -m inside an existing temporary lease')
        report['profile_settings'] = profile_scope(args.dds_participant_profile)
        frozen = json.loads(manifest.read_text())
        initial = source_state(source, frozen)
        spec = importlib.util.spec_from_file_location('dds_profile_frozen_probe', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
        probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution', probe.probe, interval=.002)
        evidence.emulator(args.qemu)
        before = identity()
        report['cgroup_incarnation_before'] = before
        lease = json.loads(args.binfmt_evidence.read_text())
        entry = (Path('/proc/sys/fs/binfmt_misc')/lease['name']).read_text()
        if (entry != lease['entry'] or os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != lease['name']
                or digest(args.qemu) != lease['qemu_sha256']
                or 'interpreter '+str(args.qemu.resolve()) not in entry or 'flags: F' not in entry):
            raise RuntimeError('verified active temporary QEMU lease required')
        report['binfmt_lease'] = {'name': lease['name'], 'entry': entry,
            'provided_path': str(args.binfmt_evidence), 'provided_sha256': digest(args.binfmt_evidence)}
        sdk_before = sdk_inventory(sdk)
        inventory = out/'sdk-static-before.json';inventory.write_text(json.dumps(sdk_before, indent=2)+'\n')
        report['sdk_static_inventory'] = {'path': str(inventory), 'sha256': digest(inventory), 'files': len(sdk_before)}
        checked(['mount', '--make-rprivate', '/'])
        bind(sdk, root, recursive=True)
        report['owned_source_mount'] = bind_frozen_work(root, source, sdk, out, checked)
        mounts.append(root/'work')
        temporary.mkdir();inputs.mkdir()
        bind(temporary, root/'tmp')
        home = out/'owned-home';home.mkdir();bind(home, root/'root')
        for name in ('usr', 'opt', 'etc', 'var/lib/dpkg'):
            bind(sdk/name, root/name, readonly=True)
        for name in ('proc', 'sys', 'dev'):
            bind(Path('/')/name, root/name, recursive=True)
        profile = inputs/'participant.xml';shutil.copy2(args.dds_participant_profile, profile)
        report['profile'] = {'source': str(args.dds_participant_profile.resolve()),
            'snapshot': str(profile), 'inside': '/tmp/dds-profile-run/participant.xml', 'sha256': digest(profile)}
        archives = out/'driver-inputs';archives.mkdir()
        report['driver_inputs'] = {}
        for name in ('run_dds_discovery.py', 'dds_profile_check.cc', 'dds_participant_100ms.xml',
                     'runtime.py', 'run_resources.py', 'rebuild_affected.py'):
            path = Path(__file__).with_name(name);destination = archives/name
            shutil.copy2(path, destination)
            report['driver_inputs'][name] = {'archive': str(destination), 'sha256': digest(destination)}
        report['original_cpp'] = {'path': str(source/'test/0073.ros-tests/support/source/ros2/native_probe.cc'),
                                 'sha256': digest(source/'test/0073.ros-tests/support/source/ros2/native_probe.cc')}
        if args.mode == 'prepare':
            cpp = inputs/'dds_profile_check.cc';shutil.copy2(Path(__file__).with_name(cpp.name), cpp)
            evidence.target('compiler', rooted(sdk, '/usr/bin/aarch64-linux-gnu-g++-13'))
            frontend = next(rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus')
                            for base in ('usr/lib', 'usr/libexec')
                            if rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus').is_file())
            evidence.target('frontend', frontend)
            argv = ['/usr/bin/aarch64-linux-gnu-g++-13', '-std=c++17', '-O1', '-g0', '-MMD',
                '-MF', '/tmp/dds-profile-run/profile-tool.d', '-I/opt/ros/jazzy/include/fastrtps',
                '-I/opt/ros/jazzy/include/fastcdr', '/tmp/dds-profile-run/dds_profile_check.cc',
                '-L/opt/ros/jazzy/lib', '-Wl,-rpath,/opt/ros/jazzy/lib', '-lfastrtps', '-lfastcdr',
                '-pthread', '-o', '/tmp/dds-profile-run/profile-tool']
            run('profile-tool-build', argv, ['compiler', 'frontend'], 180)
            report['profile_tool'] = evidence.target('profile-tool', inputs/'profile-tool')
            run('profile-tool-validate', ['/tmp/dds-profile-run/profile-tool', report['profile']['inside']], ['profile-tool'], 30)
            lines = (out/'profile-tool-validate.log').read_text().splitlines()
            parsed = json.loads(next(line for line in reversed(lines) if line.startswith('{')))
            if parsed['status'] != 'PASS' or not parsed['sdk_parser_accepted'] or not parsed['equal_outside_discovery_announcements']:
                raise RuntimeError('SDK parser did not confirm discovery-only profile')
            report['sdk_parser_result'] = parsed
            report['scope'] = 'CPP17 ARM SDK XML parser validation only; original native suite not executed'
        else:
            prepared_path = args.profile_prepared.resolve(strict=True)
            prepared = json.loads(prepared_path.read_text())
            if (prepared['status'] != 'PASS' or prepared['mode'] != 'prepare'
                    or prepared['profile']['sha256'] != digest(profile)
                    or prepared['original_cpp']['sha256'] != report['original_cpp']['sha256']
                    or digest(prepared['sdk_static_inventory']['path']) != prepared['sdk_static_inventory']['sha256']
                    or json.loads(Path(prepared['sdk_static_inventory']['path']).read_text()) != sdk_before):
                raise RuntimeError('prepared SDK parser/profile/source binding differs')
            report['profile_prepared'] = {'path': str(prepared_path), 'sha256': digest(prepared_path)}
            qualified = json.loads(args.qualified_report.read_text())
            if qualified['status'] != 'PASS':
                raise RuntimeError('qualified ARM native targets required')
            selected = {name: qualified['targets'][name] for name in ('uwvm-ros2-native-probe', 'uwvm-ros2-gateway')}
            for name, row in selected.items():
                if require_arm(row['path'])['sha256'] != row['sha256']:
                    raise RuntimeError('qualified input binary changed: '+name)
                destination = inputs/name;shutil.copy2(row['path'], destination)
                evidence.target(name, destination)
            report['selected_artifacts'] = selected
            report['qualification'] = {'path': str(args.qualified_report), 'sha256': digest(args.qualified_report)}
            report['old_default_failures'] = []
            for previous in args.old_default_reports:
                arch = json.loads(previous.read_text())
                raw = Path(arch['raw_native_report']);old = json.loads(raw.read_text())
                if not any(row['test'] == 'ros2-native' and row['status'] == 'FAIL' for row in old['results']):
                    raise RuntimeError('declared old default ROS2-native failure is absent')
                log = raw.parent/'ros2-native.log'
                if 'FAIL actual event flood subscriber discovery' not in log.read_text():
                    raise RuntimeError('old default failure was not the discovery assertion')
                native_shas = {row['sha256'] for label, row in arch['arm_targets'].items() if label.endswith('uwvm-ros2-native-probe')}
                if selected['uwvm-ros2-native-probe']['sha256'] not in native_shas:
                    raise RuntimeError('explicit-profile native binary differs from old default failure binary')
                report['old_default_failures'].append({'architecture': str(previous), 'architecture_sha256': digest(previous),
                    'report': str(raw), 'report_sha256': digest(raw), 'log': str(log), 'log_sha256': digest(log), 'status': 'FAIL'})
            env['FASTRTPS_DEFAULT_PROFILES_FILE'] = report['profile']['inside']
            report['actual_environment'] = {key: value for key, value in env.items()
                if key.startswith(('ROS_', 'RMW_', 'FAST', 'DDS', 'CYCLONE'))}
            report['cpp_runtime_environment'] = {'ROS_DOMAIN_ID': '72', 'RMW_IMPLEMENTATION': 'rmw_fastrtps_cpp',
                'ROS_SECURITY_ENABLE': 'false', 'source': 'unchanged original native main sets these values'}
            run('ros2-native-explicit-discovery-profile', ['/tmp/dds-profile-run/uwvm-ros2-native-probe',
                '/tmp/dds-profile-run/uwvm-ros2-gateway'], list(selected), 45)
            report['behavior_verdict_scope'] = 'PASS only under the explicit100ms participant discovery profile; '
            report['behavior_verdict_scope'] += 'old default discovery FAIL reports retain FAIL'
        status = 'PASS'
    except BaseException:
        report['error'] = traceback.format_exc()
    finally:
        stop()
        try:
            if initial is not None:
                report['source_preserved'] = source_state(source, frozen) == initial and digest(manifest) == report['source_manifest_sha256']
                if not report['source_preserved']:status = 'FAIL'
            if sdk_before is not None:
                report['sdk_static_preserved'] = sdk_inventory(sdk) == sdk_before
                if not report['sdk_static_preserved']:status = 'FAIL'
        except (OSError, RuntimeError):
            report['preservation_error'] = traceback.format_exc();status = 'FAIL'
        if evidence is not None:
            result = evidence.finish(status)
            status = result['status']
            report['execution_evidence'] = str(out/'execution/architecture-evidence.json')
            report['ancestor_memory_event_deltas'] = result['cgroup_ancestor_event_deltas']
            report['cgroup_incarnation_after'] = identity()
            report['same_cgroup_incarnation'] = before == report['cgroup_incarnation_after']
            if not report['same_cgroup_incarnation']:status = 'FAIL'
        report['mount_cleanup'] = []
        for path in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
            report['mount_cleanup'].append({'path': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
            if result.returncode:status = 'FAIL'
        report['remaining_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
        if report['remaining_mounts']:status = 'FAIL'
        report['status'] = status;save()
    print(report['status'], out/'report.json', flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
