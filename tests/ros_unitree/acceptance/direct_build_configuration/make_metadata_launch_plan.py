#!/usr/bin/env python3
"""Prepare a concrete Direct metadata request/launch plan; start no child.

Root must SHA-admit this separately selected callable and invoke it only inside
its current64 Source35 supervisor. The ACTUAL completed14 CURRENT full-seven pin
is mandatory. Source43 remains frozen; this routine imports only the exact
already-reviewed request deriver after source/current-context admission. The
Direct/shared makers and six-Guest compiler entrypoint are never imported or run.
"""
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import sys
import time
import types

MIB, GIB = 1024**2, 1024**3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
B = Path('/var/tmp/uwvm-ros-unitree-20261016-r1')
M = Path('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1')
PRESTAGED = {'status': 'STAGED_DIRECT_METADATA_HELPER_SOURCE_ONLY_NOT_IMPORTED', 'pins': [{'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-direct-build-metadata-source-r1/direct_maker.py', 'bytes': 19529, 'sha256': '46b00baf346291599c79e8d19e69d2539c465ace6714f04c38367f42ea60e030', 'device': 66312, 'inode': 31247199, 'mtime_ns': 1792172206065942713, 'ctime_ns': 1792172206066807345}, {'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-direct-build-metadata-source-r1/shared_maker.py', 'bytes': 35034, 'sha256': '86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb', 'device': 66312, 'inode': 31247200, 'mtime_ns': 1792172206067933918, 'ctime_ns': 1792172206067933918}, {'path': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-direct-build-metadata-source-r1/derive_request_r2.py', 'bytes': 9896, 'sha256': '397b8650fdc9582e738fdbec04dacf456689393d4778fda84b85c4f5d3a722f9', 'device': 66312, 'inode': 31247201, 'mtime_ns': 1792172206068878167, 'ctime_ns': 1792172206068878167}], 'code_root': '/var/tmp/uwvm-ros-unitree-20261016-r1/source43-unitree-direct-build-metadata-source-r1', 'actors_started': 0, 'request_created': False}
MANIFEST35_CURRENT = {'bytes': 3397553, 'ctime_ns': 1792107847810773084, 'device': 66312, 'inode': 2413939, 'mtime_ns': 1792107847810052432, 'path': '/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r35/source-manifest.json', 'sha256': 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'}
MANIFEST43_CURRENT = {'bytes': 3436243, 'ctime_ns': 1792167000715955151, 'device': 66312, 'inode': 30726130, 'mtime_ns': 1792167000715955151, 'path': '/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r43/source-manifest.json', 'sha256': '9a01f114e4091d4329392b291b5785952e99b22d8e2a48be52b7e02a7d8f0178'}
FRESH_PATHS = {'request_directory': str(B / 'source43-unitree-direct-build-metadata-request-r1'),
    'controls': str(B / 'source43-unitree-direct-build-runtime-controls-r1'),
    'admission_out': str(M / 'source43-unitree-direct-build-input-admission-r1'),
    'build_out': str(M / 'source43-unitree-direct-six-guests-build-r1')}
MAKER_STATUS = 'PASS_DIRECT_SIX_GUEST_BUILD_CONFIGURATION_AND_REAL_PREREQUISITE_ONLY_COMPILER_NOT_STARTED'


def make_metadata_launch_plan(common, root_guard, *, typed_build_current):
    """Return actual request/plan pins and literal metadata argv, without Popen.

    Root supplies a Source35-MF-bound common/probe and retains its pidfd, memory,
    deadline and storage supervisor around this call. The local resource guard
    adds512MiB self RSS,1800seconds and the unchanged8GiB backing reserve. Only
    fresh prospective path spellings are constants; no future pin is supplied.
    """
    def need(value, reason):
        if not value:
            raise ValueError(reason)

    started = time.monotonic(); selected = {}; initial_context = None
    def guard():
        root_guard()
        need(time.monotonic() - started <= 1800 and resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 512 * MIB,
             'bounded1800-second/512MiB Direct metadata planning required')
        if initial_context is not None:
            common.cgroup_preserved(initial_context, common.cgroup_state(cg))
        for root in (B, M):
            for parent in (root, *root.parents):
                space = os.statvfs(parent)
                need(space.f_bavail * space.f_frsize >= 8 * GIB, 'unchanged8GiB active backing reserve required')

    def opened(path, maximum=2 * GIB):
        guard(); result = common.opened(Path(path), maximum); guard()
        prior = selected.setdefault(result['path'], result)
        need(prior == result, 'current planning file identity changed')
        return result

    def current(pin, maximum=64 * MIB):
        need(type(pin) is dict and set(pin) == PIN and type(pin['path']) is str and
             all(type(pin[key]) is int and pin[key] >= 0 for key in PIN - {'path', 'sha256'}) and
             0 < pin['bytes'] <= maximum and pin['inode'] > 0, 'closed bounded CURRENT full-seven pin required')
        actual = opened(pin['path'], maximum)
        need(actual == pin, 'supplied current full-seven observation changed')
        return actual

    def decode(body):
        def pairs(items):
            value = {}
            for key, item in items:
                need(key not in value, 'duplicate current planning JSON member')
                value[key] = item
            return value
        return json.loads(body, object_pairs_hook=pairs,
                          parse_constant=lambda value: need(False, 'nonfinite current planning JSON'))

    def document(path):
        # The reviewed deriver's logical API is unchanged: decode EXACT actual
        # producer bytes and return their actual pin. This accessor merely
        # supplies the maintained maker's64MiB report ceiling and live guard.
        # It does not project, normalize or rewrite any producer JSON field.
        guard(); body, pin = common.body_pin(Path(path), 64 * MIB); guard()
        need(opened(path, 64 * MIB) == pin, 'actual planning document changed')
        return decode(body), pin

    def current_document(pin):
        current(pin); value, actual = document(pin['path'])
        need(actual == pin, 'actual selected document differs')
        return value

    need(callable(root_guard) and sys.platform == 'linux' and os.geteuid() == 0,
         'Root must invoke from its already admitted actual current64 supervisor')
    guard()
    typed = current_document(typed_build_current)
    need(typed.get('status') == 'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY' and
         typed.get('compiled_command_products') == 14 and typed.get('retained_command_products') == 9 and
         typed.get('fresh_command_products') == 5 and typed.get('actual_Runtime_cases_executed') == 0 and
         typed.get('source_manifest_sha256') == 'a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd',
         'actual complete successful originalSource40 Typed14 report required')
    configuration = current_document(typed['configuration']); cg = configuration['cgroup']
    need(type(cg) is dict and set(cg) == {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode',
         'expected_boot_id', 'expected_leaf', 'expected_cpus'} and cg['memory_max'] == 64 * GIB and
         cg['memory_swap_max'] == 0 and cg['expected_cpus'] == '0-31' and
         cg['expected_leaf'] == '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
         'actual successful configuration must select the exact hard64GiB/zero-swap leaf')
    initial_context = common.cgroup_state(cg)
    need(all(row['events'].get(key, 0) == 0 for row in initial_context['ancestors']
             for key in ('oom', 'oom_kill', 'oom_group_kill')), 'all actual ancestor OOM counters must remain zero')
    sources = {'bootstrap': (M.parent / 'inputs-r35/source', MANIFEST35_CURRENT),
               'Direct_runtime': (M.parent / 'inputs-r43/source', MANIFEST43_CURRENT)}
    frozen = {}
    for role, (root, mf) in sources.items():
        need(root.resolve(strict=True) == root and Path(mf['path']) == root.parent / 'source-manifest.json',
             'actual complete source/manifest role required')
        frozen[role] = current_document(mf)
    need(str(Path(common.__file__).absolute()) == str(sources['bootstrap'][0] / 'tools/ros_distro/common.py'),
         'caller common/probe must belong to complete Source35 bootstrap role')
    for relative in ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py'):
        pin = opened(sources['bootstrap'][0] / relative, 2 * MIB)
        need(frozen['bootstrap'][relative] == {key: pin[key] for key in ('bytes', 'sha256')},
             'complete Source35 bootstrap helper/probe bytes differ')
    sources_before = {role: common.source_state(root, Path(mf['path'])) for role, (root, mf) in sources.items()}
    guard()
    code = Path(PRESTAGED['code_root'])
    need(code == B / 'source43-unitree-direct-build-metadata-source-r1' and code.resolve(strict=True) == code and
         code.is_dir() and set(code.iterdir()) == {Path(pin['path']) for pin in PRESTAGED['pins']},
         'exact Root three-file Direct metadata prestage required')
    roles = {}
    for pin in PRESTAGED['pins']:
        current(pin, MIB); info = Path(pin['path']).stat(follow_symlinks=False)
        need(Path(pin['path']).parent == code and stat.S_ISREG(info.st_mode) and stat.S_IMODE(info.st_mode) == 0o400,
             'Root-staged ordinary0400 metadata source required')
        roles[Path(pin['path']).name] = pin
    need(set(roles) == {'direct_maker.py', 'shared_maker.py', 'derive_request_r2.py'} and
         roles['direct_maker.py']['sha256'] == '46b00baf346291599c79e8d19e69d2539c465ace6714f04c38367f42ea60e030' and
         roles['shared_maker.py']['sha256'] == '86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb' and
         roles['derive_request_r2.py']['sha256'] == '397b8650fdc9582e738fdbec04dacf456689393d4778fda84b85c4f5d3a722f9',
         'all three exact independently reviewed Direct metadata source roles required')
    outputs = [Path(value) for value in FRESH_PATHS.values()]
    need(all(path.parent.resolve(strict=True) == path.parent and not path.exists() and not path.is_symlink() for path in outputs) and
         all(left != right and left not in right.parents and right not in left.parents for index, left in enumerate([code, *outputs])
             for right in [code, *outputs][index + 1:]) and
         all(root != path and root not in path.parents and path not in root.parents for root, _ in sources.values()
             for path in [code, *outputs]), 'fresh canonical disjoint metadata/admission/build path plan required')
    need(M.stat().st_dev == 81 and Path('/').stat().st_dev == 66312,
         'actual explicitly selected boot3 artifact/host device facts required')
    mappings = [{'root': str(M), 'historical_device': 68, 'current_device': 81},
                {'root': '/', 'historical_device': 66307, 'current_device': 66312},
                {'root': '/', 'historical_device': 66308, 'current_device': 66312}]
    derivative = roles['derive_request_r2.py']
    body, actual = common.body_pin(Path(derivative['path']), MIB)
    need(actual == derivative, 'exact staged request deriver changed before guarded loading')
    # Only this SHA-bound metadata deriver is loaded. Neither direct/shared
    # maker nor prepare_direct_guests.py is imported or executed by this plan.
    module = types.ModuleType('root_direct_metadata_request_deriver_r2'); module.__file__ = derivative['path']
    sys.modules[module.__name__] = module
    exec(compile(body, module.__file__, 'exec'), module.__dict__)
    guarded_accessor = types.SimpleNamespace(opened=opened, document=document)
    request, provenance = module.derive_direct_request(guarded_accessor,
        typed_build_current=typed_build_current, shared_helper_current=roles['shared_maker.py'],
        source=str(sources['Direct_runtime'][0]), manifest_current=MANIFEST43_CURRENT,
        cgroup=cg, fresh_paths={key: FRESH_PATHS[key] for key in ('controls', 'admission_out', 'build_out')},
        restart_mappings=mappings)
    need(request['tools']['python']['path'] == str(Path(sys.executable).resolve(strict=True)) and
         request['typed_build'] == typed_build_current and request['source']['manifest'] == MANIFEST43_CURRENT and
         request['paths'] == {key: FRESH_PATHS[key] for key in ('controls', 'admission_out', 'build_out')},
         'actual derived Direct request/current Python/path association differs')
    for pin in request['tools'].values():
        current(pin, 2 * GIB)
    request_dir = Path(FRESH_PATHS['request_directory']); request_dir.mkdir(mode=0o700)
    def write(name, value):
        target = request_dir / name
        data = (json.dumps(value, sort_keys=True, indent=2) + '\n').encode()
        need(0 < len(data) <= 4 * MIB, 'finite ordinary metadata output required')
        fd = os.open(target, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o400)
        try:
            remaining = memoryview(data)
            while remaining:
                count = os.write(fd, remaining); need(count > 0, 'metadata write made no progress'); remaining = remaining[count:]
            os.fchmod(fd, 0o400); os.fsync(fd)
        finally:
            os.close(fd)
        pin = common.opened(target, 4 * MIB)
        need(pin['bytes'] == len(data) and pin['sha256'] == hashlib.sha256(data).hexdigest(), 'fresh metadata bytes differ')
        return pin
    request_pin = write('request.json', request)
    provenance_pin = write('derivation.json', provenance)
    argv = [request['tools']['python']['path'], '-B', roles['direct_maker.py']['path'],
            '--request', request_pin['path'], '--request-sha256', request_pin['sha256'],
            '--maker-sha256', roles['direct_maker.py']['sha256']]
    plan = {'format_version': 1, 'kind': 'direct-build-metadata-launch-plan-only',
        'status': 'PREPARED_CURRENT_DIRECT_METADATA_REQUEST_NO_MAKER_OR_COMPILER_EXECUTED',
        'request': request_pin, 'derivation': provenance_pin, 'unexecuted_metadata_argv': argv,
        'metadata_required_status': MAKER_STATUS,
        'root_supervisor_limits': {'rss_bytes': 2 * GIB, 'command_seconds': 10800, 'log_bytes': 16 * MIB},
        'current_cgroup': cg, 'source_roles': {role: {'root': str(value[0]), 'manifest': value[1]} for role, value in sources.items()},
        'source_input_pins': list(selected.values()), 'future_build_invocation': 'READ_ONLY_FROM_ACTUAL_SUCCESSFUL_METADATA_REPORT',
        'namespace_required': False, 'native_product_receipt_required': False, 'Worker_receipt_required': False,
        'actors_started': 0, 'compiler_executed': False, 'Runtime_ROS_DDS_simulator_executed': False,
        'Guest_rights_changed': False}
    plan_pin = write('launch-plan.json', plan)
    directory = os.open(request_dir, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        os.fsync(directory); os.fchmod(directory, 0o500); os.fsync(directory)
    finally:
        os.close(directory)
    for pin in [*list(selected.values()), request_pin, provenance_pin, plan_pin]:
        guard(); need(common.opened(Path(pin['path']), max(16 * MIB, pin['bytes'])) == pin, 'terminal current static/planning pin changed')
    for role, (root, mf) in sources.items():
        guard(); need(common.source_state(root, Path(mf['path'])) == sources_before[role], 'complete source role changed during planning')
    need(set(code.iterdir()) == {Path(pin['path']) for pin in PRESTAGED['pins']} and
         set(request_dir.iterdir()) == {request_dir / name for name in ('request.json', 'derivation.json', 'launch-plan.json')} and
         all(not Path(FRESH_PATHS[key]).exists() and not Path(FRESH_PATHS[key]).is_symlink()
             for key in ('controls', 'admission_out', 'build_out')), 'metadata planning must leave sealed code and future outputs unchanged')
    guard()
    return {'status': plan['status'], 'request': request_pin, 'provenance': provenance_pin, 'plan': plan_pin,
            'unexecuted_metadata_argv': argv, 'actors_started': 0, 'maker_imported': False, 'compiler_executed': False}
