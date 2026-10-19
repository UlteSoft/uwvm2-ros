"""Own one locked upstream native-suite cell; Root supplies the admitted context.

This module has no project import, actor or filesystem mutation on import.
``run_cell`` is called by Root's source-pinned current64 supervisor, in its
already owned private loopback namespace. It executes actual original upstream
workflows; configuration, discovery and native results never imply Wasm passed.
"""
from __future__ import annotations
import hashlib
import json
import os
import platform
from pathlib import Path
import re
import stat
import sys
import time
import types

MIB, GIB = 1024**2, 1024**3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
CG = {'memory_max', 'memory_swap_max', 'expected_boot_id', 'expected_leaf', 'expected_device', 'expected_inode', 'expected_cpus'}
TOKEN = re.compile(r'[a-z][a-z0-9_.-]{0,79}')
REPOSITORY = re.compile(r'https://[^\s\x00]+')
ENV = {'PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH', 'CMAKE_PREFIX_PATH', 'AMENT_PREFIX_PATH', 'ROS_PACKAGE_PATH',
       'PKG_CONFIG_PATH', 'ROS_DISTRO', 'ROS_VERSION', 'ROS_PYTHON_VERSION', 'RMW_IMPLEMENTATION',
       'ROS_DOMAIN_ID', 'ROS_MASTER_URI', 'ROS_IP', 'ROS_LOCALHOST_ONLY'}
PLAN = {'format_version', 'kind', 'cgroup', 'network_namespace', 'mount_namespace', 'profile', 'upstream_metadata', 'source_lock',
        'tools', 'roots', 'environment', 'snapshots', 'suites', 'limits', 'differential_reports', 'official_mapping', 'coverage_gaps'}


def need(value, reason):
    if not value: raise ValueError(reason)


