#!/usr/bin/env python3
"""Build real ARM64 ROS targets in a private mount namespace and inherited cgroup."""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shutil
import signal
import stat
import subprocess
import sys
import time

sys.dont_write_bytecode = True

from runtime import Evidence, require_arm


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def rooted(root, path):
    """Resolve absolute SDK symlinks inside the SDK, with a finite link budget."""
    parts = list(Path(posixpath.normpath('/' + str(path).lstrip('/'))).parts[1:])
    selected, followed = [], 0
    while parts:
        part = parts.pop(0)
        if part == '..':
            if selected: selected.pop()
            continue
        if part in ('', '.'): continue
        candidate = root.joinpath(*selected, part)
        if candidate.is_symlink():
            followed += 1
            if followed > 40: raise RuntimeError('SDK symlink loop')
            link = os.readlink(candidate)
            if link.startswith('/'): selected = []
            parts = list(Path(link).parts[1:] if link.startswith('/') else Path(link).parts) + parts
        else:
            selected.append(part)
    return root.joinpath(*selected)


def custom_version_values(values):
    """Require version-qualified inputs for the two-version base build."""
    selected = {}
    for item in values:
        version, separator, value = item.partition('=')
        if separator != '=' or version not in ('1', '2') or not value or version in selected:
            raise RuntimeError('custom base inputs must be unique 1=value and 2=value')
        selected[version] = value
    return selected

def custom_file_pin(path):
    """Pin one bounded regular input from one nonblocking opened descriptor."""
    selected = Path(path).resolve(strict=True)
    descriptor = os.open(selected, os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK | os.O_NOFOLLOW)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= 64 * 1024 * 1024:
            raise RuntimeError('custom input must be a bounded regular file')
        hasher = hashlib.sha256()
        remaining = before.st_size + 1
        length = 0
        while remaining:
            chunk = os.read(descriptor, min(1024 * 1024, remaining))
            if not chunk:
                break
            hasher.update(chunk); length += len(chunk); remaining -= len(chunk)
        after = os.fstat(descriptor)
        identity = lambda item: (item.st_dev, item.st_ino, item.st_size, item.st_mtime_ns, item.st_ctime_ns)
        if identity(before) != identity(after) or identity(after) != identity(selected.stat()) or length != after.st_size:
            raise RuntimeError('custom input changed while being pinned')
        return {'path': str(selected), 'device': after.st_dev, 'inode': after.st_ino,
                'bytes': length, 'mtime_ns': after.st_mtime_ns, 'ctime_ns': after.st_ctime_ns,
                'sha256': hasher.hexdigest()}
    finally:
        os.close(descriptor)


def custom_packages(values):
    """Parse an explicit comma/repeated package list without accepting flags."""
    packages = []
    for value in values:
        for name in value.split(','):
            if not re.fullmatch(r'[a-z][a-z0-9_]{0,62}', name) or name in packages:
                raise RuntimeError('invalid or duplicate explicit custom package')
            packages.append(name)
    if not packages or len(packages) > 128:
        raise RuntimeError('custom selection requires 1..128 explicit packages')
    return packages


