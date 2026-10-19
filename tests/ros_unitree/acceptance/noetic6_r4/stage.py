#!/usr/bin/env python3
"""Stage a reviewed fresh-all513 Noetic rebuild, without starting any actor.

Root supplies the SHA of a complete source package and a closed current-facts
request. The request explicitly selects the current boot/cgroup and existing
old failed files. This program opens their actual current identities, writes
new sealed source/controls, and publishes an UNEXECUTED owner argv. It performs
no network operation, package decode, CoW clone, trim, compiler or target run.
The old SDK/report/DEBs/ledger are never changed or relabelled as successful.
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
P = Path('/var/tmp/uwvm-ros-complete-20261015-r1'); M = P / 'acceptance-storage-r1'
B = Path('/var/tmp/uwvm-ros-unitree-20261016-r1')
S = P / 'inputs-r35/source'; MF = S.parent / 'source-manifest.json'
MH = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r6d'
OLD_CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r4'
R4 = M / 'noetic-amd64-native-sdk-acquisition-r4'; R5 = M / 'noetic-amd64-native-sdk-acquisition-r5'
TARGET = M / 'noetic-amd64-native-sdk-acquisition-r6d'
OWNER = M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r6d'
RECEIPT = B / 'noetic-amd64-native-sdk-acquisition-stage-r6d.json'
NAMES = {'producer': 'acquire-sdk.py', 'resume': 'fresh_rebuild.py', 'model': 'signed_tar_model.py',
         'storage': 'owned_storage.py', 'restart': 'requalify_after_restart.py', 'owner': 'launcher.py'}
CG_KEYS = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode', 'expected_boot_id', 'expected_leaf', 'expected_cpus'}
ORIGINAL_LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
BUDGET = {'self_rss_bytes': 512 * MIB, 'tree_rss_bytes': GIB, 'command_seconds': 43800,
          'active_seconds': 44400, 'whole_seconds': 46800, 'cleanup_seconds': 600, 'reserve_bytes': 8 * GIB}


def need(value, message):
    if not value: raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, 'duplicate stage JSON key'); result[key] = value
    return result


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite stage JSON'))


def read(path, maximum=16 * MIB):
    """Read bounded ordinary metadata before any project source executes."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical absolute stage input required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(descriptor)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, 'bounded regular stage input required')
        body = bytearray()
        while True:
            block = os.read(descriptor, min(MIB, maximum + 1 - len(body)))
            if not block: break
            body.extend(block); need(len(body) <= maximum, 'stage metadata grew')
        last, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'stage metadata changed')
        return bytes(body), {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
            'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
    finally: os.close(descriptor)


def current_context(value):
    """Require a Root-selected current incarnation, never infer reboot continuity."""
    need(type(value) is dict and set(value) == CG_KEYS and type(value['memory_max']) is int and
         type(value['memory_swap_max']) is int and value['memory_max'] == 64 * GIB and value['memory_swap_max'] == 0 and
         value['expected_leaf'] == ORIGINAL_LEAF and value['expected_cpus'] == '0-31' and
         all(type(value[key]) is int and value[key] > 0 for key in ('expected_device', 'expected_inode')) and
         type(value['expected_boot_id']) is str and len(value['expected_boot_id']) == 36, 'closed exact current7 hard64GiB zero-swap original leaf required')
    leaf = Path(ORIGINAL_LEAF); info = leaf.stat()
    need(leaf.resolve(strict=True) == leaf and (info.st_dev, info.st_ino) == (value['expected_device'], value['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == value['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + ORIGINAL_LEAF.removeprefix('/sys/fs/cgroup')] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and (leaf / 'memory.swap.max').read_text().strip() == '0' and
         (leaf / 'cpuset.cpus.effective').read_text().strip() == '0-31', 'current actual boot/physical leaf/limits differ from Root selection')
    return dict(value)


def resolver_alias():
    path = Path('/etc/resolv.conf'); first = path.lstat(); target = os.readlink(path)
    resolved = str(path.resolve(strict=True)); last = path.lstat()
    need(stat.S_ISLNK(first.st_mode) and all(getattr(first, key) == getattr(last, key) for key in
         ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')) and os.readlink(path) == target,
         'current resolver alias changed')
    need(target == '../run/systemd/resolve/stub-resolv.conf' and resolved == '/run/systemd/resolve/stub-resolv.conf',
         'explicit original resolver selection differs')
    return {'selection': str(path), 'resolved': resolved, 'links': [{'path': str(path), 'target': target, 'resolved': resolved,
        'device': first.st_dev, 'inode': first.st_ino, 'bytes': first.st_size, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}]}


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
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for role in ('request', 'payload'):
        parser.add_argument('--' + role, type=Path, required=True); parser.add_argument('--' + role + '-sha256', required=True)
    args = parser.parse_args()
    need(sys.platform == 'linux' and os.geteuid() == 0, 'actual Root Linux metadata stage required')
    raw, request_pin = read(args.request); request = parse(raw)
    need(request_pin['sha256'] == args.request_sha256 and type(request) is dict, 'Root-selected request SHA differs')
    cg = current_context(request.get('cgroup'))
    need(set(request) == {'format_version', 'kind', 'cgroup', 'old_report', 'old_outer_report', 'old_ledger', 'old_partial_sdk', 'owned_storage', 'cache_view_receipt'} and
         request['format_version'] == 1 and request['kind'] == 'noetic-r6-fresh-all513-current-facts-request', 'closed explicit current request required')
    raw, payload_pin = read(args.payload, 8 * MIB); package = parse(raw)
    need(payload_pin['sha256'] == args.payload_sha256 and set(package) == {'format_version', 'kind', 'files'} and
         package['format_version'] == 1 and package['kind'] == 'noetic-r6-reviewed-fresh-all513-source-package' and
         set(package['files']) == set(NAMES), 'closed six-role reviewed source package required')
    bodies = {}
    for role, name in NAMES.items():
        row = package['files'][role]
        need(set(row) == {'name', 'bytes', 'sha256', 'base64'} and row['name'] == name and
             type(row['bytes']) is int and 0 < row['bytes'] <= MIB, 'exact bounded source role required')
        body = base64.b64decode(row['base64'], validate=True)
        need(base64.b64encode(body).decode() == row['base64'] and len(body) == row['bytes'] and
             hashlib.sha256(body).hexdigest() == row['sha256'] and body.startswith(b'#!/usr/bin/env python3\n'), 'complete canonical reviewed source bytes required')
        bodies[role] = body
    raw, mf_pin = read(MF); frozen = parse(raw)
    need(mf_pin['bytes'] == 3397553 and mf_pin['sha256'] == MH and len(frozen) == 17882, 'original complete Source35 manifest content required')
    need(package['files']['restart']['bytes'] == 47606 and package['files']['restart']['sha256'] ==
         '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260', 'exact reviewed restart helper required')
    restart = types.ModuleType('noetic_r6_stage_restart'); restart.__file__ = str(CODE / NAMES['restart']); sys.modules[restart.__name__] = restart
    exec(compile(bodies['restart'], restart.__file__, 'exec'), restart.__dict__)
    guard = restart.CurrentGuard(str(S), frozen, cg); guard.maximum_seconds = 1800; before = guard.force()
    def pin(path, maximum=16 * MIB): return restart.opened_file(str(path), maximum, guard)
    inputs = [request_pin, payload_pin, mf_pin]
    def selected(expected):
        need(type(expected) is dict and set(expected) == PIN, 'explicit current full7 metadata selection required')
        actual = pin(expected['path'], max(16 * MIB, expected['bytes']))
        need(actual == expected, 'Root-selected current full7 changed'); inputs.append(actual); return actual
    view_selection = admit_cache_view(request['cache_view_receipt'], selected, guard)
    old_report, old_outer, old_ledger = (selected(request[key]) for key in ('old_report', 'old_outer_report', 'old_ledger'))
    need(old_report['path'] == str(R5 / 'report.json') and (old_report['bytes'], old_report['sha256']) ==
         (6283350, 'fa300972b9639153446ac2005d31944b748f7d3de12cd2f799db5abcb668ceb1') and
         old_outer['path'] == str(M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r5/report.json') and
         (old_outer['bytes'], old_outer['sha256']) == (41860, '0c32ebee7563a3d8e25c0ad54a115850986beb40410debd36a4d63b18ba9901d') and
         old_ledger['path'] == str(R5 / 'payloads.jsonl') and (old_ledger['bytes'], old_ledger['sha256']) ==
         (9988325, 'd4d6c432569455916b4fec9bfd1df2c63e7f98e6bb04975001b92526d403f021'), 'genuine interrupted r5 report/ledger bytes required, with current facts separate')
    sdk = request['old_partial_sdk']; path = Path(sdk['path']); info = path.stat(follow_symlinks=False)
    need(set(sdk) == {'path', 'device', 'inode', 'mode', 'mtime_ns', 'ctime_ns'} and path == R5 / 'sdk' and
         path.resolve(strict=True) == path and stat.S_ISDIR(info.st_mode) and
         sdk == {'path': str(path), 'device': info.st_dev, 'inode': info.st_ino, 'mode': info.st_mode,
                 'mtime_ns': info.st_mtime_ns, 'ctime_ns': info.st_ctime_ns}, 'explicit actual current unqualified old SDK directory required')
    storage = request['owned_storage']; fstrim = selected(storage['fstrim'])
    need(fstrim['path'] == '/usr/sbin/fstrim' and fstrim['bytes'] == 43544 and fstrim['sha256'] ==
         '74f99afbf098df6701fdf4bda8d258ca03b1306d131f341f7ec9530681f55b1d', 'Root-selected actual fstrim body required')
    # Recheck the explicit owned-image relation during metadata admission too.
    # Only verify() is called: no trim or clone may run from this stager.
    storage_module = types.ModuleType('noetic_r6_stage_storage')
    storage_module.__file__ = str(CODE / NAMES['storage']); sys.modules[storage_module.__name__] = storage_module
    exec(compile(bodies['storage'], storage_module.__file__, 'exec'), storage_module.__dict__)
    def storage_remember(path, expected):
        actual = pin(path); need(actual == fstrim and actual['sha256'] == expected, 'selected storage tool changed'); return actual
    def forbidden(*_args, **_kwargs): need(False, 'metadata stage may not trim, clone or create storage products')
    storage_observation = {}
    checker = storage_module.OwnedStorage(storage, safety_guard=guard, run_trim=forbidden,
        common=types.SimpleNamespace(opened=lambda path: pin(path)), remember=storage_remember,
        product=forbidden, report=storage_observation, out=CODE)
    actual_storage = checker.verify()
    seed_pin = pin(OLD_CODE / 'noetic-amd64.spec.json'); original_pin = pin(OLD_CODE / 'acquire-sdk.py', MIB); inputs += [seed_pin, original_pin]
    need((seed_pin['bytes'], seed_pin['sha256']) == (9397, '9e725094c847cfbcd8b98d647de868597ac78c440d5a7cd4d7cc70e42a28be18') and
         (original_pin['bytes'], original_pin['sha256']) == (85269, '17f7676328192a4e914dc82d8dfd65092959aff344426b46d78ad67522dc5c53'), 'exact original r4 spec/producer source required')
    raw, seed_actual = read(seed_pin['path']); seed = parse(raw); old_body, original_actual = read(original_pin['path'], MIB)
    need(seed_actual == seed_pin and original_actual == original_pin, 'old spec/source changed before metadata transform')
    extraction = lambda body: body.split(b'class SafeTree:\n', 1)[1].split(b'\n\ndef main():', 1)[0]
    need(extraction(old_body) == extraction(bodies['producer']), 'maintained SafeTree literal methods changed')
    need(len(seed['input_roots']) == 22 and str(OLD_CODE) in seed['input_roots'] and seed['target'] ==
         {'architecture': 'amd64', 'backend': 1, 'distribution': 'noetic', 'route': 'authenticated-apt-extraction', 'ubuntu_codename': 'focal'}, 'exact original22-root Noetic selection required')
    infrastructure = []
    for row in seed['input_files']:
        path = Path(row['path']).resolve(strict=True); actual = pin(path)
        need(actual['sha256'] == row['sha256'], 'explicit seed NSS/DNS input bytes changed')
        infrastructure.append(actual); inputs.append(actual)
    need({str(Path(row['path']).resolve(strict=True)) for row in seed['input_files']} ==
         {'/etc/hosts', '/etc/nsswitch.conf', '/run/systemd/resolve/stub-resolv.conf'}, 'exact seed infrastructure selections required')
    alias = resolver_alias()
    python = pin('/usr/bin/python3.14'); inputs.append(python)
    need(python['sha256'] == seed['tools']['python']['sha256'] and Path(sys.executable).resolve(strict=True) == Path(python['path']), 'actual selected metadata Python required')
    need(all(not path.exists() and not path.is_symlink() for path in (CODE, TARGET, OWNER, RECEIPT)) and
         CODE.parent.resolve(strict=True) == CODE.parent, 'all exact r6 source/output paths must be fresh')
    for path in (CODE.parent, *CODE.parents, TARGET.parent, *TARGET.parents):
        space = os.statvfs(path); need(space.f_bavail * space.f_frsize >= 8 * GIB, 'stage active backing8GiB reserve required')
    products = []
    def write(name, body, executable=False):
        guard(); path = CODE / name
        descriptor = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o500 if executable else 0o400)
        try:
            with os.fdopen(os.dup(descriptor), 'wb') as stream: stream.write(body); stream.flush(); os.fsync(stream.fileno())
        finally: os.close(descriptor)
        actual = pin(path); products.append(actual); return actual
    def document(name, value): return write(name, (json.dumps(value, sort_keys=True, indent=2) + '\n').encode())
    CODE.mkdir(mode=0o700)
    helpers = {role: write(name, bodies[role], True) for role, name in NAMES.items()}
    spec = dict(seed); spec['cgroup'] = cg
    spec['input_roots'] = [*seed['input_roots'], str(CODE), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), '/usr/sbin', str(CACHE_VIEW / 'debs')]
    spec['tools'] = {**seed['tools'], 'fstrim': {'path': fstrim['path'], 'sha256': fstrim['sha256']}}
    spec_pin = document('noetic-amd64.spec.json', spec)
    cache = {'format_version': 1, 'kind': 'noetic-signed-deb-byte-cache-inputs', 'roots':
        [str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), str(CACHE_VIEW / 'debs')],
        'provenance': [old_report, old_outer, view_selection['source_reports']['inner'], view_selection['source_reports']['outer']], 'required_hit_packages': ['ros-noetic-ros-base']}
    cache_pin = document('deb-cache-inputs.json', cache)
    continuation = {'format_version': 1, 'kind': 'noetic-fresh-all513-authenticated-CoW-subset-v1',
        'old_report': old_report, 'old_outer_report': old_outer, 'old_ledger': old_ledger, 'old_partial_sdk': sdk,
        'source_manifest_sha256': MH, 'committed_packages': 0, 'complete_packages': 513, 'owned_storage': storage}
    continuation_pin = document('fresh-rebuild.json', continuation)
    control = document('root-control.json', {'format_version': 1, 'kind': 'noetic-r6-current-root-acquisition-control',
        'source': str(S), 'manifest': str(MF), 'manifest_sha256': MH, 'manifest_pin': mf_pin, 'cgroup': cg,
        'code_root': str(CODE), 'actor_out': str(TARGET), 'owner_out': str(OWNER), 'python': python, 'helpers': helpers,
        'spec': spec_pin, 'cache': cache_pin, 'resume_control': continuation_pin, 'original_spec': seed_pin,
        'original_producer': original_pin, 'infrastructure_pins': infrastructure, 'resolver_alias': alias,
        'budgets': BUDGET, 'stage_request': request_pin, 'source_package': payload_pin, 'cache_view_receipt': request['cache_view_receipt']})
    for path in (CODE, CODE.parent):
        descriptor = os.open(path, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
        try: os.fsync(descriptor)
        finally: os.close(descriptor)
    for expected in inputs: need(pin(expected['path'], max(16 * MIB, expected['bytes'])) == expected, 'stage current input changed')
    for expected in products: need(pin(expected['path']) == expected, 'staged helper/control changed')
    need(resolver_alias() == alias, 'stage current resolver alias changed')
    need(admit_cache_view(request['cache_view_receipt'], selected, guard) == view_selection, 'whole42 original/whole41 view changed during metadata stage')
    argv = [python['path'], '-B', helpers['owner']['path'], '--control', control['path'], '--control-sha256', control['sha256']]
    report = {'format_version': 1, 'kind': 'noetic-r6-reviewed-current-source-and-control-staging',
        'status': 'PASS_STAGED_SOURCE_AND_CURRENT_METADATA_ONLY', 'input_pins': inputs, 'product_pins': products,
        'current_old_partial_sdk': sdk, 'cache_view_selection': view_selection, 'old_status_promoted': False, 'old_SDK_qualified': False,
        'actual_owned_storage_metadata_only': actual_storage,
        'historical_rows_admitted_as_completed': 0, 'actors_started': 0, 'network_executed': False,
        'CoW_executed': False, 'trim_executed': False, 'compiler_executed': False, 'target_program_executed': False,
        'cgroup_before': before, 'cgroup_after': guard.force(), 'unexecuted_owner_argv': argv}
    raw = (json.dumps(report, sort_keys=True, indent=2) + '\n').encode()
    descriptor = os.open(RECEIPT, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o400)
    try:
        with os.fdopen(os.dup(descriptor), 'wb') as stream: stream.write(raw); stream.flush(); os.fsync(stream.fileno())
    finally: os.close(descriptor)
    parent = os.open(RECEIPT.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try: os.fsync(parent)
    finally: os.close(parent)
    print(json.dumps({'status': report['status'], 'receipt': str(RECEIPT), 'unexecuted_owner_argv': argv}), flush=True)
    return 0


if __name__ == '__main__': raise SystemExit(main())
