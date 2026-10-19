#!/usr/bin/env python3
"""Prepare/run current ROS2 parameter and Action specialties on real ARM SDKs.

Preparation builds the unchanged three native probes as C++20. Execution uses
an immutable current C++17 ARM Gateway and the probes' original assertions.
No old runner, report, production source or SDK filesystem is edited.

Every invocation supplies the actual current --container-started-at. The source
root's sibling source-manifest.json must describe the complete frozen input
view. Reuse requires the immutable manifest, final report.json and matching
architecture-evidence.json from one successful preparation in this same source,
SDK, container and cgroup incarnation. PREPARED is a compilation-artifact state,
never a native behavior PASS, and a manifest left by failed cleanup is refused.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import shutil
import signal
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm, read_json_binding

PROBES = {
    'parameter-initialize': 'tests/ros_parameter_initialize/native_ros2.cc',
    'action-limits': 'tests/ros_action_limits/native_ros2.cc',
    'action-late-close': 'tests/ros_action_late_close/native_ros2.cc',
}
LIMIT_MODES = ('feedback', 'success', 'abort', 'canceled', 'feedback-edge',
               'result-edge', 'responder-feedback', 'responder-result')
LATE_MODES = ('accept', 'reject', 'accepted', 'disconnect', 'refuse', 'expired', 'expired-owner')


def save(path, value):
    Path(path).write_text(json.dumps(value, indent=2)+'\n')


def inventory(source):
    return {str(path.relative_to(source)): digest(path) for path in source.rglob('*') if path.is_file()}


def inventory_digest(value):
    """Bind a complete static SDK catalog without embedding a duplicate copy.

    The producer and consumer independently inventory every SDK input. Stream
    canonical JSON into SHA-256 so this compact companion-report field retains
    the complete catalog association without another large JSON byte buffer.
    This is input consistency, not a system-header or native behavior claim.
    """
    result = hashlib.sha256()
    for part in json.JSONEncoder(sort_keys=True, separators=(',', ':'), ensure_ascii=True).iterencode(value):
        result.update(part.encode('ascii'))
    return result.hexdigest()


def dependencies(path, source):
    values = {}
    for token in re.findall(r'/work/[^\s\\]+', Path(path).read_text().replace('\\\n', ' ')):
        normalized = posixpath.normpath(token.rstrip(':'))
        # GCC/CMake may emit an absolute Make rule target next to source
        # dependencies. A target is not an input and need not exist in /work.
        if token.endswith(':') or normalized.endswith('.o'):
            continue
        if not normalized.startswith('/work/'):
            raise RuntimeError('FAIL compiler project dependency escapes /work: '+token)
        name = normalized[len('/work/'):]
        selected = (source/name).resolve()
        if not selected.is_relative_to(source) or not selected.is_file():
            raise RuntimeError('FAIL missing frozen project dependency: '+name)
        values[name] = digest(selected)
    if not values:
        raise RuntimeError('FAIL no compiler-recorded frozen project inputs: '+str(path))
    return values


def cmake_project():
    lines = [
        'cmake_minimum_required(VERSION 3.16)',
        'project(uwvm_ros_arm_resources LANGUAGES CXX)',
        'find_package(ament_cmake REQUIRED)',
        'find_package(rclcpp REQUIRED)',
        'find_package(rclcpp_action REQUIRED)',
        'find_package(action_tutorials_interfaces REQUIRED)',
        'find_package(rcl_interfaces REQUIRED)',
    ]
    for name, path in PROBES.items():
        lines += [f'add_executable({name} /work/{path})',
                  f'target_compile_features({name} PRIVATE cxx_std_20)',
                  f'target_compile_options({name} PRIVATE -UNDEBUG -O1 -g0)',
                  f'target_include_directories({name} PRIVATE /work/src /work/third-parties/fast_io/include)',
                  f'ament_target_dependencies({name} rclcpp rclcpp_action action_tutorials_interfaces rcl_interfaces)',
                  f'target_link_libraries({name} pthread)']
    return '\n'.join(lines)+'\n'


def bind_frozen_work(root, source, sdk, owned_out, checked):
    """Replace only an inherited /work mount inside our private SDK clone."""
    root, source, sdk, owned_out = (Path(path).resolve() for path in (root, source, sdk, owned_out))
    if root != owned_out/'rootfs' or root == sdk or root/'work' == sdk/'work':
        raise RuntimeError('FAIL source mount replacement is outside the owned rootfs clone')
    target = root/'work'
    if target.is_symlink():
        raise RuntimeError('FAIL owned clone /work must be a directory, not a symlink')

    def exact(path):
        return [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
                if line.split()[4] == str(path)]

    clone = exact(root)
    if not clone or any(any(token.startswith(('shared:', 'master:', 'propagate_from:'))
                            for token in line.split()[6:line.split().index('-')]) for line in clone):
        raise RuntimeError('FAIL owned SDK rootfs clone must already be privately mounted')
    origin_before = exact(sdk/'work')
    inherited, removed = exact(target), []
    # rbind copies the caller's existing read-only /work submount. Unmounting
    # only our recursively-private clone path leaves SDK-origin/global mounts
    # intact, and avoids stacking a second mount over the inherited source.
    for _ in range(16):
        current = exact(target)
        if not current:
            break
        checked(['umount', '-R', target])
        removed.extend(current)
    else:
        raise RuntimeError('FAIL inherited source mounts did not retire from owned clone')
    checked(['mount', '--bind', source, target])
    checked(['mount', '-o', 'remount,bind,ro', target])
    visible = exact(target)
    source_stat, visible_stat = source.stat(), target.stat()
    origin_after = exact(sdk/'work')
    if len(visible) != 1 or 'ro' not in visible[0].split()[5].split(',') or (source_stat.st_dev, source_stat.st_ino) != (visible_stat.st_dev, visible_stat.st_ino):
        raise RuntimeError('FAIL visible owned source mount is not read-only exact frozen source')
    if origin_after != origin_before:
        raise RuntimeError('FAIL SDK-origin /work mount changed while replacing owned source')
    return {'owned_rootfs': str(root), 'sdk_origin': str(sdk),
            'inherited_clone_work_mounts': inherited, 'removed_clone_work_mounts': removed,
            'visible_readonly_source_mount': visible[0], 'source': str(source),
            'source_device': source_stat.st_dev, 'source_inode': source_stat.st_ino,
            'visible_device': visible_stat.st_dev, 'visible_inode': visible_stat.st_ino,
            'sdk_origin_work_mounts_before': origin_before,
            'sdk_origin_work_mounts_after': origin_after, 'sdk_origin_work_preserved': True}


def parse():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--mode', choices=('prepare', 'execute', 'all'), default='prepare')
    for name in ('source', 'rootfs', 'out', 'qemu', 'binfmt-evidence'):
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--gateway', type=Path, help='Current immutable ARM Gateway; required for execute/all')
    p.add_argument('--gateway-source', type=Path, help='Gateway compiler project source root; defaults to --source')
    p.add_argument('--gateway-mmd', type=Path, help='Gateway raw compiler dependency file; default from Gateway CMake target directory')
    p.add_argument('--gateway-flags', type=Path, help='Gateway raw flags.make; default from Gateway CMake target directory')
    p.add_argument('--gateway-origin-report', type=Path, help='Current native build report bound by SHA')
    p.add_argument('--reuse-prepared', type=Path, help='Prepared manifest.json plus final companion report/architecture; exact current source/SDK/incarnation/ELF/MMD checks')
    p.add_argument('--container-started-at', required=True,
                   help='Explicit observed Docker StartedAt for the current cgroup/container incarnation')
    p.add_argument('--suite', choices=('all', *PROBES), default='all')
    p.add_argument('--limits-domain', type=int, default=198)
    p.add_argument('--late-domain', type=int, default=199)
    a = p.parse_args()
    if not a.container_started_at or any(c in a.container_started_at for c in ('\0', '\r', '\n')):
        p.error('explicit current Docker StartedAt required')
    if a.mode in ('execute', 'all') and a.gateway is None:
        p.error('execute/all requires --gateway')
    if a.mode == 'execute' and a.reuse_prepared is None:
        p.error('execute requires --reuse-prepared; use all for an owned build and run')
    if any(not 0 <= value <= 232 for value in (a.limits_domain, a.late_domain)):
        p.error('ROS domains must be 0..232')
    if len({202, a.limits_domain, a.late_domain}) != 3:
        p.error('parameter domain202 and both Action domains must be distinct')
    return a


def main():
    args = parse()
    source, sdk = args.source.resolve(), args.rootfs.resolve()
    sys.path.insert(0, str(source/'tests/ros_cgroup'))
    from probe import probe
    if os.geteuid() != 0 or sys.platform != 'linux':
        raise RuntimeError('BLOCKED root Linux mount/chroot environment required')
    lease = json.loads(args.binfmt_evidence.read_text())
    nested = lease.get('entry')
    name = lease.get('entry_name') or lease.get('name') or (nested.get('name') if isinstance(nested, dict) else None)
    if not isinstance(name, str) or '/' in name or name in ('', '.', '..'):
        raise RuntimeError('BLOCKED valid provided binfmt entry name required')
    handler = Path('/proc/sys/fs/binfmt_misc')/name
    handler_text = handler.read_text()
    if not handler_text.startswith('enabled\n') or 'flags: F' not in handler_text or 'interpreter '+str(args.qemu.resolve()) not in handler_text:
        raise RuntimeError('BLOCKED active fixed-interpreter lease does not match provided QEMU')
    magic = next((line.split()[1] for line in handler_text.splitlines() if line.startswith('magic ')), '')
    mask = next((line.split()[1] for line in handler_text.splitlines() if line.startswith('mask ')), '')
    if len(magic) < 40 or len(mask) < 40 or bytes.fromhex(magic)[18:20] != b'\xb7\x00' or bytes.fromhex(mask)[18:20] != b'\xff\xff':
        raise RuntimeError('BLOCKED active lease does not enforce ELF183')
    if lease.get('qemu_sha256') is not None and digest(args.qemu) != lease['qemu_sha256']:
        raise RuntimeError('BLOCKED active lease QEMU SHA mismatch')
    evidence = Evidence(args.out.resolve(), probe)
    evidence.emulator(args.qemu)
    out, temp, root = evidence.out, evidence.out/'native-temp', evidence.out/'rootfs'
    temp.mkdir(); root.mkdir()
    (temp/'ros-logs').mkdir()
    source_before = inventory(source)
    save(out/'source-before.json', source_before)
    commands, children, mounts, locks = [], [], [], []
    manifest = {'status': 'FAIL', 'kind': 'ARM64 CPP20 ROS2 specialties preparation',
                'source': str(source), 'source_inventory': source_before,
                'probes': {}, 'raw_compiler_dependencies': {}, 'compiled_project_inputs': {}}
    report = {'mode': args.mode, 'suite': args.suite, 'commands': commands,
              'binfmt_lease': {'path': str(args.binfmt_evidence.resolve()),
                  'sha256_at_start': digest(args.binfmt_evidence), 'provided': lease,
                  'active_entry': handler_text}, 'rootfs': str(sdk),
              'rootfs_os_release_sha256': digest(sdk/'etc/os-release'),
              'domains': {'parameter-initialize': 202, 'action-limits': args.limits_domain,
                          'action-late-close': args.late_domain},
              'executed_driver_sha256': digest(__file__),
              'evidence_helper_sha256': digest(Path(__file__).with_name('runtime.py'))}
    status, error = 'FAIL', None
    native_loader_environment = {}
    native_input_pins = []
    # Lazy imports avoid the existing rebuild_affected -> run_resources mount
    # helper dependency. These functions perform only immutable admission.
    from rebuild_affected import sdk_inventory, sdk_root_aliases, source_state
    from gateway_qualification import pins_preserved, read_file_binding, architecture_binding
    source_manifest_path = (source.parent/'source-manifest.json').resolve(strict=True)
    frozen, source_manifest_pin = read_json_binding(source_manifest_path, limit=64*1024**2)
    if source_state(source, frozen) != source_before:
        raise RuntimeError('FAIL resource preparation requires the complete exact current source manifest')
    native_input_pins.append(source_manifest_pin)
    sdk_before = sdk_inventory(sdk)
    sdk_aliases_before = sdk_root_aliases(sdk)
    sdk_digest_before = inventory_digest(sdk_before)
    manifest.update(source_manifest_sha256=source_manifest_pin['sha256'], source_manifest_binding=source_manifest_pin)
    report.update(source=str(source), source_manifest_sha256=source_manifest_pin['sha256'],
                  source_manifest_binding=source_manifest_pin, container_started_at=args.container_started_at,
                  sdk_static_inventory_digest=sdk_digest_before,
                  preparation_qualification_scope='Three exact CPP20 ARM probes and project-MMD source closure; no full-system-MD or Runtime behavior claim')

    def checked(argv):
        subprocess.run(list(map(str, argv)), check=True, capture_output=True, timeout=30)

    def native(argv, domain=197):
        return ['/usr/bin/chroot', str(root), '/usr/bin/env', '-u', 'CC', '-u', 'CXX',
                '-u', 'LD_LIBRARY_PATH', '-u', 'LD_PRELOAD', 'PYTHONDONTWRITEBYTECODE=1',
                *[key+'='+value for key,value in sorted(native_loader_environment.items())],
                'ROS_DOMAIN_ID='+str(domain), 'RMW_IMPLEMENTATION=rmw_fastrtps_cpp',
                'ROS_SECURITY_ENABLE=false', 'ROS_LOG_DIR=/tmp/ros-logs', '/bin/bash', '-lc',
                'source /opt/ros/jazzy/setup.bash && exec "$@"', 'arm-resources', *argv]

    def run(label, argv, required=(), marker=None, timeout=300):
        log = out/(label+'.log')
        row = {'name': label, 'command': argv, 'log': str(log), 'status': 'FAIL'}
        commands.append(row)
        with log.open('xb') as stream:
            child = subprocess.Popen(argv, stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
        children.append(child)
        row['pid'] = child.pid
        evidence.start(child, label, argv)
        deadline = time.monotonic()+timeout
        while child.poll() is None:
            evidence.observe(child.pid, label)
            if time.monotonic() > deadline:
                raise TimeoutError('bounded outer command timeout: '+label)
            time.sleep(.01)
        row.update(returncode=child.returncode, log_sha256=digest(log), log_binding=read_file_binding(log))
        if child.returncode:
            raise RuntimeError('FAIL '+label+' returned '+str(child.returncode))
        if required:
            evidence.check(required, label)
        if marker and marker not in log.read_text(errors='replace'):
            raise RuntimeError('FAIL missing original semantic completion marker: '+label)
        row['observations'] = [line for line in log.read_text(errors='replace').splitlines()
                               if line.startswith(('OBSERVED', 'SDK_', 'PASS action-', 'PASS declared'))]
        row['status'] = 'PASS'
        save(out/'progress.json', report)
        print('PASS', label, flush=True)
        return row

    def interrupt(signum, frame):
        raise RuntimeError('FAIL interrupted signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupt)
    signal.signal(signal.SIGINT, interrupt)
    try:
        checked(['mount', '--rbind', sdk, root]); mounts.append(root)
        checked(['mount', '--make-rprivate', root])
        report['owned_source_mount'] = bind_frozen_work(root, source, sdk, out, checked)
        checked(['mount', '--bind', temp, root/'tmp'])
        checked(['mount', '--make-rprivate', root])
        report['active_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
        work = [line for line in report['active_owned_mounts'] if line.split()[4] == str(root/'work')]
        if len(work) != 1 or 'ro' not in work[0].split()[5].split(','):
            raise RuntimeError('FAIL frozen source mount is not read only')
        if any(any(token.startswith(('shared:', 'master:', 'propagate_from:'))
                       for token in line.split()[6:line.split().index('-')])
               for line in report['active_owned_mounts']):
            raise RuntimeError('FAIL owned rootfs mount tree is not recursively private')
        run('sdk-architecture', native(['/usr/bin/dpkg', '--print-architecture']))
        if (out/'sdk-architecture.log').read_text().strip() != 'arm64':
            raise RuntimeError('BLOCKED SDK rootfs architecture is not arm64')
        run('sdk-packages', native(['/usr/bin/dpkg-query', '-W', '-f=${Package}\t${Version}\n']))
        report['sdk_packages_sha256'] = digest(out/'sdk-packages.log')
        if args.reuse_prepared:
            prepared_path = args.reuse_prepared.resolve(strict=True)
            original, original_pin = read_json_binding(prepared_path, limit=64*1024**2)
            companion, companion_pin = read_json_binding(prepared_path.parent/'report.json', limit=64*1024**2)
            original_architecture, architecture_pin = architecture_binding(prepared_path.parent/'architecture-evidence.json', native_input_pins)
            native_input_pins.extend((original_pin, companion_pin))
            # The producer writes manifest first, records its opened binding,
            # then publishes the same final object to the two companion files.
            # No self-referential report hash or historical PASS promotion is
            # involved. A failed cleanup cannot leave an admissible manifest.
            if (original.get('status') != 'PREPARED'
                    or original.get('kind') != 'ARM64 CPP20 ROS2 specialties preparation'
                    or set(original.get('probes', {})) != set(PROBES)
                    or set(original.get('raw_compiler_dependencies', {})) != set(PROBES)
                    or original.get('source') != str(source)
                    or original.get('source_inventory') != source_before
                    or original.get('source_manifest_binding') != source_manifest_pin
                    or original.get('source_manifest_sha256') != source_manifest_pin['sha256']):
                raise RuntimeError('FAIL invalid prepared three-probe manifest')
            if (companion != original_architecture or companion.get('status') != 'PASS'
                    or companion.get('mode') not in ('prepare', 'all')
                    or companion.get('prepared_manifest') != original_pin
                    or companion.get('source') != str(source)
                    or companion.get('rootfs') != str(sdk)
                    or companion.get('source_manifest_binding') != source_manifest_pin
                    or companion.get('source_manifest_sha256') != source_manifest_pin['sha256']
                    or companion.get('container_started_at') != args.container_started_at
                    or original_architecture.get('cgroup_incarnation_before') != evidence.incarnation_before
                    or companion.get('sdk_static_inventory_digest') != sdk_digest_before
                    or companion.get('sdk_root_aliases_before') != sdk_aliases_before
                    or not all(companion.get(key) is True for key in ('source_preserved', 'sdk_inventory_preserved',
                        'sdk_root_aliases_preserved', 'native_gateway_input_pins_preserved', 'prepared_products_preserved'))):
                raise RuntimeError('FAIL prepared companion source/SDK/product/current incarnation differs')
            for name in ('compiler-version', 'probe-configure', 'probe-build'):
                matches = [row for row in companion.get('commands', []) if row.get('name') == name]
                if len(matches) != 1 or matches[0].get('status') != 'PASS' or type(matches[0].get('returncode')) is not int or matches[0]['returncode'] != 0:
                    raise RuntimeError('FAIL prepared companion has no exact successful original compiler command')
            for row in companion['commands']:
                pin = read_file_binding(row['log']); native_input_pins.append(pin)
                if pin != row.get('log_binding') or pin['sha256'] != row.get('log_sha256'):
                    raise RuntimeError('FAIL prepared original compiler/runtime log changed')
            database_pin = read_file_binding(prepared_path.parent/'compile_commands.json'); native_input_pins.append(database_pin)
            if database_pin['sha256'] != companion.get('compile_commands_sha256'):
                raise RuntimeError('FAIL prepared compiler database changed')
            report.update(reuse_prepared_manifest=original_pin, reuse_prepared_companion=companion_pin,
                          reuse_prepared_architecture=architecture_pin)
            for relative, value in original['compiled_project_inputs'].items():
                if source_before.get(relative) != value:
                    raise RuntimeError('FAIL prepared source differs from current frozen source: '+relative)
            for label, item in original['probes'].items():
                binary = Path(item['path'])
                facts = require_arm(binary)
                if facts != item:
                    raise RuntimeError('FAIL prepared probe binary changed: '+label)
                native_input_pins.append(read_file_binding(binary))
                shutil.copy2(binary, temp/label)
                evidence.target(label, temp/label)
                manifest['probes'][label] = require_arm(temp/label)
            for label, item in original['raw_compiler_dependencies'].items():
                original_mmd = Path(item['path'])
                pin = read_file_binding(original_mmd); native_input_pins.append(pin)
                if pin['sha256'] != item['sha256'] or pin['bytes'] != item['bytes'] or dependencies(original_mmd, source) != item['project_inputs']:
                    raise RuntimeError('FAIL prepared raw MMD changed: '+label)
                copied = out/(label+'-origin.d')
                shutil.copy2(original_mmd, copied)
                manifest['raw_compiler_dependencies'][label] = {**item, 'path': str(copied)}
            manifest['compiled_project_inputs'] = dict(original['compiled_project_inputs'])
        else:
            cmake = temp/'cmake'; cmake.mkdir()
            (cmake/'CMakeLists.txt').write_text(cmake_project())
            report['generated_cmake_sha256'] = digest(cmake/'CMakeLists.txt')
            run('compiler-version', native(['/usr/bin/g++', '--version']))
            run('probe-configure', native(['/usr/bin/cmake', '-S', '/tmp/cmake', '-B', '/tmp/build',
                '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_CXX_COMPILER=/usr/bin/g++', '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON']))
            # A single native compiler at any instant; parent controls other build slots.
            run('probe-build', native(['/usr/bin/cmake', '--build', '/tmp/build', '--parallel', '1']), timeout=900)
            for label, project_source in PROBES.items():
                binary = temp/'build'/label
                evidence.target(label, binary)
                manifest['probes'][label] = require_arm(binary)
                dep = temp/'build/CMakeFiles'/(label+'.dir')
                found = list(dep.rglob('*.o.d'))
                if len(found) != 1:
                    raise RuntimeError('FAIL expected exactly one original probe MMD: '+label)
                values = dependencies(found[0], source)
                if project_source not in values or any(source_before.get(key) != value for key, value in values.items()):
                    raise RuntimeError('FAIL compiler source closure mismatch: '+label)
                flags = dep/'flags.make'
                if not flags.is_file() or not any(option in flags.read_text() for option in ('-std=gnu++20', '-std=c++20')):
                    raise RuntimeError('FAIL probe is not explicitly compiled as CPP20: '+label)
                raw = out/(label+'.d'); shutil.copy2(found[0], raw)
                manifest['raw_compiler_dependencies'][label] = {'path': str(raw), 'sha256': digest(raw),
                    'bytes': raw.stat().st_size, 'project_inputs': values}
                manifest['compiled_project_inputs'].update(values)
            shutil.copy2(temp/'build/compile_commands.json', out/'compile_commands.json')
            report['compile_commands_sha256'] = digest(out/'compile_commands.json')
        manifest['status'] = 'PREPARED'
        save(out/'manifest.json', manifest)
        report['prepared_manifest'] = read_file_binding(out/'manifest.json')
        native_input_pins.append(report['prepared_manifest'])
        if args.mode in ('execute', 'all'):
            from gateway_qualification import qualify_gateway
            from native_runtime_environment import gateway_library_environment
            gateway_source = args.gateway_source.resolve() if args.gateway_source else source
            origin = args.gateway.resolve()
            if args.gateway_origin_report is None:
                raise RuntimeError('BLOCKED strict Gateway origin report required for native runtime libraries')
            strict = qualify_gateway(origin, args.gateway_origin_report,
                gateway_source.parent/'source-manifest.json', gateway_source, sdk, 2)
            loader = gateway_library_environment(strict, sdk, sdk_before)
            native_input_pins.extend(strict['input_pins']+loader['input_pins'])
            native_loader_environment.update(loader['environment'])
            report['strict_gateway_qualification'] = strict
            report['native_gateway_library_environment'] = loader
            report['native_runtime_environment_helper_sha256'] = digest(Path(__file__).with_name('native_runtime_environment.py'))
            meta = origin.parent/'CMakeFiles/uwvm-ros2-gateway.dir'
            mmd = args.gateway_mmd.resolve() if args.gateway_mmd else meta/'main.cc.o.d'
            flags = args.gateway_flags.resolve() if args.gateway_flags else meta/'flags.make'
            values = dependencies(mmd, gateway_source)
            if 'gateways/ros2/main.cc' not in values or any(source_before.get(key) != value for key, value in values.items()):
                raise RuntimeError('FAIL current Gateway compiler project closure differs from frozen source')
            if not flags.is_file():
                raise RuntimeError('FAIL current Gateway raw compiler flags are unavailable')
            # GCC13 already defaults to GNU++17, so CMake may omit -std=17.
            # Compile with the recorded target flags and assert the exact
            # language macro, rather than inventing an explicit historical flag.
            flag_fields = {}
            for line in flags.read_text().splitlines():
                if ' = ' in line:
                    key, value = line.split(' = ', 1)
                    flag_fields[key] = shlex.split(value)
            if 'CXX_FLAGS' not in flag_fields:
                raise RuntimeError('FAIL current Gateway target CXX_FLAGS missing')
            (temp/'gateway-standard.cc').write_text('static_assert(__cplusplus == 201703L, "Gateway compilation flags must select C++17");\n')
            run('gateway-cpp17-standard', native(['/usr/bin/g++',
                *flag_fields.get('CXX_DEFINES', []), *flag_fields.get('CXX_INCLUDES', []),
                *flag_fields['CXX_FLAGS'], '-c', '/tmp/gateway-standard.cc', '-o', '/tmp/gateway-standard.o']))
            shutil.copy2(origin, temp/'gateway')
            facts = evidence.target('gateway', temp/'gateway')
            if digest(origin) != facts['sha256']:
                raise RuntimeError('FAIL Gateway changed while snapshotting')
            shutil.copy2(mmd, out/'gateway-origin.d')
            shutil.copy2(flags, out/'gateway-flags.make')
            report['gateway_origin'] = {'binary': str(origin), 'arm_elf': facts,
                'raw_MMD_sha256': digest(out/'gateway-origin.d'), 'project_inputs': values,
                'flags_sha256': digest(out/'gateway-flags.make'),
                'origin_report': str(args.gateway_origin_report.resolve()) if args.gateway_origin_report else None,
                'origin_report_sha256': digest(args.gateway_origin_report) if args.gateway_origin_report else None}
            selected = list(PROBES) if args.suite == 'all' else [args.suite]
            for label in selected:
                domain = report['domains'][label]
                lock = Path('/tmp')/f'uwvm-arm-ros-domain-{domain}.lock'
                fd = os.open(lock, os.O_CREAT|os.O_EXCL|os.O_WRONLY, 0o600)
                os.write(fd, str(os.getpid()).encode()); st = os.fstat(fd); os.close(fd)
                locks.append((lock, st.st_dev, st.st_ino))
                modes = (None,) if label == 'parameter-initialize' else LIMIT_MODES if label == 'action-limits' else LATE_MODES
                inside = '/tmp/'+label if args.reuse_prepared else '/tmp/build/'+label
                for mode in modes:
                    case = 'resource-'+label+('-'+mode if mode else '')
                    argv = [inside, '/tmp/gateway', *([mode] if mode else [])]
                    marker = 'PASS declared static uninitialized parameter initializes, exact static descriptor and missing/dynamic/wrong refusals retain session' if label == 'parameter-initialize' else 'PASS '+label+' mode='+mode+' '
                    run(case, native(argv, domain), required=(label, 'gateway'), marker=marker, timeout=120)
            report['actual_resource_case_count'] = sum(row['name'].startswith('resource-') and row['status'] == 'PASS' for row in commands)
            expected = 16 if args.suite == 'all' else 1 if args.suite == 'parameter-initialize' else 8 if args.suite == 'action-limits' else 7
            if report['actual_resource_case_count'] != expected:
                raise RuntimeError('FAIL missing resource behavior cases')
            if not pins_preserved(native_input_pins):
                raise RuntimeError('FAIL selected Gateway/native library bindings changed')
        status = 'PASS'
    except BaseException as caught:
        error = traceback.format_exc()
        status = 'BLOCKED' if 'BLOCKED' in str(caught) else 'FAIL'
    finally:
        for child in reversed(children):
            if child.poll() is None:
                os.killpg(child.pid, signal.SIGTERM)
                try: child.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    os.killpg(child.pid, signal.SIGKILL); child.wait(timeout=3)
        for lock, device, inode in locks:
            try:
                stat = lock.stat()
                if (stat.st_dev, stat.st_ino) == (device, inode): lock.unlink()
                else: status = 'FAIL'
            except FileNotFoundError: pass
        report['mount_cleanup'] = []
        for mounted in reversed(mounts):
            for attempt in range(20):
                result = subprocess.run(['umount', '-R', str(mounted)], text=True, capture_output=True, timeout=15)
                report['mount_cleanup'].append({'path': str(mounted), 'returncode': result.returncode,
                                               'output': result.stdout+result.stderr})
                if result.returncode == 0: break
                time.sleep(.05)
            else: status = 'FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
        report['source_preserved'] = inventory(source) == source_before
        report['sdk_inventory_preserved'] = sdk_inventory(sdk) == sdk_before
        report['sdk_root_aliases_before'] = sdk_aliases_before
        report['sdk_root_aliases_preserved'] = sdk_root_aliases(sdk) == sdk_aliases_before
        try:
            report['prepared_products_preserved'] = all(require_arm(item['path']) == item for item in manifest['probes'].values())
        except (OSError, RuntimeError, ValueError):
            report['prepared_products_preserved'] = False
        # This includes the original current full manifest and every admitted
        # companion/log/MMD/product, including preparation-only execution.
        report['preparation_input_pins'] = native_input_pins
        report['native_gateway_input_pins_preserved'] = pins_preserved(native_input_pins)
        if not all(report[key] for key in ('sdk_inventory_preserved','sdk_root_aliases_preserved',
                                           'native_gateway_input_pins_preserved','prepared_products_preserved')): status = 'FAIL'
        if report['remaining_owned_mounts'] or not report['source_preserved']: status = 'FAIL'
        report['scope'] = 'Preparation only: CPP20 ARM native probes, no ROS/DDS behavior executed' if args.mode == 'prepare' else 'Current CPP17 ARM Gateway plus unchanged CPP20 ARM probes: static initialization1, Action byte limits8, Action lifetime7; native IPC/ROS path, no Wasm/A15 claim'
        report['error'] = error
        result = evidence.finish(status, report)
        save(out/'report.json', result)
    print(result['status'], out/'report.json', flush=True)
    return 0 if result['status'] == 'PASS' else 77 if result['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    sys.exit(main())