def custom_header(value, source, sdk, frozen):
    """Map a header only to frozen /work or immutable SDK /opt namespace."""
    raw = str(value)
    if not raw or any(character in raw for character in ('"', ';', '\\', '\n', '\r', '\0')):
        raise RuntimeError('unsafe custom header include path')
    logical = posixpath.normpath(raw)
    path = Path(raw)
    if logical.startswith('/work/'):
        relative = logical[len('/work/'):]
        actual = (source / relative).resolve(strict=True)
        scope = 'frozen_source'
        guest = logical
    elif logical.startswith('/opt/'):
        actual = rooted(sdk, logical).resolve(strict=True)
        scope = 'immutable_sdk'
        guest = logical
    elif path.is_absolute() and path.is_relative_to(source):
        relative = str(path.relative_to(source))
        actual = path.resolve(strict=True)
        scope = 'frozen_source'
        guest = '/work/' + relative
    elif path.is_absolute() and path.is_relative_to(sdk / 'opt'):
        guest = '/' + str(path.relative_to(sdk))
        actual = rooted(sdk, guest).resolve(strict=True)
        scope = 'immutable_sdk'
    else:
        raise RuntimeError('custom header must be frozen /work or immutable SDK /opt')
    pin = custom_file_pin(actual)
    if scope == 'frozen_source':
        if not actual.is_relative_to(source):
            raise RuntimeError('custom source header escapes frozen source')
        row = frozen.get(relative, {})
        if row.get('sha256') != pin['sha256'] or row.get('bytes') != pin['bytes']:
            raise RuntimeError('custom source header is not an exact frozen manifest input')
        return {'guest_path': guest, 'scope': scope, 'source_relative': relative, 'input': pin}
    if not actual.is_relative_to((sdk / 'opt').resolve(strict=True)):
        raise RuntimeError('custom SDK header escapes immutable /opt')
    return {'guest_path': guest, 'scope': scope, 'sdk_relative': str(actual.relative_to(sdk)), 'input': pin}


def custom_selection(version, registry, native, packages, prefix, source, sdk, frozen):
    """Prepare closed paired custom CMake inputs; create no process or grant."""
    supplied = (bool(registry), bool(native), bool(packages), bool(prefix))
    if not any(supplied):
        return {'enabled': False, 'version': version, 'cmake_arguments': [], 'input_pins': []}
    if not all(supplied) or version not in (1, 2):
        raise RuntimeError('custom topics require paired headers, packages and prefix')
    prefix_text = posixpath.normpath(str(prefix))
    if prefix_text != '/opt/uwvm-custom-types':
        raise RuntimeError('custom package prefix must be /opt/uwvm-custom-types')
    resolved_prefix = rooted(sdk, prefix_text).resolve(strict=True)
    if not resolved_prefix.is_dir() or not resolved_prefix.is_relative_to((sdk / 'opt').resolve(strict=True)):
        raise RuntimeError('custom package prefix escapes immutable SDK /opt')
    names = custom_packages(packages)
    common = custom_header(registry, source, sdk, frozen)
    provider = custom_header(native, source, sdk, frozen)
    configs = []
    for package in names:
        logical = prefix_text + '/share/' + package + '/cmake/' + package + 'Config.cmake'
        actual = rooted(sdk, logical).resolve(strict=True)
        if not actual.is_relative_to(resolved_prefix):
            raise RuntimeError('custom package config escapes explicit prefix')
        configs.append({'package': package, 'guest_path': logical,
                        'sdk_relative': str(actual.relative_to(sdk)), 'input': custom_file_pin(actual)})
    distro = 'noetic' if version == 1 else 'jazzy'
    arguments = ['-DUWVM_ROS_CUSTOM_REGISTRY_HEADER=' + common['guest_path'],
                 '-DUWVM_ROS_CUSTOM_ROS' + str(version) + '_HEADER=' + provider['guest_path'],
                 '-DUWVM_ROS_CUSTOM_ROS' + str(version) + '_PACKAGES=' + ';'.join(names),
                 '-DCMAKE_PREFIX_PATH=' + prefix_text + ';/opt/ros/' + distro]
    # Exact package DIR arguments prevent another installed prefix from winning
    # CMake discovery while the report claims these selected package configs.
    arguments += ['-D' + package + '_DIR=' + prefix_text + '/share/' + package + '/cmake' for package in names]
    pins = [common['input'], provider['input'], *[item['input'] for item in configs]]
    return {'enabled': True, 'version': version, 'registry_header': common, 'native_header': provider,
            'prefix': {'guest_path': prefix_text, 'sdk_relative': str(resolved_prefix.relative_to(sdk))},
            'packages': names, 'package_configs': configs, 'cmake_arguments': arguments,
            'input_pins': pins, 'scope': 'Build support only; no implicit ROS permission or entity'}


