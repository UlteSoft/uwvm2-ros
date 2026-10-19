#!/usr/bin/env python3
"""Stage explicit current CP310 prepare/run controls without starting actors.

Root freezes the complete Source42 first and supplies a SHA-bound request and
source package. ``prepare`` writes fresh source/control files only. After the
actual owner prepares its independent COPY, ``run`` consumes that real receipt,
an explicit baseline Unitree configuration and an unexecuted physics template.
The run owner supplies the real newly created network namespace later; this
stager never predicts namespace IDs, future file pins or compilation results.
"""
from __future__ import annotations
import argparse
import base64
import copy
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types

MIB, GIB = 1024 ** 2, 1024 ** 3
CG = {'memory_max': 64 * GIB, 'memory_swap_max': 0, 'expected_device': 29,
      'expected_inode': 18315, 'expected_boot_id': 'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
      'expected_leaf': '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
      'expected_cpus': '0-31'}
NAMES = {'owner': 'owner.py', 'copy': 'copy_view.py', 'restart': 'requalify_after_restart.py'}
PATHS = {'code', 'prepare_out', 'view_root', 'run_owner_out', 'namespace_out', 'run_parent', 'run_out', 'runtime_controls'}
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}


def need(value, message):
    if not value: raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, 'duplicate CP310 staging JSON key'); result[key] = value
    return result


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite staging JSON'))


