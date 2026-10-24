#!/usr/bin/env python3
"""Replay original Gateway compiler preprocessing to record system headers.

This produces dependencies only. The original build report and binary retain
their original verdict and provenance; this report never claims new codegen.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import posixpath
import shlex
import shutil
import signal
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm
from rebuild_affected import rooted, sdk_inventory, source_state
from run_resources import bind_frozen_work


def preprocessing_command(original):
    """Keep original language/defines/includes/options; remove codegen outputs."""
    argv = list(original)
    if not argv or argv[0] != '/usr/bin/aarch64-linux-gnu-g++-13':
        raise RuntimeError('original native GCC13 command required')
    retained, removed, index = [argv[0]], [], 1
    paired = {'-o', '-MF', '-MT', '-MQ'}
    modes = {'-c', '-M', '-MM', '-MD', '-MMD', '-MP', '-MG'}
    while index < len(argv):
        token = argv[index]
        if token in paired:
            if index+1 >= len(argv):
                raise RuntimeError('incomplete original compiler output option: '+token)
            removed.extend(argv[index:index+2])
            index += 2
        elif token in modes:
            removed.append(token)
            index += 1
        elif token.startswith(('-MF', '-MT', '-MQ')) or token.startswith('-o'):
            removed.append(token)
            index += 1
        else:
            retained.append(token)
            index += 1
    if retained.count('/work/src/ros/gateway/ros2/main.cc') != 1:
        raise RuntimeError('exact original Gateway translation unit required')
    return [retained[0], '-M', *retained[1:]], retained, removed


def dep_tokens(path):
    text = Path(path).read_text().replace('\\\n', ' ')
    return [posixpath.normpath(token) for token in shlex.split(text.split(':', 1)[1])]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'sdk-workspace', 'build-report', 'out', 'qemu', 'binfmt-evidence'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--manifest', type=Path)
    parser.add_argument('--timeout', type=int, default=30, help='Bounded actual GCC preprocessing timeout')
    args = parser.parse_args()
    if not 1 <= args.timeout <= 120:
        parser.error('--timeout must be 1..120 seconds')
    source = args.source.resolve(strict=True)
    sdk_workspace = args.sdk_workspace.resolve(strict=True)
    sdk = sdk_workspace/'gateway/jazzy-rootfs'
    origin_path = args.build_report.resolve(strict=True)
    origin = json.loads(origin_path.read_text())
    manifest = (args.manifest or source.parent/'source-manifest.json').resolve(strict=True)
    out = args.out.resolve()
    out.mkdir(parents=True, mode=0o700, exist_ok=False)
    root = out/'rootfs'
    mounts, evidence, active = [], None, None
    initial, frozen, sdk_before, incarnation_before = None, None, None, None
    status = 'FAIL'
    start = time.monotonic()
    output = out/'report.json'
    report = {'status': 'RUNNING', 'scope': 'Original-flags GCC -M preprocessing replay; '
        'not original compilation MMD, not new codegen or a replacement original build verdict',
        'input_build_report': str(origin_path), 'input_build_report_sha256': digest(origin_path),
        'input_build_report_status': origin['status'], 'input_build_report_error': origin.get('error'),
        'source': str(source), 'source_manifest_sha256': digest(manifest),
        'parallelism': 1, 'timeout_seconds': args.timeout, 'dependencies': {}}

    def save():
        report['seconds'] = round(time.monotonic()-start, 6)
        output.write_text(json.dumps(report, indent=2)+'\n')

    def checked(command):
        subprocess.run([str(token) for token in command], check=True, timeout=30,
                       stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def bind(original, destination, readonly=False, recursive=False):
        destination.mkdir(parents=True, exist_ok=True)
        checked(['mount', '--rbind' if recursive else '--bind', original, destination])
        mounts.append(destination)
        checked(['mount', '--make-rprivate', destination])
        if readonly:
            checked(['mount', '-o', 'remount,bind,ro', destination])

    def identity():
        facts = evidence.leaf.stat()
        return {'path': str(evidence.leaf), 'device': facts.st_dev, 'inode': facts.st_ino,
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
        raise RuntimeError('dependency replay interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    protected = {}
    try:
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('dependency replay requires unshare -m')
        if origin['source'] != str(source) or origin['source_manifest_sha256'] != digest(manifest):
            raise RuntimeError('original build and replay source manifest differ')
        if not (sdk/'.uwvm-task-owned').is_file():
            raise RuntimeError('task-owned ARM SDK required')
        for name in ('source_preserved', 'sdk_static_preserved', 'same_cgroup_incarnation'):
            if origin.get(name) is not True:
                raise RuntimeError('original build input preservation not proven: '+name)
        if not origin['builds'] or any(row['status'] != 'PASS' or row['returncode'] != 0 for row in origin['builds']):
            raise RuntimeError('original actual compiler commands did not complete successfully')
        binary = origin['targets']['uwvm-ros2-gateway']
        actual = require_arm(binary['path'])
        if actual != binary:
            raise RuntimeError('original ARM Gateway bytes or inode changed')
        report['original_binary'] = actual
        protected[str(origin_path)] = digest(origin_path)
        protected[binary['path']] = binary['sha256']
        for row in origin['build_metadata'].values():
            if digest(row['path']) != row['sha256']:
                raise RuntimeError('original compiler metadata changed')
            protected[row['path']] = row['sha256']
        metadata = origin['build_metadata']
        commands_path = Path(metadata['ros2:compile_commands.json']['path'])
        commands = json.loads(commands_path.read_text())
        selected = [row for row in commands if row['file'] == '/work/src/ros/gateway/ros2/main.cc']
        if len(selected) != 1:
            raise RuntimeError('exactly one original Gateway compile command required')
        original = selected[0].get('arguments') or shlex.split(selected[0]['command'])
        replay, retained, removed = preprocessing_command(original)
        original_dep = Path(metadata['ros2:CMakeFiles/uwvm-ros2-gateway.dir/main.cc.o.d']['path'])
        original_project = {token[len('/work/'):] for token in dep_tokens(original_dep) if token.startswith('/work/')}
        if not {'src/ros/gateway/ros2/main.cc', 'src/ros/gateway/ros2/action.h'}.issubset(original_project):
            raise RuntimeError('original MMD lacks required actual project input')
        frozen = json.loads(manifest.read_text())
        initial = source_state(source, frozen)
        spec = importlib.util.spec_from_file_location('replay_frozen_probe', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
        probe = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution', probe.probe, interval=.002)
        incarnation_before = identity()
        report['cgroup_incarnation_before'] = incarnation_before
        report['actualexecution_cgroup'] = {'before': evidence.before}
        report['emulator'] = evidence.emulator(args.qemu.resolve(strict=True))
        lease = json.loads(args.binfmt_evidence.read_text())
        active_entry = (Path('/proc/sys/fs/binfmt_misc')/lease['name']).read_text()
        if (active_entry != lease['entry'] or os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != lease['name']
                or lease['qemu_sha256'] != digest(args.qemu)
                or 'interpreter '+str(args.qemu.resolve()) not in active_entry or 'flags: F' not in active_entry):
            raise RuntimeError('verified active temporary binfmt lease required')
        report['binfmt'] = {'name': lease['name'], 'entry': active_entry,
                            'path': str(args.binfmt_evidence), 'sha256': digest(args.binfmt_evidence)}
        sdk_before = sdk_inventory(sdk)
        original_sdk_path = Path(origin['sdk_static_inputs']['path'])
        if digest(original_sdk_path) != origin['sdk_static_inputs']['sha256'] or json.loads(original_sdk_path.read_text()) != sdk_before:
            raise RuntimeError('current SDK static inputs differ from original actual compilation SDK')
        inventory = out/'sdk-static-inputs-before.json'
        inventory.write_text(json.dumps(sdk_before, indent=2)+'\n')
        report['sdk_static_inputs'] = {'path': str(inventory), 'sha256': digest(inventory), 'files': len(sdk_before)}
        protected[str(original_sdk_path)] = digest(original_sdk_path)
        archives = out/'driver-inputs';archives.mkdir()
        report['driver_inputs'] = {}
        for name in ('replay_gateway_dependencies.py', 'rebuild_affected.py', 'run_resources.py', 'runtime.py'):
            original_driver = Path(__file__).with_name(name)
            destination = archives/name
            shutil.copy2(original_driver, destination)
            report['driver_inputs'][name] = {'archive': str(destination), 'sha256': digest(destination)}
        report['original_compile_command'] = original
        report['original_working_directory'] = selected[0]['directory']
        report['replay_compile_command'] = replay
        report['retained_original_flags_and_source'] = retained
        report['removed_codegen_and_dependency_output_options'] = removed
        report['compiler_options_preserved'] = replay[:1]+replay[2:] == retained
        if not report['compiler_options_preserved']:
            raise RuntimeError('original defines/includes/language/options changed')
        checked(['mount', '--make-rprivate', '/'])
        bind(sdk, root, recursive=True)
        report['owned_source_mount'] = bind_frozen_work(root, source, sdk, out, checked)
        mounts.append(root/'work')
        temporary = out/'owned-tmp';temporary.mkdir();bind(temporary, root/'tmp')
        home = out/'owned-home';home.mkdir();bind(home, root/'root')
        for name in ('usr', 'opt', 'etc', 'var/lib/dpkg'):
            bind(sdk/name, root/name, readonly=True)
        for name in ('proc', 'sys', 'dev'):
            bind(Path('/')/name, root/name, recursive=True)
        cwd = selected[0]['directory']
        if not cwd.startswith('/tmp/') or '..' in Path(cwd).parts:
            raise RuntimeError('original working directory is outside task-owned temporary tree')
        (temporary/Path(cwd).relative_to('/tmp')).mkdir(parents=True)
        compiler = rooted(sdk, original[0])
        frontend = next((rooted(sdk, '/'+prefix+'/gcc/aarch64-linux-gnu/13/cc1plus')
                         for prefix in ('usr/lib', 'usr/libexec')
                         if rooted(sdk, '/'+prefix+'/gcc/aarch64-linux-gnu/13/cc1plus').is_file()), None)
        if frontend is None:
            raise RuntimeError('actual ARM GCC13 frontend unavailable')
        evidence.target('ros2-compiler', compiler)
        evidence.target('ros2-frontend', frontend)
        command = ['/usr/bin/chroot', str(root), '/bin/bash', '-lc',
            'source /opt/ros/jazzy/setup.bash && cd -- "$1" && shift && exec "$@"',
            'uwvm-gateway-dependency-replay', cwd, *replay]
        environment = dict(os.environ)
        for key in ('CC', 'CXX', 'LD_LIBRARY_PATH', 'LD_PRELOAD', 'QEMU_LD_PREFIX', 'CFLAGS', 'CXXFLAGS', 'LDFLAGS'):
            environment.pop(key, None)
        environment.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin', PYTHONDONTWRITEBYTECODE='1')
        report['command'] = command
        raw = out/'gateway-full-system.d'
        save()
        begun = time.monotonic()
        with raw.open('wb') as stdout, (out/'replay.log').open('wb') as stderr:
            active = subprocess.Popen(command, stdout=stdout, stderr=stderr, env=environment, start_new_session=True)
        report['pid'] = active.pid
        evidence.start(active, 'dependency-replay/', command)
        report['returncode'] = active.wait(timeout=args.timeout)
        report['preprocessing_seconds'] = round(time.monotonic()-begun, 6)
        if report['returncode']:
            raise RuntimeError('actual native GCC dependency preprocessing failed')
        evidence.check(['ros2-compiler', 'ros2-frontend'], 'dependency-replay/')
        observed = {label for sample in evidence.samples.values()
                    if sample['name'] == 'dependency-replay/' and sample['ancestry'][0] == active.pid
                    for label in sample['mapped_arm_targets']}
        if {'ros2-compiler', 'ros2-frontend'}-observed:
            raise RuntimeError('missing actual ARM GCC13/frontend mapping in replay PID subtree')
        project, headers = {}, {}
        for token in dep_tokens(raw):
            if token.startswith('/work/'):
                name = token[len('/work/'):]
                if name not in initial or digest(source/name) != initial[name]:
                    raise RuntimeError('replayed compiler project input outside frozen source: '+name)
                project[name] = initial[name]
            elif token.startswith(('/usr/', '/opt/', '/etc/')):
                actual_header = rooted(sdk, token)
                name = str(actual_header.relative_to(sdk))
                value = digest(actual_header)
                if sdk_before.get(name, {}).get('sha256') != value:
                    raise RuntimeError('actual replay SDK header differs from frozen compilation SDK: '+token)
                headers[token] = {'sha256': value, 'resolved_sdk_path': name, 'bytes': actual_header.stat().st_size}
            else:
                raise RuntimeError('unaccounted actual replay dependency: '+token)
        report['project_set_matches_original_MMD'] = set(project) == original_project
        if not report['project_set_matches_original_MMD'] or not headers:
            raise RuntimeError('replayed project inputs differ from original MMD or SDK header closure empty')
        report['dependencies']['uwvm-ros2-gateway'] = {'path': str(raw), 'sha256': digest(raw),
            'source_dependencies': project, 'sdk_header_dependencies': headers,
            'original_MMD_path': str(original_dep), 'original_MMD_sha256': digest(original_dep)}
        report['current_action_header_sha256'] = project['src/ros/gateway/ros2/action.h']
        report['no_object_outputs'] = not list(temporary.rglob('*.o')) and not list(temporary.rglob('uwvm-ros2-*'))
        if not report['no_object_outputs']:
            raise RuntimeError('dependency-only replay created a codegen artifact')
        status = 'PASS'
    except BaseException:
        report['error'] = traceback.format_exc()
    finally:
        stop()
        try:
            if initial is not None:
                report['source_preserved'] = source_state(source, frozen) == initial and digest(manifest) == report['source_manifest_sha256']
                if not report['source_preserved']:
                    status = 'FAIL'
            if sdk_before is not None:
                report['sdk_static_preserved'] = sdk_inventory(sdk) == sdk_before
                if not report['sdk_static_preserved']:
                    status = 'FAIL'
            report['original_report_binary_metadata_preserved'] = all(digest(path) == value for path, value in protected.items())
            if not report['original_report_binary_metadata_preserved']:
                status = 'FAIL'
        except (OSError, RuntimeError):
            report['preservation_error'] = traceback.format_exc()
            status = 'FAIL'
        if evidence is not None:
            result = evidence.finish(status)
            status = result['status']
            report['execution_evidence'] = str(out/'execution/architecture-evidence.json')
            report['actualexecution_cgroup']['after'] = result['cgroup_after']
            report['ancestor_memory_event_deltas'] = result['cgroup_ancestor_event_deltas']
            report['cgroup_incarnation_after'] = identity()
            report['same_cgroup_incarnation'] = report['cgroup_incarnation_after'] == incarnation_before
            if not report['same_cgroup_incarnation']:
                status = 'FAIL'
        cleanup = []
        for path in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
            cleanup.append({'mount': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
            if result.returncode:
                status = 'FAIL'
        report['mount_cleanup'] = cleanup
        report['post_cleanup_mountinfo'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
        if report['post_cleanup_mountinfo']:
            status = 'FAIL'
        report['status'] = status
        save()
    print(report['status'], output, flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