def decode(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            need(key not in result, 'duplicate native-suite control member'); result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite native-suite control'))


def run_cell(common, unitree, *, source, manifest_current, plan_current, out, guard):
    """Run real native source/build/discovery/test commands with owned pidfds.

    Root MUST admit ``common``, ``unitree`` and these exact bytes against its
    complete source manifest before this call. The guard is still called before
    any plan/helper processing and throughout all I/O/actor supervision. Root
    supplies actual current full7s; no path lookup selects a toolchain/SDK/tag.
    Original source repositories stay untouched; native builds run in fresh
    independent local Git clones with ``--no-hardlinks`` and no network access.
    """
    guard(); source, out = Path(source), Path(out)
    need(sys.platform == 'linux' and hasattr(os, 'pidfd_open'), 'actual admitted Linux pidfd lane required')
    pins, products, catalogs = {}, {}, []; owner = None; started = time.monotonic(); before = None; finalizing = False; cleanup_started = None
    next_output_walk = 0.0
    def pin(expected, limit=2 * GIB):
        guard(); need(type(expected) is dict and set(expected) == PIN, 'actual selected full7 required')
        actual = common.opened(Path(expected['path']), limit)
        need(actual == expected and (actual['path'] not in pins or pins[actual['path']] == actual), 'actual selected input changed')
        pins[actual['path']] = actual; return actual
    def document(expected, limit=64 * MIB):
        pin(expected, limit); raw, current = common.body_pin(Path(expected['path']), limit)
        need(current == expected, 'selected opened JSON changed'); return decode(raw)
    manifest = document(manifest_current, 16 * MIB)
    need(source.resolve(strict=True) == source and Path(manifest_current['path']) == source.parent / 'source-manifest.json',
         'actual complete frozen source/manifest required')
    def module(relative, name):
        raw, actual = common.body_pin(source / relative, MIB)
        need(manifest.get(relative) == {key: actual[key] for key in ('bytes', 'sha256')}, 'current manifest-bound official-suite helper required')
        pin(actual); value = types.ModuleType(name); value.__file__ = actual['path']; sys.modules[name] = value
        exec(compile(raw, value.__file__, 'exec'), value.__dict__); return value
    for helper, relative in ((common, 'tools/ros_distro/common.py'), (unitree, 'tests/ros_unitree/common.py')):
        need(Path(helper.__file__).resolve(strict=True) == source / relative, 'Root must supply the exact current admitted helper role')
        actual = common.opened(source / relative)
        need(manifest.get(relative) == {key: actual[key] for key in ('bytes', 'sha256')}, 'passed helper source differs from complete manifest'); pin(actual)
    own = common.opened(Path(__file__).resolve(strict=True))
    need(own['path'] == str(source / 'tools/ros_distro/official_suites/runner.py') and
         manifest.get('tools/ros_distro/official_suites/runner.py') == {key: own[key] for key in ('bytes', 'sha256')}, 'actual frozen runner required'); pin(own)
    formats = module('tools/ros_distro/official_suites/formats.py', 'actual_official_suite_formats')
    differential = module('tools/ros_distro/official_suites/differential.py', 'actual_official_case_differential')
    plan = document(plan_current, 16 * MIB)
    need(type(plan) is dict and set(plan) in (PLAN,PLAN|{'literal_provider_reports'}) and plan['format_version'] == 1 and
         plan['kind'] == 'owned-official-upstream-suite-cell-v1', 'closed official native-suite plan required')
    context = plan['cgroup']
    need(set(context) == CG and context['memory_max'] == 64 * GIB and context['memory_swap_max'] == 0,
         'actual current64-GiB zero-swap context required')
    before = common.cgroup_state(context)
    need(all(row['events'].get(key, 0) == 0 for row in before['ancestors'] for key in ('oom', 'oom_kill', 'oom_group_kill')),
         'all initial ancestor OOM counts must be zero')
    actual_net = os.readlink('/proc/self/ns/net')
    need(plan['network_namespace'] == actual_net and actual_net != os.readlink('/proc/%d/ns/net' % os.getppid()),
         'actual active owned private network namespace required; no ID forecast')
    interface_proof = unitree.current_loopback(actual_net)
    actual_mount = os.readlink('/proc/self/ns/mnt')
    need(plan['mount_namespace'] == actual_mount and actual_mount != os.readlink('/proc/%d/ns/mnt' % os.getppid()),
         'actual owned private mount namespace required; baseline namespaces may not be reused')
    need(out.is_absolute() and out.parent.resolve(strict=True) == out.parent and not out.exists() and not out.is_symlink() and
         source != out and source not in out.parents and out not in source.parents, 'fresh owned output outside current source required')
    roots = [Path(value) for value in plan['roots']]
    need(1 <= len(roots) <= 64 and len(set(roots)) == len(roots) and all(root.resolve(strict=True) == root and
         root.is_dir() and root != Path('/') and root != out and root not in out.parents and out not in root.parents for root in roots),
         'finite complete SDK/tool/snapshot roots outside live output required')
    def readonly_inputs():
        guard()
        need(os.readlink('/proc/self/ns/mnt') == actual_mount, 'owned provider mount namespace changed')
        with Path('/proc/self/mountinfo').open('rb') as stream: raw = stream.read(4*MIB+1)
        need(len(raw) <= 4*MIB, 'bounded actual mount inventory required')
        def spelling(raw):
            return re.sub(rb'\\([0-7]{3})', lambda match: bytes([int(match[1],8)]), raw).decode('utf-8','surrogateescape')
        mounts = []
        for line in raw.splitlines():
            left, separator, right = line.partition(b' - '); fields = left.split()
            need(separator and len(fields) >= 6, 'actual mountinfo row malformed')
            mounts.append((Path(spelling(fields[4])), fields[5].split(b',')))
        proof = []
        for root in [source, *roots]:
            enclosing = [(path, flags) for path, flags in mounts if root == path or root.is_relative_to(path)]
            need(enclosing, 'actual input mount absent')
            selected, flags = max(enclosing, key=lambda item: len(item[0].parts))
            nested = [(path, value) for path,value in mounts if path != root and path.is_relative_to(root)]
            need(b'ro' in flags and all(b'ro' in value for path,value in nested),
                 'original source/SDK/tool inputs and their nested mounts must be read-only in owned private view')
            proof.append({'root':str(root), 'mountpoint':str(selected), 'mount_options':[value.decode('ascii') for value in flags],
                          'nested_readonly_mounts':len(nested)})
        return {'namespace':actual_mount, 'providers':proof, 'mountinfo_sha256':hashlib.sha256(raw).hexdigest()}
    provider_view = readonly_inputs()
    limits = plan['limits']
    need(set(limits) == {'rss_bytes', 'command_seconds', 'log_bytes', 'output_bytes', 'wall_seconds'} and
         all(type(value) is int and value > 0 for value in limits.values()) and limits['rss_bytes'] <= 16 * GIB and
         limits['command_seconds'] <= 7200 and limits['wall_seconds'] <= 86400 and limits['log_bytes'] <= 64 * MIB and
         limits['output_bytes'] <= 16 * GIB, 'finite native-suite resource budgets required')
    # Invoke the maintained validator through its exact source-bound common
    # dependency. A previously imported historical 'common' is not this role.
    previous_common = sys.modules.get('common')
    sys.modules['common'] = common
    try: profile_api = module('tools/ros_distro/profile.py', 'actual_official_suite_profile')
    finally:
        if previous_common is None: sys.modules.pop('common', None)
        else: sys.modules['common'] = previous_common
    profile = profile_api.validate(document(plan['profile'], 16 * MIB))
    need(profile['execution']['kind'] == 'host' and all(profile['runtime'][key] == context[key] for key in CG),
         'suite runs inside admitted caller cgroup, never an unproven OCI daemon')
    catalog = document(common.opened(source / 'tools/ros_distro/distributions.json'))
    target = (profile['backend'], profile['distro'])
    need(target in {(row['backend'], row['distro']) for row in catalog['distributions']}, 'one of26 actual named targets required')
    metadata = document(plan['upstream_metadata']); lock = document(plan['source_lock'])
    need(lock.get('kind') == 'official-upstream-source-lock-v1' and lock.get('backend') == target[0] and
         lock.get('distro') == target[1] and lock.get('upstream_metadata') == plan['upstream_metadata'],
         'actual target-specific source-lock/metadata association required')
    tools = plan['tools']; need(type(tools) is dict and {'git', 'ctest', 'python'} <= set(tools) and len(tools) <= 64, 'explicit Git/CTest and selected native tools required')
    for name, selected in tools.items():
        need(TOKEN.fullmatch(name) and any(Path(selected['path']).is_relative_to(root) for root in roots), 'selected native tool lacks complete root coverage')
        pin(selected); need(os.access(selected['path'], os.X_OK), 'selected tool is not executable')
    need(Path(sys.executable).resolve(strict=True) == Path(tools['python']['path']) ==
         Path(profile['execution']['python']).resolve(strict=True), 'actual selected current coordinator Python required')
    need(profile['runtime']['mode'] in ('native', 'qemu'), 'build-only cannot execute official native suites')
    def covered(path):
        value = Path(path)
        need(value.is_absolute() and value.resolve(strict=True) == value and
             any(value == root or value.is_relative_to(root) for root in roots),
             'actual canonical provider lacks selected whole-root coverage')
        return value
    for value in [*profile['sdk']['roots'], *profile['tools']['roots']]:
        need(covered(value).is_dir(), 'actual selected SDK/tool provider directory required')
    for value in (profile['sdk']['ros_prefix'],):
        need(covered(value).is_dir(), 'actual physical ROS prefix required')
    for value in (profile['sdk']['setup'], profile['sdk']['distribution_index'], profile['sdk']['source_lock'],
                  profile['tools']['toolchain_file']):
        if value is not None: pin(common.opened(covered(value)))
    provider = None; loader_pin = None
    if profile['runtime']['mode'] == 'qemu':
        provider = covered(profile['tools']['sysroot'])
        need(provider.is_dir(), 'actual canonical selected target sysroot required')
        loader = covered(profile['runtime']['loader'])
        need(loader.is_relative_to(provider) and any(value['path'] == str(loader) for value in tools.values()),
             'actual selected target loader must be pinned within inventoried sysroot')
        loader_pin = common.opened(loader); pin(loader_pin)
        need(any(value['path'] == profile['runtime']['qemu'] for value in tools.values()),
             'actual selected host QEMU full7 required')
    else:
        host = {'amd64': 'x86_64', 'arm64': 'aarch64', 'i686': 'i386', 'i386': 'i386'}.get(platform.machine().lower(), platform.machine().lower())
        need(profile['target']['architecture'] == host, 'native target must equal actual kernel host architecture')
    def target_elf(selected):
        path = Path(selected['path']); pin(selected)
        with path.open('rb') as stream: header = stream.read(64)
        need(common.opened(path) == selected and len(header) >= 20 and header[:4] == b'\x7fELF' and
             header[5] in (1, 2), 'stable actual selected target ELF required')
        endian = {1: 'little', 2: 'big'}[header[5]]
        need((header[4], endian, int.from_bytes(header[18:20], endian)) ==
             ({32: 1, 64: 2}[profile['target']['bits']], profile['target']['endian'], profile['target']['elf_machine']),
             'actual selected target ELF architecture differs from validated profile')
        return {'file': selected, 'elf_class': header[4], 'elf_data': header[5],
                'e_machine': int.from_bytes(header[18:20], endian)}
    if loader_pin is not None: target_elf(loader_pin)
    need(type(plan['environment']) is dict and 'PATH' in plan['environment'] and set(plan['environment']) <= ENV and
         all(type(value) is str and '\0' not in value and '\n' not in value for value in plan['environment'].values()), 'explicit native SDK environment required')
    for key in ('PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH', 'CMAKE_PREFIX_PATH', 'AMENT_PREFIX_PATH', 'ROS_PACKAGE_PATH', 'PKG_CONFIG_PATH'):
        if key not in plan['environment']: continue
        need(plan['environment'][key] and all(value and Path(value).is_absolute() and Path(value).resolve(strict=True).is_dir() and
             any(Path(value).resolve(strict=True) == root or Path(value).resolve(strict=True).is_relative_to(root) for root in roots)
             for value in plan['environment'][key].split(':')), 'native SDK search paths require explicit complete physical inventories')
    need(type(plan['coverage_gaps']) is list and type(plan['snapshots']) is list and 0 < len(plan['snapshots']) <= 256 and
         type(plan['suites']) is list and 0 < len(plan['suites']) <= 512, 'finite explicit source and suite selections required')
    suite_ids = [value.get('id') for value in plan['suites']]
    need(all(type(value) is str and TOKEN.fullmatch(value) for value in suite_ids) and
         len(set(suite_ids)) == len(suite_ids), 'distinct selected suite IDs required')
    out.mkdir(mode=0o700)
    for name in ('home', 'tmp', 'cache', 'workspaces'): (out / name).mkdir(mode=0o700)
    report = {'format_version': 1, 'kind': 'owned-official-native-upstream-suite-cell', 'status': 'RUNNING',
        'source': str(source), 'source_manifest_sha256': manifest_current['sha256'], 'configuration': plan_current,
        'backend': target[0], 'distro': target[1], 'native_execution_mode': profile['runtime']['mode'], 'profile': plan['profile'], 'upstream_metadata': plan['upstream_metadata'],
        'source_lock': plan['source_lock'], 'actual_network_namespace': actual_net, 'actual_loopback': interface_proof, 'readonly_provider_view': provider_view,
        'snapshots': [], 'suites': [], 'coverage_gaps': list(plan['coverage_gaps']), 'differential_evidence': [],
        'physical_robot_access': False, 'Guest_authority': 'UNCHANGED_EMPTY', 'all_official_tests_qualified': False,
        'Wasm_official_suite_executed': False, 'commands': [], 'cleanup': {'status': 'NOT_PERFORMED'},
        'catalog_scope': 'Exact source-bound common.catalog walks every directory entry, including .git/node_modules/target; its unused EXCLUDED constant is not an inventory exclusion.'}
    def save():
        if owner is not None: report['commands'] = owner.commands
        report['input_pins'] = list(pins.values()); report['product_pins'] = list(products.values())
        common.atomic_json(out / 'report.json', report)
    def live():
        nonlocal next_output_walk
        guard(); common.cgroup_preserved(before, common.cgroup_state(context))
        need(os.readlink('/proc/self/ns/net') == actual_net, 'owned native-suite namespace changed')
        now = time.monotonic()
        need((cleanup_started is not None and now - cleanup_started < 600) if finalizing else
             now - started < limits['wall_seconds'], 'independent active or600-second cleanup deadline exhausted')
        if not finalizing:
            for path in (out, *out.parents):
                fs = os.statvfs(path); need(fs.f_bavail * fs.f_frsize >= 8 * GIB, 'backing reserve below8GiB')
        if time.monotonic() < next_output_walk: return
        readonly_inputs()
        total = 0
        for folder, dirs, files in os.walk(out, followlinks=False):
            for name in files:
                item = Path(folder) / name
                try:total += item.lstat().st_size
                except FileNotFoundError as error:
                    # The actual Gateway listener guard unlinks exactly this
                    # owned Unix socket on normal Worker retirement. Ignore
                    # only that ENOENT race in advisory output-size accounting.
                    # Every ordinary input/pin/catalog still reopens strictly.
                    if not differential.retired_case_socket(out,item,error):raise
        need(total <= limits['output_bytes'], 'whole official-suite output budget exhausted')
        next_output_walk = time.monotonic() + .5
    def product(path, maximum=64 * MIB):
        live(); value = common.opened(Path(path), maximum)
        need(value['path'] not in products or products[value['path']] == value, 'retained official-suite output changed')
        products[value['path']] = value; return value
    # A copied Focal SDK has absolute CHROOT aliases. Following their names on
    # the host could either fail closure or falsely qualify unrelated host
    # targets. The opt-in is finite, source/schema-bound and selects only an
    # ALREADY inventoried whole native producer root. No shared common module,
    # input-root set or regular physical mapping boundary is modified.
    literal_reports=plan.get('literal_provider_reports',[])
    need(type(literal_reports) is list and len(literal_reports)<=16,'finite explicit literal provider reports required')
    literal_roots={};literal_support=None
    for expected in literal_reports:
        selected=pin(expected);native=document(selected)
        root=Path(selected['path']).parent
        need(root in roots and selected['path']==str(root/'report.json') and root not in literal_roots and
            native.get('schema')=='uwvm-unitree-noetic-native-products-v1' and
            native.get('status')=='PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED' and
            native.get('source')==str(source) and native.get('source_manifest_sha256')==manifest_current['sha256'] and
            native.get('qualified_primary_translation_units')==3 and not native.get('native_finalization_errors') and
            all(native.get(name) is True for name in ('source_preserved','input_pins_preserved','whole_roots_preserved',
                'original_sdk_preserved','original_official_upstream_preserved','official_source_copy_unmodified',
                'execution_view_preserved','acquisition_source_preserved')) and
            type(native.get('cleanup')) is list and native['cleanup'] and
            all(value.get('status')=='PASS' and not value.get('remaining') for value in native['cleanup']) and
            set(native['execution_view_after'])=={'sdk','work'} and
            native['execution_view_after']['sdk']['root']==str(root/'sdk-rootfs') and
            native['execution_view_after']['work']['root']==str(root/'native-work'),
            'only exact already-selected successful current-source Noetic producer root may use literal CHROOT catalog')
        literal_roots[root]=selected
    if literal_roots:literal_support=module('tests/ros_unitree/noetic_native_support.py','official_literal_native_provider_inventory')
    def inventory(root,path):
        if root in literal_roots:
            value=literal_support.tree(common,root,path,live)
            return dict(value,catalog_format='literal-chroot-tree-v1',literal_provider_report=literal_roots[root])
        return common.catalog(root,path,roots)
    environment = {**plan['environment'], 'HOME': str(out / 'home'), 'TMPDIR': str(out / 'tmp'), 'XDG_CACHE_HOME': str(out / 'cache'),
        'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8', 'PYTHONDONTWRITEBYTECODE': '1', 'GIT_OPTIONAL_LOCKS': '0',
        'GIT_CONFIG_NOSYSTEM': '1', 'GIT_CONFIG_GLOBAL': '/dev/null', 'GIT_TERMINAL_PROMPT': '0',
        'MAKEFLAGS': '-j1', 'CMAKE_BUILD_PARALLEL_LEVEL': '1', 'OMP_NUM_THREADS': '1', 'PIP_NO_INDEX': '1'}
    snapshots = {}; original_snapshots = {}; tracked = {}; source_before = None
    def target_prefix():
        if profile['runtime']['mode'] == 'native': return []
        emulator = profile['runtime']['qemu']
        need(any(value['path'] == emulator for value in tools.values()) and profile['tools']['sysroot'],
             'explicit pinned user-QEMU and actual target sysroot required')
        return [emulator, '-L', profile['tools']['sysroot']]
    def command(label, argv, cwd, env=None):
        row = owner.run(label, argv, cwd, environment if env is None else env); save(); return row
    def output(row):
        raw, actual = common.body_pin(Path(row['log']['path']), limits['log_bytes']); need(actual == row['log'], 'actual command log changed'); return raw
    try:
        source_before = common.source_state(source, Path(manifest_current['path']))
        for index, root in enumerate(roots):
            live(); row = inventory(root, out / ('inputs-before-%02d.jsonl' % index)); product(row['catalog']['path'], 512 * MIB); catalogs.append(row)
        owner = common.SerialRunner(out, {key: limits[key] for key in ('rss_bytes', 'command_seconds', 'log_bytes')}, live)
        git = tools['git']['path']
        for selected in plan['snapshots']:
            need(set(selected) == {'id', 'repository', 'commit', 'root'} and TOKEN.fullmatch(selected['id']) and
                 selected['id'] not in snapshots and REPOSITORY.fullmatch(selected['repository']) and
                 re.fullmatch('[0-9a-f]{40}', selected['commit']), 'closed pinned native source snapshot required')
            need(selected in lock['snapshots'], 'source selection absent from actual reviewed target source lock')
            root = Path(selected['root']); need(root.resolve(strict=True) == root and any(root == base or root.is_relative_to(base) for base in roots), 'snapshot lacks complete physical root inventory')
            base = [git, '--no-optional-locks', '-c', 'core.fsmonitor=false', '-C', str(root)]
            head = output(command(selected['id'] + '-head', [*base, 'rev-parse', 'HEAD'], out)).decode().strip()
            # SerialRunner prefixes timing/accounting nowhere; stdout/stderr are
            # merged raw bytes. Git progress is suppressed on the selected calls.
            need(head == selected['commit'], 'actual snapshot HEAD differs from locked commit')
            origin = output(command(selected['id'] + '-origin', [*base, 'remote', 'get-url', 'origin'], out)).decode().strip()
            need(origin == selected['repository'], 'snapshot remote differs from original selected upstream')
            clean = output(command(selected['id'] + '-clean', [*base, 'status', '--porcelain=v1', '--untracked-files=all'], out))
            need(not clean, 'dirty upstream snapshot refused')
            tree = output(command(selected['id'] + '-tree', [*base, 'ls-tree', '-r', '-z', '--full-tree', selected['commit']], out))
            entries = []
            for entry in tree.split(b'\0'):
                if not entry: continue
                left, name = entry.split(b'\t', 1); mode, kind, digest = left.split()
                need(kind == b'blob' and mode in (b'100644', b'100755', b'120000'), 'unresolved submodule/special Git entry is an acquisition gap')
                relative = name.decode('utf-8', 'strict')
                need(relative and not relative.startswith('/') and '..' not in Path(relative).parts and '\n' not in relative,
                     'clean complete Git source path required')
                path = root / relative
                if mode == b'120000': raw = os.fsencode(os.readlink(path)); blob = hashlib.sha1(b'blob ' + str(len(raw)).encode() + b'\0' + raw).hexdigest()
                else:
                    current = common.opened(path); pin(current); blob_hash = hashlib.sha1(b'blob ' + str(current['bytes']).encode() + b'\0')
                    with path.open('rb') as stream:
                        while True:
                            live(); block = stream.read(MIB)
                            if not block: break
                            blob_hash.update(block)
                    need(common.opened(path) == current, 'source changed during Git blob verification'); blob = blob_hash.hexdigest()
                need(blob == digest.decode(), 'actual source body differs from committed Git blob')
                entries.append({'path': relative, 'mode': mode.decode(), 'blob': blob})
            need(entries and len(entries) <= 200000, 'bounded complete committed snapshot required')
            destination = out / 'workspaces' / selected['id']
            command(selected['id'] + '-independent-clone', [git, '-c', 'core.fsmonitor=false', 'clone', '--quiet', '--local', '--no-hardlinks', '--no-checkout', '--', str(root), str(destination)], out)
            command(selected['id'] + '-owned-checkout', [git, '--no-optional-locks', '-C', str(destination), 'checkout', '--quiet', '--detach', selected['commit']], out)
            snapshots[selected['id']] = destination; original_snapshots[selected['id']] = root
            tracked[selected['id']] = {entry['path']: entry for entry in entries}
            # Actual copied file identities are independent, not merely a clone
            # option assertion. Git's selected original blobs remain the basis.
            for entry in entries:
                relative = entry['path']; copied = destination / relative; original = root / relative
                if entry['mode'] == '120000':
                    need(copied.is_symlink() and os.readlink(copied) == os.readlink(original), 'owned source symlink differs from original Git tree')
                else:
                    old = common.opened(original); new = common.opened(copied)
                    need((old['bytes'], old['sha256']) == (new['bytes'], new['sha256']) and
                         (old['device'], old['inode']) != (new['device'], new['inode']) and copied.stat().st_nlink == 1 and
                         bool(copied.stat().st_mode & 0o111) == (entry['mode'] == '100755'), 'owned checkout is not a separate same-byte original file')
            report['snapshots'].append({'selection': selected, 'committed_entries': len(entries), 'owned_independent_clone': str(destination),
                'hardlink_optimization_requested': False, 'unresolved_submodules': 0, 'scope': 'Original selected Git blobs verified; worktree is separately owned'})
            save()
        need(len(lock['snapshots']) == len(plan['snapshots']), 'selected source lock contains unselected required snapshots')
        def expand(value, replacements):
            need(type(value) is str and '\0' not in value and '\n' not in value, 'clean explicit upstream argument required')
            for key, replacement in replacements.items(): value = value.replace('{' + key + '}', str(replacement))
            need('{' not in value and '}' not in value, 'unknown upstream workflow placeholder'); return value
        for ordinal, spec in enumerate(plan['suites']):
            expected_suite = {'id', 'snapshot', 'engine', 'workflow_inputs', 'commands', 'build', 'pytest_tool', 'pytest_paths',
                'junit_roots', 'coverage_gaps', 'source_test_files', 'ignored_packages', 'disabled_features', 'requested_permutations'}
            if spec['engine'] == 'gtest': expected_suite.add('gtest')
            if spec['engine'] == 'unittest': expected_suite.add('unittest')
            need(set(spec) == expected_suite and TOKEN.fullmatch(spec['id']) and spec['snapshot'] in snapshots and spec['engine'] in ('ctest', 'pytest', 'gtest', 'unittest', 'upstream-junit') and
                spec['workflow_inputs'] and type(spec['commands']) is list and len(spec['commands']) <= 128,
                'closed original upstream workflow/discovery universe required')
            suite_out = out / ('suite-%03d' % ordinal); suite_out.mkdir(mode=0o700); workspace = snapshots[spec['snapshot']]
            replacements = {'workspace': workspace, 'suite': suite_out, 'out': out, 'source': source}
            row = {'id': spec['id'], 'snapshot': spec['snapshot'], 'engine': spec['engine'], 'status': 'RUNNING',
                'coverage_gaps': list(spec['coverage_gaps']), 'source_test_files': spec['source_test_files'],
                'ignored_packages': spec['ignored_packages'], 'disabled_features': spec['disabled_features'],
                'requested_permutations': spec['requested_permutations'], 'universes': [], 'actual_upstream_workflow_completed': False}
            report['suites'].append(row); save()
            for expected in spec['workflow_inputs']:
                pin(expected); original = Path(expected['path'])
                need(original.is_relative_to(original_snapshots[spec['snapshot']]) and
                     str(original.relative_to(original_snapshots[spec['snapshot']])) in tracked[spec['snapshot']],
                     'workflow evidence must be an actual committed selected upstream file')
            need(type(spec['source_test_files']) is list and all(value in tracked[spec['snapshot']] for value in spec['source_test_files']),
                 'declared original test-source paths must belong to the locked Git tree')
            candidates = {name for name in tracked[spec['snapshot']] if any(part in ('test', 'tests', 'rostest') for part in Path(name).parts) or
                          Path(name).name.startswith(('test_', 'test-')) or name.endswith('.test')}
            missing_sources = sorted(candidates - set(spec['source_test_files']))
            if missing_sources: row['coverage_gaps'].append({'unregistered_source_test_candidates': missing_sources,
                'scope': 'Conservative source-file candidates, not invented actual testcase counts'})
            try:
                # Old/stale fixture XML cannot become this run's native result.
                for value in spec['junit_roots']:
                    result_root = Path(expand(value, replacements))
                    need(result_root.is_relative_to(out) and (not result_root.exists() or
                         (result_root.resolve(strict=True).is_relative_to(out) and not list(result_root.rglob('*.xml')))),
                         'fresh owned native result universe required before upstream commands')
                for step, item in enumerate(spec['commands']):
                    need(set(item) == {'tool', 'arguments', 'cwd', 'upstream_reference'} and item['tool'] in tools and
                         item['upstream_reference'] in spec['workflow_inputs'], 'actual pinned original workflow association required')
                    cwd = Path(expand(item['cwd'], replacements)); need(cwd == workspace or cwd.is_relative_to(out), 'native workflow cwd must belong to owned output')
                    command('suite-%03d-workflow-%03d' % (ordinal, step), [tools[item['tool']]['path'], *[expand(value, replacements) for value in item['arguments']]], cwd)
                row['actual_upstream_workflow_completed'] = True
                if spec['engine'] == 'ctest':
                    build = Path(expand(spec['build'], replacements)); need(build.is_relative_to(out), 'actual owned configured CTest build required')
                    discovery_row = command('suite-%03d-ctest-discover' % ordinal, [tools['ctest']['path'], '--test-dir', str(build), '--show-only=json-v1'], suite_out)
                    discovered = formats.ctest(output(discovery_row)); junit_path = suite_out / 'ctest-actual.xml'
                    try:
                        command('suite-%03d-ctest-all' % ordinal, [tools['ctest']['path'], '--test-dir', str(build), '-j1', '--output-on-failure',
                            '--timeout', str(limits['command_seconds']), '--output-junit', str(junit_path)], suite_out)
                    finally:
                        if junit_path.exists():
                            result = formats.junit(common.body_pin(junit_path, 64 * MIB)[0]); product(junit_path)
                            row['universes'].append(formats.reconcile_ctest(discovered, result))
                    row['coverage_gaps'].append('CTEST_WRAPPERS_DO_NOT_ESTABLISH_INNER_GTEST_OR_PYTHON_UNIVERSE_COMPLETENESS')
                elif spec['engine'] == 'gtest':
                    selected = spec['gtest']
                    need(set(selected) == {'binary', 'arguments', 'include_disabled'} and type(selected['arguments']) is list and
                         type(selected['include_disabled']) is bool, 'explicit complete native gtest job required')
                    binary = Path(expand(selected['binary'], replacements)); need(binary.is_relative_to(out) and
                        binary.resolve(strict=True) == binary and os.access(binary, os.X_OK), 'actual owned native test executable required')
                    actual = product(binary, 2 * GIB)
                    with binary.open('rb') as stream: header = stream.read(64)
                    endian = {1: 'little', 2: 'big'}.get(header[5]) if len(header) >= 20 else None
                    need(header[:4] == b'\x7fELF' and endian == profile['target']['endian'] and
                         {1: 32, 2: 64}.get(header[4]) == profile['target']['bits'] and
                         int.from_bytes(header[18:20], endian) == profile['target']['elf_machine'], 'actual original native test target ELF differs')
                    arguments = [expand(value, replacements) for value in selected['arguments']]
                    need(not any(value.startswith(('--gtest_filter', '--gtest_output', '--gtest_list_tests', '--gtest_also_run_disabled_tests'))
                         for value in arguments), 'complete inner universe cannot be hidden behind a filter/output override')
                    prefix = []
                    if profile['runtime']['mode'] == 'qemu':
                        emulator = profile['runtime']['qemu']; need(any(value['path'] == emulator for value in tools.values()), 'actual explicit selected user-QEMU required')
                        prefix = [emulator, '-L', profile['tools']['sysroot']]
                    else: need(profile['runtime']['mode'] == 'native', 'build-only cannot execute an inner native universe')
                    listing = command('suite-%03d-gtest-discover' % ordinal, [*prefix, str(binary), *arguments, '--gtest_list_tests'], suite_out)
                    discovered = formats.gtest(output(listing)); junit_path = suite_out / 'gtest-actual.xml'
                    disabled = ['--gtest_also_run_disabled_tests'] if selected['include_disabled'] else []
                    try:
                        command('suite-%03d-gtest-all' % ordinal, [*prefix, str(binary), *arguments, *disabled,
                            '--gtest_output=xml:' + str(junit_path)], suite_out)
                    finally:
                        if junit_path.exists():
                            result = formats.junit(common.body_pin(junit_path, 64 * MIB)[0]); product(junit_path)
                            row['universes'].append(formats.reconcile_gtest(discovered, result, include_disabled=selected['include_disabled']))
                elif spec['engine'] == 'pytest':
                    need(spec['pytest_tool'] in tools and type(spec['pytest_paths']) is list and spec['pytest_paths'], 'actual SDK pytest interpreter and complete explicit paths required')
                    plugin = source / 'tools/ros_distro/official_suites/uwvm_official_pytest.py'; actual = common.opened(plugin)
                    need(manifest.get(str(plugin.relative_to(source))) == {key: actual[key] for key in ('bytes', 'sha256')}, 'actual source-pinned pytest observation plugin required'); pin(actual)
                    env = {**environment, 'PYTHONPATH': str(plugin.parent) + (':' + environment['PYTHONPATH'] if environment.get('PYTHONPATH') else '')}
                    paths = [expand(value, replacements) for value in spec['pytest_paths']]
                    need(all(Path(value).is_absolute() and Path(value).is_relative_to(workspace) for value in paths), 'pytest paths must belong to actual owned original source')
                    target_elf(tools[spec['pytest_tool']])
                    exe = tools[spec['pytest_tool']]['path']; collection = suite_out / 'pytest-collection.json'
                    env.update(UWVM_OFFICIAL_COLLECTION_FILE=str(collection), UWVM_OFFICIAL_RUN_FILE=str(suite_out / 'collect-no-tests.jsonl'))
                    command('suite-%03d-pytest-discover' % ordinal, [*target_prefix(), exe, '-B', '-m', 'pytest', '-p', 'uwvm_official_pytest', '--collect-only', *paths], workspace, env)
                    original = product(collection); run_collection = suite_out / 'pytest-run-collection.json'; events = suite_out / 'pytest-phases.jsonl'; junit_path = suite_out / 'pytest-actual.xml'
                    env.update(UWVM_OFFICIAL_COLLECTION_FILE=str(run_collection), UWVM_OFFICIAL_RUN_FILE=str(events))
                    try:
                        command('suite-%03d-pytest-all' % ordinal, [*target_prefix(), exe, '-B', '-m', 'pytest', '-p', 'uwvm_official_pytest', '--junitxml=' + str(junit_path), *paths], workspace, env)
                    finally:
                        if events.exists() and run_collection.exists():
                            before_data = common.body_pin(collection, 64 * MIB)[0]; after_data = common.body_pin(run_collection, 64 * MIB)[0]
                            need(formats.decode(before_data) == formats.decode(after_data), 'actual pytest collection changed between discovery and execution')
                            row['universes'].append(formats.pytest_events(before_data, common.body_pin(events, 64 * MIB)[0])); product(run_collection); product(events)
                        if junit_path.exists(): product(junit_path)
                elif spec['engine'] == 'unittest':
                    selected = spec['unittest']
                    need(type(selected) is dict and set(selected) == {'python_tool','top','modules'} and
                         selected['python_tool'] in tools and type(selected['modules']) is list and selected['modules'] and
                         len(selected['modules']) <= 4096 and len(set(selected['modules'])) == len(selected['modules']) and
                         all(type(value) is str and re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*(\.[A-Za-z_][A-Za-z0-9_]*)*', value)
                             for value in selected['modules']), 'explicit whole-module unittest universe required')
                    top = Path(expand(selected['top'], replacements))
                    need(top.resolve(strict=True) == top and top.is_dir() and
                         (top == workspace or top.is_relative_to(workspace)), 'unittest module top must belong to actual original owned checkout')
                    # Require each requested import to resolve to a committed
                    # original module/package, not a classname/method filter or
                    # an ambient SDK module with the same convenient name.
                    for name in selected['modules']:
                        basename = top.joinpath(*name.split('.'))
                        alternatives = (basename.with_suffix('.py'), basename / '__init__.py')
                        matches = [value for value in alternatives if str(value.relative_to(workspace)) in tracked[spec['snapshot']]]
                        need(len(matches) == 1 and str(matches[0].relative_to(workspace)) in spec['source_test_files'],
                             'selected whole unittest module must have one declared original Git source')
                    observer = source / 'tools/ros_distro/official_suites/uwvm_official_unittest.py'
                    actual = common.opened(observer)
                    need(manifest.get(str(observer.relative_to(source))) == {name:actual[name] for name in ('bytes','sha256')},
                         'actual current source-bound unittest observer required'); pin(actual)
                    target_elf(tools[selected['python_tool']]); exe = tools[selected['python_tool']]['path']
                    collection = suite_out / 'unittest-collection.json'
                    prefix = [*target_prefix(), exe, '-B', str(observer), '--top', str(top)]
                    command('suite-%03d-unittest-discover' % ordinal,
                            [*prefix, '--collection', str(collection), '--collect-only', *selected['modules']], workspace)
                    product(collection)
                    run_collection = suite_out / 'unittest-run-collection.json'; events = suite_out / 'unittest-events.jsonl'
                    try:
                        command('suite-%03d-unittest-all' % ordinal,
                                [*prefix, '--collection', str(run_collection), '--events', str(events), *selected['modules']], workspace)
                    finally:
                        if run_collection.exists():
                            original = common.body_pin(collection, 64*MIB)[0]; executed = common.body_pin(run_collection, 64*MIB)[0]
                            need(formats.decode(original) == formats.decode(executed), 'actual unittest loader universe changed between discovery and execution')
                            product(run_collection)
                            raw_events = common.body_pin(events,64*MIB)[0] if events.exists() else b''
                            if events.exists(): product(events)
                            row['universes'].append(formats.unittest_events(original,raw_events))
                else:
                    row['coverage_gaps'].append('JUNIT_ONLY_OBSERVATIONS_CANNOT_ESTABLISH_COMPLETE_NATIVE_DISCOVERY')
            except Exception as error:
                row.update(status='FAIL', error=repr(error))
                # Keep independent native suites running after an upstream test
                # failure, but never continue after the containment guard fails.
                live()
            finally:
                # Even a nonzero upstream test exit retains actual partial
                # JUnit identities. Missing future suites stay UNEXECUTED.
                try:
                    for value in spec['junit_roots']:
                        root = Path(expand(value, replacements)); need(root.is_relative_to(out) and root.is_dir() and
                            root.resolve(strict=True) == root, 'actual owned physical complete native result root required')
                        found = sorted(root.rglob('*.xml')); need(len(found) <= 16384, 'bounded native JUnit result file inventory required')
                        for path in found:
                            need(not path.is_symlink() and path.resolve(strict=True).is_relative_to(root), 'native result cannot alias an old/external file')
                            raw, actual = common.body_pin(path.resolve(strict=True), 64 * MIB); product(path.resolve(strict=True))
                            try: observed = formats.junit(raw)
                            except Exception as error:
                                row.setdefault('unparsed_native_result_files', []).append({'input': actual, 'error': repr(error)})
                                row['coverage_gaps'].append('UNPARSED_OR_NON_JUNIT_NATIVE_RESULT_FILE'); continue
                            row.setdefault('native_junit_files', []).append({'input': actual, 'observation': observed,
                                'scope': 'Separate inner/result-file universe; never added to outer CTest or pytest counts'})
                    row['summaries'] = [formats.summary(value) for value in row['universes']]
                    row['native_result_file_summaries'] = [{'input': item['input'], **formats.summary(item['observation'])}
                        for item in row.get('native_junit_files', [])]
                    if spec['ignored_packages'] or spec['disabled_features']: row['coverage_gaps'].append('IGNORED_PACKAGES_OR_DISABLED_FEATURES_REMAIN')
                    if not row['universes']: row['coverage_gaps'].append('NO_COMPLETE_ACTUAL_DISCOVERY_UNIVERSE')
                    row['status'] = 'NATIVE_DISCOVERED_UNIVERSES_PASSED_WITH_EXPLICIT_GAPS' if row['summaries'] and all(value['complete_success'] for value in row['summaries'] + row['native_result_file_summaries']) else 'NATIVE_FAILURES_SKIPS_DISABLED_OR_UNEXECUTED'

                except BaseException as result_error:
                    row.setdefault('result_accounting_errors', []).append(repr(result_error))
                    row['status'] = 'FAIL'
                if row.get('error'): row['status'] = 'FAIL'
                save()
        # Current custom acceptance remains separately bound raw evidence. It is
        # NOT an official-test mapping and cannot satisfy any native universe.
        for expected in plan['differential_reports']:
            value = document(expected)
            report['differential_evidence'].append({'input': expected, 'original_kind': value.get('kind'), 'original_status': value.get('status'),
                'scope': 'Separate preserved custom Wasm/Gateway receipt; per-official-test equivalence NOT_ESTABLISHED'})
        report['official_differential'] = differential.run(common, unitree, source=source, frozen=manifest,
            manifest_current=manifest_current, profile=profile, source_lock=lock, suite_specs=plan['suites'], suites=report['suites'],
            mapping=document(plan['official_mapping']) if plan['official_mapping'] is not None else None,
            roots=roots, catalogs=catalogs, snapshot_roots=original_snapshots, out=out, limits=limits,
            environment=environment, guard=live, pin=pin, document=document, product=product, module=module, save=save)
        report['Wasm_official_suite_executed'] = report['official_differential']['actual_Wasm_cases_executed'] > 0
        report['status'] = 'COMPLETED_NATIVE_UPSTREAM_WORKFLOWS_WITH_EXPLICIT_COVERAGE_GAPS_NOT_ALL_OFFICIAL_PASS'
    except BaseException as error: report.update(status='FAIL', error=repr(error))
    finally:
        finalizing = True; cleanup_started = time.monotonic(); next_output_walk = 0.0; errors = []
        observed_suite_ids = {row['id'] for row in report['suites']}
        report['unexecuted_suites'] = [{'id': value['id'], 'status': 'UNEXECUTED', 'case_count': None}
            for value in plan['suites'] if value['id'] not in observed_suite_ids]
        try:
            report['cleanup'] = owner.close() if owner else {'status': 'PASS', 'remaining': [], 'scope': 'NO_ACTOR_STARTED'}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup'].get('remaining'), 'owned native upstream cleanup incomplete')
        except BaseException as error: errors.append(repr(error))
        for field, operation in (
            ('source_preserved', lambda: common.source_state(source, Path(manifest_current['path'])) == source_before),
            ('input_pins_preserved', lambda: all(common.opened(Path(value['path'])) == value for value in pins.values())),
            ('products_preserved', lambda: all(common.opened(Path(value['path'])) == value for value in products.values()))):
            try: report[field] = operation(); need(report[field], field + ' failed')
            except BaseException as error: errors.append(repr(error))
        try:
            after = [inventory(root, out / ('inputs-after-%02d.jsonl' % index)) for index, root in enumerate(roots)]
            report['whole_roots_preserved'] = len(after) == len(catalogs) and all({k:v for k,v in a.items() if k != 'catalog'} ==
                {k:v for k,v in b.items() if k != 'catalog'} for a,b in zip(catalogs, after))
            need(report['whole_roots_preserved'], 'original SDK/tool/snapshot root changed'); report['inventories_before'] = catalogs; report['inventories_after'] = after
            live(); report['actual_loopback_after'] = unitree.current_loopback(actual_net)
            report['cgroup_after'] = common.cgroup_state(context)
        except BaseException as error: errors.append(repr(error))
        report['finalization_errors'] = errors
        if errors: report['status'] = 'FAIL'
        save()
    return report