def read(path, maximum):
    path = Path(path); need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical regular stage input required')
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, 'bounded ordinary staging input required')
        data = bytearray()
        while True:
            block = os.read(descriptor, min(MIB, maximum + 1 - len(data)))
            if not block: break
            data.extend(block); need(len(data) <= maximum, 'staging input grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(data) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'staging input changed')
        return bytes(data), {'path': str(path), 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
            'device': before.st_dev, 'inode': before.st_ino, 'mtime_ns': before.st_mtime_ns, 'ctime_ns': before.st_ctime_ns}
    finally: os.close(descriptor)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--phase', choices=('prepare', 'run'), required=True)
    for name in ('request', 'payload'):
        parser.add_argument('--' + name, type=Path, required=True); parser.add_argument('--' + name + '-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True); args = parser.parse_args()
    need(sys.platform == 'linux' and os.geteuid() == 0, 'actual Root Linux metadata staging required')
    leaf = Path(CG['expected_leaf']); info = leaf.stat()
    need((info.st_dev, info.st_ino) == (29, 18315) and Path('/proc/sys/kernel/random/boot_id').read_text().strip() == CG['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::/' + str(leaf.relative_to('/sys/fs/cgroup'))] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and (leaf / 'memory.swap.max').read_text().strip() == '0',
         'exact hard64GiB/swap0 membership before source load required')
    raw, request_pin = read(args.request, 16 * MIB); request = parse(raw)
    need(request_pin['sha256'] == args.request_sha256 and set(request) == {'format_version', 'kind', 'source', 'manifest',
        'manifest_sha256', 'cgroup', 'paths', 'host_tools', 'cached_view', 'run_inputs'} and
        request['format_version'] == 1 and request['kind'] == 'unitree-cp310-physics-staging-request-v1' and
        request['cgroup'] == CG and set(request['paths']) == PATHS and
        set(request['host_tools']) == {'python', 'mount', 'umount', 'chroot', 'unshare', 'ip'}, 'closed explicit Root staging request required')
    raw, payload_pin = read(args.payload, 8 * MIB); payload = parse(raw)
    need(payload_pin['sha256'] == args.payload_sha256 and set(payload) == {'format_version', 'kind', 'files'} and
         payload['format_version'] == 1 and payload['kind'] == 'unitree-cp310-physics-reviewed-source-package' and
         set(payload['files']) == set(NAMES), 'three explicit reviewed source roles required')
    bodies = {}
    for role, name in NAMES.items():
        row = payload['files'][role]
        need(set(row) == {'name', 'bytes', 'sha256', 'base64'} and row['name'] == name and type(row['bytes']) is int and
             0 < row['bytes'] <= MIB, 'closed bounded reviewed Python source required')
        body = base64.b64decode(row['base64'], validate=True)
        need(base64.b64encode(body).decode() == row['base64'] and len(body) == row['bytes'] and
             hashlib.sha256(body).hexdigest() == row['sha256'] and body.startswith(b'#!/usr/bin/env python3\n'), 'canonical exact Python source bytes required')
        bodies[role] = body
    source, manifest = Path(request['source']), Path(request['manifest'])
    need(source.is_absolute() and source.resolve(strict=True) == source and source.name == 'source' and source.parent.name == 'inputs-r42' and
         manifest == source.parent / 'source-manifest.json', 'actual complete Root-frozen Source42 selection required')
    raw, mf_pin = read(manifest, 16 * MIB)
    need(mf_pin['sha256'] == request['manifest_sha256'], 'explicit complete frozen source manifest SHA differs')
    frozen = parse(raw)
    need(payload['files']['restart']['bytes'] == 47606 and payload['files']['restart']['sha256'] ==
         '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260', 'exact independently reviewed restart module required')
    restart = types.ModuleType('cp310_stage_restart'); restart.__file__ = 'reviewed_restart_source'; sys.modules[restart.__name__] = restart
    exec(compile(bodies['restart'], restart.__file__, 'exec'), restart.__dict__)
    guard = restart.CurrentGuard(str(source), frozen, CG); guard.maximum_seconds = 1800; before = guard.force()
    paths = {key: Path(value) for key, value in request['paths'].items()}
    need(all(path.is_absolute() and '..' not in path.parts for path in paths.values()) and len(set(paths.values())) == len(paths) and
         paths['run_out'].parent == paths['run_parent'] and all(source != path and source not in path.parents and path not in source.parents
         for path in paths.values()), 'explicit distinct output/control/view paths disjoint from frozen source required')
    out = args.out
    need(out.is_absolute() and out.parent.resolve(strict=True) == out.parent and not out.exists() and not out.is_symlink() and
         all(out != path and out not in path.parents and path not in out.parents for path in paths.values()), 'fresh external staging report directory required')
    for path in (out.parent, *out.parents):
        fs = os.statvfs(path); need(fs.f_bavail * fs.f_frsize >= 8 * GIB, 'eight-GiB backing reserve before metadata staging required')
    out.mkdir(mode=0o700); inputs = [request_pin, payload_pin, mf_pin]; products = []; bridges = []
    def observe(expected, maximum=64 * MIB):
        need(type(expected) is dict and set(expected) == PIN, 'actual selected current full7 required')
        current = restart.opened_file(expected['path'], max(maximum, expected['bytes']), guard)
        need(current == expected, 'explicit current metadata observation changed'); inputs.append(current); return current
    def document(expected, maximum=16 * MIB):
        need(expected['bytes'] <= maximum <= 16 * MIB, 'bounded collected staging metadata required')
        observe(expected, maximum); actual, body = restart.opened_file(expected['path'], maximum, guard, collect=True)
        need(actual == expected, 'selected document changed while parsing'); return parse(body)
    def write(directory, name, body):
        guard(); path = directory / name
        descriptor = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o400)
        try:
            with os.fdopen(os.dup(descriptor), 'wb') as stream: stream.write(body); stream.flush(); os.fsync(stream.fileno())
        finally: os.close(descriptor)
        current = restart.opened_file(str(path), 16 * MIB, guard); products.append(current); return current
    def json_file(directory, name, value):
        return write(directory, name, (json.dumps(value, sort_keys=True, indent=2) + '\n').encode())
    tools = {}
    for name, row in request['host_tools'].items():
        need(type(row) is dict and set(row) in ({'invocation', 'current_pin'}, {'invocation', 'historical_pin'}), 'explicit current or historical tool selection required')
        if 'current_pin' in row: current = observe(row['current_pin'])
        else:
            previous = row['historical_pin']
            need(previous['device'] == 66307, 'only persistent original host-tool device renumbering may be bridged')
            relation = restart.requalify_persistent_pin(previous, previous['path'], guard,
                historical_device=66307, current_device=66308)
            current = relation['current_pin']; bridges.append(relation); observe(current)
        need(Path(row['invocation']).is_absolute() and Path(row['invocation']).resolve(strict=True) == Path(current['path']) and
             os.access(current['path'], os.X_OK), 'selected executable invocation must name actual current physical bytes')
        tools[name] = {'invocation': row['invocation'], 'pin': current}
    need(Path(sys.executable).resolve(strict=True) == Path(tools['python']['pin']['path']), 'stage must use the explicitly selected host interpreter')
    cache = request['cached_view']
    need(set(cache) == {'root', 'report', 'catalog'} and cache['root'] ==
         '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/unitree-native-model-preflights-r1/rootfs', 'exact historical preflight cached root required')
    old = document(cache['report'])
    need(cache['report']['bytes'] == 7051404 and cache['report']['sha256'] == '8c281ba2eec9ea5f7e83ae4a6eb8e1721f9f184c66b227e88894eb9b73905697' and
         old['status'] == 'COMPLETED_EXPLICIT_NATIVE_PREFLIGHTS_ONLY' and old['execution_view_after']['catalog'] == cache['catalog'] and
         old['execution_view_after']['root'] == cache['root'] and old['execution_view_after']['bytes'] == 1266016500,
         'genuine preserved old preflight is historical content provenance only')
    relation = restart.requalify_persistent_pin(cache['catalog'], cache['catalog']['path'], guard,
        historical_device=81, current_device=68); bridges.append(relation); observe(relation['current_pin'])
    if args.phase == 'prepare':
        need(request['run_inputs'] is None and all(not path.exists() and not path.is_symlink() for path in
             (paths['code'], paths['prepare_out'], paths['view_root'])), 'fresh initial source/preparation/view only; no run result forecast')
        paths['code'].mkdir(mode=0o700)
        helpers = {role: write(paths['code'], name, bodies[role]) for role, name in NAMES.items()}
        run_pin = None
    else:
        helpers = {}
        for role, name in NAMES.items():
            actual = restart.opened_file(str(paths['code'] / name), MIB, guard)
            need((actual['bytes'], actual['sha256']) == (payload['files'][role]['bytes'], payload['files'][role]['sha256']), 'prepare source role changed')
            observe(actual); helpers[role] = actual
        run = request['run_inputs']
        need(set(run) == {'view_receipt', 'baseline_config', 'physics_template', 'readonly_bind_roots'}, 'four explicit completed run inputs required')
        prepared = document(run['view_receipt']); baseline = document(run['baseline_config']); physics = document(run['physics_template'])
        need(prepared.get('kind') == 'unitree-cp310-controlled-physics-view' and prepared.get('status') == 'PREPARED_CONTENT_ONLY_NOT_EXECUTED' and
             prepared.get('root') == str(paths['view_root']) and all(prepared.get(key) is True for key in
             ('source_preserved', 'inputs_preserved', 'products_preserved')) and not prepared.get('error') and not prepared.get('errors'),
             'actual preserved content-only view receipt required before run metadata')
        observe(prepared['python'])
        need(physics.get('kind') == 'unitree-go2-three-direct-physics-v1' and physics.get('expected_network_namespace') is None and
             set(physics['cp310']) == {'root', 'python', 'chroot', 'catalog_roots', 'environment', 'view_receipt'} and
             baseline['cgroup'] == CG and baseline['simulation']['controllers'] is baseline['simulation']['direct_runtime'] is None and
             'physics_selection' not in baseline['inputs'], 'unexecuted closed baseline and unforecast-network template required')
        cfg = copy.deepcopy(baseline); selected = copy.deepcopy(physics)
        additions = {'cp310_python': prepared['python'], 'cp310_chroot': tools['chroot']['pin'], 'cp310_view_receipt': run['view_receipt']}
        for label, pin in additions.items():
            thin = {key: pin[key] for key in ('path', 'bytes', 'sha256')}
            need(label not in cfg['inputs'] or cfg['inputs'][label] == thin, 'existing actual CP310 label conflicts with new content receipt')
            cfg['inputs'][label] = thin
        original_python_label = cfg['simulation']['python_input']; cfg['simulation']['python_input'] = 'cp310_python'
        cfg['input_roots'] = list(dict.fromkeys([*cfg['input_roots'], *prepared['catalog_roots'], str(Path(run['view_receipt']['path']).parent)]))
        need(len(cfg['input_roots']) <= 63 and str(paths['view_root']) not in cfg['input_roots'], 'leave one immutable runtime-control root; never inventory whole mounted root')
        cp = selected['cp310']; cp.update(root=str(paths['view_root']), python='cp310_python', chroot='cp310_chroot', view_receipt='cp310_view_receipt')
        cp['catalog_roots'] = list(dict.fromkeys([*prepared['catalog_roots'], *cp['catalog_roots']]))
        baseline_pin = json_file(out, 'baseline-with-current-view.json', cfg)
        physics_pin = json_file(out, 'physics-with-current-view-unexecuted.json', selected)
        run_pin = json_file(out, 'run-metadata-selection.json', {'format_version': 1, 'kind': 'unitree-cp310-physics-run-metadata-v1',
            'view_receipt': run['view_receipt'], 'baseline_config': baseline_pin, 'physics_template': physics_pin,
            'readonly_bind_roots': run['readonly_bind_roots']})
    control = {'format_version': 1, 'kind': 'unitree-cp310-physics-root-owner-v1', 'source': str(source), 'manifest': str(manifest),
        'manifest_sha256': request['manifest_sha256'], 'manifest_pin': mf_pin, 'cgroup': CG,
        'paths': request['paths'], 'helpers': helpers, 'host_tools': tools, 'cached_view': cache, 'run_selection': run_pin}
    control_pin = json_file(out, 'root-control.json', control)
    for directory in (out, paths['code']):
        descriptor = os.open(directory, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
        try: os.fsync(descriptor)
        finally: os.close(descriptor)
        descriptor = os.open(directory.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
        try: os.fsync(descriptor)
        finally: os.close(descriptor)
    for expected in inputs: need(restart.opened_file(expected['path'], max(64 * MIB, expected['bytes']), guard) == expected, 'stage input changed')
    for expected in products: need(restart.opened_file(expected['path'], 16 * MIB, guard) == expected, 'stage product changed')
    receipt = {'format_version': 1, 'kind': 'unitree-cp310-physics-source-control-staging', 'phase': args.phase,
        'status': 'STAGED_SOURCE_AND_CURRENT_METADATA_ONLY_NOT_EXECUTED', 'input_pins': inputs, 'product_pins': products,
        'historical_current_tool_and_catalog_bridges': bridges, 'actors_started': 0, 'commands': [],
        'cgroup_before': before, 'cgroup_after': guard.force(), 'network_namespace_forecast': False,
        'unexecuted_owner_argv': [tools['python']['invocation'], '-B', helpers['owner']['path'], '--phase', args.phase,
            '--control', control_pin['path'], '--control-sha256', control_pin['sha256']]}
    json_file(out, 'report.json', receipt)
    print(json.dumps({'status': receipt['status'], 'report': str(out / 'report.json'), 'unexecuted_owner_argv': receipt['unexecuted_owner_argv']}), flush=True)
    return 0


if __name__ == '__main__': raise SystemExit(main())
