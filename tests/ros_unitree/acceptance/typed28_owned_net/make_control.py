#!/usr/bin/env python3
"""Root-callable metadata constructor; source author never invokes it.

Only an ACTUAL successful completed14 CURRENT full-seven argument permits the
control write. All other selections below are exact actual Root boot3 receipts,
not projected future identities. The callable starts no child, creates no netns
and executes no external helper. Root loads it only by its reviewed source SHA
inside the existing exact Source35/current64 supervisor and retains its guard.
"""
import hashlib
import json
import os
from pathlib import Path
import stat
import sys

MIB, GIB = 1024**2, 1024**3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
B = Path('/var/tmp/uwvm-ros-unitree-20261016-r1')
M = Path('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1')
PRESTAGED = {'status': 'STAGED_SOURCE_AND_LOOPBACK_XML_ONLY_NO_CONTROL_OR_ACTORS', 'code_root': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-typed28-owned-net-controls-r1', 'pins': [{'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-typed28-owned-net-controls-r1/owner.py', 'bytes': 39191, 'sha256': 'b0f935556f340c1cd0a8297ddb7cb2f3fa8f38d68d39d42e7af80d7727b17d1a', 'device': 66312, 'inode': 31247194, 'mtime_ns': 1792172031353793983, 'ctime_ns': 1792172031355504675}, {'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-typed28-owned-net-controls-r1/derive_request.py', 'bytes': 18902, 'sha256': 'cecc4f80ef68d3c9e7c6f48325b0d427e59e3f68d035a1dfc9d59ef0d70f28b9', 'device': 66312, 'inode': 31247195, 'mtime_ns': 1792172031357039921, 'ctime_ns': 1792172031357504765}, {'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-typed28-owned-net-controls-r1/maker.py', 'bytes': 35034, 'sha256': '86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb', 'device': 66312, 'inode': 31247196, 'mtime_ns': 1792172031359101172, 'ctime_ns': 1792172031359504856}, {'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-typed28-owned-net-controls-r1/cyclonedds.xml', 'bytes': 358, 'sha256': 'aeb1a746c302de11fa5233221b7aed9dd1f550888b3327b3de37c2f9dc5016b7', 'device': 66312, 'inode': 31247197, 'mtime_ns': 1792172031360917448, 'ctime_ns': 1792172031360917448}], 'control_created': False, 'actors_started': 0}
WORKERS_CURRENT = {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source37-native-four-runtime-build-r2/workers/report.json', 'bytes': 10438490, 'sha256': '18932cb8abc7d9998eb7431f751d6d1feea687cb380f1b247eef12750de0fb64', 'device': 81, 'inode': 578812, 'mtime_ns': 1792113898049082283, 'ctime_ns': 1792113898059082436}
NATIVE_CURRENT = {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source43-native74-at-rest-r2/report.json', 'bytes': 1169663, 'sha256': '8503a1b4c02c8b385648b2bd12a338202a8d31d4dfaf0ee8608b12c2b3cfbda7', 'device': 81, 'inode': 773800, 'mtime_ns': 1792168044278737992, 'ctime_ns': 1792168044282738065}
CP3_CURRENT = {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source43-unitree-go2-cp310-physics-view-preparation-r3/report.json', 'bytes': 17685, 'sha256': 'bcbc8323f0744d58a777d050735b268ae6e1dd1a07a7ac8d18f39a1825bf4644', 'device': 81, 'inode': 823318, 'mtime_ns': 1792171006883159370, 'ctime_ns': 1792171006888177672}
CACHE_CURRENT = {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/noetic-amd64-native-sdk-cacheview-r6d/report.json', 'bytes': 39015, 'sha256': '9985cef5f0398c4f48b05142c9b6850e2b6c14b286c4f9de64b759af8ba6de9a', 'device': 81, 'inode': 784448, 'mtime_ns': 1792170363876389353, 'ctime_ns': 1792170363876389353}

LIMITS = {'self_rss_bytes': 1536 * MIB, 'tree_rss_bytes': 4 * GIB,
          'active_seconds': 43200, 'whole_seconds': 43800, 'cleanup_seconds': 600,
          'metadata_seconds': 10800, 'transport_seconds': 31200,
          'log_bytes': 16 * MIB, 'output_bytes': 8 * GIB, 'reserve_bytes': 8 * GIB}
NEW_PATHS = {'code_root': str(B / 'source43-unitree-typed28-owned-net-controls-r1'),
    'owner_out': str(M / 'source43-unitree-typed28-owned-net-root-supervisor-r1'),
    'namespace_out': str(M / 'source43-unitree-typed28-owned-net-namespace-r1'),
    'request_directory': str(B / 'source43-unitree-typed28-owned-net-request-r1'),
    'metadata_controls': str(B / 'source43-unitree-typed28-runtime-controls-r1'),
    'admission_out': str(M / 'source43-unitree-typed28-owned-net-input-admission-r1'),
    'transport_out': str(M / 'source43-unitree-typed28-transport-r1')}


def make_control(common, guard, *, typed_build_current):
    """Write one new immutable control and return actual pin/unexecuted argv.

    Root must already SHA-bind this module before calling. ``common`` is the
    actual retained complete Source35's independently bound common/probe; it
    supplies stable ordinary hashing, not a path-to-PATH execution fallback.
    This function never imports owner/deriver/maker, forecasts a namespace,
    derives current cgroup facts from an old actor, or starts the emitted argv.
    """
    def need(value, reason):
        if not value:
            raise ValueError(reason)

    pins = {}
    def current(pin):
        guard()
        need(type(pin) is dict and set(pin) == PIN and type(pin['path']) is str and
             all(type(pin[key]) is int and pin[key] >= 0 for key in PIN - {'path', 'sha256'}) and
             0 < pin['bytes'] <= 64 * MIB and pin['inode'] > 0,
             'closed finite actual current observation required')
        observed = common.opened(Path(pin['path']), max(16 * MIB, pin['bytes']))
        need(observed == pin and (pin['path'] not in pins or pins[pin['path']] == observed), 'actual current static pin changed')
        pins[pin['path']] = observed
        return observed

    def document(pin):
        current(pin)
        body, actual = common.body_pin(Path(pin['path']), 64 * MIB)
        need(actual == pin, 'actual selected JSON changed while decoded')
        def pairs(items):
            value = {}
            for key, item in items:
                need(key not in value, 'duplicate current JSON member')
                value[key] = item
            return value
        return json.loads(body, object_pairs_hook=pairs,
                          parse_constant=lambda value: need(False, 'nonfinite current JSON value'))

    need(callable(guard) and sys.platform == 'linux' and os.geteuid() == 0,
         'Root must call from its actual current64 Linux metadata supervisor')
    guard()
    typed = document(typed_build_current)
    need(typed.get('status') == 'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY' and
         typed.get('compiled_command_products') == 14 and typed.get('retained_command_products') == 9 and
         typed.get('fresh_command_products') == 5 and typed.get('actual_Runtime_cases_executed') == 0 and
         typed.get('source_manifest_sha256') == 'a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd',
         'only actual complete successful originalSource40 Typed14 permits control construction')
    configuration = document(typed['configuration'])
    # The actual completed producer configuration is the ONLY current context
    # selection. Cached CP/Noetic actor cgroups are never current authority.
    cg = configuration['cgroup']
    need(type(cg) is dict and set(cg) == {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode',
         'expected_boot_id', 'expected_leaf', 'expected_cpus'} and cg['memory_max'] == 64 * GIB and
         cg['memory_swap_max'] == 0 and cg['expected_cpus'] == '0-31' and
         cg['expected_leaf'] == '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
         'closed actual successful producer current64/zero-swap context required')
    before = common.cgroup_state(cg)
    need(all(row['events'].get(key, 0) == 0 for row in before['ancestors'] for key in ('oom', 'oom_kill', 'oom_group_kill')),
         'all actual ancestor OOM counters must remain zero')
    cached = document(CACHE_CURRENT)
    need(cached['status'] == 'PREPARED_COW_DEB_CANDIDATES_ONLY_NOT_NEW_SIGNATURE_AUTHORITY',
         'exact observed metadata cache view required solely for its Source35 manifest selection')
    cache_control = document(cached['control'])
    manifest35 = current(cache_control['manifest_pin'])
    need(manifest35['sha256'] == 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396' and
         cache_control['source'] == str(M.parent / 'inputs-r35/source') and
         str(Path(common.__file__).absolute()) == str(Path(cache_control['source']) / 'tools/ros_distro/common.py'),
         'actual complete Source35 bootstrap/helper role required')
    mf35 = document(manifest35)
    for relative in ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py'):
        pin = common.opened(Path(cache_control['source']) / relative, 2 * MIB)
        need(mf35[relative] == {key: pin[key] for key in ('bytes', 'sha256')}, 'complete Source35 helper/probe bytes differ')
        current(pin)
    cp = document(CP3_CURRENT)
    need(cp['status'] == 'PREPARED_CONTENT_ONLY_NOT_EXECUTED' and cp['source_manifest_sha256'] ==
         '9a01f114e4091d4329392b291b5785952e99b22d8e2a48be52b7e02a7d8f0178',
         'actual content-only CP3 receipt required solely for current manifest/tool selections')
    cp_control = document(cp['control'])
    manifest43 = current(cp_control['manifest_pin'])
    need(manifest43['sha256'] == '9a01f114e4091d4329392b291b5785952e99b22d8e2a48be52b7e02a7d8f0178' and
         cp_control['source'] == str(M.parent / 'inputs-r43/source'), 'actual complete Source43 manifest selection required')
    native = document(NATIVE_CURRENT)
    need(native['status'] == 'PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED' and
         native['preparation_mode'] == 'retained74-at-rest' and native['execution_performed'] is False and
         native['retained_compilation']['translation_units'] == 74,
         'actual distinct current Native74 retained receipt required')
    workers = document(WORKERS_CURRENT)
    need(workers['status'] == 'PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED' and
         workers['source_manifest_sha256'] == 'c8a2bde2c12fab767ca31eba68a08c10093dc5844ef76ea2dec8f87ae9738ee7',
         'actual unchanged four-profile Worker37 report required')
    tools = {name: current(cp_control['host_tools'][name]['pin']) for name in ('unshare', 'ip')}
    python = current(cp_control['host_tools']['python']['pin'])
    need(str(Path(sys.executable).resolve(strict=True)) == python['path'] and
         all(os.access(pin['path'], os.X_OK) for pin in [python, *tools.values()]), 'actual current host executable tools/Python required')
    code = Path(NEW_PATHS['code_root']); control_path = code / 'root-control.json'
    need(code.resolve(strict=True) == code and code.is_dir() and str(code) == PRESTAGED['code_root'] and
         set(code.iterdir()) == {Path(pin['path']) for pin in PRESTAGED['pins']} and not control_path.exists() and
         not control_path.is_symlink(), 'actual four-file Root prestage and absent control required')
    selected = {}
    for previous in PRESTAGED['pins']:
        pin = current(previous); info = Path(pin['path']).stat(follow_symlinks=False)
        need(Path(pin['path']).parent == code and stat.S_ISREG(info.st_mode) and stat.S_IMODE(info.st_mode) == 0o400,
             'exact already sealed Root-prestaged source/XML files required')
        selected[Path(pin['path']).name] = pin
    need(set(selected) == {'owner.py', 'derive_request.py', 'maker.py', 'cyclonedds.xml'} and
         selected['owner.py']['sha256'] == 'b0f935556f340c1cd0a8297ddb7cb2f3fa8f38d68d39d42e7af80d7727b17d1a' and
         selected['derive_request.py']['sha256'] == 'cecc4f80ef68d3c9e7c6f48325b0d427e59e3f68d035a1dfc9d59ef0d70f28b9' and
         selected['maker.py']['sha256'] == '86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb' and
         selected['cyclonedds.xml']['sha256'] == 'aeb1a746c302de11fa5233221b7aed9dd1f550888b3327b3de37c2f9dc5016b7',
         'exact independently reviewed prestaged owner/deriver/maker and real Root loopback XML required')
    future = [Path(value) for key, value in NEW_PATHS.items() if key != 'code_root']
    need(all(path.parent.resolve(strict=True) == path.parent and not path.exists() and not path.is_symlink() for path in future) and
         all(left != right and left not in right.parents and right not in left.parents for index, left in enumerate([code, *future])
             for right in [code, *future][index + 1:]) and len(os.fsencode(Path(NEW_PATHS['transport_out']) / 's27')) == 102,
         'closed fresh disjoint Root path plan and exact102-byte UNIX socket maximum required')
    for root in (B, M):
        for parent in (root, *root.parents):
            fs = os.statvfs(parent)
            need(fs.f_bavail * fs.f_frsize >= 8 * GIB, 'unchanged eight-GiB active backing reserve required')
    need(M.stat().st_dev == 81 and Path('/').stat().st_dev == 66312,
         'actual explicitly selected boot3 artifact and host device facts required')
    mappings = [{'root': str(M), 'historical_device': 68, 'current_device': 81},
                {'root': '/', 'historical_device': 66307, 'current_device': 66312},
                {'root': '/', 'historical_device': 66308, 'current_device': 66312}]
    control = {'format_version': 1, 'kind': 'uwvm-unitree-typed28-owned-net-control-v1', 'cgroup': cg,
        'bootstrap_source': {'root': cache_control['source'], 'manifest': manifest35},
        'source': {'root': cp_control['source'], 'manifest': manifest43}, 'python': python,
        'helpers': {'owner': selected['owner.py'], 'deriver': selected['derive_request.py'], 'maker': selected['maker.py']},
        'tools': tools, 'inputs': {'typed_build': typed_build_current, 'workers': WORKERS_CURRENT,
            'native_retained': NATIVE_CURRENT, 'cyclonedds_xml': selected['cyclonedds.xml']},
        'restart_mappings': mappings, 'paths': NEW_PATHS, 'limits': LIMITS}
    # This is the sole mutation: publish one fresh ordinary control, fsync it
    # and its already-owned directory, then remove directory write permission.
    # No helper candidate is imported, no child starts, and no namespace exists
    # merely because this metadata file is now available.
    body = (json.dumps(control, sort_keys=True, indent=2) + '\n').encode()
    directory = os.open(code, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
    descriptor = None
    try:
        first = os.fstat(directory)
        need(stat.S_IMODE(first.st_mode) == 0o700, 'Root prestage directory must still have original700 mode')
        descriptor = os.open(control_path.name, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC,
                             0o400, dir_fd=directory)
        view = memoryview(body)
        while view:
            size = os.write(descriptor, view); need(size > 0, 'control write made no progress'); view = view[size:]
        os.fchmod(descriptor, 0o400); os.fsync(descriptor); os.close(descriptor); descriptor = None
        os.fsync(directory); os.fchmod(directory, 0o500); os.fsync(directory)
        after, named = os.fstat(directory), code.stat(follow_symlinks=False)
        need((first.st_dev, first.st_ino) == (after.st_dev, after.st_ino) == (named.st_dev, named.st_ino),
             'held Root code directory identity changed')
    finally:
        if descriptor is not None:
            os.close(descriptor)
        os.close(directory)
    control_pin = common.opened(control_path, MIB)
    need(control_pin['bytes'] == len(body) and control_pin['sha256'] == hashlib.sha256(body).hexdigest() and
         set(code.iterdir()) == {control_path, *(Path(pin['path']) for pin in PRESTAGED['pins'])}, 'exact sealed five-file Root control closure differs')
    current(control_pin)
    for pin in list(pins.values()):
        current(pin)
    common.cgroup_preserved(before, common.cgroup_state(cg)); guard()
    argv = [python['path'], '-B', selected['owner.py']['path'], '--phase', 'outer', '--control', control_pin['path'],
            '--control-sha256', control_pin['sha256'], '--owner-sha256', selected['owner.py']['sha256']]
    return {'status': 'CONSTRUCTED_ACTUAL_CURRENT_TYPED28_OWNER_CONTROL_ONLY_NO_ACTORS',
        'control': control_pin, 'unexecuted_owner_argv': argv, 'source_input_pins': list(pins.values()),
        'cgroup_selected_only_from_actual_successful14_configuration': typed['configuration'],
        'current_namespace_forecast': False, 'actors_started': 0, 'Runtime_ROS_DDS_executed': False,
        'Guest_rights_changed': False, 'code_directory_mode': 0o500}
