#!/usr/bin/env python3
"""Own fresh CP310 content preparation and an explicitly selected physics run.

``prepare`` only qualifies and independently copies cached file content.
``run`` starts the same-PID unshare/host-Python coordinator in a fresh private
mount and network namespace. Only the two selected CP310 Python witnesses use
chroot; native Worker, Gateway and controllers retain the host linker view.
All actions require the current hard64GiB/swap0 leaf and explicit current pins.
No physical robot interface or hardware device is admitted by this owner.
"""
from __future__ import annotations
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import signal
import stat
import sys
import time
import traceback
import types

MIB, GIB = 1024 ** 2, 1024 ** 3
CG = None
CG_KEYS = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode',
           'expected_boot_id', 'expected_leaf', 'expected_cpus'}
ORIGINAL_LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
TREE_LIMITS = {'file_bytes': 2 * GIB, 'catalog_bytes': 64 * MIB, 'catalog_entries': 131072,
               'total_bytes': 3 * GIB, 'seconds': 7200}
TOOLS = {'python', 'mount', 'umount', 'chroot', 'unshare', 'ip'}
PATH_KEYS = {'code', 'prepare_out', 'view_root', 'run_owner_out', 'namespace_out', 'run_parent', 'run_out', 'runtime_controls'}


def need(value, message):
    if not value: raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, 'duplicate CP310 owner JSON key'); result[key] = value
    return result


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite owner JSON'))


