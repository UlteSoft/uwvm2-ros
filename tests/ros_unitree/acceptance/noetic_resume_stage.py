#!/usr/bin/env python3
"""Materialize a reviewed Noetic r5 source package and current metadata controls.

This metadata-only stage validates the exact current boot/cgroup, frozen
Source35, original r4 source/spec and Root's observed partial checkpoint. It
writes one fresh sealed control directory, fsyncs it, and publishes the actual
owner command using the newly observed control SHA. It starts no child process,
network request, compiler, package tool, SDK extraction or Runtime.

The payload contains four reviewed Python source roles as canonical Base64,
length and SHA256. The caller must independently review its complete immutable
bytes and pass its SHA256. That source packaging hash is not a Wasm signature
scheme. Future inode/time/control observations are never guessed in a template.
"""
from __future__ import annotations
import argparse
import base64
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types

MIB, GIB = 1024 ** 2, 1024 ** 3
P = Path('/var/tmp/uwvm-ros-complete-20261015-r1')
M = P / 'acceptance-storage-r1'; B = Path('/var/tmp/uwvm-ros-unitree-20261016-r1')
S = P / 'inputs-r35/source'; MF = S.parent / 'source-manifest.json'
MH = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r5'
OLD_CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r4'
R4 = M / 'noetic-amd64-native-sdk-acquisition-r4'
TARGET = M / 'noetic-amd64-native-sdk-acquisition-r5'
OWNER = M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r5'
RECEIPT = B / 'noetic-amd64-native-sdk-acquisition-stage-r5.json'
CG = {'memory_max': 64 * GIB, 'memory_swap_max': 0, 'expected_device': 29,
      'expected_inode': 18315, 'expected_boot_id': 'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
      'expected_leaf': '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
      'expected_cpus': '0-31'}
NAMES = {'producer': 'acquire-sdk.py', 'resume': 'noetic_partial_resume.py',
         'restart': 'requalify_after_restart.py', 'owner': 'launcher.py'}
BUDGET = {'self_rss_bytes': 512 * MIB, 'tree_rss_bytes': GIB, 'command_seconds': 43800,
          'active_seconds': 44400, 'whole_seconds': 46800, 'cleanup_seconds': 600,
          'reserve_bytes': 8 * GIB}
SDK = {'path': str(R4 / 'sdk'), 'device': 68, 'inode': 721974, 'mode': 16832,
       'mtime_ns': 1792120848328543512, 'ctime_ns': 1792120848328543512}
LEDGER = {'path': str(R4 / 'payloads.jsonl'), 'bytes': 7719810,
          'sha256': '6f05b3f2b70e8a332de261d48ad715a3ec86c579dad45235d93b81b77d4adb24',
          'device': 68, 'inode': 721975, 'mtime_ns': 1792120938215932278, 'ctime_ns': 1792120938215932278}
LAST = {'path': str(R4 / 'deb-00254-checked-tar-retirement.json'), 'bytes': 3035,
        'sha256': 'b00548fbc513f739b511d0d2c0dfccc6440a54f09ed8ea5a8fca7e6b1ab31961',
        'device': 68, 'inode': 743901, 'mtime_ns': 1792120931931454977, 'ctime_ns': 1792120931931835217}
INFRASTRUCTURE = [
    {'path': '/etc/hosts', 'bytes': 246, 'sha256': '6da2afd315836b079be1fc1aa9c7d7420ae684fdd0158e24333a7a77a52d1904',
     'device': 66308, 'inode': 14156170, 'mtime_ns': 1767104811669632872, 'ctime_ns': 1767104811669632872},
    {'path': '/etc/nsswitch.conf', 'bytes': 594, 'sha256': 'f053e77f9d706a20cb52deecdf9602b1635ea64a3be77a4cb32205bfebb17d3e',
     'device': 66308, 'inode': 14156199, 'mtime_ns': 1779281785829008398, 'ctime_ns': 1779281785829008398},
    {'path': '/run/systemd/resolve/stub-resolv.conf', 'bytes': 936,
     'sha256': '7b4d45022f47d4da0f1e78b1e3ae7b72f445002561395033698d684da83afd4b',
     'device': 27, 'inode': 6177, 'mtime_ns': 1792147910964367489, 'ctime_ns': 1792147910964372047}]
