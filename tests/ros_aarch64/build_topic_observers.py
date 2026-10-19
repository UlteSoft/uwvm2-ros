#!/usr/bin/env python3
"""Build test-owned C++17 topic observers in the real ARM Noetic/Jazzy SDKs.

Uses a provided verified temporary binfmt lease, never creates a handler.
No DDS behavior runs during preparation; SDK origins and frozen inputs stay
unchanged. Native compiler/frontend maps and raw MMD inputs are recorded.
"""
import argparse
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
from run_resources import bind_frozen_work, inventory, save


def cmake(version):
    name = f'uwvm-ros{version}-topic-observer'
    rows = ['cmake_minimum_required(VERSION 3.16)', 'project(arm_topic_observer LANGUAGES CXX)',
            'set(CMAKE_CXX_STANDARD 17)', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)']
    rows += (['find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)'] if version == 1 else
             ['find_package(ament_cmake REQUIRED)', 'find_package(rclcpp REQUIRED)', 'find_package(std_msgs REQUIRED)'])
    rows += [f'add_executable({name} /tmp/addon/topic_observer_ros{version}.cc)',
             f'target_compile_options({name} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD -MF /tmp/observer.d)']
    rows += ([f'target_include_directories({name} PRIVATE ${{catkin_INCLUDE_DIRS}})',
              f'target_link_libraries({name} PRIVATE ${{catkin_LIBRARIES}} pthread)'] if version == 1 else
             [f'ament_target_dependencies({name} rclcpp std_msgs)', f'target_link_libraries({name} pthread)'])
    return '\n'.join(rows)+'\n'


def rooted(sdk, guest):
    """Resolve guest absolute symlinks within its SDK, never against the host."""
    sdk = Path(sdk).resolve()
    pending = guest.split('/')
    parts = []
    links = 0
    while pending:
        item = pending.pop(0)
        if item in ('', '.'):
            continue
        if item == '..':
            if parts: parts.pop()
            continue
        candidate = sdk.joinpath(*parts, item)
        if candidate.is_symlink():
            links += 1
            if links > 40:
                raise RuntimeError('FAIL SDK compiler input has a symlink loop')
            target = os.readlink(candidate)
            if target.startswith('/'): parts = []
            pending = target.split('/')+pending
        else:
            parts.append(item)
    return sdk.joinpath(*parts)


def inputs(mmd, sdk, temp):
    sdk, temp = Path(sdk).resolve(), Path(temp).resolve()
    # Split after the Make target, then normalize every actual dependency.
    value = Path(mmd).read_text().replace('\\\n', ' ')
    if ':' not in value:
        raise RuntimeError('FAIL invalid Make compiler dependency rule')
    values = {}
    for token in shlex.split(value.split(':', 1)[1]):
        normalized = posixpath.normpath(token)
        if normalized.startswith('/tmp/'):
            selected = (temp/normalized[len('/tmp/'):]).resolve()
            if not selected.is_relative_to(temp):
                raise RuntimeError('FAIL compiler addon dependency escapes owned temp')
        elif normalized.startswith(('/usr/', '/opt/')):
            selected = rooted(sdk, normalized)
            if not selected.is_relative_to(sdk):
                raise RuntimeError('FAIL compiler SDK dependency escapes its ARM root')
        else:
            raise RuntimeError('FAIL unexpected native compiler input: '+token)
        if not selected.is_file():
            raise RuntimeError('FAIL missing native compiler input: '+str(selected))
        values[str(selected)] = {'logical_path': normalized, 'path': str(selected), 'sha256': digest(selected)}
    if not values:
        raise RuntimeError('FAIL compiler has no recorded inputs')
    return list(values.values())