def custom_inputs_preserved(selection):
    """Refuse replacement, mutation or deletion of every explicitly pinned input."""
    return all(custom_file_pin(pin['path']) == pin for pin in selection['input_pins'])


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace', type=Path, required=True)
    p.add_argument('--sdk-workspace', type=Path, required=True)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--qemu', type=Path, required=True)
    p.add_argument('--container-started-at', required=True,
                   help='Observed Docker State.StartedAt for this cgroup incarnation')
    p.add_argument('--tag', default='arm64-r1')
    p.add_argument('--target-timeout', type=int, default=900)
    p.add_argument('--timeout', type=int, default=7000)
    p.add_argument('--custom-registry-header', type=Path)
    p.add_argument('--custom-native-header', action='append', default=[], help='Repeat exactly 1=header and 2=header')
    p.add_argument('--custom-packages', action='append', default=[], help='Repeat exactly 1=pkg,... and 2=pkg,...')
    p.add_argument('--custom-prefix', type=Path)
    a = p.parse_args()
    if not re.fullmatch(r'[a-z0-9]+(?:-[a-z0-9]+)*', a.tag):
        p.error('tag must contain lowercase letters, digits and single hyphens')
    if a.target_timeout <= 0 or a.timeout <= 0:
        p.error('timeouts must be positive')
    native_headers = custom_version_values(a.custom_native_header)
    native_packages = custom_version_values(a.custom_packages)
    custom_supplied = (bool(a.custom_registry_header), bool(native_headers), bool(native_packages), bool(a.custom_prefix))
    if any(custom_supplied) and (not all(custom_supplied) or set(native_headers) != {'1', '2'} or set(native_packages) != {'1', '2'}):
        p.error('custom base build requires paired headers and packages for both ROS versions plus the common registry/prefix')
    source = a.source.resolve()
    work = a.workspace.resolve()
    sdk = a.sdk_workspace.resolve()
    sys.path.insert(0, str(source/'tests/ros_cgroup'))
    from probe import probe

    # Bind mounts must never escape into the caller's shared namespace.
    if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
        raise RuntimeError('BLOCKED launch this driver with unshare -m')
    out = work/'gateway'/('arm64-'+a.tag+'-provenance')
    out.mkdir(parents=True, exist_ok=False)
    start = time.monotonic()
    evidence = Evidence(out/'execution', probe, interval=.01)
    def incarnation():
        stat = evidence.leaf.stat()
        return {'cgroup_path': str(evidence.leaf), 'device': stat.st_dev, 'inode': stat.st_ino,
                'ctime_ns': stat.st_ctime_ns, 'container_started_at': a.container_started_at,
                'membership': Path('/proc/self/cgroup').read_text().splitlines()}
    before_incarnation = incarnation()
    emulator = evidence.emulator(a.qemu.resolve(strict=True))
    lease_name = os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY')
    if not lease_name or not lease_name.startswith('uwvm-ros-arm64-'):
        raise RuntimeError('BLOCKED task-owned binfmt lease is required')
    lease_entry = (Path('/proc/sys/fs/binfmt_misc')/lease_name).read_text()
    if 'interpreter '+str(a.qemu.resolve()) not in lease_entry or 'flags: F' not in lease_entry:
        raise RuntimeError('BLOCKED binfmt interpreter differs from pinned QEMU')
    mounts = []
    roots = {}
    builds = {}
    rows = []
    active = None
    frozen_manifest = json.loads((work/'inputs/source-manifest.json').read_text())
    def source_state():
        files = {str(path.relative_to(source)): path for path in source.rglob('*') if path.is_file()}
        missing = set(frozen_manifest)-set(files)
        if missing:
            raise RuntimeError('FAIL declared frozen inputs are missing: '+','.join(sorted(missing)))
        cache = []
        for name in sorted(set(files)-set(frozen_manifest)):
            path = files[name]
            if '__pycache__' not in Path(name).parts or path.suffix != '.pyc':
                raise RuntimeError('FAIL unlisted potential compiler input: '+name)
            cache.append({'path': name, 'sha256': sha(path), 'bytes': path.stat().st_size,
                          'mtime_ns': path.stat().st_mtime_ns,
                          'scope': 'Derived Python bytecode outside declared/compiler inputs'})
        values = {name: sha(files[name]) for name in frozen_manifest}
        if any(values[name] != row['sha256'] or files[name].stat().st_size != row['bytes']
               for name, row in frozen_manifest.items()):
            raise RuntimeError('FAIL source differs from provided frozen source manifest')
        return values, cache
    initial, cache_before = source_state()
    report = {'status': 'RUNNING', 'created_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'source': str(source), 'source_files': len(initial), 'parallelism': 1,
              'sdk_workspace': str(sdk), 'builds': rows, 'targets': {}, 'mounts': [],
              'emulator': emulator, 'binfmt': {'name': lease_name, 'entry': lease_entry},
              'cgroup_incarnation_before': before_incarnation,
              'oom_delta_scope': 'Only this fresh cgroup incarnation; interrupted prior build rows are not included',
              'non_input_cache_before': cache_before}
    report['custom_selection'] = {}
    driver_inputs = out/'driver-inputs'
    driver_inputs.mkdir()
    report['driver_inputs'] = {}
    for path in (Path(__file__), Path(__file__).with_name('runtime.py')):
        archive = driver_inputs/path.name
        shutil.copy2(path, archive)
        report['driver_inputs'][path.name] = {'archive': str(archive), 'sha256': sha(archive)}
    report_file = out/'build-report.json'

    def save():
        report['seconds'] = round(time.monotonic()-start, 3)
        report_file.write_text(json.dumps(report, indent=2)+'\n')

    def interrupted(signum, frame):
        raise RuntimeError('FAIL interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)

    def checked(command):
        subprocess.run([str(x) for x in command], check=True, timeout=30,
                       stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def bind(origin, destination, readonly=False, recursive=False):
        destination.mkdir(parents=True, exist_ok=True)
        checked(['mount', '--rbind' if recursive else '--bind', origin, destination])
        mounts.append(destination)
        checked(['mount', '--make-rprivate', destination])
        if readonly:
            checked(['mount', '-o', 'remount,bind,ro', destination])
        report['mounts'].append({'source': str(origin), 'destination': str(destination),
                                 'readonly': readonly, 'recursive': recursive})

    environment = os.environ.copy()
    for name in ('CC', 'CXX', 'LD_LIBRARY_PATH', 'CFLAGS', 'CXXFLAGS', 'LDFLAGS'):
        environment.pop(name, None)
    environment.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',
                       PYTHONDONTWRITEBYTECODE='1')

    def native(version, arguments):
        distro = 'noetic' if version == 1 else 'jazzy'
        return ['/usr/bin/chroot', str(roots[version]), '/bin/bash', '-lc',
                f'source /opt/ros/{distro}/setup.bash && exec "$@"', 'uwvm-arm64-build', *arguments]

    def execute(name, command, required=(), timeout=None):
        nonlocal active
        remaining = a.timeout-(time.monotonic()-start)
        if remaining <= 0:
            raise RuntimeError('FAIL total gateway build deadline')
        bounded = min(timeout or a.target_timeout, remaining)
        row = {'name': name, 'command': command, 'timeout_seconds': bounded, 'status': 'RUNNING'}
        rows.append(row)
        save()
        begun = time.monotonic()
        with (out/(name+'.log')).open('wb') as log:
            active = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT,
                                      env=environment, start_new_session=True)
        evidence.start(active, name, command)
        row['pid'] = active.pid
        save()
        try:
            row['returncode'] = active.wait(timeout=bounded)
            row['status'] = 'PASS' if row['returncode'] == 0 else 'FAIL'
            if row['returncode']:
                raise RuntimeError('FAIL native ARM build command: '+name)
            evidence.check(required, name)
        except BaseException:
            row['status'] = 'FAIL'
            if active.poll() is None:
                try:
                    os.killpg(active.pid, signal.SIGTERM)
                    active.wait(timeout=5)
                except (ProcessLookupError, subprocess.TimeoutExpired):
                    try:
                        os.killpg(active.pid, signal.SIGKILL)
                    except ProcessLookupError:
                        pass
                    active.wait(timeout=5)
            raise
        finally:
            row['seconds'] = round(time.monotonic()-begun, 3)
            active = None
            save()
        print('PASS '+name, flush=True)

    status = 'FAIL'
    try:
        checked(['mount', '--make-rprivate', '/'])
        for version, distro in ((1, 'noetic'), (2, 'jazzy')):
            origin = sdk/'gateway'/(distro+'-rootfs')
            if not (origin/'.uwvm-task-owned').is_file():
                raise RuntimeError('BLOCKED only a task-owned ARM SDK rootfs may be modified')
            selected_custom = custom_selection(version, a.custom_registry_header, native_headers.get(str(version)),
                [native_packages[str(version)]] if str(version) in native_packages else [],
                a.custom_prefix, source, origin, frozen_manifest)
            report['custom_selection'][str(version)] = selected_custom
            root = work/'gateway'/(distro+'-rootfs')
            bind(origin, root, recursive=True)
            roots[version] = root
            bind(source, root/'work', readonly=True)
            for name in ('proc', 'sys', 'dev'):
                bind(Path('/')/name, root/name, recursive=True)
            major = '9' if version == 1 else '13'
            compiler = origin/f'usr/bin/aarch64-linux-gnu-g++-{major}'
            candidates = [origin/f'{directory}/gcc/aarch64-linux-gnu/{major}/cc1plus'
                          for directory in ('usr/lib', 'usr/libexec')]
            frontend = next((path for path in candidates if path.is_file()), None)
            if frontend is None:
                raise RuntimeError('BLOCKED native ARM compiler frontend is unavailable')
            evidence.target(f'ros{version}-compiler', compiler)
            evidence.target(f'ros{version}-frontend', frontend)
            evidence.target(f'ros{version}-cmake', origin/'usr/bin/cmake')
            # Make sure the actual configured toolchain was sampled before short
            # configure checks; later compilation requires the frontend mapping.
            execute(f'ros{version}-compiler-version', native(version, ['/usr/bin/g++', '--version']),
                    [f'ros{version}-compiler'], timeout=60)
            execute(f'ros{version}-package-inventory', native(version,
                    ['/usr/bin/dpkg-query', '-W', '-f=${Package}\t${Version}\t${Architecture}\n']), timeout=60)
            package_file = out/f'ros{version}-package-inventory.log'
            report.setdefault('package_inventories', {})[str(version)] = {
                'path': str(package_file), 'sha256': sha(package_file),
                'supplemental_extractions': str(sdk/'provenance/supplemental-packages.json')}
            build = origin/'tmp'/f'ros{version}-{a.tag}-build'
            if build.exists():
                raise RuntimeError('BLOCKED fresh native build path is required')
            builds[version] = build
            execute(f'ros{version}-configure', native(version, ['cmake', '-S', '/work/gateways/ros'+str(version),
                    '-B', '/tmp/'+build.name, '-DCMAKE_BUILD_TYPE=RelWithDebInfo', '-DCMAKE_CXX_FLAGS=-MMD',
                    '-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=-O2 -g0 -DNDEBUG', *selected_custom['cmake_arguments']]),
                    [f'ros{version}-cmake'])

        # Produce both backends' behavior witnesses first, then less urgent probes.
        priorities = {1: ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe'),
                      2: ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe',
                          'lifecycle-probe', 'secure-probe')}
        remainder = ('perf-probe', 'ipc-service-probe', 'five-module-probe')
        for stage in ('priority', 'remainder'):
            for version in (1, 2):
                for kind in priorities[version] if stage == 'priority' else remainder:
                    target = f'uwvm-ros{version}-'+kind
                    execute(f'ros{version}-build-'+kind, native(version,
                            ['cmake', '--build', '/tmp/'+builds[version].name, '--target', target, '--parallel', '1']),
                            [f'ros{version}-frontend'])
                    binary = builds[version]/target
                    report['targets'][target] = require_arm(binary)
                    save()

        if len(report['targets']) != 18:
            raise RuntimeError('FAIL expected 18 actual ARM64 native targets')
        compiled = {}
        metadata = {}
        for version, build in builds.items():
            archive = out/f'ros{version}-metadata'
            for path in [build/'CMakeCache.txt', *build.rglob('flags.make'), *build.rglob('link.txt'),
                         *build.rglob('*.d'), *build.rglob('depend.make')]:
                destination = archive/path.relative_to(build)
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(path.read_bytes())
                metadata[f'ros{version}:'+str(path.relative_to(build))] = {
                    'path': str(destination), 'sha256': sha(path)}
                if path.name.endswith('.d') or path.name == 'depend.make':
                    names = {posixpath.normpath(value.rstrip(':'))[len('/work/'):]
                             for value in re.findall(r'/work/[^\s\\]+', path.read_text())
                             if not value.rstrip(':').endswith('.o')}
                    if names:
                        values = {}
                        for name in sorted(names):
                            actual = roots[version]/'work'/name
                            if name not in initial or sha(actual) != initial[name] or sha(source/name) != initial[name]:
                                raise RuntimeError('FAIL compiler input differs from frozen source: '+name)
                            values[name] = initial[name]
                        compiled[f'ros{version}:'+str(path.relative_to(build))] = values
        for name, value in initial.items():
            if sha(source/name) != value:
                raise RuntimeError('FAIL frozen source changed during native builds: '+name)
        final_inputs, cache_after = source_state()
        if final_inputs != initial:
            raise RuntimeError('FAIL frozen source inventory changed during native builds')
        report['custom_inputs_preserved'] = all(custom_inputs_preserved(item) for item in report['custom_selection'].values())
        if not report['custom_inputs_preserved']:
            raise RuntimeError('FAIL custom selection inputs changed during base compilation')
        report['non_input_cache_after'] = cache_after
        for version, build in builds.items():
            pending = build.parent/f'ros{version}-build-{a.tag}-pending'
            pending.symlink_to(build.name)
            pending.replace(build.parent/f'ros{version}-build')
        report.update(compiled_source_dependencies=compiled, build_metadata=metadata,
                      source_manifest_sha256=sha(work/'inputs/source-manifest.json'),
                      sdk_repository_signatures=json.loads((sdk/'provenance/repository-signatures.json').read_text()),
                      sdk_supplemental_packages=json.loads((sdk/'provenance/supplemental-packages.json').read_text()),
                      canonical_paths={str(version): str(build.parent/f'ros{version}-build')
                                       for version, build in builds.items()},
                      rootfs_mountinfo=[line for line in Path('/proc/self/mountinfo').read_text().splitlines()
                                        if any(str(root) in line for root in roots.values())])
        status = 'PASS'
    except BaseException as error:
        report['error'] = str(error)
        print('FAIL '+str(error), flush=True)
    finally:
        try:
            report['custom_inputs_preserved'] = all(custom_inputs_preserved(item) for item in report['custom_selection'].values())
            if not report['custom_inputs_preserved']:
                status = 'FAIL'
        except BaseException as error:
            status = 'FAIL'
            report['custom_input_final_error'] = str(error)
        verification = evidence.finish(status)
        report['execution_evidence'] = str(out/'execution/architecture-evidence.json')
        report['status'] = verification['status']
        report['cgroup_incarnation_after'] = incarnation()
        if any(before_incarnation[key] != report['cgroup_incarnation_after'][key]
               for key in ('device', 'inode', 'membership')):
            report['status'] = 'FAIL'
            report['incarnation_error'] = 'cgroup identity changed during this fresh build'
        cleanup = []
        for mount in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(mount)], stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE, text=True, timeout=30)
            cleanup.append({'mount': str(mount), 'returncode': result.returncode, 'stderr': result.stderr})
        if any(row['returncode'] for row in cleanup):
            report['status'] = 'FAIL'
        report['mount_cleanup'] = cleanup
        report['post_cleanup_mountinfo'] = [
            line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if any(str(root) in line for root in roots.values())]
        if report['post_cleanup_mountinfo']:
            report['status'] = 'FAIL'
        save()
    print(report['status']+' '+str(report_file), flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