RESOLVER = {'selection': '/etc/resolv.conf', 'resolved': '/run/systemd/resolve/stub-resolv.conf',
    'links': [{'path': '/etc/resolv.conf', 'target': '../run/systemd/resolve/stub-resolv.conf',
        'resolved': '/run/systemd/resolve/stub-resolv.conf', 'device': 66308, 'inode': 14155781,
        'bytes': len('../run/systemd/resolve/stub-resolv.conf'), 'mtime_ns': 1744737327000000000, 'ctime_ns': 1767104840541976507}]}


def need(value, message):
    if not value: raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, 'duplicate source package key'); result[key] = value
    return result


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite source package JSON'))


def read(path, maximum):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical ordinary stage input required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd); need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, 'bounded regular stage input required')
        data = bytearray()
        while True:
            block = os.read(fd, min(MIB, maximum + 1 - len(data)))
            if not block: break
            data.extend(block); need(len(data) <= maximum, 'stage input grew')
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        need(len(data) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'stage input changed')
        return bytes(data), {'path': str(path), 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
            'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
    finally: os.close(fd)


def early_guard():
    need(sys.platform == 'linux' and os.geteuid() == 0, 'Root Linux metadata stage required')
    leaf = Path(CG['expected_leaf']); info = leaf.stat()
    need((info.st_dev, info.st_ino) == (29, 18315) and Path('/proc/sys/kernel/random/boot_id').read_text().strip() == CG['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::/' + str(leaf.relative_to('/sys/fs/cgroup'))] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and (leaf / 'memory.swap.max').read_text().strip() == '0',
         'exact current 64-GiB zero-swap cgroup before helper load required')


def alias():
    path = Path('/etc/resolv.conf'); first = path.lstat(); target = os.readlink(path)
    resolved = str(path.resolve(strict=True)); last = path.lstat()
    need(stat.S_ISLNK(first.st_mode) and all(getattr(first, key) == getattr(last, key) for key in
         ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')) and os.readlink(path) == target,
         'current resolver alias changed')
    return {'selection': str(path), 'resolved': resolved, 'links': [{'path': str(path), 'target': target, 'resolved': resolved,
        'device': first.st_dev, 'inode': first.st_ino, 'bytes': first.st_size, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}]}


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--payload', type=Path, required=True); parser.add_argument('--payload-sha256', required=True)
    args = parser.parse_args(); early_guard()
    raw, payload_pin = read(args.payload, 8 * MIB)
    need(payload_pin['sha256'] == args.payload_sha256, 'reviewed full source package SHA differs')
    package = parse(raw)
    need(set(package) == {'format_version', 'kind', 'files'} and package['format_version'] == 1 and
         package['kind'] == 'noetic-r5-reviewed-partial-sdk-continuation-source-package' and
         set(package['files']) == set(NAMES), 'four closed reviewed source roles required')
    bodies = {}
    for role, name in NAMES.items():
        value = package['files'][role]
        need(set(value) == {'name', 'bytes', 'sha256', 'base64'} and value['name'] == name and
             type(value['bytes']) is int and 0 < value['bytes'] <= MIB, 'bounded exact Python source role required')
        body = base64.b64decode(value['base64'], validate=True)
        need(base64.b64encode(body).decode() == value['base64'] and len(body) == value['bytes'] and
             hashlib.sha256(body).hexdigest() == value['sha256'] and body.startswith(b'#!/usr/bin/env python3\n'),
             'canonical complete reviewed source bytes required')
        bodies[role] = body
    raw, mf_pin = read(MF, 16 * MIB); frozen = parse(raw)
    need(mf_pin['bytes'] == 3397553 and mf_pin['sha256'] == MH and len(frozen) == 17882, 'complete original Source35 content required')
    # The exact reviewed restart module checks the manifest-bound probe before
    # executing it. Only this metadata guard is loaded during staging.
    need(package['files']['restart']['bytes'] == 47606 and package['files']['restart']['sha256'] ==
         '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260', 'exact independently reviewed restart helper required')
    restart = types.ModuleType('noetic_r5_stage_restart'); restart.__file__ = str(CODE / NAMES['restart']); sys.modules[restart.__name__] = restart
    exec(compile(bodies['restart'], restart.__file__, 'exec'), restart.__dict__)
    guard = restart.CurrentGuard(str(S), frozen, CG); guard.maximum_seconds = 900; before = guard.force()
    def pin(path, maximum=16 * MIB):
        return restart.opened_file(str(path), maximum, guard)
    inputs = [payload_pin, mf_pin]
    def observed(expected):
        actual = pin(expected['path'], max(16 * MIB, expected['bytes']))
        need(actual == expected, 'Root selected current metadata observation changed'); inputs.append(actual); return actual
    for expected in [LEDGER, LAST, *INFRASTRUCTURE]: observed(expected)
    need(alias() == RESOLVER, 'Root selected resolver alias changed')
    sdk = Path(SDK['path']); info = sdk.stat(follow_symlinks=False)
    need(stat.S_ISDIR(info.st_mode) and not sdk.is_symlink() and sdk.resolve(strict=True) == sdk and
         {key: value for key, value in SDK.items() if key != 'path'} == {'device': info.st_dev, 'inode': info.st_ino,
            'mode': info.st_mode, 'mtime_ns': info.st_mtime_ns, 'ctime_ns': info.st_ctime_ns}, 'exact current unqualified partial SDK directory required')
    old_spec = pin(OLD_CODE / 'noetic-amd64.spec.json'); inputs.append(old_spec)
    old_producer = pin(OLD_CODE / 'acquire-sdk.py', MIB); inputs.append(old_producer)
    need(old_spec['bytes'] == 9397 and old_spec['sha256'] == '9e725094c847cfbcd8b98d647de868597ac78c440d5a7cd4d7cc70e42a28be18' and
         old_producer['bytes'] == 85269 and old_producer['sha256'] == '17f7676328192a4e914dc82d8dfd65092959aff344426b46d78ad67522dc5c53',
         'original exact r4 producer/spec sources required')
    seed_raw, current_seed = read(old_spec['path'], MIB); producer_raw, current_producer = read(old_producer['path'], MIB)
    need(current_seed == old_spec and current_producer == old_producer, 'old source/spec changed before metadata transformation')
    extraction = lambda body: body.split(b'class SafeTree:\n', 1)[1].split(b'\n\ndef main():', 1)[0]
    need(extraction(producer_raw) == extraction(bodies['producer']), 'SafeTree extraction methods must remain unchanged')
    seed = parse(seed_raw)
    need(seed['target'] == {'architecture': 'amd64', 'backend': 1, 'distribution': 'noetic', 'route': 'authenticated-apt-extraction',
         'ubuntu_codename': 'focal'} and len(seed['input_roots']) == 22 and str(OLD_CODE) in seed['input_roots'], 'exact original Noetic r4 selection required')
    old_report = pin(R4 / 'report.json', 16 * MIB); inputs.append(old_report)
    old_outer = pin(M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r4/report.json', 16 * MIB); inputs.append(old_outer)
    need((old_report['bytes'], old_report['sha256']) == (3615127, 'f37195059b17d68573edd9e1b072dc32c933dc3a1a9fc7fed8ec4a9f9f738bc3') and
         (old_outer['bytes'], old_outer['sha256']) == (57193, '49272f388877f7fb118c9adf0f2ef32dd2e6fbee6654e5e86eafa051cbd01b0b'),
         'exact genuine historical failed reports required')
    python = pin('/usr/bin/python3.14', 16 * MIB); inputs.append(python)
    need(python['sha256'] == seed['tools']['python']['sha256'] and Path(sys.executable).resolve(strict=True) == Path(python['path']),
         'actual metadata interpreter must be the selected inventoried Python')
    need(all(not path.exists() and not path.is_symlink() for path in (CODE, TARGET, OWNER, RECEIPT)) and
         CODE.parent.resolve(strict=True) == CODE.parent and RECEIPT.parent.resolve(strict=True) == RECEIPT.parent,
         'fresh exact r5 controls/output/receipt only; preserve any existing attempt')
    for path in (CODE.parent, *CODE.parents, TARGET.parent, *TARGET.parents):
        space = os.statvfs(path); need(space.f_bavail * space.f_frsize >= 8 * GIB, 'backing-filesystem reserve required before staging')
    products = []
    def write(name, body, executable=False):
        guard(); path = CODE / name
        fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o500 if executable else 0o400)
        try:
            with os.fdopen(os.dup(fd), 'wb') as stream: stream.write(body); stream.flush(); os.fsync(stream.fileno())
        finally: os.close(fd)
        actual = pin(path); products.append(actual); return actual
    def document(name, value):
        return write(name, (json.dumps(value, sort_keys=True, indent=2) + '\n').encode())
    CODE.mkdir(mode=0o700)
    helpers = {role: write(name, bodies[role], True) for role, name in NAMES.items()}
    spec = dict(seed); spec['cgroup'] = CG
    spec['input_roots'] = [*seed['input_roots'], str(CODE), str(R4 / 'apt/downloads')]
    spec_pin = document('noetic-amd64.spec.json', spec)
    cache = {'format_version': 1, 'kind': 'noetic-signed-deb-byte-cache-inputs',
        'roots': [str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads')],
        'provenance': [old_report, old_outer], 'required_hit_packages': ['ros-noetic-ros-base']}
    cache_pin = document('deb-cache-inputs.json', cache)
    continuation = {'format_version': 1, 'kind': 'noetic-r4-partial-sdk-r5-reconciliation', 'old_report': old_report,
        'old_outer_report': old_outer, 'old_ledger': LEDGER, 'old_sdk': SDK, 'last_retirement': LAST,
        'committed_packages': 255, 'complete_packages': 513, 'checkpoint': {'ledger_end': 6784738, 'entries': 19690, 'bytes': 414777798},
        'source_manifest_sha256': MH, 'current_artifact_device': 68,
        'scope': 'FRESH_SIGNED_METADATA_FIRST; CONTENT_RECONCILIATION_THEN_SDK_DIRECTORY_ONLY_ATOMIC_TRANSFER'}
    continuation_pin = document('partial-sdk-resume.json', continuation)
    root_control = {'format_version': 1, 'kind': 'noetic-r5-current-root-acquisition-control', 'source': str(S), 'manifest': str(MF),
        'manifest_sha256': MH, 'manifest_pin': mf_pin, 'cgroup': CG, 'code_root': str(CODE), 'actor_out': str(TARGET),
        'owner_out': str(OWNER), 'python': python, 'helpers': helpers, 'spec': spec_pin, 'cache': cache_pin,
        'resume_control': continuation_pin, 'original_spec': old_spec, 'original_producer': old_producer,
        'infrastructure_pins': INFRASTRUCTURE, 'resolver_alias': RESOLVER, 'budgets': BUDGET}
    control = document('root-control.json', root_control)
    directory = os.open(CODE, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try: os.fsync(directory)
    finally: os.close(directory)
    parent = os.open(CODE.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try: os.fsync(parent)
    finally: os.close(parent)
    for expected in inputs: need(pin(expected['path'], max(16 * MIB, expected['bytes'])) == expected, 'stage current input changed')
    for expected in products: need(pin(expected['path']) == expected, 'stage source/control product changed')
    need(alias() == RESOLVER, 'stage resolver alias changed at finalization')
    report = {'format_version': 1, 'kind': 'noetic-r5-reviewed-source-and-control-materialization',
        'status': 'PASS_STAGED_SOURCE_AND_CURRENT_METADATA_ONLY', 'payload': payload_pin, 'input_pins': inputs,
        'product_pins': products, 'current_partial_sdk': SDK, 'old_sdk_qualified': False, 'historical_failed_reports_promoted': False,
        'commands': [], 'actors_started': 0, 'network_executed': False, 'compiler_executed': False,
        'SDK_extraction_executed': False, 'target_program_executed': False, 'cgroup_before': before, 'cgroup_after': guard.force(),
        'unexecuted_owner_argv': [python['path'], '-B', str(CODE / NAMES['owner']), '--control', control['path'], '--control-sha256', control['sha256']],
        'scope': 'SOURCE_AND_CURRENT_CONTROL_STAGING_ONLY; FRESH_SIGNATURES_AND_PARTIAL_TREE_RECONCILIATION_REMAIN_UNEXECUTED'}
    raw = (json.dumps(report, sort_keys=True, indent=2) + '\n').encode()
    fd = os.open(RECEIPT, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o400)
    try:
        with os.fdopen(os.dup(fd), 'wb') as stream: stream.write(raw); stream.flush(); os.fsync(stream.fileno())
    finally: os.close(fd)
    parent = os.open(RECEIPT.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try: os.fsync(parent)
    finally: os.close(parent)
    print(json.dumps({'status': report['status'], 'receipt': str(RECEIPT), 'unexecuted_owner_argv': report['unexecuted_owner_argv']}), flush=True)
    return 0


if __name__ == '__main__': raise SystemExit(main())
