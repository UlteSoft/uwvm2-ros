#!/usr/bin/env python3
"""Own one explicitly staged Noetic r6 acquisition in the current 64 GiB leaf.

The owner validates current pins and the exact reviewed r4-to-r6 spec delta,
then starts one serial producer. It never installs packages, runs target code,
tests ROS, or promotes the interrupted r5 SDK. On interruption it gives only its
same-birth direct coordinator a bounded TERM grace period before the frozen
child-first pidfd cleanup. No process group or numeric-PID signal is used.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import stat
import sys
import time
import traceback
import types

MIB, GIB = 1024 ** 2, 1024 ** 3
P = Path('/var/tmp/uwvm-ros-complete-20261015-r1')
M = P / 'acceptance-storage-r1'; B = Path('/var/tmp/uwvm-ros-unitree-20261016-r1')
S = P / 'inputs-r35/source'; MF = S.parent / 'source-manifest.json'
MH = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r6d'
OLD_CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r4'
TARGET = M / 'noetic-amd64-native-sdk-acquisition-r6d'
OUT = M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r6d'
R4 = M / 'noetic-amd64-native-sdk-acquisition-r4'
CG = None
CG_KEYS = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode', 'expected_boot_id', 'expected_leaf', 'expected_cpus'}
ORIGINAL_LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
R5 = M / 'noetic-amd64-native-sdk-acquisition-r5'

BUDGET = {'self_rss_bytes': 512 * MIB, 'tree_rss_bytes': GIB, 'command_seconds': 43800,
          'active_seconds': 44400, 'whole_seconds': 46800, 'cleanup_seconds': 600,
          'reserve_bytes': 8 * GIB}
NAMES = {'producer': 'acquire-sdk.py', 'resume': 'fresh_rebuild.py', 'model': 'signed_tar_model.py',
         'storage': 'owned_storage.py', 'restart': 'requalify_after_restart.py', 'owner': 'launcher.py'}
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}


def need(value, message):
    if not value: raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, 'duplicate owner JSON key'); result[key] = value
    return result


def read(path, limit=16 * MIB):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute metadata required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= limit, 'bounded regular owner input required')
        body = bytearray()
        while True:
            block = os.read(fd, min(MIB, limit + 1 - len(body)))
            if not block: break
            body.extend(block); need(len(body) <= limit, 'owner input exceeded bound')
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'owner input changed')
        return bytes(body), {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
            'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
    finally: os.close(fd)


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite owner JSON'))


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


"""Source fragment embedded in both r6d metadata owners by the sealed factory.

This fragment uses the existing owner's read/parse/need/Path/os/stat interface.
It is not imported from an ambient path. The package contains its literal code
inside the reviewed owner, so its receipt and current facts are checked before
the unchanged producer is allowed to start. No failed status is converted.
"""

CACHE_VIEW = M / 'noetic-amd64-native-sdk-cacheview-r6d'
R6C = M / 'noetic-amd64-native-sdk-acquisition-r6c'
CACHE_PREPARER_SHA = '816acab660e49ed2e4193ca39d4a195db8da8c0b3af1ea766ed372f3dfaf8258'
CACHE_PARTIAL_SHA = 'c8cf4f99beed03466a108ead46b2c0c559df4c6a37448b9dfa3c547cb6c0ff2e'


def cache_pin(value, maximum=2 * GIB):
    need(type(value) is dict and set(value) == PIN and type(value['path']) is str and
         Path(value['path']).is_absolute() and str(Path(value['path'])) == value['path'] and
         '..' not in Path(value['path']).parts and not any(c in value['path'] for c in ('\0', '\n', '\r')) and
         type(value['sha256']) is str and len(value['sha256']) == 64 and all(c in '0123456789abcdef' for c in value['sha256']) and
         all(type(value[k]) is int and value[k] >= 0 for k in PIN - {'path', 'sha256'}) and
         value['inode'] > 0 and value['bytes'] <= maximum, 'bounded literal current cache full7 required')
    return value


def cache_directory(path):
    """Open the complete absolute spelling with no followed parent links."""
    path = Path(path)
    need(path.is_absolute() and str(path) != '/' and '..' not in path.parts, 'absolute finite cache directory required')
    fd = os.open('/', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        for name in path.parts[1:]:
            child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=fd)
            os.close(fd)
            fd = child
        return fd
    except BaseException:
        os.close(fd)
        raise


def cache_facts(info):
    return [info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns, info.st_nlink]


def cache_names(fd, guard):
    """Keep directory enumeration independent of an owner's held FD cursor."""
    guard()
    before = cache_facts(os.fstat(fd))
    reader = os.open('.', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=fd)
    try:
        need(before == cache_facts(os.fstat(reader)), 'fresh cache directory reader differs')
        result = []
        with os.scandir(reader) as entries:
            for entry in entries:
                need(len(result) < 100 and 0 < len(entry.name) <= 255 and entry.name not in ('.', '..') and
                     not any(c in entry.name for c in ('\0', '\n', '\r')), 'bounded cache name list required')
                result.append(entry.name)
        need(before == cache_facts(os.fstat(reader)) == cache_facts(os.fstat(fd)), 'cache directory facts changed')
        return sorted(result)
    finally:
        os.close(reader)


