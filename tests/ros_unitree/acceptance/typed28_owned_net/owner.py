#!/usr/bin/env python3
"""Own one complete Typed28 acceptance in a new loopback-only network namespace.

This separately selected external control program has outer/namespace phases.
Root supplies SHA-bound current observations; no future process or namespace is
predicted. The outer owns a same-PID unshare direct child by retained pidfd. The
inner enables only its private lo, derives a closed metadata request, runs the
reviewed metadata maker and then its exact28-case invocation in that same live
namespace. There is no mount/chroot/cgroup change, physical interface, download,
compiler, simulator or physical-robot action in this owner.
"""
from __future__ import annotations

import argparse
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

sys.dont_write_bytecode = True
MIB, GIB = 1024**2, 1024**3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
CG_KEYS = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode',
           'expected_boot_id', 'expected_leaf', 'expected_cpus'}
LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
SOURCE35_SHA = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
SOURCE43_SHA = '9a01f114e4091d4329392b291b5785952e99b22d8e2a48be52b7e02a7d8f0178'
HELPER_SHA = {'deriver': 'cecc4f80ef68d3c9e7c6f48325b0d427e59e3f68d035a1dfc9d59ef0d70f28b9',
              'maker': '86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb'}
PATHS = {'code_root', 'owner_out', 'namespace_out', 'request_directory', 'metadata_controls',
         'admission_out', 'transport_out'}
LIMITS = {'self_rss_bytes': 1536 * MIB, 'tree_rss_bytes': 4 * GIB,
          'active_seconds': 43200, 'whole_seconds': 43800, 'cleanup_seconds': 600,
          'metadata_seconds': 10800, 'transport_seconds': 31200,
          'log_bytes': 16 * MIB, 'output_bytes': 8 * GIB, 'reserve_bytes': 8 * GIB}
METADATA_STATUS = 'PASS_TYPED28_CONFIGURATION_AND_REAL_INPUT_ADMISSION_ONLY_TRANSPORT_NOT_EXECUTED'
TRANSPORT_STATUS = 'PASS_UNITREE_FOURTEEN_SAME_MAIN_COMMANDS_TWENTY_EIGHT_ACTUAL_TYPED_DDS_CASES'
OWNER_STATUS = 'PASS_OWNED_PRIVATE_LOOPBACK_TYPED28_METADATA_AND_TRANSPORT'


def need(value, reason):
    if not value:
        raise ValueError(reason)


def pairs(items):
    result = {}
    for key, value in items:
        need(key not in result, 'duplicate JSON member: ' + key)
        result[key] = value
    return result


def decode(raw):
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda value: need(False, 'nonfinite JSON value'))