def read(path, maximum=16 * MIB):
    path = Path(path); need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical ordinary metadata required')
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, 'bounded ordinary owner metadata required')
        body = bytearray()
        while True:
            block = os.read(descriptor, min(MIB, maximum + 1 - len(body)))
            if not block: break
            body.extend(block); need(len(body) <= maximum, 'owner metadata grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'owner metadata changed')
        return bytes(body), {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
            'device': before.st_dev, 'inode': before.st_ino, 'mtime_ns': before.st_mtime_ns, 'ctime_ns': before.st_ctime_ns}
    finally: os.close(descriptor)


def physical_cgroup_private_transition(common, previous, current):
    """Admit only removal of propagation annotations by make-rprivate.

    This is not an old-boot projection. Both observations are actual current
    snapshots in this one newly created namespace. Mount IDs, physical mount
    facts, permission options and every policy/process field must still agree.
    Later checks retain complete unmodified snapshots through cgroup_preserved.
    """
    values = [copy.deepcopy(previous), copy.deepcopy(current)]; raw = []
    for value in values:
        selected = [row['controllerless_hierarchy_root']['mount'] for row in value['ancestors']
                    if 'controllerless_hierarchy_root' in row]
        need(len(selected) == 1, 'one original physical controllerless root required')
        mount = selected[0]; line = mount['raw_mountinfo']; left, right = line.split(' - ')
        fields = left.split(); tail = right.split()
        need(len(fields) >= 6 and len(tail) == 3 and fields[0] == mount['mount_id'] and
             fields[2:5] == [mount['major_minor'], mount['root'], mount['mount_point']] and
             tail[:2] == ['cgroup2', 'cgroup2'] and all(re.fullmatch(r'(?:shared|master|propagate_from):[1-9][0-9]*|unbindable', field)
             for field in fields[6:]), 'closed actual cgroup2 propagation observation required')
        raw.append({'raw_mountinfo': line, 'base': fields[:6], 'optional': fields[6:], 'super': tail})
        del mount['raw_mountinfo']
    need(raw[0]['base'] == raw[1]['base'] and raw[0]['super'] == raw[1]['super'] and not raw[1]['optional'],
         'make-rprivate changed more than its propagation annotations')
    common.cgroup_preserved(values[0], values[1])
    return {'kind': 'current-namespace-make-rprivate-propagation-only', 'before_actual': previous,
            'after_actual': current, 'raw_mount_annotations': raw, 'historical_boot_projection': False}


def current_context(value):
    """Validate Root's SHA-bound current selection before project helper load.

    Only the boot and physical cgroup identity may vary between explicitly
    selected incarnations of the same task container. Memory, zero swap, CPU
    selection and the exact original leaf spelling remain mandatory. Nothing
    is inferred from an old producer or converted into execution continuity.
    """
    need(type(value) is dict and set(value) == CG_KEYS and
         value['memory_max'] == 64 * GIB and value['memory_swap_max'] == 0 and
         type(value['memory_max']) is int and type(value['memory_swap_max']) is int and
         value['expected_leaf'] == ORIGINAL_LEAF and value['expected_cpus'] == '0-31' and
         all(type(value[key]) is int and value[key] > 0 for key in ('expected_device', 'expected_inode')) and
         type(value['expected_boot_id']) is str and len(value['expected_boot_id']) == 36,
         'closed explicit current selection of original hard64GiB zero-swap task leaf required')
    leaf = Path(ORIGINAL_LEAF); info = leaf.stat()
    need(leaf.resolve(strict=True) == leaf and
         (info.st_dev, info.st_ino) == (value['expected_device'], value['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == value['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + ORIGINAL_LEAF.removeprefix('/sys/fs/cgroup')] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and
         (leaf / 'memory.swap.max').read_text().strip() == '0' and
         (leaf / 'cpuset.cpus.effective').read_text().strip() == '0-31',
         'Root selected current boot/leaf physical facts or hard resource limits differ')
    return dict(value)


def main():
    global CG
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--phase', choices=('prepare', 'run', 'profile', 'namespace'), required=True)
    parser.add_argument('--control', type=Path, required=True); parser.add_argument('--control-sha256', required=True)
    parser.add_argument('--handoff', type=Path); parser.add_argument('--handoff-sha256')
    args = parser.parse_args(); started = time.monotonic()
    need(sys.platform == 'linux' and os.geteuid() == 0 and hasattr(os, 'pidfd_open'), 'actual Root Linux pidfd ownership required')
    raw, control_pin = read(args.control); control = parse(raw)
    need(control_pin['sha256'] == args.control_sha256 and type(control) is dict, 'Root selected owner control SHA differs')
    CG = current_context(control.get('cgroup'))
    need(control_pin['sha256'] == args.control_sha256 and set(control) == {'format_version', 'kind', 'source', 'manifest',
        'manifest_sha256', 'manifest_pin', 'cgroup', 'paths', 'helpers', 'host_tools', 'cached_view', 'run_selection'} and
        control['format_version'] == 1 and control['kind'] == 'unitree-cp310-physics-root-owner-v1' and control['cgroup'] == CG and
        set(control['paths']) == PATH_KEYS and set(control['helpers']) == {'owner', 'copy', 'restart'} and
        set(control['host_tools']) == TOOLS, 'closed current CP310 physics owner control required')
    paths = {key: Path(value) for key, value in control['paths'].items()}
    need(all(path.is_absolute() and '..' not in path.parts for path in paths.values()) and len(set(paths.values())) == len(paths) and
         paths['run_out'].parent == paths['run_parent'] and all(paths['view_root'] not in paths[key].parents and
         paths[key] not in paths['view_root'].parents for key in PATH_KEYS - {'view_root'}), 'distinct explicit fresh owner/view/run paths required')
    source, manifest = Path(control['source']), Path(control['manifest'])
    need(source.is_absolute() and source.resolve(strict=True) == source and manifest == source.parent / 'source-manifest.json' and
         all(source != path and source not in path.parents and path not in source.parents for path in paths.values()), 'complete frozen source and outputs must be disjoint')
    raw, mf_pin = read(manifest); frozen = parse(raw)
    need(mf_pin == control['manifest_pin'] and mf_pin['sha256'] == control['manifest_sha256'] and type(frozen) is dict and frozen,
         'actual current full source manifest required')
    bodies = {}; bootstrap = [control_pin, mf_pin]
    for relative in ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py', 'tests/ros_unitree/common.py'):
        body, pin = read(source / relative, MIB)
        need(frozen.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')}, 'full-manifest-bound helper required')
        bodies[relative] = body; bootstrap.append(pin)
    common = types.ModuleType('cp310_owner_frozen_common'); common.__file__ = str(source / 'tools/ros_distro/common.py')
    sys.modules[common.__name__] = common
    exec(compile(bodies['tools/ros_distro/common.py'], common.__file__, 'exec'), common.__dict__)
    common.install_interrupts(); initial_cg = common.cgroup_state(CG)
    need(all(row['events'].get(key, 0) == 0 for row in initial_cg['ancestors'] for key in ('oom', 'oom_kill', 'oom_group_kill')),
         'all current ancestor OOM counters must start at zero')
    before_cg = None if args.phase == 'namespace' else initial_cg
    out = paths['prepare_out'] if args.phase == 'prepare' else paths['run_owner_out'] if args.phase in ('run', 'profile') else paths['namespace_out']
    need(out.parent.resolve(strict=True) == out.parent and not out.exists() and not out.is_symlink(), 'fresh exact phase output required')
    out.mkdir(mode=0o700)
    pins = {}; products = {}; runner = None; cleanup_runner = None; finalizing = False; mounted = []; source_before = None
    report = {'format_version': 1, 'kind': 'unitree-cp310-physics-owner-phase', 'phase': args.phase, 'status': 'RUNNING',
        'source': str(source), 'source_manifest_sha256': control['manifest_sha256'], 'control': control_pin,
        'cgroup_initial_actual': initial_cg, 'commands': [], 'physical_hardware_access': False,
        'Guest_authority': 'UNCHANGED_EMPTY', 'errors': []}

    def guard():
        need(time.monotonic() - started < ((7800 if args.phase == 'prepare' else 43800) if finalizing else (7200 if args.phase == 'prepare' else 43200)), 'finite active/cleanup CP310 owner deadline exceeded')
        current = common.cgroup_state(CG)
        need(all(row['events'].get(key, 0) == 0 for row in current['ancestors'] for key in ('oom', 'oom_kill', 'oom_group_kill')),
             'current ancestor OOM counter changed')
        if before_cg is not None: common.cgroup_preserved(before_cg, current)
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 512 * MIB, 'owner metadata/copy RSS exceeded512MiB')
        if not finalizing:
            for path in (out, *out.parents):
                space = os.statvfs(path); need(space.f_bavail * space.f_frsize >= 8 * GIB, 'backing filesystem reserve below8GiB')
        return current

    def remember(pin):
        need(type(pin) is dict and set(pin) == PIN, 'actual selected current full7 required')
        actual = common.opened(Path(pin['path']), max(64 * MIB, pin['bytes']))
        need(actual == pin and (actual['path'] not in pins or pins[actual['path']] == actual), 'selected current input changed')
        pins[actual['path']] = actual; return actual

    def product(path, maximum=64 * MIB):
        pin = common.opened(Path(path), maximum)
        need(pin['path'] not in products or products[pin['path']] == pin, 'retained product changed')
        products[pin['path']] = pin; return pin

    def document(pin, maximum=64 * MIB):
        remember(pin); raw, actual = read(pin['path'], maximum); need(actual == pin, 'opened selected JSON changed'); return parse(raw)

    def profile_rules(selection):
        """Bind a Root-selected profile mode and original measured DATA before actors.

        A fresh private namespace is not continuation of the baseline namespace.
        No baseline report/config/input is rewritten or allowed to supply current
        kernel state. The same closed actual cgroup selection is checked live.
        """
        profile = selection.get('kind') == 'unitree-cp310-direct3-profile-metadata-v1'
        expected = {'format_version', 'kind', 'view_receipt', 'baseline_config', 'physics_template', 'readonly_bind_roots'}
        if profile: expected.add('profile_evidence')
        need(set(selection) == expected and selection['format_version'] == 1 and
             (profile or selection['kind'] == 'unitree-cp310-physics-run-metadata-v1'), 'closed selected physics/profile owner mode required')
        if not profile: return None
        evidence = selection['profile_evidence']
        need(type(evidence) is dict and set(evidence) == {'baseline_report', 'baseline_config', 'physics_selection', 'recipe'},
             'four exact actual profile evidence roles required')
        relative = 'tests/ros_unitree/direct3_profile.py'
        body, actual = read(source / relative, MIB)
        need(frozen.get(relative) == {key: actual[key] for key in ('bytes', 'sha256')}, 'complete-manifest profile predicates required')
        remember(actual); module = types.ModuleType('cp310_owner_direct3_profile'); module.__file__ = actual['path']
        sys.modules[module.__name__] = module; exec(compile(body, module.__file__, 'exec'), module.__dict__)
        old = document(evidence['baseline_report']); original = document(evidence['baseline_config'])
        physics = document(evidence['physics_selection']); recipe = evidence['recipe']
        need(old['source'] == str(source) and old['source_manifest_sha256'] == control['manifest_sha256'] and
             old['configuration'] == evidence['baseline_config'] and original['cgroup'] == CG and
             {key: evidence['physics_selection'][key] for key in ('path', 'bytes', 'sha256')} == original['inputs']['physics_selection'] and
             any(pin == evidence['physics_selection'] for pin in old['input_pins']), 'actual preserved same-source baseline/config/selection association required')
        need(type(recipe) is dict and set(recipe) in ({'language', 'vtune', 'roots', 'seconds'},
                 {'language', 'vtune', 'roots', 'seconds', 'collection_policy'}) and
             type(recipe['seconds']) is int and 1 <= recipe['seconds'] <= 120, 'explicit bounded actual hardware recipe required')
        collection_policy = module.profile_policy(recipe)
        module.select_case(old, recipe['language'], collection_policy=collection_policy)
        remember(recipe['vtune'])
        need(type(recipe['roots']) is list and 1 <= len(recipe['roots']) <= 8 and
             len(set(recipe['roots'])) == len(recipe['roots']), 'finite distinct actual profiler/provider roots required')
        for value in recipe['roots']:
            root = Path(value)
            need(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and root != Path('/') and
                 all(root != path and root not in path.parents and path not in root.parents for path in
                     (source, paths['run_parent'], paths['namespace_out'], paths['runtime_controls'])),
                 'selected complete profiler roots must exclude frozen source and live owned outputs')
        need(any(Path(recipe['vtune']['path']).is_relative_to(Path(root)) for root in recipe['roots']) and
             os.access(recipe['vtune']['path'], os.X_OK), 'actual selected executable VTune/provider association required')
        report.update(profile_mode=True, selected_language=recipe['language'], original_baseline=evidence['baseline_report'],
                      hardware_collection_policy=collection_policy,
                      physical_hardware_access=True, physical_hardware_scope='HOST_PMU_ONLY_NO_ROBOT',
                      hardware_privilege_setup_performed=False, physical_robot_access=False)
        return module, old, original, physics, recipe

    def load(role):
        selected = remember(control['helpers'][role]); body, actual = read(selected['path'], MIB)
        need(actual == selected and Path(selected['path']).parent == paths['code'], 'exact staged source role required')
        module = types.ModuleType('cp310_phase_' + role); module.__file__ = selected['path']; sys.modules[module.__name__] = module
        exec(compile(body, module.__file__, 'exec'), module.__dict__); return module

    def save():
        report['input_pins'] = list(pins.values()); report['product_pins'] = list(products.values())
        if runner: report['commands'] = runner.commands
        common.atomic_json(out / 'report.json', report)

    environment = {'PATH': '/usr/bin:/bin', 'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8', 'PYTHONDONTWRITEBYTECODE': '1',
                   'GOMAXPROCS': '1', 'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1', 'MKL_NUM_THREADS': '1'}
    tools = {}; prepared = None; source_qualified = copy_qualified = None; immutable_catalogs = []
    mounts_before = None
    def mountpoints():
        return {re.sub(r'\\([0-7]{3})', lambda match: chr(int(match[1], 8)), line.split()[4])
                for line in Path('/proc/self/mountinfo').read_text().splitlines()}
    save()
    try:
        for pin in bootstrap: remember(pin)
        need(control['helpers']['owner']['path'] == str(Path(__file__).absolute()), 'actual self source selected')
        copier = load('copy'); restart = load('restart')
        for name, selection in control['host_tools'].items():
            need(set(selection) == {'invocation', 'pin'} and Path(selection['invocation']).is_absolute() and
                 Path(selection['invocation']).resolve(strict=True) == Path(selection['pin']['path']), 'closed pinned host-tool invocation required')
            pin = remember(selection['pin']); need(os.access(pin['path'], os.X_OK), 'selected host tool is not executable')
            tools[name] = selection['invocation']
        need(Path(tools['python']).resolve(strict=True) == Path(sys.executable).resolve(strict=True), 'host coordinator must use selected host Python')
        source_before = common.source_state(source, manifest); report['source_before'] = source_before
        if args.phase == 'prepare':
            cache = control['cached_view']; need(set(cache) == {'root', 'report', 'catalog'}, 'closed cached view proof required')
            prior = document(cache['report'])
            need(prior.get('status') == 'COMPLETED_EXPLICIT_NATIVE_PREFLIGHTS_ONLY' and prior.get('execution_view_preserved') is True and
                 prior.get('source_preserved') is True and prior.get('inputs_preserved') is True and prior.get('products_preserved') is True and
                 prior.get('cleanup', {}).get('status') == 'PASS' and not prior.get('remaining_mounts') and not prior.get('finalization_errors') and
                 prior['execution_view_after']['root'] == cache['root'] and prior['execution_view_after']['catalog'] == cache['catalog'],
                 'original preserved preflight view is content provenance only')
            source_qualified = restart.requalify_tree(cache['catalog'], cache['root'], out / 'cached-view-current.jsonl', guard, TREE_LIMITS)
            remember(source_qualified['historical_catalog']['current_pin'])
            product(out / 'cached-view-current.jsonl')
            report['cached_view_content_qualification'] = source_qualified
            report['copy'] = copier.stream_copy(cache['root'], paths['view_root'], out / 'independent-copy-ledger.jsonl', guard)
            product(out / 'independent-copy-ledger.jsonl')
            copy_qualified = restart.requalify_tree(cache['catalog'], str(paths['view_root']), out / 'fresh-view-current.jsonl', guard, TREE_LIMITS)
            product(out / 'fresh-view-current.jsonl')
            report['fresh_view_content_qualification'] = copy_qualified
            expected = prior['actual_view_python']; actual = common.opened(paths['view_root'] / 'usr/bin/python3.10')
            need((actual['bytes'], actual['sha256']) == (expected['bytes'], expected['sha256']) and
                 (actual['device'], actual['inode']) != (expected['device'], expected['inode']), 'fresh independent CP310 executable bytes differ')
            report.update(kind='unitree-cp310-controlled-physics-view', root=str(paths['view_root']), python=actual,
                          catalog_roots=[str(paths['view_root'] / name) for name in ('usr', 'opt', 'etc') if (paths['view_root'] / name).is_dir()],
                          target_code_executed=False, historical_preflight_status_promoted=False, content_prepared=True)
        elif args.phase in ('run', 'profile'):
            selection = document(control['run_selection']); profiling = profile_rules(selection)
            need((args.phase == 'profile') == (profiling is not None), 'explicit Root phase must match selected profile metadata')
            prepared = document(selection['view_receipt'])
            need(prepared['kind'] == 'unitree-cp310-controlled-physics-view' and prepared['status'] == 'PREPARED_CONTENT_ONLY_NOT_EXECUTED' and
                 prepared['root'] == str(paths['view_root']) and all(prepared.get(key) is True for key in
                 ('source_preserved', 'inputs_preserved', 'products_preserved')), 'actual prepared content-only CP310 COPY required')
            handoff = out / 'namespace-handoff.json'
            common.atomic_json(handoff, {'kind': 'same-PID-unshare-direct-child-handoff', 'outer_identity': common.identity(os.getpid()),
                'parent_mount_namespace': os.readlink('/proc/self/ns/mnt'), 'parent_network_namespace': os.readlink('/proc/self/ns/net'),
                'control': control_pin, 'source_manifest': mf_pin})
            handoff_pin = product(handoff)
            runner = common.SerialRunner(out, {'rss_bytes': 8 * GIB, 'command_seconds': 39600, 'log_bytes': 16 * MIB}, guard)
            argv = [tools['unshare'], '--mount', '--net', '--', tools['python'], '-B', str(Path(__file__).absolute()),
                '--phase', 'namespace', '--control', control_pin['path'], '--control-sha256', control_pin['sha256'],
                '--handoff', handoff_pin['path'], '--handoff-sha256', handoff_pin['sha256']]
            runner.run('private-mount-network-physics-owner', argv, paths['code'], environment)
            raw, inner_pin = read(paths['namespace_out'] / 'report.json', 64 * MIB); inner = parse(raw)
            expected_status = 'COMPLETED_EXPLICIT_DIRECT3_HARDWARE_PROFILE_ONLY' if profiling else 'COMPLETED_EXPLICIT_DIRECT_PHYSICS_ONLY'
            need(inner.get('status') == expected_status and not inner.get('remaining_mounts') and
                 not inner.get('finalization_errors') and inner['cleanup']['status'] == 'PASS', 'actual completed selected private namespace owner required')
            product(inner_pin['path']); report['namespace_report'] = inner_pin
            report['profile_completed' if profiling else 'run_completed'] = True
        else:
            need(args.handoff is not None and args.handoff_sha256 is not None, 'explicit current parent handoff required')
            raw, handoff_pin = read(args.handoff); handoff = parse(raw)
            need(handoff_pin['sha256'] == args.handoff_sha256 and handoff['control'] == control_pin and handoff['source_manifest'] == mf_pin and
                 handoff['kind'] == 'same-PID-unshare-direct-child-handoff' and
                 common.birth(common.identity(os.getppid())) == common.birth(handoff['outer_identity']) and
                 os.readlink('/proc/%d/ns/mnt' % os.getppid()) == handoff['parent_mount_namespace'] and
                 os.readlink('/proc/%d/ns/net' % os.getppid()) == handoff['parent_network_namespace'], 'actual same-birth direct outer parent handoff required')
            remember(handoff_pin)
            own_mnt, own_net = os.readlink('/proc/self/ns/mnt'), os.readlink('/proc/self/ns/net')
            need(own_mnt != handoff['parent_mount_namespace'] and own_net != handoff['parent_network_namespace'] and
                 re.fullmatch(r'mnt:\[[1-9][0-9]*\]', own_mnt) and re.fullmatch(r'net:\[[1-9][0-9]*\]', own_net), 'fresh distinct actual mount/network namespaces required')
            report.update(mount_namespace=own_mnt, network_namespace=own_net, actual_parent_handoff=handoff_pin)
            runner = common.SerialRunner(out, {'rss_bytes': 512 * MIB, 'command_seconds': 30, 'log_bytes': 16 * MIB}, guard)
            runner.run('make-root-propagation-private', [tools['mount'], '--make-rprivate', '/'], out, environment)
            after_private = common.cgroup_state(CG)
            report['private_propagation_cgroup_proof'] = physical_cgroup_private_transition(common, initial_cg, after_private)
            before_cg = after_private
            runner.run('bring-only-private-loopback-up', [tools['ip'], 'link', 'set', 'dev', 'lo', 'up'], out, environment)
            unitree = types.ModuleType('cp310_owner_unitree_common'); unitree.__file__ = str(source / 'tests/ros_unitree/common.py')
            sys.modules[unitree.__name__] = unitree
            exec(compile(bodies['tests/ros_unitree/common.py'], unitree.__file__, 'exec'), unitree.__dict__)
            report['actual_loopback'] = unitree.current_loopback(own_net)
            selection = document(control['run_selection'])
            profiling = profile_rules(selection)
            # The explicit profile recipe alone enables host PMU access. No
            # hardware device, ROS grant or Guest clock/thread right is added.
            prepared = document(selection['view_receipt'])
            need(prepared.get('kind') == 'unitree-cp310-controlled-physics-view' and prepared.get('status') == 'PREPARED_CONTENT_ONLY_NOT_EXECUTED' and
                 prepared.get('root') == str(paths['view_root']) and all(prepared.get(key) is True for key in
                 ('source_preserved', 'inputs_preserved', 'products_preserved')) and not prepared.get('error') and not prepared.get('errors'),
                 'actual preserved fresh content-only view receipt required')
            remember(prepared['python'])
            baseline = document(selection['baseline_config']); physics = document(selection['physics_template'])
            need(set(baseline) == {'format_version', 'cgroup', 'input_roots', 'inputs', 'environment', 'limits', 'upstreams', 'simulation', 'benchmark', 'profiler'} and
                 baseline['cgroup'] == CG and baseline['benchmark'] is None and
                 ((profiling is None and baseline['profiler'] is None) or
                  (profiling is not None and type(baseline['profiler']) is dict and 'direct3' in baseline['profiler'])) and
                 baseline['simulation']['controllers'] is baseline['simulation']['direct_runtime'] is None and
                 baseline['limits']['jobs'] == 1 and baseline['limits']['rss_bytes'] <= 8 * GIB and
                 baseline['limits']['command_seconds'] <= 600, 'closed finite immutable simulation baseline required')
            need(physics.get('format_version') == 1 and physics.get('kind') in ('unitree-go2-three-direct-physics-v1','unitree-go2-three-modular-direct-physics-v1') and
                 physics.get('expected_network_namespace') is None and set(physics['cp310']) ==
                 {'root', 'python', 'chroot', 'catalog_roots', 'environment', 'view_receipt'},
                 'unexecuted explicit physics template with unforecast namespace required')
            need(all(not path.exists() and not path.is_symlink() for path in (paths['runtime_controls'], paths['run_parent'])) and
                 paths['runtime_controls'].parent.resolve(strict=True) == paths['runtime_controls'].parent and
                 paths['run_parent'].parent.resolve(strict=True) == paths['run_parent'].parent,
                 'fresh separately owned runtime controls and writable run parent required')
            paths['runtime_controls'].mkdir(mode=0o700); paths['run_parent'].mkdir(mode=0o700)
            # Source/code/config paths keep their actual absolute spellings
            # inside chroot. Only immutable COPY subtrees are inventoried;
            # the whole root will contain dynamic proc/cgroup/RW bind mounts.
            immutable_roots = [Path(value) for value in prepared['catalog_roots']]
            need(immutable_roots and all(path.parent == paths['view_root'] and path.name in ('usr', 'opt', 'etc')
                 for path in immutable_roots), 'only immutable selected copied subtrees are admitted')
            cp = physics['cp310']
            need(cp['root'] == str(paths['view_root']) and cp['python'] == 'cp310_python' and cp['chroot'] == 'cp310_chroot' and
                 cp['view_receipt'] == 'cp310_view_receipt' and isinstance(cp['catalog_roots'], list) and cp['catalog_roots'],
                 'maker must bind actual view labels and catalog roots before namespace launch')
            need(all(str(path) in cp['catalog_roots'] for path in immutable_roots), 'every immutable COPY subtree must be mapped')
            need(cp['environment'].get('ROS_DOMAIN_ID') == '1' and cp['environment'].get('MUJOCO_GL') == 'osmesa' and
                 cp['environment'].get('PYTHONDONTWRITEBYTECODE') == '1', 'explicit CP310 domain1/OSMesa/no-bytecode environment required')
            physics['expected_network_namespace'] = own_net
            for key, name in (('HOME', 'home'), ('TMPDIR', 'tmp'), ('XDG_CACHE_HOME', 'cache'), ('XDG_CONFIG_HOME', 'config')):
                cp['environment'][key] = str(paths['run_out'] / name)
            for name, pin in (('cp310_python', prepared['python']), ('cp310_chroot', control['host_tools']['chroot']['pin']),
                              ('cp310_view_receipt', selection['view_receipt'])):
                expected = {key: pin[key] for key in ('path', 'bytes', 'sha256')}
                need(baseline['inputs'].get(name) == expected, 'maker actual CP310 input association differs')
            need(baseline['simulation']['python_input'] == 'cp310_python', 'actual selected COPY Python must drive simulation')
            physics_path = paths['runtime_controls'] / 'physics-selection.json'
            common.atomic_json(physics_path, physics); physics_pin = product(physics_path)
            baseline['inputs']['physics_selection'] = {key: physics_pin[key] for key in ('path', 'bytes', 'sha256')}
            baseline['input_roots'] = list(dict.fromkeys([*baseline['input_roots'], str(paths['runtime_controls'])]))
            roots = [Path(value) for value in baseline['input_roots']]
            need(1 <= len(roots) <= 64 and paths['view_root'] not in roots and
                 all(path != Path('/') and not (path == paths['run_parent'] or path in paths['run_parent'].parents or
                 paths['run_parent'] in path.parents or path == paths['namespace_out'] or path in paths['namespace_out'].parents or
                 paths['namespace_out'] in path.parents) for path in roots), 'finite immutable Frame roots must exclude dynamic view/outputs')
            config_path = paths['runtime_controls'] / 'unitree-inputs.json'
            if profiling is not None:
                rules, old_report, original_config, original_physics, recipe = profiling
                actual_profile_inputs = dict(zip(rules.INPUT_LABELS, (selection['profile_evidence']['baseline_report'],
                    selection['profile_evidence']['baseline_config'], selection['profile_evidence']['physics_selection'], recipe['vtune'])))
                rules.exact_adaptation(original_config, baseline, recipe['roots'], actual_profile_inputs, str(paths['runtime_controls']))
                rules.same_physics(original_physics, physics)
                need(own_net != original_physics['expected_network_namespace'], 'new actual private profiling namespace required')
                expected_profile = {'installation': None, 'vtune_input': 'direct3-profile-vtune', 'self_checker_input': None,
                    'benchmark_report_input': 'direct3-profile-baseline', 'target': {'executable': physics['workers']['ros2'], 'arguments': []},
                    'target_input': physics['workers']['ros2'], 'target_stdin_input': None, 'target_role': 'wasm', 'seconds': recipe['seconds'],
                    'direct3': {'language': recipe['language'], 'selection_input': 'physics_selection',
                        'baseline_config_input': 'direct3-profile-config', 'baseline_selection_input': 'direct3-profile-selection'}}
                if 'collection_policy' in recipe:
                    expected_profile['direct3']['collection_policy'] = rules.profile_policy(recipe)
                need(baseline['profiler'] == expected_profile and
                     baseline['inputs']['direct3-profile-vtune'] == {key: recipe['vtune'][key] for key in ('path', 'bytes', 'sha256')},
                     'profile recipe cannot alter target, measured language, hardware selection or parent-paced control')
                report['baseline_adaptation'] = {'original_report': selection['profile_evidence']['baseline_report'],
                    'original_config': selection['profile_evidence']['baseline_config'],
                    'original_selection': selection['profile_evidence']['physics_selection'],
                    'changed': ['explicit profiler recipe/four metadata inputs/complete profiler roots',
                                'actual fresh network namespace', 'fresh owned output paths'],
                    'simulation_fields_changed': False, 'historical_reports_rewritten': False}
            common.atomic_json(config_path, baseline); config_pin = product(config_path)
            final_dir = copier.directory(paths['runtime_controls'])
            try: os.fsync(final_dir)
            finally: os.close(final_dir)
            providers = [Path(value) for value in selection['readonly_bind_roots']]
            need(len(providers) <= 64 and len(set(providers)) == len(providers), 'finite distinct provider bind roots required')
            for path in providers:
                need(path.is_absolute() and path.resolve(strict=True) == path and path.is_dir() and
                     str(path).startswith('/var/tmp/') and path != paths['view_root'] and
                     all(path != dynamic and path not in dynamic.parents and dynamic not in path.parents
                     for dynamic in (paths['namespace_out'], paths['run_parent'], paths['runtime_controls'])),
                     'provider bind must be exact immutable artifact/source subtree, never host OS or live output root')
            binds = []; destinations = set(); root = paths['view_root']
            def plan(origin, logical, *, readonly=True, file=False):
                origin, logical = Path(origin), Path(logical)
                need(logical.is_absolute() and '..' not in logical.parts and str(logical) not in destinations, 'unique clean absolute bind destination required')
                target = root / logical.relative_to('/')
                target.parent.mkdir(mode=0o755, parents=True, exist_ok=True)
                need(target.parent.resolve(strict=True).is_relative_to(root), 'bind destination parent escaped fresh COPY')
                if file:
                    if not target.exists(): target.touch(mode=0o400, exist_ok=False)
                    need(target.is_file() and not target.is_symlink(), 'ordinary fresh file bind destination required')
                else:
                    target.mkdir(mode=0o755, exist_ok=True)
                    need(target.is_dir() and not target.is_symlink(), 'ordinary fresh directory bind destination required')
                destinations.add(str(logical)); binds.append({'origin': str(origin), 'destination': str(target),
                    'logical': str(logical), 'readonly': readonly, 'file': file})
            # All destinations are created before the root becomes read-only.
            for path in list(dict.fromkeys([source, paths['runtime_controls'], Path(selection['view_receipt']['path']).parent,
                                          *providers, *immutable_roots])):
                plan(path, path)
            plan(manifest, manifest, file=True)
            plan(paths['run_parent'], paths['run_parent'], readonly=False)
            plan(Path('/proc'), Path('/proc'))
            plan(Path('/sys/fs/cgroup'), Path('/sys/fs/cgroup'))
            devices = []
            for name, expected_rdev in (('null', os.makedev(1, 3)), ('zero', os.makedev(1, 5)),
                                        ('urandom', os.makedev(1, 9)), ('random', os.makedev(1, 8))):
                device = Path('/dev') / name; state = device.stat(follow_symlinks=False)
                need(stat.S_ISCHR(state.st_mode) and state.st_rdev == expected_rdev and not device.is_symlink(),
                     'only exact selected nonhardware kernel character devices may enter COPY')
                devices.append({'path': str(device), 'device': state.st_dev, 'inode': state.st_ino, 'rdev': state.st_rdev})
                plan(device, device, file=True)
            # Full immutable subtree baselines are taken only after creating
            # mount destinations. No catalog traverses /proc, cgroup or RW run.
            for index, path in enumerate(immutable_roots):
                guard(); item = common.catalog(path, out / ('copy-subtree-before-%02d.jsonl' % index), immutable_roots)
                product(item['catalog']['path']); immutable_catalogs.append(item)
            mounts_before = mountpoints()
            def mount(label, argv, target):
                mounted.append(Path(target))
                runner.run(label, argv, out, environment)
            mount('bind-independent-view-root', [tools['mount'], '--bind', str(root), str(root)], root)
            runner.run('readonly-independent-view-root', [tools['mount'], '-o', 'remount,bind,ro', str(root)], out, environment)
            for index, item in enumerate(binds):
                mount('bind-selected-%02d' % index, [tools['mount'], '--bind', item['origin'], item['destination']], item['destination'])
                if item['readonly']:
                    runner.run('readonly-selected-%02d' % index, [tools['mount'], '-o', 'remount,bind,ro', item['destination']], out, environment)
            report.update(bindings=binds, selected_nonhardware_devices=devices, actual_configuration=config_pin,
                          actual_physics_selection=physics_pin, immutable_COPY_subtrees_before=immutable_catalogs,
                          host_linked_actors_chrooted=False, sysfs_remounted=False)
            unitree.current_loopback(own_net); guard(); save()
            runner.limits = {'rss_bytes': 8 * GIB, 'command_seconds': 36000, 'log_bytes': 16 * MIB}
            entry = 'profile_vtune.py' if profiling else 'run_direct_physics.py'
            tail = ['--action', 'direct3-collect'] if profiling else ['--selection-input', 'physics_selection']
            argv = [tools['python'], '-B', str(source / 'tests/ros_unitree' / entry), '--source', str(source),
                '--manifest', str(manifest), '--manifest-sha256', control['manifest_sha256'], '--config', config_pin['path'],
                '--config-sha256', config_pin['sha256'], '--out', str(paths['run_out']), *tail]
            report['host_coordinator_argv'] = argv
            runner.run('actual-selected-Direct3-hardware-profile' if profiling else 'actual-three-language-Go2-controlled-physics', argv, source, environment)
            raw, result_pin = read(paths['run_out'] / 'report.json', 64 * MIB); result = parse(raw)
            need(result.get('physical_robot_access') is False and all(result.get(key) is True for key in
                 ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved')) and
                 not result.get('error') and not result.get('finalization_errors'), 'actual current source/input preservation required')
            if profiling:
                rules, old_report, original_config, original_physics, recipe = profiling
                need(result.get('kind') == rules.PROFILE_KIND and result.get('status') == rules.PROFILE_STATUS and
                     result.get('selected_language') == recipe['language'] and
                     result.get('hardware_collection_policy') == rules.profile_policy(recipe) and result.get('baseline_report') == selection['profile_evidence']['baseline_report'] and
                     result.get('physical_hardware_access') is True and result.get('hardware_privilege_setup_performed') is False and
                     result.get('performance_qualified') is False and set(result.get('direct3_profiled_controls', {})) == {'hotspots', 'uarch-exploration'} and
                     set(result.get('actual_hardware_event_evidence', {})) == {'hotspots', 'uarch-exploration'} and
                     all(result['actual_hardware_event_evidence'][key] for key in ('hotspots', 'uarch-exploration')),
                     'two actual hardware collections/replays required; preparation cannot supply profiles or native speed')
                report['actual_hardware_profile_report'] = result_pin; report['profile_completed'] = True
            else:
                need(result.get('status') == ('PASS_THREE_ACTUAL_MODULAR_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES'
                     if physics['kind']=='unitree-go2-three-modular-direct-physics-v1' else
                     'PASS_THREE_ACTUAL_STANDALONE_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES') and
                     result.get('kind') == ('unitree-three-modular-direct-Guest-controlled-physics'
                     if physics['kind']=='unitree-go2-three-modular-direct-physics-v1' else
                     'unitree-three-direct-Guest-controlled-physics') and
                     len(result.get('cases', [])) == 3 and result.get('actual_default_denied_witnesses') == 3,
                     'actual complete controlled physics report required; namespace success alone is insufficient')
                report['actual_physics_report'] = result_pin; report['run_completed'] = True
            product(result_pin['path']); report['actual_loopback_after'] = unitree.current_loopback(own_net)
    except BaseException as error:
        report.update(status='FAIL', error=repr(error), traceback=traceback.format_exc())
    finally:
        finalizing = True; errors = []
        def finish(label, operation):
            try: operation()
            except BaseException as error:
                errors.append({'stage': label, 'error': repr(error)}); report['status'] = 'FAIL'
        def cooperative():
            if not runner: return
            for child in runner.children:
                if child.poll() is not None: continue
                first, current = runner.roots.get(child.pid), common.identity(child.pid)
                need(first is not None and current is not None and common.birth(first) == common.birth(current) and
                     current['parent_pid'] == os.getpid(), 'same-birth direct coordinator required before TERM')
                runner.acquire(child.pid, os.getpid()); key = common.birth(first)
                need(key in runner.owned or child.poll() is not None, 'live direct coordinator lacks pidfd')
                if key not in runner.owned: continue
                try: signal.pidfd_send_signal(runner.owned[key]['fd'], signal.SIGTERM)
                except ProcessLookupError: pass
                deadline = min(started + (7200 if args.phase == 'prepare' else 43200), time.monotonic() + 600)
                while child.poll() is None and time.monotonic() < deadline:
                    guard(); runner.discover(); need(runner.rss() <= 8 * GIB, 'cleanup process RSS exceeded8GiB'); time.sleep(0.05)
        def cleanup():
            report['cleanup'] = runner.close() if runner else {'status': 'PASS', 'remaining': [], 'scope': 'NO_ACTOR_STARTED'}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup']['remaining'] and not report['cleanup'].get('errors'), 'owned pidfd cleanup incomplete')
        finish('same-birth-direct-coordinator-600-second-grace', cooperative)
        finish('child-first-owned-pidfd-cleanup', cleanup)
        if runner:
            for row in runner.commands:
                path = out / (row['label'] + '.log')
                if path.exists(): finish('retained-final-log', lambda path=path: product(path))
            if (out / 'commands.json').exists(): finish('retained-command-ledger', lambda: product(out / 'commands.json'))
        if mounted:
            def unmount_owned():
                nonlocal cleanup_runner
                cleanup_out = out / 'mount-cleanup'; cleanup_out.mkdir(mode=0o700)
                cleanup_runner = common.SerialRunner(cleanup_out, {'rss_bytes': 512 * MIB, 'command_seconds': 30, 'log_bytes': 16 * MIB}, guard)
                try:
                    for index, target in enumerate(reversed(mounted)):
                        if str(target) in mountpoints():
                            cleanup_runner.run('unmount-owned-%02d' % index, [tools['umount'], str(target)], out, environment)
                finally:
                    report['mount_cleanup'] = cleanup_runner.close()
                    report['mount_cleanup_commands'] = cleanup_runner.commands
                need(report['mount_cleanup']['status'] == 'PASS' and not report['mount_cleanup']['remaining'], 'owned unmount actor cleanup incomplete')
                for row in cleanup_runner.commands:
                    if 'log' in row: product(row['log']['path'])
                if (cleanup_out / 'commands.json').exists(): product(cleanup_out / 'commands.json')
            finish('reverse-owned-mount-cleanup', unmount_owned)
            def mounts_gone():
                report['remaining_mounts'] = [str(path) for path in mounted if str(path) in mountpoints()]
                need(not report['remaining_mounts'] and mounts_before == mountpoints(), 'owned mount remains or unrelated namespace mount changed')
            finish('exact-owned-mount-set-restored', mounts_gone)
        else: report['remaining_mounts'] = []
        def preserve():
            need(source_before is not None and common.source_state(source, manifest) == source_before, 'complete frozen source changed or baseline missing')
            for pin in pins.values(): need(common.opened(Path(pin['path']), max(64 * MIB, pin['bytes'])) == pin, 'selected current input changed')
            if source_qualified is not None: restart._preserve_tree(source_qualified, guard, TREE_LIMITS)
            if copy_qualified is not None: restart._preserve_tree(copy_qualified, guard, TREE_LIMITS)
            for index, previous in enumerate(immutable_catalogs):
                current = common.catalog(Path(previous['root']), out / ('copy-subtree-after-%02d.jsonl' % index),
                                         [Path(value['root']) for value in immutable_catalogs])
                product(current['catalog']['path'])
                need({key: value for key, value in previous.items() if key != 'catalog'} ==
                     {key: value for key, value in current.items() if key != 'catalog'}, 'immutable copied subtree changed during physics')
            for pin in products.values(): need(common.opened(Path(pin['path']), max(64 * MIB, pin['bytes'])) == pin, 'retained phase product changed')
            report.update(source_preserved=True, inputs_preserved=True, products_preserved=True, cgroup_after=guard())
        finish('source-current-input-and-content-preservation', preserve)
        report['finalization_errors'] = errors
        if report['status'] == 'RUNNING' and not errors:
            if report.get('content_prepared'): report['status'] = 'PREPARED_CONTENT_ONLY_NOT_EXECUTED'
            elif report.get('profile_completed'): report['status'] = 'COMPLETED_EXPLICIT_DIRECT3_HARDWARE_PROFILE_ONLY'
            elif report.get('run_completed'): report['status'] = 'COMPLETED_EXPLICIT_DIRECT_PHYSICS_ONLY'
        save()
    return 0 if report['status'] in ('PREPARED_CONTENT_ONLY_NOT_EXECUTED', 'COMPLETED_EXPLICIT_DIRECT_PHYSICS_ONLY', 'COMPLETED_EXPLICIT_DIRECT3_HARDWARE_PROFILE_ONLY') else 1


if __name__ == '__main__': raise SystemExit(main())