def cache_tree(snapshot, root, expected_files, remember, guard, *, sealed):
    """Require every current directory and file to match the prepared catalog.

    This is a complete finite tree comparison, not a pathname/digest selection.
    Empty, extra, renamed, linked, or mounted siblings are refused. The whole
    original42 tree is checked separately from the41 independent CoW clones.
    """
    need(type(snapshot) is dict and set(snapshot) == {'root', 'root_facts', 'entries', 'files', 'directories', 'sha256', 'rows'} and
         snapshot['root'] == str(root) and type(snapshot['rows']) is list and len(snapshot['rows']) <= 100,
         'closed bounded complete cache catalog required')
    catalog, wanted = snapshot['rows'], {}
    for row in catalog:
        need(type(row) is dict and set(row) == ({'relative', 'kind', 'facts'} if row.get('kind') == 'directory' else
             {'relative', 'kind', 'facts', 'file'}) and row['kind'] in ('file', 'directory') and
             type(row['relative']) is str and row['relative'] not in wanted and
             (row['relative'] == '' or (not Path(row['relative']).is_absolute() and '..' not in Path(row['relative']).parts and
              str(Path(row['relative'])) == row['relative'])) and
             type(row['facts']) is list and len(row['facts']) == 7 and all(type(v) is int for v in row['facts']),
             'unique literal full-fact cache row required')
        wanted[row['relative']] = row
    need({name for name, row in wanted.items() if row['kind'] == 'file'} == set(expected_files) and
         {name for name, row in wanted.items() if row['kind'] == 'directory'} ==
         {''} | {str(Path(name).parent) for name in expected_files}, 'exact cache paths including directories required')
    fd = cache_directory(root)
    root_info = os.fstat(fd)
    seen = set()

    def walk(held, relative, depth):
        guard()
        need(depth <= 2 and relative in wanted and relative not in seen, 'cache depth/name differs')
        seen.add(relative)
        row = wanted[relative]
        initial = os.fstat(held)
        need(row['kind'] == 'directory' and cache_facts(initial) == row['facts'] and
             initial.st_dev == root_info.st_dev and (not sealed or stat.S_IMODE(initial.st_mode) == 0o555),
             'prepared cache directory identity/mode changed')
        first = cache_names(held, guard)
        for name in first:
            rel = name if not relative else relative + '/' + name
            need(rel in wanted and rel not in seen, 'unlisted current cache entry')
            info = os.stat(name, dir_fd=held, follow_symlinks=False)
            entry = wanted[rel]
            need(cache_facts(info) == entry['facts'] and info.st_dev == root_info.st_dev, 'cache entry facts/mount identity changed')
            if entry['kind'] == 'directory':
                child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=held)
                try:
                    walk(child, rel, depth + 1)
                    need(cache_facts(os.fstat(child)) == cache_facts(os.stat(name, dir_fd=held, follow_symlinks=False)), 'cache child name changed')
                finally:
                    os.close(child)
            else:
                seen.add(rel)
                need(stat.S_ISREG(info.st_mode) and info.st_nlink == 1 and
                     (not sealed or stat.S_IMODE(info.st_mode) == 0o444) and
                     cache_pin(entry['file'], GIB) == expected_files[rel] and
                     entry['file']['path'] == str(root / rel), 'ordinary single-link cache file differs')
                remember(entry['file'])
                need(cache_facts(info) == cache_facts(os.stat(name, dir_fd=held, follow_symlinks=False)), 'cache file changed after hash')
        need(first == cache_names(held, guard) and cache_facts(initial) == cache_facts(os.fstat(held)), 'whole cache directory changed')

    try:
        walk(fd, '', 0)
        reopened = cache_directory(root)
        try:
            need(cache_facts(root_info) == snapshot['root_facts'] == cache_facts(os.fstat(reopened)), 'absolute cache root changed')
        finally:
            os.close(reopened)
    finally:
        os.close(fd)
    need(seen == set(wanted) and snapshot['entries'] == len(wanted) and snapshot['files'] == len(expected_files) and
         snapshot['directories'] == len(wanted) - len(expected_files) and snapshot['sha256'] ==
         hashlib.sha256(b''.join((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode() for row in catalog)).hexdigest(),
         'complete cache catalog count/digest differs')


def admit_cache_view(expected, remember, guard):
    """Bind the actual content-only receipt; fresh package authority stays later."""
    def document(pin, path):
        cache_pin(pin, 4 * MIB)
        need(pin['path'] == str(path), 'exact cache-view document path required')
        remember(pin)
        body, actual = read(path, 4 * MIB)
        need(actual == pin, 'opened cache-view document differs from selected current pin')
        return parse(body)

    value = document(expected, CACHE_VIEW / 'report.json')
    need(value['kind'] == 'noetic-r6d-finite-CoW-DEB-candidate-view' and
         value['status'] == 'PREPARED_COW_DEB_CANDIDATES_ONLY_NOT_NEW_SIGNATURE_AUTHORITY' and
         value['root'] == str(CACHE_VIEW / 'debs') and value['registered_candidates'] == value['reflinks_used'] == 41 and
         value['actors_started'] == 0 and value['cleanup'] == {'status': 'PASS_NO_ACTORS', 'remaining': []} and value['errors'] == [] and
         all(value[k] is False for k in ('network_executed', 'signature_authority_reestablished', 'compiler_executed',
             'package_extraction_executed', 'target_program_executed', 'trim_executed', 'old_failed_status_promoted',
             'old_inputs_modified', 'ordinary_copy_fallback')) and
         all(value[k] is True for k in ('source_preserved', 'inputs_preserved', 'products_preserved')) and
         value['preparer']['sha256'] == CACHE_PREPARER_SHA and value['source_before'] == value['source_after'] and
         value['source_before']['root'] == str(S) and value['source_before']['files'] == 17882 and
         value['requirement'] == 'FRESH_SIGNED513_CLOSURE_AND_CONTROLS_AND_ALL513_EXTRACTION_AND_FINAL_TREE_REQUIRED',
         'genuine finite candidate-only preparation scope required')
    # The source snapshot uses its complete MF-bound streaming digest. Its root
    # and file count are checked above; MF35 itself remains a separate owner pin.
    remember(cache_pin(value['preparer'], MIB))
    remember(cache_pin(value['control'], MIB))
    sources = value['source_reports']
    need(type(sources) is dict and set(sources) == {'inner', 'outer'} and
         (sources['inner']['bytes'], sources['inner']['sha256']) ==
         (1048328, 'dae93ed7054d73e483221317bd131de300dea7cbb1fa63d308835c1f74592940') and
         (sources['outer']['bytes'], sources['outer']['sha256']) ==
         (58936, '23d3ab88e91faae9f9622f6246b54ad17865fd91fa6c20aaa345d2ea285160e9'), 'exact genuine failed r6c raw provenance required')
    inner = document(sources['inner'], R6C / 'report.json')
    outer = document(sources['outer'], M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r6c/report.json')
    need(inner['status'] == 'FAILED_ACQUISITION' and inner['packages'] == [] and outer['status'] == 'FAIL' and
         outer['actual_terminal_or_interrupted_inner_report'] == sources['inner'], 'old cancelled reports remain failed')
    selected = {}
    for pin in inner['product_pins']:
        cache_pin(pin)
        path = Path(pin['path'])
        if path.is_relative_to(R6C / 'model-DEBs'):
            relative = str(path.relative_to(R6C / 'model-DEBs'))
            need(relative not in selected, 'repeated old registered cache pin')
            selected[relative] = pin
    candidates = {}
    need(type(value['candidate_pins']) is list and len(value['candidate_pins']) == 41 and len(selected) == 41, 'exact41 current cloned and original candidate pins required')
    for pin in value['candidate_pins']:
        cache_pin(pin, GIB)
        path = Path(pin['path'])
        need(path.is_relative_to(CACHE_VIEW / 'debs'), 'candidate outside prepared view')
        relative = str(path.relative_to(CACHE_VIEW / 'debs'))
        parts = relative.split('/')
        need(len(parts) == 2 and len(parts[0]) == 5 and parts[0].isdigit() and parts[1].endswith('.deb') and
             relative in selected and relative not in candidates and 0 < pin['bytes'] <= GIB and
             (pin['bytes'], pin['sha256'], pin['device']) == (selected[relative]['bytes'], selected[relative]['sha256'], selected[relative]['device']) and
             pin['inode'] != selected[relative]['inode'], 'independent-inode full-byte candidate relation differs')
        candidates[relative] = pin
    need(set(candidates) == set(selected) and {name.split('/')[0] for name in selected} == {'%05d' % i for i in range(330, 371)}, 'exact registered ordinal330..370 selection required')
    need(type(value['excluded_original_files']) is list and len(value['excluded_original_files']) == 1, 'exactly one original excluded partial required')
    partial = cache_pin(value['excluded_original_files'][0], GIB)
    need(partial['path'] == str(R6C / 'model-DEBs/00371/libboost-graph1.71-dev_1.71.0-6ubuntu6_amd64.deb') and
         (partial['bytes'], partial['sha256']) == (87900, CACHE_PARTIAL_SHA) and value['historical_signed_partial'] ==
         {'bytes': 317296, 'sha256': 'c4c342d737ec15ca5b2e8f1e29a50f7c84b777b7634020b8127d08fab8899ac0'},
         'old truncated file excluded only from new view')
    all_old = {**selected, str(Path(partial['path']).relative_to(R6C / 'model-DEBs')): partial}
    before = document(value['source_catalog_before'], CACHE_VIEW / 'source42-before.json')
    after = document(value['source_catalog_after'], CACHE_VIEW / 'source42-after.json')
    view = document(value['view_catalog'], CACHE_VIEW / 'view41.json')
    ledger = document(value['clone_ledger'], CACHE_VIEW / 'clones41.json')
    need(before == after and type(ledger) is list and len(ledger) == 41, 'original42 catalogs or finite clone ledger differs')
    seen = set()
    for row in ledger:
        need(type(row) is dict and set(row) == {'relative', 'source_current', 'view_current', 'clone'} and
             row['relative'] in candidates and row['relative'] not in seen and row['source_current'] == selected[row['relative']] and
             row['view_current'] == candidates[row['relative']] and row['clone']['kind'] ==
             'same-filesystem-independent-inode-FICLONE-full-read-validation' and row['clone']['ioctl_performed'] is True and
             row['clone']['ordinary_copy_fallback'] is False, 'actual finite CoW clone evidence differs')
        seen.add(row['relative'])
    cache_tree(before, R6C / 'model-DEBs', all_old, remember, guard, sealed=False)
    cache_tree(view, CACHE_VIEW / 'debs', candidates, remember, guard, sealed=True)
    return {'receipt': expected, 'source_reports': sources, 'source_catalog': value['source_catalog_before'],
            'view_catalog': value['view_catalog'], 'candidates': list(candidates.values()),
            'scope': 'CONTENT_CANDIDATES_ONLY_FRESH513_SIGNATURES_AND_ALL513_EXTRACTION_REQUIRED'}


def main():
    global CG
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--control', type=Path, required=True); parser.add_argument('--control-sha256', required=True)
    args = parser.parse_args(); started = time.monotonic()
    need(sys.platform == 'linux' and os.geteuid() == 0 and hasattr(os, 'pidfd_open'), 'Linux Root pidfd owner required')
    raw, control_pin = read(args.control); control = parse(raw)
    need(control_pin['sha256'] == args.control_sha256 and type(control) is dict, 'Root selected current control SHA differs')
    CG = current_context(control.get('cgroup'))
    need(control_pin['sha256'] == args.control_sha256 and set(control) == {
        'format_version', 'kind', 'source', 'manifest', 'manifest_sha256', 'manifest_pin', 'cgroup', 'code_root',
        'actor_out', 'owner_out', 'python', 'helpers', 'spec', 'cache', 'resume_control', 'original_spec',
        'original_producer', 'infrastructure_pins', 'resolver_alias', 'budgets', 'stage_request', 'source_package', 'cache_view_receipt'} and
        control['format_version'] == 1 and control['kind'] == 'noetic-r6-current-root-acquisition-control',
        'closed exact reviewed owner control required')
    need(control['source'] == str(S) and control['manifest'] == str(MF) and control['manifest_sha256'] == MH and
         control['cgroup'] == CG and control['code_root'] == str(CODE) and control['actor_out'] == str(TARGET) and
         control['owner_out'] == str(OUT) and control['budgets'] == BUDGET and Path(__file__) == CODE / NAMES['owner'],
         'exact fresh r6 paths, current cgroup and bounded acquisition scope required')
    raw, mf_pin = read(MF); frozen = parse(raw)
    need(mf_pin == control['manifest_pin'] and mf_pin['sha256'] == MH and mf_pin['bytes'] == 3397553 and
         len(frozen) == 17882, 'actual complete current Source35 manifest required')
    bootstrap = [control_pin, mf_pin]; bodies = {}
    for relative in ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py'):
        body, pin = read(S / relative, MIB)
        need(frozen[relative] == {key: pin[key] for key in ('bytes', 'sha256')}, 'frozen owner/probe differs')
        bodies[relative] = body; bootstrap.append(pin)
    common = types.ModuleType('noetic_r6_root_frozen_common'); common.__file__ = str(S / 'tools/ros_distro/common.py')
    sys.modules[common.__name__] = common
    exec(compile(bodies['tools/ros_distro/common.py'], common.__file__, 'exec'), common.__dict__)
    common.install_interrupts(); before = common.cgroup_state(CG)
    need(all(row['events'].get(key, 0) == 0 for row in before['ancestors'] for key in
         ('oom', 'oom_kill', 'oom_group_kill')), 'zero current ancestor OOM counters required')
    pins = {}; products = {}; runner = None; finalizing = False; roots = []; catalogs = []; source_before = None
    need(not OUT.exists() and not OUT.is_symlink() and OUT.parent.resolve(strict=True) == OUT.parent and
         not TARGET.exists() and not TARGET.is_symlink(), 'fresh r6 owner and actor outputs required')
    OUT.mkdir(mode=0o700)
    report = {'format_version': 1, 'kind': 'noetic-r6-root-owned-fresh-all513-rebuild', 'status': 'RUNNING',
        'source': str(S), 'source_manifest_sha256': MH, 'control': control_pin, 'cgroup_before': before,
        'commands': [], 'errors': [], 'compiler_executed': False, 'target_program_executed': False,
        'compatibility_qualified': False, 'old_failed_status_promoted': False, 'Guest_authority': 'UNCHANGED_EMPTY'}

    def guard():
        elapsed = time.monotonic() - started
        need(elapsed < BUDGET['whole_seconds'] and (finalizing or elapsed < BUDGET['active_seconds']), 'bounded owner deadline exceeded')
        now = common.cgroup_state(CG); common.cgroup_preserved(before, now)
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= BUDGET['self_rss_bytes'], 'owner self RSS exceeded')
        if not finalizing:
            for path in (OUT, *OUT.parents):
                space = os.statvfs(path)
                need(space.f_bavail * space.f_frsize >= BUDGET['reserve_bytes'], 'active owner backing-filesystem reserve exhausted')
        return now

    def remember(expected):
        need(type(expected) is dict and set(expected) == PIN, 'actual current full7 required')
        actual = common.opened(Path(expected['path']), max(16 * MIB, expected['bytes']))
        need(actual == expected and (actual['path'] not in pins or pins[actual['path']] == actual), 'current owner input changed')
        pins[actual['path']] = actual; return actual

    def product(path, expected=None):
        # The producer permits a signed compressed DEB up to1GiB. Reopening an
        # admitted product must stream its actual bounded size rather than
        # impose the smaller metadata bootstrap limit or collect it in RAM.
        limit = 128 * MIB
        if expected is not None:
            need(type(expected) is dict and set(expected) == PIN and type(expected['bytes']) is int and
                 0 <= expected['bytes'] <= 2 * GIB, 'bounded actual product full7 required')
            limit = max(limit, expected['bytes'])
        actual = common.opened(Path(path), limit)
        need(expected is None or actual == expected, 'owned product changed')
        need(actual['path'] not in products or products[actual['path']] == actual, 'owned product changed after registration')
        products[actual['path']] = actual; return actual

    def save():
        report['input_pins'] = list(pins.values()); report['product_pins'] = list(products.values())
        if runner: report['commands'] = runner.commands
        common.atomic_json(OUT / 'report.json', report)

    def load(role):
        expected = control['helpers'][role]; remember(expected); body, actual = read(expected['path'], MIB)
        need(actual == expected and expected['path'] == str(CODE / NAMES[role]), 'exact staged helper role required')
        module = types.ModuleType('noetic_r6_owner_' + role); module.__file__ = expected['path']; sys.modules[module.__name__] = module
        exec(compile(body, module.__file__, 'exec'), module.__dict__); return module

    save()
    try:
        guard()
        for pin in [*bootstrap, control['stage_request'], control['source_package']]: remember(pin)
        need(set(control['helpers']) == set(NAMES), 'six exact reviewed helper roles required')
        for pin in control['helpers'].values(): remember(pin)
        need(control['helpers']['owner']['path'] == str(Path(__file__)), 'current owner self selected')
        python = remember(control['python'])
        need(Path(sys.executable).resolve(strict=True) == Path(python['path']) == Path('/usr/bin/python3.14') and
             python['sha256'] == 'b8d8288faefdd300201f43fcf00f6f539a27218eeed3a3dff5ab10b9c4c99700', 'exact selected interpreter required')
        producer = load('producer'); restart = load('restart'); load('resume'); load('model'); load('storage')
        documents = {}
        for role, name in (('spec', 'noetic-amd64.spec.json'), ('cache', 'deb-cache-inputs.json'), ('resume_control', 'fresh-rebuild.json')):
            pin = remember(control[role]); body, actual = read(pin['path'])
            need(actual == pin and pin['path'] == str(CODE / name), 'exact current staged document role required')
            documents[role] = parse(body)
        seed_pin = remember(control['original_spec']); seed_raw, _ = read(seed_pin['path']); seed = parse(seed_raw)
        need(seed_pin['path'] == str(OLD_CODE / 'noetic-amd64.spec.json') and seed_pin['bytes'] == 9397 and
             seed_pin['sha256'] == '9e725094c847cfbcd8b98d647de868597ac78c440d5a7cd4d7cc70e42a28be18', 'genuine r4 reviewed seed spec required')
        old_producer = remember(control['original_producer']); old_body, _ = read(old_producer['path'], MIB)
        need(old_producer['path'] == str(OLD_CODE / 'acquire-sdk.py') and old_producer['bytes'] == 85269 and
             old_producer['sha256'] == '17f7676328192a4e914dc82d8dfd65092959aff344426b46d78ad67522dc5c53', 'genuine r4 producer source required')
        new_body, _ = read(CODE / NAMES['producer'], MIB)
        extraction = lambda body: body.split(b'class SafeTree:\n', 1)[1].split(b'\n\ndef main():', 1)[0]
        need(extraction(old_body) == extraction(new_body), 'maintained SafeTree methods changed')
        expected_spec = dict(seed); expected_spec['cgroup'] = CG
        expected_spec['input_roots'] = [*seed['input_roots'], str(CODE), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), '/usr/sbin', str(CACHE_VIEW / 'debs')]
        expected_spec['tools'] = {**seed['tools'], 'fstrim': {'path': '/usr/sbin/fstrim', 'sha256':
            '74f99afbf098df6701fdf4bda8d258ca03b1306d131f341f7ec9530681f55b1d'}}
        need(documents['spec'] == expected_spec, 'spec differs beyond current cgroup, five explicit roots and pinned fstrim')
        roots = producer.validate_spec(documents['spec'])
        continuation_control = documents['resume_control']
        need(continuation_control['kind'] == 'noetic-fresh-all513-authenticated-CoW-subset-v1' and
             continuation_control['committed_packages'] == 0 and continuation_control['complete_packages'] == 513 and
             continuation_control['old_report']['path'] == str(R5 / 'report.json') and
             (continuation_control['old_report']['bytes'], continuation_control['old_report']['sha256']) ==
             (6283350, 'fa300972b9639153446ac2005d31944b748f7d3de12cd2f799db5abcb668ceb1') and
             continuation_control['old_outer_report']['sha256'] == '0c32ebee7563a3d8e25c0ad54a115850986beb40410debd36a4d63b18ba9901d' and
             continuation_control['old_ledger']['sha256'] == 'd4d6c432569455916b4fec9bfd1df2c63e7f98e6bb04975001b92526d403f021' and
             continuation_control['old_partial_sdk']['path'] == str(R5 / 'sdk'), 'genuine interrupted r5 content-only provenance and zero continued prefix required')
        for key in ('old_report', 'old_outer_report', 'old_ledger'): remember(continuation_control[key])
        remember(continuation_control['owned_storage']['fstrim'])
        view_selection = admit_cache_view(control['cache_view_receipt'], remember, guard)
        report['cache_view_selection'] = view_selection
        need(documents['cache']['roots'] == [str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), str(CACHE_VIEW / 'debs')] and
             documents['cache']['provenance'] == [continuation_control['old_report'], continuation_control['old_outer_report'], view_selection['source_reports']['inner'], view_selection['source_reports']['outer']] and
             documents['cache']['required_hit_packages'] == ['ros-noetic-ros-base'], 'exact four retained caches and genuine failed provenance required')
        need(all(not (TARGET == root or TARGET in root.parents or root in TARGET.parents or OUT == root or OUT in root.parents or root in OUT.parents)
                 for root in roots), 'actor/owner outputs must be disjoint from every input root')
        for pin in control['infrastructure_pins']: remember(pin)
        need(producer.alias_binding('/etc/resolv.conf') == control['resolver_alias'], 'current resolver alias changed')
        source_before = common.source_state(S, MF); report['source_before'] = source_before
        for index, root in enumerate(roots):
            guard(); entry = common.catalog(root, OUT / ('host-before-%02d.jsonl' % index), roots)
            product(entry['catalog']['path']); catalogs.append(entry)
        report['input_roots_before'] = catalogs
        runner = common.SerialRunner(OUT, {'rss_bytes': BUDGET['tree_rss_bytes'],
            'command_seconds': BUDGET['command_seconds'], 'log_bytes': 32 * MIB}, guard)
        argv = [python['path'], '-B', str(CODE / NAMES['producer']), '--source', str(S), '--manifest', str(MF),
            '--manifest-sha256', MH, '--spec', control['spec']['path'], '--spec-sha256', control['spec']['sha256'],
            '--deb-cache', control['cache']['path'], '--deb-cache-sha256', control['cache']['sha256'],
            '--resume-control', control['resume_control']['path'], '--resume-control-sha256', control['resume_control']['sha256'],
            '--resume-helper', control['helpers']['resume']['path'], '--resume-helper-sha256', control['helpers']['resume']['sha256'],
            '--restart-helper', control['helpers']['restart']['path'], '--restart-helper-sha256', control['helpers']['restart']['sha256'],
            '--model-helper', control['helpers']['model']['path'], '--model-helper-sha256', control['helpers']['model']['sha256'],
            '--storage-helper', control['helpers']['storage']['path'], '--storage-helper-sha256', control['helpers']['storage']['sha256'],
            '--out', str(TARGET)]
        report['actual_acquisition_argv'] = argv; save()
        environment = {'LANG': 'C', 'LC_ALL': 'C', 'PATH': '/usr/bin', 'PYTHONDONTWRITEBYTECODE': '1',
            'GOMAXPROCS': '1', 'MAKEFLAGS': '-j1', 'DPKG_DEB_THREADS_MAX': '1'}
        runner.run('noetic-r6-authenticated-partial-sdk-continuation', argv, CODE, environment)
        raw, result_pin = read(TARGET / 'report.json', 64 * MIB); result = parse(raw); product(result_pin['path'], result_pin)
        need(result['status'] == 'PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT' and result['source_manifest_sha256'] == MH and
             result['spec'] == control['spec'] and result['resume_control'] == control['resume_control'] and
             len(result['packages']) == 513 and result['acquisition_complete'] is True and result['cleanup']['status'] == 'PASS' and
             not result['cleanup']['remaining'] and result['source_preserved'] is True and result['inputs_preserved'] is True and
             result['products_preserved'] is True and all(row['status'] == 'PASS' and row.get('returncode') == 0 for row in result['commands']),
             'complete actual fresh-authenticated preserved 513-package r6 extraction required')
        continuation = result['continuation']
        need(continuation['actual_old_inner_status'] == 'RUNNING' and continuation['actual_old_outer_status'] == 'FAIL' and
             continuation['old_status_promoted'] is False and continuation['historical_rows_admitted_as_completed'] == 0 and
             continuation['old_cgroup_or_process_continuity'] == 'NOT_CLAIMED' and continuation['old_SDK_renamed_or_modified'] is False and
             continuation['old_SDK_preserved'] is True and continuation['all_packages_freshly_extracted'] is True and
             continuation['fresh_package_count'] == 513 and continuation['fresh_all513_virtual_model'] is True and
             continuation['authenticated_subset_clone_completed'] is True and continuation['ordinary_copy_fallback'] is False and
             continuation['new_SafeTree_initial_state'] == {'entries': 0, 'bytes': 0, 'sequence': 0, 'pending': []} and
             continuation['new_payload_ledger_initial_bytes'] == 0 and continuation['old_ledger_used_as_authority'] is False,
             'honest fresh-all513 extraction/model/ledger/independent-CoW-content contract required')
        need(result['owned_storage']['files_deleted'] is False and result['owned_storage']['root_filesystems_trimmed'] is False and
             len(result['owned_storage']['trim_barriers']) >= 1027, 'owned unused-block barriers required for all model and extraction tar retirements')
        for pin in result['input_pins']: remember(pin)
        for pin in result['product_pins']: product(pin['path'], pin)
        report['actual_sdk'] = result['sdk']; report['acquisition_report'] = result_pin
        report['acquisition_completed'] = True
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
                first, now = runner.roots.get(child.pid), common.identity(child.pid)
                need(first is not None and now is not None and common.birth(first) == common.birth(now) and
                     now['parent_pid'] == os.getpid(), 'only same-birth direct producer may receive TERM')
                runner.acquire(child.pid, os.getpid()); key = common.birth(first)
                need(key in runner.owned or child.poll() is not None, 'live direct producer lacks pidfd ownership')
                if key not in runner.owned: continue
                try: signal.pidfd_send_signal(runner.owned[key]['fd'], signal.SIGTERM)
                except ProcessLookupError: pass
                deadline = min(time.monotonic() + BUDGET['cleanup_seconds'], started + BUDGET['whole_seconds'])
                while child.poll() is None and time.monotonic() < deadline:
                    guard(); runner.discover()
                    need(runner.rss() <= BUDGET['tree_rss_bytes'], 'cleanup owned RSS bound exceeded')
                    time.sleep(0.05)
        def cleanup():
            report['cleanup'] = runner.close() if runner else {'status': 'PASS', 'remaining': [], 'scope': 'NO_ACTOR_STARTED'}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup']['remaining'] and not report['cleanup'].get('errors'),
                 'child-first owned-pidfd cleanup incomplete')
        finish('same-birth-direct-coordinator-600-second-grace', cooperative)
        finish('child-first-owned-pidfd-fallback', cleanup)
        def actual_failure_metadata():
            path = TARGET / 'report.json'
            if path.exists():
                body, pin = read(path, 64 * MIB); actual = parse(body); product(path, pin)
                report['actual_terminal_or_interrupted_inner_report'] = pin
                report['actual_inner_status'] = actual.get('status')
                report['actual_inner_report_promoted'] = False
        finish('actual-inner-failure-metadata', actual_failure_metadata)
        if runner:
            for row in runner.commands:
                path = OUT / (row['label'] + '.log')
                if path.exists(): finish('final-raw-log', lambda path=path: product(path))
            if (OUT / 'commands.json').exists(): finish('final-command-ledger', lambda: product(OUT / 'commands.json'))
        def preserve():
            need(source_before is not None and len(catalogs) == len(roots), 'initial complete input closure was not admitted')
            need(common.source_state(S, MF) == source_before, 'frozen complete Source35 changed')
            need(admit_cache_view(control['cache_view_receipt'], remember, guard) == view_selection, 'whole42 original/whole41 view or receipt changed')
            after = []
            for index, root in enumerate(roots):
                guard(); entry = producer.read_only_catalog(common, root, roots, guard)
                need({k: v for k, v in entry.items() if k not in {'catalog', 'catalog_disposition'}} ==
                     {k: v for k, v in catalogs[index].items() if k != 'catalog'}, 'complete immutable input root changed')
                after.append(entry)
            for pin in pins.values(): need(common.opened(Path(pin['path']), max(16 * MIB, pin['bytes'])) == pin, 'current selected input changed')
            for pin in products.values(): need(common.opened(Path(pin['path']), max(128 * MIB, pin['bytes'])) == pin, 'retained product changed')
            need(producer.alias_binding('/etc/resolv.conf') == control['resolver_alias'], 'current resolver alias changed at finalization')
            report.update(input_roots_after=after, source_preserved=True, inputs_preserved=True,
                          products_preserved=True, cgroup_after=guard())
        finish('current-source-roots-pins-and-alias-preservation', preserve)
        report['finalization_errors'] = errors
        if report.get('acquisition_completed') and report['status'] == 'RUNNING' and not errors:
            report['status'] = 'PASS_AUTHENTICATED_FRESH_ALL513_INDEPENDENT_COW_CONTENT_SYSROOT_ONLY'
        report['elapsed_seconds'] = time.monotonic() - started; save()
    return 0 if report['status'].startswith('PASS_AUTHENTICATED_') else 1


if __name__ == '__main__': raise SystemExit(main())
