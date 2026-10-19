#!/usr/bin/env python3
"""Build genuine Noetic legacy Unitree products; execute metadata only.

Root invokes this frozen entry inside an owned private mount namespace and the
selected unchanged hard64GiB/zero-swap cgroup. A terminal successful fresh513
acquisition AND its successful independent Root owner are mandatory inputs.
The official eleven-message package is copied without editing its build files,
generated, installed and checked against its fresh devel headers. This lane
then builds the real identity exporter, custom ROS1 Gateway and independent
legacy transport witness. The primary TU count comes from the actual three
compile databases; no undocumented old six-TU package is asserted or promoted.

Only the cooperative, stopped, descriptor-qualified metadata exporter executes.
No Master, Gateway context, TCPROS delivery, Wasm, robot or simulator executes.
All such acceptance belongs to separately owned future execution lanes.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import socket
import stat
import sys
import time
import types
import traceback

sys.dont_write_bytecode = True
import common as unitree
import noetic_native_support as support

MIB, GIB = 1024**2, 1024**3
SCHEMA = 'uwvm-unitree-noetic-native-products-v1'
STATUS = 'PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED'
SOURCE35_SHA = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
FAMILY = 'unitree_legged_msgs'
COMMIT = 'b989870124913091fbe75e0dcfb047eb4ca00a09'
MESSAGES = ('BmsCmd', 'BmsState', 'Cartesian', 'HighCmd', 'HighState', 'IMU',
            'LED', 'LowCmd', 'LowState', 'MotorCmd', 'MotorState')


class NativeFrame(unitree.Frame):
    """Use the ordinary closed Frame, with cleanup independent of free space.

    A low-space active failure cannot prevent owned process/mount retirement or
    preservation. SDK symlinks are literal chroot names; they never authorize a
    host target merely because pathlib can follow that spelling on the host.
    The normal complete input/source inventories and pin rules are unchanged.
    """
    def __init__(self, args):
        self.finalizing = False; self.started = time.monotonic()
        self.logical_output_roots = []; self.output_checked = 0; self.overlay_paths = set()
        self.final_output_ready = False
        super().__init__(args, 'unitree-noetic-fresh-native-preparation')

    def guard(self):
        self.common.cgroup_preserved(self.before, self.common.cgroup_state(self.config['cgroup']))
        if not self.finalizing:
            unitree.require(time.monotonic() - self.started <= 21600 and
                resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 1536 * MIB,
                'finite six-hour/1536MiB native coordinator budget exhausted')
            for root in (self.out, *self.out.parents):
                value = os.statvfs(root)
                unitree.require(value.f_bavail * value.f_frsize >= 8 * GIB,
                    'unchanged eight-GiB artifact/backing reserve exhausted')
        if not hasattr(self, 'limits') or self.finalizing and not self.final_output_ready or \
                time.monotonic() - self.output_checked < .5:
            return
        size = 0
        for folder, dirs, names in os.walk(self.out, followlinks=False):
            for name in list(dirs) + names:
                path = Path(folder) / name
                try: info = path.lstat()
                except FileNotFoundError:
                    # The active compiler/Make/atomic publisher may retire a
                    # temporary after directory enumeration. This affects
                    # only the changing OWNED output size audit. Exact source,
                    # SDK, dependency, input and terminal catalogs never skip.
                    unitree.require(not self.finalizing, 'terminal owned output disappeared')
                    continue
                if path in self.overlay_paths:
                    if name in dirs: dirs.remove(name)
                    continue  # Exact owned overlays have separate mount/at-rest evidence.
                if stat.S_ISLNK(info.st_mode):
                    if not any(path.is_relative_to(root) for root in self.logical_output_roots):
                        target = path.resolve(strict=True)
                        unitree.require(target.is_relative_to(self.out) or any(target == root or
                            target.is_relative_to(root) for root in self.roots), 'owned non-SDK alias escapes closure')
                    size += info.st_size
                elif stat.S_ISREG(info.st_mode): size += info.st_size
                else: unitree.require(stat.S_ISDIR(info.st_mode), 'owned special output node refused')
        unitree.require(size <= self.limits['output_bytes'], 'complete owned native output budget exhausted')
        self.output_checked = time.monotonic()


def main():
    parser = unitree.arguments(__doc__)
    parser.add_argument('--selection-input', required=True)
    args = parser.parse_args(); frame = owner = None; mounted = []; failure = None
    try:
        frame = NativeFrame(args); common = frame.common
        frame.unitree = unitree
        unitree.require(os.geteuid() == 0 and os.uname().machine == 'x86_64', 'Root Linux x86_64 build required')
        unitree.require(all(re.fullmatch(r'/[A-Za-z0-9_./-]+', str(path)) for path in (frame.source, frame.out)),
            'explicit CMake flag path spellings must need no shell or list escaping')
        frozen, mf = common.document(frame.manifest); frame.frozen = frozen
        def source(relative):
            path = frame.source / relative; pin = frame.pin(path)
            unitree.require(frozen.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')},
                'new complete source freeze does not bind maintained native input: ' + relative)
            return path
        for relative in ('tests/ros_unitree/prepare_noetic_native.py', 'tests/ros_unitree/noetic_native_support.py'):
            source(relative)
        selection, _ = common.document(frame.input(args.selection_input))
        unitree.exact(selection, ('format_version', 'kind', 'acquisition', 'tools', 'sdk_tools', 'upstream'),
            'fresh Noetic native selection')
        unitree.require(selection['format_version'] == 1 and selection['kind'] == 'uwvm-unitree-noetic-native-selection-v1',
            'explicit fresh native selection required')
        unitree.exact(selection['acquisition'], ('inner', 'owner', 'manifest'), 'terminal authenticated acquisition pair/source')
        def document(label, maximum=64 * MIB):
            path = frame.input(label); raw, pin = common.body_pin(path, maximum)
            unitree.require(frame.pin(path) == pin, 'selected native prerequisite changed while decoded')
            return unitree.decode(raw), pin
        acquired, acquired_pin = document(selection['acquisition']['inner'])
        acquired_owner, owner_pin = document(selection['acquisition']['owner'])
        unitree.require(acquired['kind'] == 'authenticated-apt-sdk-acquisition' and
            acquired['status'] == 'PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT' and
            acquired['source_manifest_sha256'] == SOURCE35_SHA and acquired['acquisition_complete'] is True and
            acquired['target'] == {'backend': 1, 'distribution': 'noetic', 'ubuntu_codename': 'focal',
                'architecture': 'amd64', 'route': 'authenticated-apt-extraction'} and
            len(acquired['packages']) == 513 and len({(p['package'], p['version'], p['architecture']) for p in acquired['packages']}) == 513 and
            all(acquired.get(key) is True for key in ('source_preserved', 'inputs_preserved', 'products_preserved')) and
            acquired['cleanup']['status'] == 'PASS' and not acquired['cleanup']['remaining'] and
            all(row['status'] == 'PASS' and row.get('returncode') == 0 for row in acquired['commands']),
            'actual complete preserved freshly authenticated all513 Noetic acquisition required')
        unitree.require(acquired_owner['kind'] == 'noetic-r6-root-owned-fresh-all513-rebuild' and
            acquired_owner['status'] == 'PASS_AUTHENTICATED_FRESH_ALL513_INDEPENDENT_COW_CONTENT_SYSROOT_ONLY' and
            acquired_owner['acquisition_report'] == acquired_pin and acquired_owner['actual_sdk'] == acquired['sdk'] and
            acquired_owner['acquisition_completed'] is True and acquired_owner['cleanup']['status'] == 'PASS' and
            not acquired_owner['cleanup']['remaining'] and not acquired_owner['finalization_errors'] and
            all(acquired_owner.get(key) is True for key in ('source_preserved', 'inputs_preserved', 'products_preserved')),
            'actual independent successful Root acquisition owner required; partial SDK is inadmissible')
        acquisition_manifest = frame.input(selection['acquisition']['manifest'])
        unitree.require(frame.pin(acquisition_manifest)['sha256'] == SOURCE35_SHA and
            acquisition_manifest == Path(acquired['source']).parent / 'source-manifest.json',
            'separate complete current Source35 acquisition manifest required')
        acquisition_source = common.source_state(Path(acquired['source']), acquisition_manifest)
        # Old actor/cgroup identities remain provenance. This new producer's
        # own current cgroup is exclusively selected by its new closed Frame.
        original_sdk = Path(acquired['sdk']['root'])
        unitree.require(original_sdk.resolve(strict=True) == original_sdk and
            any(original_sdk == root or original_sdk.is_relative_to(root) for root in frame.roots) and
            original_sdk == Path(acquired_pin['path']).parent / 'sdk', 'entire actual SDK input root required')
        selected_catalog = acquired['sdk']['catalog']
        unitree.require(frame.pin(selected_catalog['path']) == selected_catalog, 'actual final authenticated SDK catalog changed')
        current_catalog = common.catalog(original_sdk, frame.out / 'authenticated-sdk-current.jsonl', frame.roots)
        unitree.require({k: v for k, v in current_catalog.items() if k != 'catalog'} ==
            {k: v for k, v in acquired['sdk'].items() if k != 'catalog'}, 'actual final acquisition SDK bytes/identities differ')
        unitree.exact(selection['tools'], ('mount', 'umount', 'chroot', 'python'), 'host native-build tools')
        tools = {name: frame.input(label) for name, label in selection['tools'].items()}
        unitree.require(all(os.access(path, os.X_OK) for path in tools.values()) and
            tools['python'] == Path(sys.executable).resolve(strict=True), 'exact explicit host tools/interpreter required')
        unitree.exact(selection['sdk_tools'], ('cxx', 'cmake', 'make', 'env', 'python', 'loader', 'master'),
            'explicit signed SDK tools')
        original_tools = {}
        for name, row in selection['sdk_tools'].items():
            unitree.exact(row, ('logical', 'input'), 'SDK tool spelling/current observation')
            actual = frame.pin(support.rooted(original_sdk, row['logical']))
            unitree.require(actual == frame.inputs[row['input']], 'selected SDK logical alias differs from actual current pin')
            original_tools[name] = actual
        unitree.require(selection['sdk_tools']['python']['logical'] == '/usr/bin/python3.8' and
            selection['sdk_tools']['master']['logical'] == '/opt/ros/noetic/bin/rosmaster' and
            selection['sdk_tools']['cxx']['logical'].startswith('/usr/bin/') and
            selection['sdk_tools']['cmake']['logical'] == '/usr/bin/cmake' and
            selection['sdk_tools']['make']['logical'] == '/usr/bin/make' and
            selection['sdk_tools']['env']['logical'] == '/usr/bin/env', 'actual Focal build/Master paths required')
        upstream_name = selection['upstream']; upstream = frame.config['upstreams'][upstream_name]
        unitree.require(upstream['repository'].removeprefix('https://github.com/').removesuffix('.git') ==
            'unitreerobotics/unitree_ros_to_real' and upstream['commit'] == COMMIT,
            'genuine selected official legacy upstream required')
        official_root = Path(upstream['root']); package = official_root / FAMILY
        unitree.require(package.is_dir() and not package.is_symlink(), 'complete original official catkin package required')
        profile, _ = common.document(source('sdk/robots/unitree/profiles/unitree_legged_msgs.json'))
        unitree.require(profile['family'] == FAMILY and profile['ros_version'] == 1 and profile['commit'] == COMMIT and
            profile['default_authority'] == [] and len(profile['definitions']) == 11 and
            {row['type'] for row in profile['definitions']} == {FAMILY + '/' + name for name in MESSAGES},
            'complete frozen eleven-type legacy profile required')
        own_mnt = os.readlink('/proc/self/ns/mnt'); parent_mnt = os.readlink('/proc/%d/ns/mnt' % os.getppid())
        own_net = os.readlink('/proc/self/ns/net'); parent_net = os.readlink('/proc/%d/ns/net' % os.getppid())
        unitree.require(own_mnt != parent_mnt and own_net != parent_net and
            [name for _, name in socket.if_nameindex()] == ['lo'],
            'Root must retain direct ownership of this offline private mount/network-namespace producer')
        frame.report.update(schema=SCHEMA, status='RUNNING', acquisition={'inner': acquired_pin, 'owner': owner_pin},
            acquisition_source_manifest_sha256=SOURCE35_SHA, mount_namespace={'actual': own_mnt, 'parent': parent_mnt},
            acquisition_source_current_state=acquisition_source,
            network_namespace={'actual': own_net, 'parent': parent_net, 'actual_interfaces': socket.if_nameindex()},
            unsupported_historical_six_TU_claim='NO_VERIFIED_REPORT_SELECTED_NOT_PROMOTED',
            expected_primary_roles=['identity_exporter', 'gateway', 'legacy_peer'], products={},
            ROS_TCPROS_executed=False, Runtime_executed=False, Guest_rights_changed=False,
            scope='FRESH_OFFICIAL11_GENERATION_INSTALL_NATIVE_COMPILE_AND_DESCRIPTOR_QUALIFIED_METADATA_ONLY')
        owner = unitree.Actors(frame)
        def run(label, argv, environment=None):
            child, row = owner.start(label, list(map(str, argv)), environment=environment or frame.environment,
                data=b'', json_output=False)
            owner.finish(child); return row
        def put(path, raw):
            path = Path(path); path.parent.mkdir(parents=True, exist_ok=True)
            with path.open('xb') as output: output.write(raw); output.flush(); os.fsync(output.fileno())
            return frame.pin(path)
        root = frame.out / 'sdk-rootfs'; work = frame.out / 'native-work'; work.mkdir(mode=0o700)
        frame.logical_output_roots.extend([root, work / 'official-package'])
        sdk_before = support.tree(common, original_sdk, frame.out / 'original-sdk-before.jsonl', frame.guard)
        clone = support.clone_tree(common, original_sdk, root, frame.out / 'sdk-copy.jsonl', frame.guard)
        sdk_copy = support.tree(common, root, frame.out / 'new-sdk-before-overlays.jsonl', frame.guard)
        unitree.require(sdk_before['semantic_sha256'] == sdk_copy['semantic_sha256'], 'complete independent SDK copy differs')
        frame.report['independent_sdk_copy'] = {**clone, 'original': sdk_before, 'actual_copy': sdk_copy}
        copied_package = work / 'official-package'
        package_before = support.tree(common, package, frame.out / 'official-package-before.jsonl', frame.guard)
        package_copy = support.clone_tree(common, package, copied_package, frame.out / 'official-package-copy.jsonl', frame.guard)
        copied_before = support.tree(common, copied_package, frame.out / 'official-package-copy-before.jsonl', frame.guard)
        unitree.require(package_before['semantic_sha256'] == copied_before['semantic_sha256'], 'unmodified official package copy differs')
        frame.report['official_package_copy'] = package_copy
        # This exact official commit installs include/${PROJECT_NAME}/ even
        # though its complete Git tree has no include directory.  Supply only
        # the two missing EMPTY directory containers in the independent copy;
        # never edit CMake/message/source bytes or inject a generated header.
        original_cmake = frame.pin(package / 'CMakeLists.txt')
        copied_cmake = frame.pin(copied_package / 'CMakeLists.txt')
        unitree.require(original_cmake['bytes'] == 753 and original_cmake['sha256'] ==
            'eb8de3144d953c97db7eccf9c07f964ba34b54fecd7f32c416308703ba76f6b9' and
            all(original_cmake[key] == copied_cmake[key] for key in ('bytes', 'sha256')),
            'empty include adaptation is restricted to the exact observed official CMake bytes')
        original_include, copied_include = package / 'include', copied_package / 'include'
        unitree.require(all(not path.exists() and not path.is_symlink() for path in
            (original_include, original_include / FAMILY, copied_include, copied_include / FAMILY)),
            'the exact two include containers must be absent before owned staging')
        for directory in (copied_include, copied_include / FAMILY):
            directory.mkdir(mode=0o700)
            directory.chmod(0o755)  # Finish metadata before its first catalog.
        def empty_include_tree():
            for directory in (copied_include, copied_include / FAMILY):
                info = directory.lstat()
                unitree.require(stat.S_ISDIR(info.st_mode) and not directory.is_symlink() and
                    directory.resolve(strict=True) == directory, 'ordinary owned include staging directories required')
            with os.scandir(copied_include) as entries:
                first = next(entries, None)
                unitree.require(first is not None and first.name == FAMILY and next(entries, None) is None,
                    'owned include parent may contain only its one package directory')
            with os.scandir(copied_include / FAMILY) as entries:
                unitree.require(next(entries, None) is None, 'owned package include directory must contain no header or alias')
        def package_catalog_rows(inventory):
            raw, pin = common.body_pin(Path(inventory['catalog']['path']), MIB)
            unitree.require(pin == inventory['catalog'] == frame.pin(pin['path']), 'complete package catalog changed')
            rows = {}
            for line in raw.splitlines():
                value = unitree.decode(line)
                unitree.require(value['relative'] not in rows, 'unique complete package catalog names required')
                rows[value['relative']] = value
            return rows
        empty_include_tree()
        staged_before = support.tree(common, copied_package, frame.out / 'official-package-staged-before.jsonl', frame.guard)
        copied_rows, staged_rows = package_catalog_rows(copied_before), package_catalog_rows(staged_before)
        added = {'include', 'include/' + FAMILY}
        unitree.require(set(staged_rows) == set(copied_rows) | added and not set(copied_rows) & added and
            all(staged_rows[name] == row for name, row in copied_rows.items()) and
            all(staged_rows[name]['kind'] == 'directory' and staged_rows[name]['mode'] == 0o755 for name in added),
            'owned source staging may add only the two ordinary empty directories, preserving every original copy row')
        adaptation = {'kind': 'EXACT_OFFICIAL_MISSING_INCLUDE_EMPTY_DIRECTORY_STAGING_V1',
            'official_commit': COMMIT, 'original_cmake': original_cmake, 'copied_cmake': copied_cmake,
            'original_before_clone': package_before, 'copied_before_adaptation': copied_before,
            'staged_before_build': staged_before,
            'added_directories': [{'path': str(copied_package / name), 'catalog_row': staged_rows[name]} for name in sorted(added)],
            'include_parent_entries': [FAMILY], 'package_include_entries': [],
            'all_preexisting_copy_rows_unchanged': True, 'original_upstream_edited': False,
            'copied_source_files_edited': False, 'headers_added_by_adaptation': False,
            'scope': 'Exact two empty directory containers in the owned package copy only; no whole-tree equality after staging is claimed.'}
        frame.report['official_package_staging_adaptation'] = adaptation
        frame.save()
        copied_tools = {name: frame.pin(support.rooted(root, row['logical'])) for name, row in selection['sdk_tools'].items()}
        for name, pin in copied_tools.items():
            unitree.require(all(pin[key] == original_tools[name][key] for key in ('bytes', 'sha256')),
                'new SDK tool bytes differ from authenticated original')
        noetic = support.rooted(root, '/opt/ros/noetic')
        libdirs = [support.rooted(root, value) for value in ('/opt/ros/noetic/lib', '/usr/lib/x86_64-linux-gnu', '/lib/x86_64-linux-gnu')]
        libdirs = list(dict.fromkeys(map(str, libdirs)))
        install = work / 'legacy-install'; (install / 'lib').mkdir(parents=True)
        libraries = ':'.join([str(install / 'lib'), *libdirs]); loader = copied_tools['loader']
        # The compiler/linker runs inside the copied SDK chroot, whereas the
        # resulting executable runs through the independent physical loader.
        # Transitive link-only searches therefore use logical chroot paths;
        # the persistent DT_RPATH still names the actual physical copy.  Keep
        # both choices explicit so a host library cannot satisfy a missing
        # authenticated SDK dependency during the native build.
        link_search = '/owned/legacy-install/lib:/opt/ros/noetic/lib:/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu'
        linker_flags = ('-Wl,--dynamic-linker=' + loader['path'] +
            ' -Wl,--disable-new-dtags,-rpath,' + libraries +
            ' -Wl,-rpath-link,' + link_search + ' -Wl,--trace')
        def native(argv):
            environment = ['PATH=/usr/bin:/bin', 'LANG=C.UTF-8', 'LC_ALL=C.UTF-8', 'HOME=/owned/home',
                'TMPDIR=/owned/tmp', 'PYTHONDONTWRITEBYTECODE=1', 'PYTHONNOUSERSITE=1',
                'PYTHONPATH=/opt/ros/noetic/lib/python3/dist-packages:/usr/lib/python3/dist-packages',
                'CMAKE_PREFIX_PATH=/owned/legacy-install:/opt/ros/noetic',
                'ROS_PACKAGE_PATH=/owned/legacy-install/share:/opt/ros/noetic/share',
                'LD_LIBRARY_PATH=/owned/legacy-install/lib:/opt/ros/noetic/lib:/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu',
                'MAKEFLAGS=-j1', 'OMP_NUM_THREADS=1', 'OPENBLAS_NUM_THREADS=1']
            return [tools['chroot'], root, selection['sdk_tools']['env']['logical'], '-i', *environment, *argv]
        for name in ('home', 'tmp'): (work / name).mkdir(mode=0o700)
        def mount_views():
            unitree.require(not mounted and os.readlink('/proc/self/ns/mnt') == own_mnt, 'exact owned private mount view required')
            run('native-mount-private-%d' % len(frame.report['commands']), [tools['mount'], '--make-rprivate', '/'])
            for origin, target, readonly in ((frame.source, root / 'work', True), (work, root / 'owned', False)):
                unitree.require(not target.is_symlink(), 'owned directory mount target cannot be an SDK alias')
                target.mkdir(parents=True, exist_ok=True)
                unitree.require(target.resolve(strict=True) == target and stat.S_ISDIR(target.lstat().st_mode),
                    'canonical ordinary owned directory mount target required')
                frame.overlay_paths.add(target)
                run('native-bind-%d' % len(frame.report['commands']), [tools['mount'], '--bind', origin, target])
                mounted.append(target)
                if readonly: run('native-readonly-%d' % len(frame.report['commands']),
                    [tools['mount'], '-o', 'remount,bind,ro', target])
            # Only the current host null character device is overlaid. It is
            # never inventoried as an ordinary SDK input and is retired before
            # a complete at-rest output/catalog or target metadata execution.
            null = Path('/dev/null').stat()
            unitree.require(stat.S_ISCHR(null.st_mode) and null.st_rdev == os.makedev(1, 3), 'actual basic null device required')
            target = root / 'dev/null'
            unitree.require(not target.parent.is_symlink() and not target.is_symlink(),
                'null mount target and parent cannot follow SDK aliases')
            target.parent.mkdir(parents=True, exist_ok=True)
            unitree.require(target.parent.resolve(strict=True) == target.parent,
                'canonical ordinary null mount parent required')
            if not target.exists(): target.touch(mode=0o600)
            unitree.require(not target.is_symlink() and stat.S_ISREG(target.lstat().st_mode), 'ordinary null placeholder required')
            frame.overlay_paths.add(target)
            run('native-null-%d' % len(frame.report['commands']), [tools['mount'], '--bind', '/dev/null', target]); mounted.append(target)
        def unmount_views():
            while mounted:
                target = mounted[-1]
                run('native-unmount-%d' % len(frame.report['commands']), [tools['umount'], target]); mounted.pop()
        cmake = selection['sdk_tools']['cmake']['logical']; cxx = selection['sdk_tools']['cxx']['logical']
        make = selection['sdk_tools']['make']['logical']; python = selection['sdk_tools']['python']['logical']
        def configure(label, project, build, extra):
            build.mkdir(mode=0o700)
            logical = lambda path: '/owned/' + str(Path(path).relative_to(work))
            # GNU9 with CMake3.16 can emit -MD/-MF without any explicit -MT.
            # Keep the official project unchanged and set the compile rule for
            # every configured target instead. Place the full dependency flags
            # after <FLAGS>, so an earlier target-specific -MF cannot select a
            # different file. Each object gets its own file and explicit rule
            # target; build_proof still verifies the real verbose argv and the
            # complete retained dependency rule, including all system headers.
            compile_object = ('<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> '
                '-MD -MF <OBJECT>.full.d -MT <OBJECT> -o <OBJECT> -c <SOURCE>')
            return run(label + '-configure', native([cmake, '-S', project, '-B', logical(build), '-G', 'Unix Makefiles',
                '-DCMAKE_CXX_COMPILER=' + cxx, '-DCMAKE_MAKE_PROGRAM=' + make, '-DPYTHON_EXECUTABLE=' + python,
                '-DCMAKE_CXX_COMPILE_OBJECT=' + compile_object,
                '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON', '-DCMAKE_BUILD_TYPE=', '-DCATKIN_ENABLE_TESTING=OFF',
                '-DBUILD_TESTING=OFF', '-DCMAKE_SKIP_RPATH=TRUE',
                '-DCMAKE_CXX_FLAGS=-O1 -g0 -UNDEBUG -fno-fast-math -ffp-contract=off -MD',
                '-DCMAKE_EXE_LINKER_FLAGS=' + linker_flags, '-DCMAKE_PREFIX_PATH=/owned/legacy-install;/opt/ros/noetic', *extra]))
        def build(label, folder, target):
            return run(label + '-build', native([cmake, '--build', '/owned/' + str(folder.relative_to(work)),
                '--target', target, '--verbose', '--', '-j1']))
        mount_views()
        official_build = work / 'official-build'
        configure('official11', '/owned/official-package', official_build,
            ['-DCATKIN_DEVEL_PREFIX=/owned/legacy-devel', '-DCMAKE_INSTALL_PREFIX=/owned/legacy-install'])
        build('official11-generate-and-install', official_build, 'install')
        header_pairs = []
        for name in MESSAGES:
            devel = work / 'legacy-devel/include' / FAMILY / (name + '.h')
            installed = install / 'include' / FAMILY / (name + '.h')
            unitree.require(not devel.is_symlink() and not installed.is_symlink(), 'fresh ordinary generated/installed headers required')
            a, b = frame.pin(devel), frame.pin(installed)
            unitree.require(all(a[key] == b[key] for key in ('bytes', 'sha256')), 'unmodified install overwrote fresh generated header')
            header_pairs.append({'message': name, 'fresh_devel': a, 'installed': b})
        official_db = official_build / 'compile_commands.json'
        unitree.require(not official_db.exists() or common.document(official_db)[0] == [],
            'official message generation unexpectedly introduced C++ TUs; actual inventory needs a new explicit review')
        frame.report['official11'] = {'source_copy': package_copy, 'headers': header_pairs,
            'primary_cpp_translation_units': 0, 'CMakeLists_unmodified': frame.pin(copied_package / 'CMakeLists.txt')}
        selected = work / 'identity-selection'; selected.mkdir(mode=0o700)
        includes = ''.join('#include <' + FAMILY + '/' + name + '.h>\n' for name in MESSAGES)
        body = '// Literal complete frozen legacy profile; confers no Guest capability.\n#pragma once\n'
        body += '#define UWVM_UNITREE_ROS_VERSION 1\n' + includes
        body += '#define UWVM_UNITREE_NATIVE_MESSAGES(M) \\\n' + ' \\\n'.join('  M(' + FAMILY + '::' + name + ', "' + FAMILY + '/' + name + '")' for name in MESSAGES) + '\n'
        put(selected / 'selection.hpp', body.encode())
        put(selected / 'selection.cmake', b'set(UWVM_UNITREE_FAMILY "unitree_legged_msgs")\nset(UWVM_UNITREE_ROS_VERSION "1")\n')
        for relative, name in (('sdk/integrations/unitree/CMakeLists.txt', 'CMakeLists.txt'),
                               ('sdk/integrations/unitree/native_identity.cpp', 'native_identity.cpp')):
            raw, pin = common.body_pin(source(relative)); copied = put(selected / name, raw)
            unitree.require(all(copied[key] == pin[key] for key in ('bytes', 'sha256')), 'unchanged exporter source staging differs')
        catalog = {'format': 'uwvm-ros-custom-bounded-v2', 'types': []}
        for row in profile['definitions']:
            relative = row['path']; raw, pin = common.body_pin(official_root / relative, MIB)
            put(selected / 'catalog' / relative, raw)
            catalog['types'].append({'ros_version': 1, 'type': row['type'], 'file': relative, 'bounds': row['bounds']})
        template = selected / 'catalog/catalog.preparation.json'
        put(template, (json.dumps(catalog, indent=2) + '\n').encode())
        export_build = work / 'identity-build'
        configure('legacy-identities', '/owned/identity-selection', export_build,
            ['-DUWVM_SOURCE=/work', '-DUWVM_UNITREE_SELECTION_DIR=/owned/identity-selection',
             '-DUWVM_UNITREE_CXX_STANDARD=17', '-DUWVM_UNITREE_CXX11_ABI=SDK_DEFAULT'])
        exporter_row = build('legacy-identities', export_build, 'unitree-native-identities')
        unmount_views()
        # The at-rest SDK remains an independent copy. Only known newly
        # created mount-point directories/null placeholder can differ from the
        # pre-overlay tree; no old char-device observation is used as a pin.
        def elf(common, path, kind=None):
            return support.elf(common, path, kind, guard=frame.guard)
        exporter = export_build / 'unitree-native-identities'
        exporter_proof = support.build_proof(frame, root, work, export_build, exporter_row,
            selected / 'native_identity.cpp', exporter, elf)
        exporter_proof['original_frozen_source'] = frame.pin(source('sdk/integrations/unitree/native_identity.cpp'))
        def file_catalog(label):
            paths = {}
            for base in (root, work):
                for folder, dirs, names in os.walk(base, followlinks=False):
                    dirs.sort(); names.sort()
                    for name in names:
                        path = Path(folder) / name
                        if path.is_symlink(): continue
                        unitree.require(path.is_file(), 'at-rest execution catalog contains special file')
                        pin = common.opened(path); paths[pin['path']] = pin
            output = frame.out / (label + '-execution-files.jsonl')
            with output.open('xb') as stream:
                for name in sorted(paths): stream.write((json.dumps(paths[name], sort_keys=True) + '\n').encode())
                stream.flush(); os.fsync(stream.fileno())
            return list(paths.values()), frame.pin(output)
        environment = {'LD_LIBRARY_PATH': libraries, 'PYTHONHOME': str(support.rooted(root, '/usr')),
            'PYTHONPATH': ':'.join(str(support.rooted(root, value)) for value in
                ('/opt/ros/noetic/lib/python3/dist-packages', '/usr/lib/python3/dist-packages')),
            'ROS_PACKAGE_PATH': str(install / 'share') + ':' + str(noetic / 'share'),
            'CMAKE_PREFIX_PATH': str(install) + ':' + str(noetic), 'ROS_DISTRO': 'noetic',
            'PATH': ':'.join(dict.fromkeys(str(support.rooted(root, value)) for value in ('/usr/bin', '/bin')))}
        execution_env = {**frame.environment, **environment, 'PYTHONNOUSERSITE': '1'}
        files, _ = file_catalog('before-export')
        metadata, metadata_pin, map_proof = support.stopped_export(frame, owner, frame.pin(exporter), files, execution_env)
        exported = unitree.decode(metadata)
        unitree.require(exported['format'] == 'uwvm-ros-official-bounded-native-v2' and exported['ros_version'] == 1 and
            set(exported['types']) == {FAMILY + '/' + name for name in MESSAGES} and
            all(re.fullmatch('[0-9a-f]{32}', value['native_identity']) for value in exported['types'].values()),
            'actual complete compiled ROS1 identities required')
        binding_row = run('bind-official11-native-identities', [tools['python'], '-B',
            source('tests/ros_custom_types/bounded-v2/bind_catalog.py'), '--template', template,
            '--native-metadata', metadata_pin['path'], '--output', work / 'bound/catalog.json'])
        binding_raw, binding_pin = common.body_pin(Path(binding_row['stdout']), 4 * MIB)
        binding = unitree.decode(binding_raw)
        unitree.require(binding['status'] == 'NATIVE_IDENTITY_BINDING_SOURCE_ONLY' and
            binding['official_export'] == {key: metadata_pin[key] for key in ('path', 'bytes', 'sha256')},
            'actual unchanged native binder provenance required')
        generated = work / 'generated'
        run('generate-complete-native-bound-legacy11', [tools['python'], '-B', source('sdk/tools/custom_types.py'),
            work / 'bound/catalog.json', '--out', generated])
        generation, generation_pin = common.document(generated / 'generation.json')
        unitree.require(generation['status'] == 'GENERATED_SOURCE_ONLY' and generation['native_registry_emitted'] is True and
            generation['format'] == 'uwvm-ros-custom-bounded-v2' and len(generation['records']) == 11 and
            generation['packages'] == {'ros1': [FAMILY], 'ros2': []} and generation['implicit_permissions'] == [] and
            set(generation['outputs']) == {'bounded_records.h', 'bounded_records.hpp', 'bounded_records.rs',
                'bounded_records.go', 'bounded_records.ts', 'bounded_records.mjs', 'bounded_interfaces.h',
                'bounded_interfaces.hpp', 'manifest.hpp', 'native_ros1.hpp', 'native_ros2.hpp'} and
            len(generation['types']) == 11 and {row['type']: row['native_identity'] for row in generation['types']} ==
            {name: row['native_identity'] for name, row in exported['types'].items()}, 'complete newly native-bound generator output required')
        for name, value in generation['outputs'].items():
            pin = frame.pin(generated / name)
            unitree.require({key: pin[key] for key in ('bytes', 'sha256')} == value, 'actual complete generated output differs')
        mount_views()
        gateway_build = work / 'gateway-build'
        configure('legacy-gateway', '/work/gateways/ros1', gateway_build,
            ['-DUWVM_ROS1_BUILD_PROBES=OFF', '-DUWVM_ROS1_CXX_STANDARD=17',
             '-DUWVM_ROS_CUSTOM_REGISTRY_HEADER=/owned/generated/manifest.hpp',
             '-DUWVM_ROS_CUSTOM_ROS1_HEADER=/owned/generated/native_ros1.hpp',
             '-DUWVM_ROS_CUSTOM_ROS1_PACKAGES=unitree_legged_msgs'])
        gateway_row = build('legacy-gateway', gateway_build, 'uwvm-ros1-gateway')
        peer_project = work / 'peer-project'; peer_project.mkdir(mode=0o700)
        raw, _ = common.body_pin(source('tests/ros_unitree/native_legacy_ros1_CMakeLists.txt'))
        put(peer_project / 'CMakeLists.txt', raw)
        peer_build = work / 'peer-build'
        configure('legacy-witness', '/owned/peer-project', peer_build,
            ['-DUWVM_SOURCE=/work', '-DUWVM_NOETIC_PREFIX=/opt/ros/noetic', '-DUWVM_LEGACY_PREFIX=/owned/legacy-install'])
        peer_row = build('legacy-witness', peer_build, 'unitree-native-legacy-ros1-peer')
        unmount_views()
        products = {'identity_exporter': exporter_proof,
            'gateway': support.build_proof(frame, root, work, gateway_build, gateway_row,
                source('gateways/ros1/main.cc'), gateway_build / 'uwvm-ros1-gateway', elf),
            'legacy_peer': support.build_proof(frame, root, work, peer_build, peer_row,
                source('tests/ros_unitree/native_legacy_ros1_peer.cpp'), peer_build / 'unitree-native-legacy-ros1-peer', elf)}
        for row in products.values():
            image = row['ELF']
            unitree.require(image['PT_INTERP'] == [loader['path']] and image['DT_RPATH'] == [libraries] and
                image['DT_RUNPATH'] == [], 'actual direct native loader/RPATH differs from independent SDK copy')
        files, catalog_pin = file_catalog('final')
        elf_files = []
        for pin in files:
            with Path(pin['path']).open('rb') as stream: header = stream.read(4)
            if header == b'\x7fELF': elf_files.append(elf(common, Path(pin['path'])))
        elf_catalog_pin = put(frame.out / 'execution-elf-catalog.json', (json.dumps(elf_files, sort_keys=True) + '\n').encode())
        final_sdk = support.tree(common, root, frame.out / 'execution-sdk-final.jsonl', frame.guard)
        final_work = support.tree(common, work, frame.out / 'execution-work-final.jsonl', frame.guard)
        sdk_copy_preservation = support.preserve_sdk_copy(common, sdk_copy, final_sdk, frame.guard)
        original_after = support.tree(common, original_sdk, frame.out / 'original-sdk-after.jsonl', frame.guard)
        package_after = support.tree(common, package, frame.out / 'official-package-after.jsonl', frame.guard)
        unitree.require(original_after['physical_sha256'] == sdk_before['physical_sha256'] and
            package_after['physical_sha256'] == package_before['physical_sha256'], 'original complete SDK/upstream changed')
        empty_include_tree()
        copied_after = support.tree(common, copied_package, frame.out / 'official-package-copy-after.jsonl', frame.guard)
        unitree.require(copied_after['physical_sha256'] == staged_before['physical_sha256'],
            'official source copy or explicitly empty staging directories changed during its build')
        adaptation.update(staged_after_build=copied_after, empty_include_tree_preserved=True,
            all_staged_physical_rows_preserved=True)
        launch = {name: {'executable': row['binary'], 'arguments': []} for name, row in products.items()
            if name in ('gateway', 'legacy_peer')}
        launch['master'] = {'executable': loader, 'arguments': ['--library-path', libraries,
            copied_tools['python']['path'], copied_tools['master']['path'], '--core', '-p', '{port}'],
            'python': copied_tools['python'], 'script': copied_tools['master'], 'port_placeholder': '{port}'}
        frame.report.update(products=products, qualified_primary_translation_units=sum(row['primary_translation_units'] for row in products.values()),
            configure_probe_policy='ACTUAL_CONFIGURE_LOGS_RETAINED_NOT_COUNTED_AS_PRIMARY_TUS',
            family={'name': FAMILY, 'ros_version': 1, 'message_count': 11, 'profile': frame.pin(source('sdk/robots/unitree/profiles/unitree_legged_msgs.json')),
                'catalog': frame.pin(work / 'bound/catalog.json'), 'binding_receipt': binding_pin,
                'native_metadata': metadata_pin, 'native_export_mapping': map_proof,
                'generation': frame.pin(generated / 'generation.json'), 'registry': frame.pin(generated / 'manifest.hpp'),
                'native_header': frame.pin(generated / 'native_ros1.hpp'),
                'generated_directory': str(generated), 'all_generated_outputs': {name: frame.pin(generated / name) for name in generation['outputs']}},
            execution_file_catalog=catalog_pin, execution_elf_catalog=elf_catalog_pin,
            execution_view_after={'sdk': final_sdk, 'work': final_work}, launch=launch, environment=environment,
            copied_sdk_preservation=sdk_copy_preservation,
            host_tools={name: frame.pin(path) for name, path in tools.items()},
            sdk_tools={name: {'logical': selection['sdk_tools'][name]['logical'],
                'source_current': original_tools[name], 'copy_actual': pin} for name, pin in copied_tools.items()},
            original_sdk_preserved=True, original_official_upstream_preserved=True,
            official_source_copy_unmodified=True, all_target_products_fresh=True,
            official_source_copy_scope='ALL_PREEXISTING_SOURCE_ROWS_UNCHANGED_WITH_EXPLICIT_TWO_EMPTY_DIRECTORY_ADAPTATION',
            metadata_export_executed=True, native_contexts_executed=0,
            unsupported_historical_six_TU_claim='UNVERIFIED_DOCUMENTATION_NO_OLD_REPORT_PROMOTED')
        # The at-rest execution closures must still match after every child
        # retires. The outer Root owner independently checks this report and
        # its source/input/product evidence before any future TCPROS launch.
        frame.report['at_rest_preservation_before_cleanup'] = {'sdk': final_sdk, 'work': final_work}
        frame.report['prepared_terminal_status'] = STATUS
    except BaseException as error:
        failure = repr(error)
        if frame is None: raise
        frame.report.update(error=failure, traceback=traceback.format_exc())
    finally:
        if frame is not None:
            frame.finalizing = True; frame.output_checked = 0
            errors = []
            closed_owner = owner; closed_ok = False
            if owner is not None:
                try:
                    result = owner.close()
                    unitree.require(result['status'] == 'PASS', 'all owned build children must retire before unmount')
                    frame.report.setdefault('cleanup', []).append(result)
                    closed_ok = True
                except BaseException as error: errors.append('process_cleanup: ' + repr(error))
            # Actors uses the exact same held pidfds for cleanup. Do not start
            # an unmount while an unproven compiler/exporter can still exist.
            if mounted and not errors:
                try:
                    # Keep the closed actor's list position until the new
                    # serial cleanup actor has chosen its DISTINCT retirement
                    # ledger basename. No closed actor is closed twice.
                    owner = unitree.Actors(frame)
                    while mounted:
                        target = mounted[-1]
                        run('native-final-unmount-%d' % len(frame.report['commands']), [tools['umount'], target]); mounted.pop()
                except BaseException as error: errors.append('mount_cleanup: ' + repr(error))
            if closed_ok:
                frame.actors.remove(closed_owner)
            frame.report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
                if line.split()[4] == str(frame.out) or line.split()[4].startswith(str(frame.out) + '/')]
            if frame.report['remaining_owned_mounts']: errors.append('owned mount remained')
            if not failure and not errors:
                try:
                    empty_include_tree()
                    after_sdk = support.tree(frame.common, root, frame.out / 'execution-sdk-preserved.jsonl', frame.guard)
                    after_work = support.tree(frame.common, work, frame.out / 'execution-work-preserved.jsonl', frame.guard)
                    prior = frame.report['execution_view_after']
                    unitree.require(after_sdk['physical_sha256'] == prior['sdk']['physical_sha256'] and
                        after_work['physical_sha256'] == prior['work']['physical_sha256'], 'complete native execution closure changed at cleanup')
                    frame.report['execution_view_preserved'] = True
                    adaptation['post_cleanup_empty_include_tree_preserved'] = True
                    unitree.require(frame.common.source_state(Path(acquired['source']), acquisition_manifest) ==
                        acquisition_source, 'complete separate acquisition source role changed')
                    frame.report['acquisition_source_preserved'] = True
                except BaseException as error: errors.append('execution_preservation: ' + repr(error))
            frame.report['native_finalization_errors'] = errors
            if failure or errors: frame.report['status'] = 'FAIL'
            else: frame.report['status'] = STATUS
            frame.final_output_ready = not mounted and not errors
            frame.output_checked = 0
            # Frame.finish normally recognizes PASS_* only. This preparation
            # intentionally uses PREPARED_* and returns success only after its
            # full cleanup/source/input/root preservation is explicitly done.
            frame.finish(frame.report['status'], failure)
    return 0 if frame is not None and frame.report['status'] == STATUS and not errors else 1


if __name__ == '__main__':
    raise SystemExit(main())