def read(path, maximum=2 * MIB):
    """Read a bounded physical ordinary file before loading any project code."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical physical input required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, 'bounded ordinary input required')
        chunks, size = [], 0
        while True:
            block = os.read(descriptor, min(MIB, maximum - size + 1))
            if not block:
                break
            chunks.append(block); size += len(block)
            need(size <= maximum, 'bounded input grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(size == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'opened input changed')
        raw = b''.join(chunks)
        return raw, {'path': str(path), 'bytes': size, 'sha256': hashlib.sha256(raw).hexdigest(),
            'device': before.st_dev, 'inode': before.st_ino, 'mtime_ns': before.st_mtime_ns, 'ctime_ns': before.st_ctime_ns}
    finally:
        os.close(descriptor)


def full(pin):
    need(type(pin) is dict and set(pin) == PIN and type(pin['path']) is str and
         type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}', pin['sha256']) and
         all(type(pin[key]) is int and pin[key] >= 0 for key in PIN - {'path', 'sha256'}) and
         0 < pin['bytes'] <= 2 * GIB and pin['inode'] > 0, 'closed bounded current full-seven pin required')
    return pin


def raw_identity(pid):
    """Observe one Linux task birth without importing a project helper."""
    text = (Path('/proc') / str(pid) / 'stat').read_text()
    fields = text[text.rindex(')') + 2:].split()
    return {'pid': pid, 'parent_pid': int(fields[1]), 'starttime': int(fields[19]), 'state': fields[0]}


def birth(value):
    need(type(value) is dict and set(value) == {'pid', 'parent_pid', 'starttime', 'state'} and
         all(type(value[key]) is int and value[key] > 0 for key in ('pid', 'parent_pid', 'starttime')),
         'actual Linux task identity required')
    return value['pid'], value['starttime']


def current_context(value):
    """Admit the actual selected hard leaf with stdlib before external imports."""
    need(type(value) is dict and set(value) == CG_KEYS and
         type(value['memory_max']) is int and value['memory_max'] == 64 * GIB and
         type(value['memory_swap_max']) is int and value['memory_swap_max'] == 0 and
         value['expected_leaf'] == LEAF and value['expected_cpus'] == '0-31' and
         all(type(value[key]) is int and value[key] > 0 for key in ('expected_device', 'expected_inode')) and
         type(value['expected_boot_id']) is str and re.fullmatch('[0-9a-f-]{36}', value['expected_boot_id']),
         'closed current selection of original hard64GiB zero-swap leaf required')
    leaf = Path(LEAF); info = leaf.stat()
    need(leaf.resolve(strict=True) == leaf and (info.st_dev, info.st_ino) ==
         (value['expected_device'], value['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == value['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + LEAF.removeprefix('/sys/fs/cgroup')] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and
         (leaf / 'memory.swap.max').read_text().strip() == '0' and
         (leaf / 'cpuset.cpus.effective').read_text().strip() == '0-31', 'actual current cgroup facts differ')
    return dict(value)


def namespace_handoff(value, control):
    """Validate the actual direct parent BEFORE loading inner project helpers."""
    need(type(value) is dict and set(value) == {'format_version', 'kind', 'outer_identity',
         'parent_network_namespace', 'cgroup', 'source_manifest'} and value['format_version'] == 1 and
         value['kind'] == 'uwvm-unitree-typed28-owned-net-handoff-v1' and value['cgroup'] == control['cgroup'] and
         value['source_manifest'] == control['source']['manifest'] and
         birth(raw_identity(os.getppid())) == birth(value['outer_identity']) and
         os.readlink('/proc/%d/ns/net' % os.getppid()) == value['parent_network_namespace'],
         'same-birth live direct outer parent/current source handoff required')
    own = os.readlink('/proc/self/ns/net')
    need(re.fullmatch(r'net:\[[1-9][0-9]*\]', own) and own != value['parent_network_namespace'],
         'actual private network namespace must differ from parent')
    return own


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--phase', choices=('outer', 'namespace'), required=True)
    parser.add_argument('--control', type=Path, required=True)
    parser.add_argument('--control-sha256', required=True)
    parser.add_argument('--owner-sha256', required=True)
    parser.add_argument('--handoff', type=Path); parser.add_argument('--handoff-sha256')
    args = parser.parse_args(); started = time.monotonic()
    need(sys.platform == 'linux' and os.geteuid() == 0 and hasattr(os, 'pidfd_open') and
         hasattr(signal, 'pidfd_send_signal'), 'actual Root Linux pidfd lane required')
    raw, control_pin = read(args.control)
    need(control_pin['sha256'] == args.control_sha256 and re.fullmatch('[0-9a-f]{64}', args.owner_sha256),
         'explicit selected control and owner source SHA256 required')
    control = decode(raw); cg = current_context(control.get('cgroup'))
    need(type(control) is dict and set(control) == {'format_version', 'kind', 'cgroup', 'bootstrap_source', 'source',
         'python', 'helpers', 'tools', 'inputs', 'restart_mappings', 'paths', 'limits'} and control['format_version'] == 1 and
         control['kind'] == 'uwvm-unitree-typed28-owned-net-control-v1' and control['limits'] == LIMITS and
         type(control['helpers']) is dict and set(control['helpers']) == {'owner', 'deriver', 'maker'} and
         type(control['tools']) is dict and set(control['tools']) == {'unshare', 'ip'} and
         type(control['inputs']) is dict and set(control['inputs']) == {'typed_build', 'workers', 'native_retained', 'cyclonedds_xml'} and
         type(control['paths']) is dict and set(control['paths']) == PATHS, 'closed explicit Typed28 owner control required')
    for selected in [control['python'], *control['helpers'].values(), *control['tools'].values(), *control['inputs'].values()]:
        full(selected)
    need(control['helpers']['owner']['path'] == str(Path(__file__).absolute()) and
         control['helpers']['owner']['sha256'] == args.owner_sha256, 'exact separately selected owner bytes required')
    owner_body, owner_pin = read(__file__)
    need(owner_pin == control['helpers']['owner'], 'current owner physical source changed')
    paths = {key: Path(value) for key, value in control['paths'].items()}
    need(all(type(control['paths'][key]) is str and path.is_absolute() and '..' not in path.parts and
         path.parent.resolve(strict=True) == path.parent for key, path in paths.items()) and
         all(left != right and left not in right.parents and right not in left.parents for index, left in enumerate(paths.values())
             for right in list(paths.values())[index + 1:]) and
         paths['code_root'].resolve(strict=True) == paths['code_root'] and paths['code_root'].is_dir() and
         len(os.fsencode(paths['transport_out'] / 's27')) < 104, 'canonical disjoint actual owner/short transport paths required')
    need(args.control.parent == paths['code_root'] and all(Path(pin['path']).parent == paths['code_root']
         for pin in control['helpers'].values()), 'owner/control/three helper roles must use the sealed code root')
    allowed_code = {args.control, *(Path(pin['path']) for pin in control['helpers'].values())}
    xml_path = Path(control['inputs']['cyclonedds_xml']['path'])
    if xml_path.parent == paths['code_root']:
        allowed_code.add(xml_path)
    need(set(paths['code_root'].iterdir()) == allowed_code, 'closed static code root file set required')
    handoff_pin, handoff, selected_ns = None, None, None
    if args.phase == 'namespace':
        need(args.handoff is not None and args.handoff_sha256 is not None and args.handoff.parent == paths['owner_out'],
             'actual outer owner handoff pin required')
        body, handoff_pin = read(args.handoff)
        need(handoff_pin['sha256'] == args.handoff_sha256, 'actual outer handoff bytes differ')
        handoff = decode(body); selected_ns = namespace_handoff(handoff, control)
    else:
        need(args.handoff is None and args.handoff_sha256 is None, 'outer phase accepts no fabricated child handoff')

    # The Source35 bootstrap guard is an explicit historical SOURCE ROLE, not
    # an old cgroup observation. All file pins and the current control are new.
    source_roles, source_bodies, initial_inputs = {}, {}, {control_pin['path']: control_pin, owner_pin['path']: owner_pin}
    for role, wanted in (('bootstrap_source', SOURCE35_SHA), ('source', SOURCE43_SHA)):
        section = control[role]
        need(type(section) is dict and set(section) == {'root', 'manifest'}, 'closed complete source role required')
        root, mf = Path(section['root']), full(section['manifest'])
        need(root.is_absolute() and root.resolve(strict=True) == root and mf['path'] == str(root.parent / 'source-manifest.json') and
             mf['sha256'] == wanted and all(root != path and root not in path.parents and path not in root.parents
             for path in paths.values()), 'exact complete source/manifest role and disjoint outputs required')
        body, actual = read(mf['path'], 16 * MIB)
        need(actual == mf, 'current complete source manifest pin differs')
        frozen = decode(body); need(type(frozen) is dict and frozen, 'complete source manifest dictionary required')
        source_roles[role] = (root, mf, frozen); initial_inputs[mf['path']] = actual
        relatives = ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py')
        if role == 'source':
            relatives += ('tests/ros_unitree/common.py', 'tests/ros_unitree/acceptance/native74_r6/native_producer.py')
        for relative in relatives:
            body, pin = read(root / relative, 2 * MIB)
            need(frozen.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')}, 'complete manifest helper binding differs')
            source_bodies[role, relative] = body; initial_inputs[pin['path']] = pin
    need(source_roles['bootstrap_source'][0] != source_roles['source'][0], 'separate Source35/43 source roles required')

    def module(name, path, body):
        result = types.ModuleType(name); result.__file__ = str(path); sys.modules[name] = result
        exec(compile(body, result.__file__, 'exec'), result.__dict__)
        return result

    common35 = module('typed28_net_bootstrap_common35', source_roles['bootstrap_source'][0] / 'tools/ros_distro/common.py',
                      source_bodies['bootstrap_source', 'tools/ros_distro/common.py'])
    common35._CGROUP_PROBE = module('typed28_net_bootstrap_probe35', source_roles['bootstrap_source'][0] / 'tests/ros_cgroup/probe.py',
                                  source_bodies['bootstrap_source', 'tests/ros_cgroup/probe.py'])
    initial_cg = common35.cgroup_state(cg)
    need(all(row['events'].get(key, 0) == 0 for row in initial_cg['ancestors']
         for key in ('oom', 'oom_kill', 'oom_group_kill')), 'all actual ancestor OOM counters must start at zero')
    common = module('typed28_net_common43', source_roles['source'][0] / 'tools/ros_distro/common.py',
                    source_bodies['source', 'tools/ros_distro/common.py'])
    common._CGROUP_PROBE = module('typed28_net_probe43', source_roles['source'][0] / 'tests/ros_cgroup/probe.py',
                                source_bodies['source', 'tests/ros_cgroup/probe.py'])
    common.install_interrupts(); common.cgroup_preserved(initial_cg, common.cgroup_state(cg))
    out = paths['owner_out'] if args.phase == 'outer' else paths['namespace_out']
    need(not out.exists() and not out.is_symlink(), 'fresh exact owner phase output required')
    if args.phase == 'outer':
        need(all(not paths[key].exists() and not paths[key].is_symlink() for key in PATHS - {'code_root'}),
             'all actual owner and future maker/transport outputs must be fresh')
    else:
        need(all(not paths[key].exists() and not paths[key].is_symlink() for key in
             ('request_directory', 'metadata_controls', 'admission_out', 'transport_out')), 'fresh inner metadata/transport outputs required')
    out.mkdir(mode=0o700)
    pins, products = dict(initial_inputs), {}
    if handoff_pin:
        pins[handoff_pin['path']] = handoff_pin
    runner = None; unitree = None; finalizing = False; next_guard = 0.0; next_size = 0.0
    sources_before, static_before, runtime_before = {}, [], []
    static_roots, runtime_roots = [], []
    phase_success = False
    report = {'format_version': 1, 'kind': 'uwvm-unitree-typed28-owned-private-net-acceptance', 'phase': args.phase,
        'status': 'RUNNING', 'control': control_pin, 'owner': owner_pin, 'cgroup_before': initial_cg,
        'source_roles': {role: {'root': str(value[0]), 'manifest': value[1]} for role, value in source_roles.items()},
        'commands': [], 'Guest_rights_changed': False, 'physical_robot_access': False,
        'mount_operations_performed': False, 'cgroup_changes_performed': False, 'compiler_executed': False,
        'namespace_created_by_this_phase': args.phase == 'namespace', 'finalization_errors': []}

    def save():
        report['commands'] = [] if runner is None else runner.commands
        common.atomic_json(out / 'report.json', report)

    def guard(force=False):
        nonlocal next_guard, next_size
        now = time.monotonic()
        need(now - started <= LIMITS['whole_seconds' if finalizing else 'active_seconds'], 'finite owner/cleanup deadline exceeded')
        if not force and now < next_guard:
            return
        next_guard = now + .5
        current_context(cg)
        if args.phase == 'namespace' and not finalizing:
            need(namespace_handoff(handoff, control) == selected_ns, 'active same-birth outer parent/private namespace changed')
        common35.cgroup_preserved(initial_cg, common35.cgroup_state(cg))
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= LIMITS['self_rss_bytes'], 'owner metadata RSS exceeded1536MiB')
        need(runner is None or runner.rss() + resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= LIMITS['tree_rss_bytes'],
             'whole owned tree exceeds4GiB RSS')
        if not finalizing:
            for root in paths.values():
                for ancestor in (root.parent, *root.parent.parents):
                    space = os.statvfs(ancestor)
                    need(space.f_bavail * space.f_frsize >= LIMITS['reserve_bytes'], 'eight-GiB backing reserve exhausted')
        if now >= next_size:
            next_size = now + 2
            size = 0
            for key, root in paths.items():
                if key == 'code_root' or not root.exists():
                    continue
                for folder, dirs, files in os.walk(root, followlinks=False):
                    for name in [*dirs, *files]:
                        selected = Path(folder) / name
                        try:
                            info = selected.lstat()
                        except FileNotFoundError:
                            # The owned Runtime removes exact live sockets; the
                            # frozen atomic publisher renames JSON.pending files.
                            # No input or terminal-preservation read uses this rule.
                            need((selected.parent == paths['transport_out'] and
                                  re.fullmatch(r's(?:[01][0-9]|2[0-7])', name)) or name.endswith('.json.pending'),
                                 'unexpected owned output disappeared during size audit')
                            continue
                        need(stat.S_ISREG(info.st_mode) or stat.S_ISDIR(info.st_mode) or
                             (stat.S_ISSOCK(info.st_mode) and selected.parent == paths['transport_out'] and
                              re.fullmatch(r's(?:[01][0-9]|2[0-7])', name) and info.st_uid == os.getuid()),
                             'ordinary output or exact owned live Typed28 socket required')
                        if stat.S_ISREG(info.st_mode):
                            size += info.st_size
                        need(size <= LIMITS['output_bytes'], 'aggregate owned output exceeds8GiB')
        return initial_cg

    def remember(pin):
        full(pin); guard()
        actual = common.opened(Path(pin['path']), max(64 * MIB, pin['bytes']))
        need(actual == pin and (pin['path'] not in pins or pins[pin['path']] == actual), 'selected current input changed')
        pins[pin['path']] = actual
        return actual

    def document(pin, maximum=64 * MIB):
        remember(pin); body, again = read(pin['path'], maximum)
        need(again == pin, 'current selected JSON changed')
        return decode(body)

    def product(path, maximum=64 * MIB):
        guard(); pin = common.opened(Path(path), maximum)
        need(pin['path'] not in products or products[pin['path']] == pin, 'retained product changed')
        products[pin['path']] = pin
        return pin

    def load_helper(role):
        pin = remember(control['helpers'][role]); body, actual = read(pin['path'])
        need(actual == pin and pin['sha256'] == HELPER_SHA[role], 'exact reviewed external helper bytes required')
        return module('typed28_net_external_' + role, pin['path'], body)

    def snapshots(roots, prefix):
        result = []
        for index, root in enumerate(roots):
            guard(); row = common.catalog(root, out / (prefix + '-%02d.jsonl' % index), roots)
            product(row['catalog']['path']); result.append(row)
        return result

    def same_catalog(left, right):
        return {key: value for key, value in left.items() if key != 'catalog'} == \
               {key: value for key, value in right.items() if key != 'catalog'}

    def check_frame_result(result, expected, namespace):
        need(result.get('status') == expected and result.get('source') == control['source']['root'] and
             result.get('source_manifest_sha256') == SOURCE43_SHA and all(result.get(key) is True for key in
             ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved')) and
             not result.get('finalization_errors'), 'actual complete preserved source43 Frame result required')
        for cleanup in result.get('cleanup', []):
            need(cleanup['status'] == 'PASS' and not cleanup.get('remaining'), 'actual Frame owned cleanup failed')
        if expected == METADATA_STATUS:
            need(result.get('actors_started') == result.get('actual_Runtime_cases_executed') == 0 and
                 result.get('compiler_executed') is False and result.get('ROS_DDS_simulator_executed') is False and
                 result.get('transport_output_absent') is True, 'metadata stage must not execute transport/targets')
        else:
            cases = result.get('cases', [])
            need(result.get('actual_Runtime_cases_executed') == 28 and len(cases) == 28 and
                 len({row['case'] for row in cases}) == 28 and sum(row['positive'] for row in cases) == 14 and
                 all(row['status'] == 'PASS_ACTUAL_TYPED_DDS_CASE' and row['worker_exit'] == row['native_exit'] == 0 and
                 row['cleanup']['status'] == 'PASS' for row in cases) and result.get('optional_WASI_rights') == [] and
                 result.get('default_ROS_authority') == [] and result['admission']['network']['network_namespace'] == namespace,
                 'all28 actual same-main denied/positive cases and exact namespace required')

    environment = {'PATH': '/usr/bin:/bin', 'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8', 'PYTHONDONTWRITEBYTECODE': '1',
        'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1', 'MKL_NUM_THREADS': '1', 'GOMAXPROCS': '1'}
    try:
        guard(True)
        for selected in [control['python'], *control['helpers'].values(), *control['tools'].values(), *control['inputs'].values()]:
            remember(selected)
        need(str(Path(sys.executable).resolve(strict=True)) == control['python']['path'] and
             all(os.access(pin['path'], os.X_OK) for pin in [control['python'], *control['tools'].values()]),
             'actual current executing Python and executable unshare/ip selections required')
        for role, (root, mf, _) in source_roles.items():
            guard(); sources_before[role] = common.source_state(root, Path(mf['path']))
        static_roots = [paths['code_root']]
        if xml_path.parent != paths['code_root']:
            static_roots.append(xml_path.parent)
        need(all(root.resolve(strict=True) == root and all(root != path and root not in path.parents and path not in root.parents
             for key, path in paths.items() if key != 'code_root') for root in static_roots), 'static input roots must be disjoint from outputs')
        static_before = snapshots(static_roots, 'static-before')
        runner = common.SerialRunner(out, {'rss_bytes': LIMITS['tree_rss_bytes'],
            'command_seconds': LIMITS['active_seconds'], 'log_bytes': LIMITS['log_bytes']}, guard)
        if args.phase == 'outer':
            handoff_path = out / 'namespace-handoff.json'
            common.atomic_json(handoff_path, {'format_version': 1, 'kind': 'uwvm-unitree-typed28-owned-net-handoff-v1',
                'outer_identity': common.identity(os.getpid()), 'parent_network_namespace': os.readlink('/proc/self/ns/net'),
                'cgroup': cg, 'source_manifest': control['source']['manifest']})
            hp = product(handoff_path)
            argv = [control['tools']['unshare']['path'], '--net', '--', control['python']['path'], '-B', owner_pin['path'],
                '--phase', 'namespace', '--control', control_pin['path'], '--control-sha256', control_pin['sha256'],
                '--owner-sha256', owner_pin['sha256'], '--handoff', hp['path'], '--handoff-sha256', hp['sha256']]
            save(); runner.run('actual-owned-private-net-coordinator', argv, paths['code_root'], environment)
            inner_pin = product(paths['namespace_out'] / 'report.json')
            inner = document(inner_pin)
            need(inner['status'] == OWNER_STATUS and inner.get('source_preserved') is True and inner.get('inputs_preserved') is True and
                 inner.get('products_preserved') is True and inner.get('whole_roots_preserved') is True and
                 inner.get('cleanup', {}).get('status') == 'PASS' and not inner.get('finalization_errors') and
                 inner.get('actual_Runtime_cases_executed') == 28, 'actual completed inner private-net acceptance required')
            report.update(namespace_report=inner_pin, actual_Runtime_cases_executed=28,
                          actual_transport_report=inner['actual_transport_report'], network_namespace=inner['network_namespace'])
            phase_success = True
        else:
            need(namespace_handoff(handoff, control) == selected_ns, 'actual inner handoff changed')
            report.update(network_namespace=selected_ns, actual_parent_handoff=handoff_pin,
                          namespace_owner_identity=common.identity(os.getpid()))
            runner.limits = {'rss_bytes': 512 * MIB, 'command_seconds': 30, 'log_bytes': LIMITS['log_bytes']}
            runner.run('bring-only-owned-private-loopback-up', [control['tools']['ip']['path'], 'link', 'set', 'dev', 'lo', 'up'],
                       paths['code_root'], environment)
            unitree = module('typed28_net_unitree43', source_roles['source'][0] / 'tests/ros_unitree/common.py',
                             source_bodies['source', 'tests/ros_unitree/common.py'])
            report['actual_loopback_before'] = unitree.current_loopback(selected_ns)
            native = module('typed28_net_elf_decoder43', source_roles['source'][0] / 'tests/ros_unitree/acceptance/native74_r6/native_producer.py',
                            source_bodies['source', 'tests/ros_unitree/acceptance/native74_r6/native_producer.py'])
            metadata, deriver = load_helper('maker'), load_helper('deriver')
            request, provenance = deriver.derive_typed28_request(common, metadata, unitree, native.elf, guard,
                typed_build_current=control['inputs']['typed_build'], workers_current=control['inputs']['workers'],
                native_retained_current=control['inputs']['native_retained'], metadata_maker_current=control['helpers']['maker'],
                source=control['source']['root'], manifest_current=control['source']['manifest'], python_current=control['python'],
                cgroup=cg, parent_handoff_current=handoff_pin, owned_namespace_identity=report['namespace_owner_identity'],
                cyclonedds_current=control['inputs']['cyclonedds_xml'], restart_mappings=control['restart_mappings'],
                fresh_paths={'controls': str(paths['metadata_controls']), 'admission_out': str(paths['admission_out']),
                             'transport_out': str(paths['transport_out'])})
            paths['request_directory'].mkdir(mode=0o700)
            common.atomic_json(paths['request_directory'] / 'request.json', request)
            common.atomic_json(paths['request_directory'] / 'derivation.json', provenance)
            request_pin = product(paths['request_directory'] / 'request.json')
            product(paths['request_directory'] / 'derivation.json')
            report['actual_request'] = request_pin
            runner.limits = {'rss_bytes': 2 * GIB, 'command_seconds': LIMITS['metadata_seconds'], 'log_bytes': LIMITS['log_bytes']}
            argv = [control['python']['path'], '-B', control['helpers']['maker']['path'], '--request', request_pin['path'],
                '--request-sha256', request_pin['sha256'], '--maker-sha256', HELPER_SHA['maker']]
            save(); runner.run('actual-typed28-metadata-and-real-input-admission', argv, paths['code_root'], environment)
            admission_pin = product(paths['admission_out'] / 'report.json')
            admission = document(admission_pin); check_frame_result(admission, METADATA_STATUS, selected_ns)
            report['actual_metadata_report'] = admission_pin
            config_pin = admission['derived_configuration']; config = document(config_pin)
            selection_pin = admission['derived_selection']; remember(selection_pin)
            invocation_pin = admission['future_invocation']; invocation = document(invocation_pin, MIB)
            expected = [control['python']['path'], '-B', str(source_roles['source'][0] / 'tests/ros_unitree/run_typed_transport.py'),
                '--source', control['source']['root'], '--manifest', control['source']['manifest']['path'],
                '--manifest-sha256', SOURCE43_SHA, '--config', config_pin['path'], '--config-sha256', config_pin['sha256'],
                '--selection-input', 'typed-transport-selection', '--out', str(paths['transport_out'])]
            need(type(invocation) is dict and set(invocation) == {'argv', 'execution_performed'} and
                 invocation['execution_performed'] is False and invocation['argv'] == expected and config['cgroup'] == cg and
                 set(config['environment']) == {'ROS_DOMAIN_ID', 'RMW_IMPLEMENTATION', 'ROS_SECURITY_ENABLE', 'CYCLONEDDS_URI'} and
                 all(config[key] is None for key in ('simulation', 'benchmark', 'profiler')),
                 'only exact actual maker-published source43 Typed28 invocation may execute')
            runtime_roots = [Path(root) for root in config['input_roots']]
            need(1 <= len(runtime_roots) <= 64 and len(set(runtime_roots)) == len(runtime_roots) and
                 all(root.resolve(strict=True) == root and root.is_dir() for root in runtime_roots), 'complete actual Frame root closure required')
            runtime_before = snapshots(runtime_roots, 'runtime-before')
            previous = {row['root']: row for row in admission['inventories_after']}
            need(set(previous) == {row['root'] for row in runtime_before} and all(same_catalog(previous[row['root']], row)
                 for row in runtime_before), 'independent owner inventories must equal actual maker terminal root facts')
            need(not paths['transport_out'].exists() and not paths['transport_out'].is_symlink(), 'metadata maker must leave transport output absent')
            namespace_handoff(handoff, control); unitree.current_loopback(selected_ns); guard(True)
            runner.limits = {'rss_bytes': 2 * GIB, 'command_seconds': LIMITS['transport_seconds'], 'log_bytes': LIMITS['log_bytes']}
            save(); runner.run('actual-twenty-eight-same-main-typed-DDS-cases', expected, source_roles['source'][0], environment)
            result_pin = product(paths['transport_out'] / 'report.json')
            result = document(result_pin); check_frame_result(result, TRANSPORT_STATUS, selected_ns)
            report.update(actual_transport_report=result_pin, actual_Runtime_cases_executed=28,
                          actual_loopback_after=unitree.current_loopback(selected_ns))
            namespace_handoff(handoff, control)
            phase_success = True
    except BaseException as error:
        report.update(status='FAIL', error=repr(error), traceback=traceback.format_exc())
    finally:
        finalizing = True; next_guard = 0.0; errors = []
        def finish(stage, operation):
            try:
                operation()
            except BaseException as error:
                errors.append({'stage': stage, 'error': repr(error)}); report['status'] = 'FAIL'
        def cooperative():
            if runner is None:
                return
            for child in runner.children:
                if child.poll() is not None:
                    continue
                previous, current = runner.roots.get(child.pid), common.identity(child.pid)
                need(previous is not None and current is not None and common.birth(previous) == common.birth(current) and
                     current['parent_pid'] == os.getpid(), 'same-birth owned direct child required before cooperative TERM')
                runner.acquire(child.pid, os.getpid()); key = common.birth(previous)
                need(key in runner.owned or child.poll() is not None, 'direct child lacks retained pidfd')
                if key not in runner.owned:
                    continue
                try:
                    signal.pidfd_send_signal(runner.owned[key]['fd'], signal.SIGTERM)
                except ProcessLookupError:
                    pass
                deadline = min(started + LIMITS['whole_seconds'], time.monotonic() + LIMITS['cleanup_seconds'])
                while child.poll() is None and time.monotonic() < deadline:
                    guard(True); runner.discover()
                    need(runner.rss() <= LIMITS['tree_rss_bytes'], 'owned cooperative cleanup RSS exceeded')
                    time.sleep(.05)
        def cleanup():
            report['cleanup'] = runner.close() if runner else {'status': 'PASS', 'remaining': [], 'scope': 'NO_ACTOR_STARTED'}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup']['remaining'] and
                 not report['cleanup'].get('errors'), 'complete child-first owned pidfd cleanup required')
        finish('same-birth-cooperative-direct-child-600-second-tail', cooperative)
        finish('child-first-retained-pidfd-cleanup', cleanup)
        if runner:
            for row in runner.commands:
                if 'log' in row:
                    finish('retained-final-command-log', lambda row=row: product(row['log']['path']))
            if (out / 'commands.json').exists():
                finish('retained-final-command-ledger', lambda: product(out / 'commands.json'))
        def partial_reports():
            # Inspect only three exact known child receipt locations after
            # cleanup. Their unchanged raw status is evidence, never success.
            observed = []
            for role, path in (('namespace', paths['namespace_out'] / 'report.json'),
                               ('metadata', paths['admission_out'] / 'report.json'),
                               ('transport', paths['transport_out'] / 'report.json')):
                if path == out / 'report.json' or not path.exists():
                    continue
                pin = product(path)
                body, actual = read(path, 64 * MIB)
                need(actual == pin, 'observed terminal/partial child receipt changed')
                raw_result = decode(body)
                need(type(raw_result) is dict, 'ordinary child receipt dictionary required')
                observed.append({'role': role, 'pin': pin, 'raw_status': raw_result.get('status'),
                                 'raw_status_promoted': False})
            report['observed_child_reports_after_cleanup'] = observed
        finish('exact-child-receipt-observations-after-owned-cleanup', partial_reports)
        def preserve():
            need(len(static_before) == len(static_roots) >= 1,
                 'complete static input baselines required before whole-root preservation claim')
            need(not runtime_roots or len(runtime_before) == len(runtime_roots),
                 'complete selected Runtime input baselines required before whole-root preservation claim')
            for role, (root, mf, _) in source_roles.items():
                guard(True)
                need(role in sources_before and common.source_state(root, Path(mf['path'])) == sources_before[role],
                     'complete source role changed or baseline missing')
            for pin in pins.values():
                guard(); need(common.opened(Path(pin['path']), max(64 * MIB, pin['bytes'])) == pin, 'current selected input changed')
            if static_before:
                after = snapshots([Path(row['root']) for row in static_before], 'static-after')
                need(all(same_catalog(left, right) for left, right in zip(static_before, after)), 'whole static code/profile roots changed')
                report['static_roots_before'], report['static_roots_after'] = static_before, after
            if runtime_before:
                after = snapshots(runtime_roots, 'runtime-after')
                need(all(same_catalog(left, right) for left, right in zip(runtime_before, after)), 'whole actual Runtime input roots changed')
                report['runtime_roots_before'], report['runtime_roots_after'] = runtime_before, after
            for pin in products.values():
                guard(); need(common.opened(Path(pin['path']), max(64 * MIB, pin['bytes'])) == pin, 'retained owner product changed')
            if args.phase == 'namespace':
                need(unitree is not None, 'source-bound live loopback helper must have been admitted')
                need(namespace_handoff(handoff, control) == selected_ns, 'owned private namespace changed at finalization')
                report['actual_loopback_final'] = unitree.current_loopback(selected_ns)
            report.update(source_preserved=True, inputs_preserved=True, products_preserved=True, whole_roots_preserved=True,
                          cgroup_after=common35.cgroup_state(cg))
            common35.cgroup_preserved(initial_cg, report['cgroup_after']); guard(True)
        finish('complete-source-current-pins-static-and-Runtime-roots-preservation', preserve)
        report['input_pins'], report['product_pins'] = list(pins.values()), list(products.values())
        report['finalization_errors'] = errors
        if phase_success and report['status'] == 'RUNNING' and not errors:
            report['status'] = OWNER_STATUS
        save()
    return 0 if report['status'] == OWNER_STATUS else 1


if __name__ == '__main__':
    raise SystemExit(main())