def reuse_observer(prepared_path, version, source, addon, sdk):
    """Reuse an unchanged observer with its original actual compilation proof."""
    prepared_path = Path(prepared_path).resolve()
    original = json.loads(prepared_path.read_text())
    if original.get('status') != 'PREPARED' or set(original.get('observers', {})) != {'1', '2'}:
        raise RuntimeError('FAIL reuse needs a qualified prepared two-observer manifest')
    binding = original['preparation_report']
    path = Path(binding['path'])
    report = json.loads(path.read_text())
    if digest(path) != binding['sha256'] or report.get('status') != 'PASS' or report.get('fatal_errors') or report.get('remaining_owned_processes') or report.get('remaining_owned_mounts') or not report.get('source_preserved'):
        raise RuntimeError('FAIL reused observer original preparation is not qualified')
    row = json.loads(json.dumps(original['observers'][str(version)]))
    binary = Path(row['path']).resolve()
    temp = binary.parent.parent
    if binary != prepared_path.parent/f'ros{version}/native-temp/build/uwvm-ros{version}-topic-observer':
        raise RuntimeError('FAIL reused observer binary is outside original owned preparation')
    if require_arm(binary)['sha256'] != row['sha256'] or digest(source/f'tests/ros{version}/native_probe.cc') != row['original_observer_source_sha256'] or digest(addon/f'topic_observer_ros{version}.cc') != row['addon_source_sha256']:
        raise RuntimeError('FAIL reused observer exact source/ELF binding changed')
    for key in ('compiler', 'native_frontend'):
        item = row[key]
        if not Path(item['path']).resolve().is_relative_to(sdk) or require_arm(item['path'])['sha256'] != item['sha256']:
            raise RuntimeError('FAIL reused observer native compiler SDK changed')
    for key in ('flags', 'compile_commands', 'generated_cmake', 'link'):
        if digest(row[key+'_path']) != row[key+'_sha256']:
            raise RuntimeError('FAIL reused observer compiler metadata changed: '+key)
    flags = Path(row['flags_path']).read_text()
    commands = json.loads(Path(row['compile_commands_path']).read_text())
    command = next(x['command'] for x in commands if x['file'] == f'/tmp/addon/topic_observer_ros{version}.cc')
    if any(option not in flags or option not in command for option in ('-std=gnu++17', '-MD', '-MF /tmp/observer.d')):
        raise RuntimeError('FAIL reused observer original CPP17/MD flags missing')
    raw = Path(row['raw_MMD_path'])
    if digest(raw) != row['raw_MMD_sha256'] or digest(row['actual_compile_MMD_origin']) != row['raw_MMD_sha256'] or inputs(raw, sdk, temp) != row['compiled_inputs']:
        raise RuntimeError('FAIL reused observer actual MMD/source/SDK closure changed')
    for key in ('sha256', 'addon_source_sha256', 'compiler', 'native_frontend', 'compiled_inputs', 'raw_MMD_sha256'):
        if report['observers'][str(version)].get(key) != row[key]:
            raise RuntimeError('FAIL reused manifest differs from original qualified report')
    build = next(x for x in report['commands'] if x['name'] == f'ros{version}-build')
    expected = report['cgroup_before']['membership']
    positives = [x for x in report['actual_process_samples'] if x['name'] == build['name'] and x['ancestry'][0] == build['pid'] and f'ros{version}:native-cxx-frontend' in x['mapped_arm_targets'] and f'/tmp/addon/topic_observer_ros{version}.cc' in x['command']]
    if build['status'] != 'PASS' or build['returncode'] != 0 or not positives or report['cgroup_before']['effective_memory_max'] != 68719476736:
        raise RuntimeError('FAIL reused observer lacks actual original ARM source compilation proof')
    if any(not x['live_cgroup_procs'] or x['membership'] != expected or x['kernel_exe_elf']['e_machine'] != 62 or x['kernel_exe_elf']['sha256'] not in report['expected_host_emulator_sha256'] for x in positives):
        raise RuntimeError('FAIL reused observer original compiler QEMU/cgroup proof invalid')
    row['reuse_prepared_binding'] = {'manifest_path': str(prepared_path), 'manifest_sha256': digest(prepared_path),
        'preparation_report': binding,
        'scope': 'Original actual native compilation reused unchanged; no new compiler execution for this observer'}
    return row


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for key in ('source', 'ros1-rootfs', 'ros2-rootfs', 'out', 'qemu', 'binfmt-evidence'):
        p.add_argument('--'+key, type=Path, required=True)
    p.add_argument('--addon', type=Path, default=Path(__file__).parent)
    p.add_argument('--versions', nargs='+', type=int, choices=(1, 2), default=[1, 2],
                   help='Versions to actually compile; omitted versions need --reuse-prepared')
    p.add_argument('--reuse-prepared', type=Path,
                   help='Read-only qualified two-observer manifest for unchanged omitted versions')
    args = p.parse_args()
    if len(set(args.versions)) != len(args.versions):
        p.error('--versions must be unique')
    if set(args.versions) != {1, 2} and args.reuse_prepared is None:
        p.error('omitted observer versions require --reuse-prepared')
    if os.geteuid() != 0 or sys.platform != 'linux':
        raise RuntimeError('BLOCKED root Linux mount/chroot environment required')
    # Enter a private mount namespace once, before touching any mountpoint.
    if os.environ.get('UWVM_ARM_TOPIC_BUILD_NS') != '1':
        command = ['unshare', '--mount', '--propagation', 'private', '--', sys.executable, __file__, *sys.argv[1:]]
        return subprocess.run(command, env={**os.environ, 'UWVM_ARM_TOPIC_BUILD_NS': '1'}).returncode
    source, addon = args.source.resolve(), args.addon.resolve()
    lease = json.loads(args.binfmt_evidence.read_text())
    nested = lease.get('entry')
    name = lease.get('entry_name') or lease.get('name') or (nested.get('name') if isinstance(nested, dict) else None)
    if not isinstance(name, str) or '/' in name or name in ('', '.', '..'):
        raise RuntimeError('BLOCKED valid provided binfmt entry required')
    handler = (Path('/proc/sys/fs/binfmt_misc')/name).read_text()
    fields = dict(line.split(None, 1) for line in handler.splitlines()[1:] if ' ' in line)
    magic, mask = bytes.fromhex(fields.get('magic', '')), bytes.fromhex(fields.get('mask', ''))
    if not handler.startswith('enabled\n') or 'F' not in fields.get('flags:', '') or fields.get('interpreter') != str(args.qemu.resolve()) or len(magic) < 20 or len(mask) < 20 or magic[18:20] != b'\xb7\x00' or mask[18:20] != b'\xff\xff':
        raise RuntimeError('BLOCKED provided lease is not fixed QEMU for ELF183')
    if lease.get('qemu_sha256') != digest(args.qemu):
        raise RuntimeError('BLOCKED provided lease QEMU SHA mismatch')
    sys.path.insert(0, str(source/'tests/ros_cgroup'))
    from probe import probe
    evidence = Evidence(args.out.resolve(), probe)
    evidence.emulator(args.qemu)
    source_before = inventory(source)
    report = {'commands': [], 'mounts': [], 'observers': {}, 'binfmt_entry': name,
              'binfmt_evidence_sha256': digest(args.binfmt_evidence),
              'executed_driver_sha256': digest(__file__), 'runtime_sha256': digest(Path(__file__).with_name('runtime.py')),
              'scope': 'Actual native ARM C++17 observer preparation and explicitly bound unchanged reuse; no DDS/ROS behavior executed',
              'actually_compiled_versions': args.versions, 'reused_versions': []}
    children, mounts = [], []
    manifest = {'status': 'PREPARING', 'observers': {}}
    status, error = 'FAIL', None

    def checked(argv):
        subprocess.run(list(map(str, argv)), check=True, capture_output=True, timeout=30)

    def run(label, argv, required=(), timeout=300):
        path = evidence.out/(label+'.log')
        with path.open('xb') as stream:
            process = subprocess.Popen(argv, stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
        children.append(process)
        row = {'name': label, 'command': argv, 'pid': process.pid, 'status': 'FAIL', 'log': str(path)}
        report['commands'].append(row)
        evidence.start(process, label, argv)
        end = time.monotonic()+timeout
        while process.poll() is None:
            evidence.observe(process.pid, label)
            if time.monotonic() >= end:
                raise RuntimeError('FAIL observer build outer command deadline: '+label)
            time.sleep(.01)
        row.update(returncode=process.returncode, log_sha256=digest(path))
        if process.returncode:
            raise RuntimeError('FAIL observer build command: '+label)
        if required:
            evidence.check(required, label)
        row['status'] = 'PASS'
        save(evidence.out/'progress.json', report)

    def interrupted(signum, frame):
        raise RuntimeError('FAIL observer build interrupted signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted); signal.signal(signal.SIGINT, interrupted)
    try:
        for version in sorted({1, 2}-set(args.versions)):
            sdk = getattr(args, 'ros'+str(version)+'_rootfs').resolve()
            row = reuse_observer(args.reuse_prepared, version, source, addon, sdk)
            manifest['observers'][str(version)] = row
            report['observers'][str(version)] = row
            report['reused_versions'].append(version)
        for version in (1, 2):
            if version not in args.versions:
                continue
            sdk = getattr(args, 'ros'+str(version)+'_rootfs').resolve()
            owned = evidence.out/f'ros{version}'; owned.mkdir()
            root, temp = owned/'rootfs', owned/'native-temp'; root.mkdir(); temp.mkdir()
            copied = temp/'addon'; copied.mkdir()
            src = addon/f'topic_observer_ros{version}.cc'
            source_sha = digest(src)
            shutil.copy2(src, copied/src.name)
            if digest(copied/src.name) != source_sha:
                raise RuntimeError('FAIL observer addon changed while copying')
            cm = temp/'cmake'; cm.mkdir(); (cm/'CMakeLists.txt').write_text(cmake(version))
            checked(['mount', '--rbind', sdk, root]); mounts.append(root)
            checked(['mount', '--make-rprivate', root])
            report['mounts'].append(bind_frozen_work(root, source, sdk, owned, checked))
            checked(['mount', '--bind', temp, root/'tmp'])
            checked(['mount', '--make-rprivate', root])
            gcc_version = '9' if version == 1 else '13'
            frontends = [sdk/parent/'aarch64-linux-gnu'/gcc_version/'cc1plus'
                         for parent in ('usr/lib/gcc', 'usr/libexec/gcc')
                         if (sdk/parent/'aarch64-linux-gnu'/gcc_version/'cc1plus').is_file()]
            if len(frontends) != 1:
                raise RuntimeError('BLOCKED exact ARM GCC'+gcc_version+' C++ frontend is missing/ambiguous')
            frontend = frontends[0]
            compiler = sdk/'usr/bin'/('aarch64-linux-gnu-g++-'+gcc_version)
            cmake_binary = sdk/'usr/bin/cmake'
            frontend_label = f'ros{version}:native-cxx-frontend'
            cmake_label = f'ros{version}:native-cmake'
            evidence.target(frontend_label, frontend); evidence.target(cmake_label, cmake_binary)
            compiler_facts = require_arm(compiler)
            distro = 'noetic' if version == 1 else 'jazzy'

            def native(argv):
                return ['chroot', str(root), '/usr/bin/env', '-u', 'LD_LIBRARY_PATH', '-u', 'LD_PRELOAD',
                        '-u', 'CC', '-u', 'CXX', 'PYTHONDONTWRITEBYTECODE=1', '/bin/bash', '-lc',
                        f'source /opt/ros/{distro}/setup.bash && exec "$@"', 'arm-topic-build', *argv]

            run(f'ros{version}-compiler-version', native(['/usr/bin/aarch64-linux-gnu-g++-'+gcc_version, '--version']))
            run(f'ros{version}-configure', native(['/usr/bin/cmake', '-S', '/tmp/cmake', '-B', '/tmp/build',
                '-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-'+gcc_version,
                '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON']), required=(cmake_label,))
            run(f'ros{version}-build', native(['/usr/bin/cmake', '--build', '/tmp/build', '--parallel', '1']),
                required=(frontend_label, cmake_label), timeout=600)
            name = f'uwvm-ros{version}-topic-observer'
            binary = temp/'build'/name
            meta = temp/'build/CMakeFiles'/(name+'.dir')
            mmds = list(meta.rglob('*.o.d'))
            if len(mmds) > 1:
                raise RuntimeError('FAIL observer raw compiler dependencies ambiguous')
            actual_mmd = mmds[0] if mmds else temp/'observer.d'
            if not actual_mmd.is_file():
                raise RuntimeError('FAIL actual observer compilation did not retain compiler dependencies')
            mmd = owned/'observer.d'; shutil.copy2(actual_mmd, mmd)
            flags = owned/'flags.make'; shutil.copy2(meta/'flags.make', flags)
            if '-std=gnu++17' not in flags.read_text():
                raise RuntimeError('FAIL observer compilation is not explicit C++17')
            if '-MD' not in flags.read_text() or '-MF /tmp/observer.d' not in flags.read_text():
                raise RuntimeError('FAIL observer real flags do not record explicit compiler MMD capture')
            copied_commands = owned/'compile_commands.json'
            shutil.copy2(temp/'build/compile_commands.json', copied_commands)
            if '-MD' not in copied_commands.read_text() or '-MF /tmp/observer.d' not in copied_commands.read_text():
                raise RuntimeError('FAIL observer actual compile command lacks compiler dependency capture')
            copied_link = owned/'link.txt'; shutil.copy2(meta/'link.txt', copied_link)
            copied_cmake = owned/'CMakeLists.txt'; shutil.copy2(cm/'CMakeLists.txt', copied_cmake)
            facts = require_arm(binary)
            row = {**facts, 'compiler': compiler_facts, 'native_frontend': require_arm(frontend),
                   'original_observer_source_sha256': digest(source/f'tests/ros{version}/native_probe.cc'),
                   'addon_source_sha256': source_sha, 'compiled_inputs': inputs(mmd, sdk, temp),
                   'raw_MMD_path': str(mmd), 'raw_MMD_sha256': digest(mmd),
                   'actual_compile_MMD_origin': str(actual_mmd),
                   'flags_path': str(flags), 'flags_sha256': digest(flags),
                   'compile_commands_path': str(copied_commands), 'compile_commands_sha256': digest(copied_commands),
                   'link_path': str(copied_link), 'link_sha256': digest(copied_link),
                   'generated_cmake_path': str(copied_cmake), 'generated_cmake_sha256': digest(copied_cmake)}
            expected_source = str((copied/src.name).resolve())
            if expected_source not in {value['path'] for value in row['compiled_inputs']}:
                raise RuntimeError('FAIL observer source absent from actual compiler MMD')
            manifest['observers'][str(version)] = row
            report['observers'][str(version)] = row
            checked(['umount', '-R', root]); mounts.remove(root)
        manifest['status'] = 'PREPARED'
        save(evidence.out/'manifest.json', manifest)
        status = 'PASS'
    except BaseException:
        error = traceback.format_exc()
    finally:
        for process in reversed(children):
            if process.poll() is None:
                os.killpg(process.pid, signal.SIGTERM)
                try: process.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    os.killpg(process.pid, signal.SIGKILL); process.wait(timeout=3)
        report['mount_cleanup'] = []
        for root in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(root)], capture_output=True, text=True, timeout=30)
            report['mount_cleanup'].append({'path': str(root), 'returncode': result.returncode,
                                            'output': result.stdout+result.stderr})
            if result.returncode: status = 'FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4].startswith(str(evidence.out)+'/')]
        report['source_preserved'] = inventory(source) == source_before
        if report['remaining_owned_mounts'] or not report['source_preserved']: status = 'FAIL'
        report['error'] = error
        result = evidence.finish(status, report)
        save(evidence.out/'report.json', result)
        # Reuse accepts only fully qualified preparation, including sampler/cleanup.
        if result['status'] != 'PASS':
            manifest['status'] = 'FAIL'; save(evidence.out/'manifest.json', manifest)
        else:
            manifest['preparation_report'] = {'path': str(evidence.out/'report.json'),
                                              'sha256': digest(evidence.out/'report.json')}
            save(evidence.out/'manifest.json', manifest)
    print(result['status'], evidence.out/'report.json', flush=True)
    return 0 if result['status'] == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
