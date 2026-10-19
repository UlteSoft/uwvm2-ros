#!/usr/bin/env python3
"""Construct a current-source Typed28 control from actual Root observations.

Root SHA-binds this callable before loading it inside its existing current64
supervisor. It starts no child, imports no candidate helper, creates no network
namespace and executes no compiler, Runtime or ROS. Future source/MF hashes,
process identities and namespace numbers are never forecast or embedded here.
The only mutation is one new control in Root's already-owned sealed prestage.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys

MIB, GIB = 1024**2, 1024**3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
CG = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode', 'expected_boot_id', 'expected_leaf', 'expected_cpus'}
LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
BOOTSTRAP_SHA = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
WORKER_SHA = '18932cb8abc7d9998eb7431f751d6d1feea687cb380f1b247eef12750de0fb64'
HELPER_SOURCE = {'owner': 'tests/ros_unitree/acceptance/ownedtyped28_current/owner.py',
    'deriver': 'tests/ros_unitree/acceptance/ownedtyped28_current/derive_request.py',
    'maker': 'tests/ros_unitree/acceptance/make_typed_transport_configuration.py'}
PATH_KEYS = {'code_root', 'owner_out', 'namespace_out', 'request_directory', 'metadata_controls', 'admission_out', 'transport_out'}
LIMITS = {'self_rss_bytes': 1536 * MIB, 'tree_rss_bytes': 4 * GIB,
    'active_seconds': 43200, 'whole_seconds': 43800, 'cleanup_seconds': 600,
    'metadata_seconds': 10800, 'transport_seconds': 31200, 'log_bytes': 16 * MIB,
    'output_bytes': 8 * GIB, 'reserve_bytes': 8 * GIB}


def need(value, reason):
    if not value:
        raise ValueError(reason)


def stage_sources(common, guard, *, source, manifest_current, code_root, cyclonedds_current):
    """Copy exactly three manifest-bound helpers and the actual XML into fresh code.

    This optional Root-callable prestager supplies *actual new* full7 pins for
    ``make_control``. It never copies or predicts the complete manifest inside
    its source tree, invokes a helper, creates a control or starts an actor.
    Failed partial output stays named for inspection; it is never overwritten,
    removed, reused or described as a completed prestage.
    """
    need(callable(guard) and sys.platform == 'linux' and os.geteuid() == 0, 'actual Root guarded metadata stage required')
    guard()
    source, code_root = Path(source), Path(code_root)
    need(source.is_absolute() and source.resolve(strict=True) == source and source.is_dir() and
         code_root.is_absolute() and code_root.parent.resolve(strict=True) == code_root.parent and
         '..' not in code_root.parts and not code_root.exists() and not code_root.is_symlink() and
         source != code_root and source not in code_root.parents and code_root not in source.parents,
         'fresh canonical prestage disjoint from complete source required')
    raw, manifest_pin = common.body_pin(Path(manifest_current['path']), 16 * MIB)
    need(manifest_pin == manifest_current and manifest_pin['path'] == str(source.parent / 'source-manifest.json'),
         'actual complete current manifest observation differs')
    def pairs(items):
        value = {}
        for key, item in items:
            need(key not in value, 'duplicate complete manifest key')
            value[key] = item
        return value
    frozen = json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite manifest'))
    before = common.source_state(source, Path(manifest_pin['path']))
    own = common.opened(Path(__file__).absolute(), 2 * MIB)
    need(frozen.get('tests/ros_unitree/acceptance/ownedtyped28_current/make_control.py') ==
         {key: own[key] for key in ('bytes', 'sha256')}, 'prestager source must equal its complete-manifest entry')
    for ancestor in (code_root.parent, *code_root.parent.parents):
        fs = os.statvfs(ancestor)
        need(fs.f_bavail * fs.f_frsize >= 8 * GIB, 'active prestage backing reserve exhausted')
    xml_raw, xml_pin = common.body_pin(Path(cyclonedds_current['path']), MIB)
    need(xml_pin == cyclonedds_current, 'actual selected Cyclone XML changed')
    sources = {}
    for role, relative in HELPER_SOURCE.items():
        body, pin = common.body_pin(source / relative, 2 * MIB)
        need(frozen.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')}, 'complete source helper changed before copy')
        sources[role] = (body, pin)
    code_root.mkdir(mode=0o700)
    directory = os.open(code_root, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
    staged = {}
    try:
        first = os.fstat(directory)
        for role, name in (('owner', 'owner.py'), ('deriver', 'derive_request.py'), ('maker', 'maker.py'),
                           ('cyclonedds_xml', 'cyclonedds.xml')):
            guard()
            body, original = (xml_raw, xml_pin) if role == 'cyclonedds_xml' else sources[role]
            descriptor = os.open(name, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC,
                                 0o400, dir_fd=directory)
            try:
                pending = memoryview(body)
                while pending:
                    guard(); count = os.write(descriptor, pending)
                    need(count > 0, 'source copy made no progress'); pending = pending[count:]
                os.fchmod(descriptor, 0o400); os.fsync(descriptor)
            finally:
                os.close(descriptor)
            pin = common.opened(code_root / name, 2 * MIB)
            need(all(pin[key] == original[key] for key in ('bytes', 'sha256')) and
                 (pin['device'], pin['inode']) != (original['device'], original['inode']) and
                 (code_root / name).stat().st_nlink == 1, 'independent complete prestaged source copy required')
            staged[role] = pin
        os.fsync(directory)
        after, named = os.fstat(directory), code_root.stat(follow_symlinks=False)
        need((first.st_dev, first.st_ino) == (after.st_dev, after.st_ino) == (named.st_dev, named.st_ino) and
             stat.S_IMODE(after.st_mode) == 0o700 and set(code_root.iterdir()) == {Path(pin['path']) for pin in staged.values()},
             'exact held fresh prestage directory changed')
    finally:
        os.close(directory)
    need(common.source_state(source, Path(manifest_pin['path'])) == before and
         common.opened(Path(xml_pin['path']), MIB) == xml_pin, 'source/XML changed during prestage')
    for pin in staged.values():
        need(common.opened(Path(pin['path']), 2 * MIB) == pin, 'actual new prestage copy changed')
    guard()
    return {'status': 'STAGED_CURRENT_SOURCE_HELPERS_AND_LOOPBACK_XML_ONLY_NO_CONTROL_OR_ACTORS',
        'code_root': str(code_root), 'helpers_current': {role: staged[role] for role in HELPER_SOURCE},
        'cyclonedds_current': staged['cyclonedds_xml'], 'original_cyclonedds': xml_pin,
        'source': str(source), 'manifest': manifest_pin, 'source_preserved': True,
        'control_created': False, 'actors_started': 0, 'Runtime_ROS_DDS_executed': False}


def make_control(common, guard, *, source, manifest_current, bootstrap_source,
        bootstrap_manifest_current, cgroup, typed_build_current, workers_current,
        native_retained_current, cp310_receipt_current, python_current,
        tools_current, helpers_current, cyclonedds_current, fresh_paths, restart_mappings):
    """Write one actual current control and return its still-unexecuted argv.

    ``common`` is Root's explicitly source-bound retained Source35 common/probe.
    The new complete source is independently supplied, fully verified and used
    solely through its actual manifest entries. Staged helper full7 pins must
    match those entries before publication; their hashes are not compiled into
    an owner/MF/self-reference cycle. Historical Typed40/Worker37/Native39/CP3
    roles remain original evidence, never current actor/cgroup authority.
    """
    need(callable(guard) and sys.platform == 'linux' and os.geteuid() == 0,
         'actual Root current64 metadata supervisor required')
    pins = {}
    def current(pin, maximum=64 * MIB):
        guard()
        need(type(pin) is dict and set(pin) == PIN and type(pin['path']) is str and
             type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}', pin['sha256']) and
             all(type(pin[key]) is int and pin[key] >= 0 for key in PIN - {'path', 'sha256'}) and
             0 < pin['bytes'] <= maximum and pin['inode'] > 0, 'closed bounded current full7 observation required')
        path = Path(pin['path'])
        need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute current file required')
        actual = common.opened(path, maximum)
        need(actual == pin and (pin['path'] not in pins or pins[pin['path']] == actual), 'actual observed input changed')
        pins[pin['path']] = actual
        return actual

    def document(pin, maximum=64 * MIB):
        current(pin, maximum)
        raw, observed = common.body_pin(Path(pin['path']), maximum)
        need(observed == pin, 'opened current document changed')
        def pairs(items):
            value = {}
            for key, item in items:
                need(key not in value, 'duplicate actual control document key')
                value[key] = item
            return value
        return json.loads(raw, object_pairs_hook=pairs,
                          parse_constant=lambda _: need(False, 'nonfinite current document'))

    # Current authority comes only from Root's explicit closed7 and live kernel
    # observations, never an old successful producer's cgroup snapshot.
    guard()
    need(type(cgroup) is dict and set(cgroup) == CG and
         type(cgroup['memory_max']) is int and cgroup['memory_max'] == 64 * GIB and
         type(cgroup['memory_swap_max']) is int and cgroup['memory_swap_max'] == 0 and
         cgroup['expected_leaf'] == LEAF and cgroup['expected_cpus'] == '0-31', 'original hard64/swap0/cpus leaf required')
    before = common.cgroup_state(cgroup)
    need(all(row['events'].get(key, 0) == 0 for row in before['ancestors']
             for key in ('oom', 'oom_kill', 'oom_group_kill')), 'all current ancestor OOM counters must be zero')
    source, bootstrap_source = Path(source), Path(bootstrap_source)
    need(source.is_absolute() and source.resolve(strict=True) == source and source.is_dir() and
         bootstrap_source.is_absolute() and bootstrap_source.resolve(strict=True) == bootstrap_source and
         bootstrap_source.is_dir() and source != bootstrap_source and
         manifest_current['path'] == str(source.parent / 'source-manifest.json') and
         bootstrap_manifest_current['path'] == str(bootstrap_source.parent / 'source-manifest.json') and
         bootstrap_manifest_current['sha256'] == BOOTSTRAP_SHA and
         Path(common.__file__).absolute() == bootstrap_source / 'tools/ros_distro/common.py',
         'independent actual current complete source and retained bootstrap source required')
    frozen = document(manifest_current, 16 * MIB)
    bootstrap_frozen = document(bootstrap_manifest_current, 16 * MIB)
    need(type(frozen) is dict and frozen and type(bootstrap_frozen) is dict and bootstrap_frozen,
         'both complete manifest dictionaries required')
    source_before = common.source_state(source, Path(manifest_current['path']))
    bootstrap_before = common.source_state(bootstrap_source, Path(bootstrap_manifest_current['path']))
    for relative in ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py'):
        pin = common.opened(bootstrap_source / relative, 2 * MIB)
        need(bootstrap_frozen.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')}, 'bootstrap source helper differs')
        current(pin, 2 * MIB)
    own_relative = 'tests/ros_unitree/acceptance/ownedtyped28_current/make_control.py'
    own = common.opened(Path(__file__).absolute(), 2 * MIB)
    need(frozen.get(own_relative) == {key: own[key] for key in ('bytes', 'sha256')},
         'loaded constructor must equal its actual selected complete-manifest source entry')
    current(own, 2 * MIB)

    typed = document(typed_build_current)
    need(typed.get('status') == 'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY' and
         typed.get('compiled_command_products') == 14 and typed.get('retained_command_products') == 9 and
         typed.get('fresh_command_products') == 5 and typed.get('actual_Runtime_cases_executed') == 0 and
         typed.get('source_manifest_sha256') == 'a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd' and
         all(typed.get(key) is True for key in ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved')) and
         not typed.get('error') and not typed.get('finalization_errors'), 'actual completed preserved original Typed14 required')
    workers = document(workers_current)
    need(workers_current['sha256'] == WORKER_SHA and
         workers.get('status') == 'PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED' and
         workers.get('source_manifest_sha256') == 'c8a2bde2c12fab767ca31eba68a08c10093dc5844ef76ea2dec8f87ae9738ee7',
         'unchanged actual original Worker37 content role required')
    retained = document(native_retained_current)
    need(retained.get('schema') == 'uwvm-native74-retained-products-at-rest-v1' and
         retained.get('status') == 'PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED' and
         retained.get('preparation_mode') == 'retained74-at-rest' and retained.get('execution_performed') is False and
         retained['retained_compilation']['translation_units'] == 74, 'actual distinct retained Native74 content role required')
    cp = document(cp310_receipt_current)
    need(cp.get('status') == 'PREPARED_CONTENT_ONLY_NOT_EXECUTED', 'CP3 is an actual retained content-only tool provenance role')
    cp_control = document(cp['control'])
    need(type(tools_current) is dict and set(tools_current) == {'unshare', 'ip'}, 'exact actual unshare/ip selections required')
    for name, pin in {**tools_current, 'python': python_current}.items():
        current(pin, 128 * MIB)
        need(pin == cp_control['host_tools'][name]['pin'] and os.access(pin['path'], os.X_OK),
             'current physical tool must match actual retained CP3 control provenance')
    need(Path(sys.executable).resolve(strict=True) == Path(python_current['path']), 'actual executing selected host Python required')

    need(type(helpers_current) is dict and set(helpers_current) == set(HELPER_SOURCE) and
         type(fresh_paths) is dict and set(fresh_paths) == PATH_KEYS, 'closed current helpers/fresh path plan required')
    paths = {key: Path(value) for key, value in fresh_paths.items()}
    need(all(type(fresh_paths[key]) is str and path.is_absolute() and '..' not in path.parts and
         path.parent.resolve(strict=True) == path.parent for key, path in paths.items()) and
         all(left != right and left not in right.parents and right not in left.parents for index, left in enumerate(paths.values())
             for right in list(paths.values())[index + 1:]) and len(os.fsencode(paths['transport_out'] / 's27')) < 104,
         'canonical disjoint current paths and bounded UNIX socket prefix required')
    code = paths['code_root']; control_path = code / 'root-control.json'
    need(code.resolve(strict=True) == code and code.is_dir() and not control_path.exists() and
         not control_path.is_symlink() and stat.S_IMODE(code.stat().st_mode) == 0o700 and
         all(not path.exists() and not path.is_symlink() for key, path in paths.items() if key != 'code_root') and
         all(root != path and root not in path.parents and path not in root.parents
             for root in (source, bootstrap_source) for path in paths.values()), 'fresh owned prestage/future outputs required')
    selected = {**helpers_current, 'cyclonedds_xml': cyclonedds_current}
    need(set(code.iterdir()) == {Path(pin['path']) for pin in selected.values()}, 'exact four-file prestage required')
    for role, pin in selected.items():
        current(pin, 2 * MIB)
        path = Path(pin['path']); info = path.stat(follow_symlinks=False)
        need(path.parent == code and stat.S_ISREG(info.st_mode) and stat.S_IMODE(info.st_mode) == 0o400 and info.st_nlink == 1,
             'ordinary readonly exact Root-prestaged source/XML required')
        if role in HELPER_SOURCE:
            need(frozen.get(HELPER_SOURCE[role]) == {key: pin[key] for key in ('bytes', 'sha256')},
                 'each prestaged helper must equal its actual complete-manifest entry')
    need({Path(pin['path']).name for pin in helpers_current.values()} == {'owner.py', 'derive_request.py', 'maker.py'} and
         Path(cyclonedds_current['path']).name == 'cyclonedds.xml', 'closed ordinary prestage basenames required')
    need(type(restart_mappings) is list and len(restart_mappings) <= 64, 'finite explicit historical/current root mappings required')
    seen = set()
    for mapping in restart_mappings:
        need(type(mapping) is dict and set(mapping) == {'root', 'historical_device', 'current_device'} and
             type(mapping['historical_device']) is int and type(mapping['current_device']) is int,
             'closed explicit device-only historical mapping required')
        root = Path(mapping['root'])
        need(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and
             root.stat().st_dev == mapping['current_device'] and mapping['historical_device'] != mapping['current_device'] and
             (str(root), mapping['historical_device']) not in seen, 'actual unique root/device mapping required')
        seen.add((str(root), mapping['historical_device']))
    for path in paths.values():
        for ancestor in (path.parent, *path.parent.parents):
            fs = os.statvfs(ancestor)
            need(fs.f_bavail * fs.f_frsize >= 8 * GIB, 'unchanged active eight-GiB backing reserve required')
    control = {'format_version': 1, 'kind': 'uwvm-unitree-typed28-owned-net-control-v1', 'cgroup': cgroup,
        'bootstrap_source': {'root': str(bootstrap_source), 'manifest': bootstrap_manifest_current},
        'source': {'root': str(source), 'manifest': manifest_current}, 'python': python_current,
        'helpers': helpers_current, 'tools': tools_current, 'inputs': {'typed_build': typed_build_current,
            'workers': workers_current, 'native_retained': native_retained_current,
            'cp310_receipt': cp310_receipt_current, 'cyclonedds_xml': cyclonedds_current},
        'restart_mappings': restart_mappings, 'paths': fresh_paths, 'limits': LIMITS}
    body = (json.dumps(control, indent=2, sort_keys=True) + '\n').encode()
    directory = os.open(code, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
    descriptor = None
    try:
        before_dir = os.fstat(directory)
        need(stat.S_IMODE(before_dir.st_mode) == 0o700, 'owned prestage directory must remain700 before publication')
        descriptor = os.open('root-control.json', os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_CLOEXEC | os.O_NOFOLLOW,
                             0o400, dir_fd=directory)
        pending = memoryview(body)
        while pending:
            guard(); count = os.write(descriptor, pending)
            need(count > 0, 'control write made no progress'); pending = pending[count:]
        os.fchmod(descriptor, 0o400); os.fsync(descriptor); os.close(descriptor); descriptor = None
        os.fsync(directory); os.fchmod(directory, 0o500); os.fsync(directory)
        after, named = os.fstat(directory), code.stat(follow_symlinks=False)
        need((before_dir.st_dev, before_dir.st_ino) == (after.st_dev, after.st_ino) == (named.st_dev, named.st_ino),
             'held prestage directory identity changed')
    finally:
        if descriptor is not None:
            os.close(descriptor)
        os.close(directory)
    control_pin = current(common.opened(control_path, MIB), MIB)
    need(control_pin['bytes'] == len(body) and control_pin['sha256'] == hashlib.sha256(body).hexdigest() and
         set(code.iterdir()) == {control_path, *(Path(pin['path']) for pin in selected.values())}, 'published exact static closure differs')
    for pin in list(pins.values()):
        current(pin, max(64 * MIB, pin['bytes']))
    need(common.source_state(source, Path(manifest_current['path'])) == source_before and
         common.source_state(bootstrap_source, Path(bootstrap_manifest_current['path'])) == bootstrap_before,
         'complete original bootstrap/current source changed during construction')
    common.cgroup_preserved(before, common.cgroup_state(cgroup)); guard()
    argv = [python_current['path'], '-B', helpers_current['owner']['path'], '--phase', 'outer',
        '--control', control_pin['path'], '--control-sha256', control_pin['sha256'],
        '--owner-sha256', helpers_current['owner']['sha256']]
    return {'status': 'CONSTRUCTED_ACTUAL_CURRENT_TYPED28_OWNER_CONTROL_ONLY_NO_ACTORS', 'control': control_pin,
        'unexecuted_owner_argv': argv, 'input_pins': list(pins.values()), 'current_source_state': source_before,
        'bootstrap_source_state': bootstrap_before, 'cgroup': cgroup,
        'current_cgroup_selection_origin': 'ROOT_EXPLICIT_CLOSED7_AND_LIVE_KERNEL_NOT_OLD_PRODUCER_CONTEXT',
        'original_typed14_configuration': typed['configuration'], 'retained_cp310_tool_provenance': cp310_receipt_current,
        'actors_started': 0, 'Runtime_ROS_DDS_executed': False, 'namespace_forecast': False,
        'Guest_rights_changed': False, 'code_directory_mode': 0o500}
