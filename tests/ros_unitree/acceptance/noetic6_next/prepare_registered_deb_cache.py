#!/usr/bin/env python3
"""Prepare one finite, independent-inode CoW view of failed-r6c DEB candidates.

This is an offline content operation, not an SDK build or signature verifier.
Root must explicitly select this source, its control, the current hard64GiB
zero-swap cgroup, and the genuine failed reports. Exactly41 registered DEBs are
cloned; the42nd truncated, unregistered DEB and every original remain intact.
The next acquisition must independently authenticate all513 packages again.

The program starts no child, makes no network request, decodes no archive,
trims no filesystem, and removes no input. Unsupported FICLONE is an error;
there is no ordinary data-copy fallback. A failed fresh output is retained.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import resource
import stat
import sys
import time
import traceback
import types

MIB, GIB = 1024 ** 2, 1024 ** 3
P = Path('/var/tmp/uwvm-ros-complete-20261015-r1')
M = P / 'acceptance-storage-r1'
S = P / 'inputs-r35/source'
MF = S.parent / 'source-manifest.json'
MH = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
OLD = M / 'noetic-amd64-native-sdk-acquisition-r6c'
CACHE = OLD / 'model-DEBs'
OUT = M / 'noetic-amd64-native-sdk-cacheview-r6d'
LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
DIR = {'path', 'device', 'inode', 'mode', 'mtime_ns', 'ctime_ns'}
CG = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode',
      'expected_boot_id', 'expected_leaf', 'expected_cpus'}
FIELDS = ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_nlink')
HELPERS = {
    'restart': (47606, '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260'),
    'storage': (10863, 'cdc84eb4b55c727d74a382b019d4faa6f7e65778ec1c07be082073d8c37e1d97'),
}
RAW = {
    'inner': (str(OLD / 'report.json'), 1048328, 'dae93ed7054d73e483221317bd131de300dea7cbb1fa63d308835c1f74592940'),
    'outer': (str(M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r6c/report.json'),
              58936, '23d3ab88e91faae9f9622f6246b54ad17865fd91fa6c20aaa345d2ea285160e9'),
}
PARTIAL = {
    'path': str(CACHE / '00371/libboost-graph1.71-dev_1.71.0-6ubuntu6_amd64.deb'),
    'bytes': 87900, 'sha256': 'c8cf4f99beed03466a108ead46b2c0c559df4c6a37448b9dfa3c547cb6c0ff2e',
    'device': 81, 'inode': 778536, 'mtime_ns': 1586078011000000000, 'ctime_ns': 1792168275849680347,
}
SIGNED_PARTIAL = {'bytes': 317296, 'sha256': 'c4c342d737ec15ca5b2e8f1e29a50f7c84b777b7634020b8127d08fab8899ac0'}


def need(value, message):
    if not value:
        raise ValueError(message)


def facts(info):
    return tuple(getattr(info, name) for name in FIELDS)


def fullpin(path, info, digest):
    return {'path': str(path), 'bytes': info.st_size, 'sha256': digest,
            'device': info.st_dev, 'inode': info.st_ino,
            'mtime_ns': info.st_mtime_ns, 'ctime_ns': info.st_ctime_ns}


def validate_pin(value):
    need(type(value) is dict and set(value) == PIN and
         type(value['sha256']) is str and re.fullmatch('[0-9a-f]{64}', value['sha256']) and
         all(type(value[k]) is int and value[k] >= 0 for k in PIN - {'path', 'sha256'}) and
         value['inode'] > 0, 'closed ordinary current full7 required')
    absolute(value['path'])
    return value


def absolute(value):
    need(type(value) is str and value.startswith('/') and value != '/' and len(value) <= 4096 and
         str(PurePosixPath(value)) == value and '..' not in PurePosixPath(value).parts and
         not any(c in value for c in ('\0', '\n', '\r')), 'literal normalized absolute path required')
    return value


def directory(path):
    """Open every parent without following a symlink, including at bootstrap."""
    absolute(str(path))
    descriptor = os.open('/', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        for name in PurePosixPath(path).parts[1:]:
            child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=descriptor)
            os.close(descriptor)
            descriptor = child
        return descriptor
    except BaseException:
        os.close(descriptor)
        raise


def read(path, maximum, guard, *, collect=False):
    """Hash one stable named ordinary file, retaining only small metadata."""
    path = Path(absolute(str(path)))
    need(type(maximum) is int and 0 <= maximum <= 2 * GIB and
         (not collect or maximum <= 16 * MIB), 'finite read/metadata bound required')
    parent = directory(path.parent)
    descriptor = None
    try:
        descriptor = os.open(path.name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC, dir_fd=parent)
        initial = os.fstat(descriptor)
        need(stat.S_ISREG(initial.st_mode) and 0 <= initial.st_size <= maximum, 'bounded ordinary input required')
        body = bytearray() if collect else None
        digest, size = hashlib.sha256(), 0
        while True:
            guard()
            block = os.read(descriptor, min(MIB, maximum - size + 1))
            if not block:
                break
            size += len(block)
            need(size <= maximum, 'opened file exceeded its explicit limit')
            digest.update(block)
            if collect:
                body.extend(block)
        reopened = directory(path.parent)
        try:
            need(facts(initial) == facts(os.fstat(descriptor)) == facts(os.stat(path.name, dir_fd=parent, follow_symlinks=False)) ==
                 facts(os.stat(path.name, dir_fd=reopened, follow_symlinks=False)) and size == initial.st_size and
                 (os.fstat(parent).st_dev, os.fstat(parent).st_ino) == (os.fstat(reopened).st_dev, os.fstat(reopened).st_ino),
                 'opened file/absolute named parent changed')
        finally:
            os.close(reopened)
        return fullpin(path, initial, digest.hexdigest()), bytes(body) if collect else None
    finally:
        if descriptor is not None:
            os.close(descriptor)
        os.close(parent)


def parse(body):
    def pairs(rows):
        result = {}
        for key, value in rows:
            need(key not in result, 'duplicate control/report JSON key')
            result[key] = value
        return result
    return json.loads(body, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite JSON'))


def current(value):
    """Require Root's closed current incarnation before executing any helper."""
    need(type(value) is dict and set(value) == CG and
         type(value['memory_max']) is int and value['memory_max'] == 64 * GIB and
         type(value['memory_swap_max']) is int and value['memory_swap_max'] == 0 and
         value['expected_leaf'] == LEAF and value['expected_cpus'] == '0-31' and
         all(type(value[k]) is int and value[k] > 0 for k in ('expected_device', 'expected_inode')) and
         type(value['expected_boot_id']) is str and re.fullmatch(r'[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}', value['expected_boot_id']),
         'explicit original-leaf current7 hard64GiB/swap0/cpus0-31 required')
    selected = Path(LEAF)
    fd = directory(selected)
    try:
        info = os.fstat(fd)
        need((info.st_dev, info.st_ino) == (value['expected_device'], value['expected_inode']) and
             Path('/proc/sys/kernel/random/boot_id').read_text().strip() == value['expected_boot_id'] and
             Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + LEAF.removeprefix('/sys/fs/cgroup')] and
             (selected / 'memory.max').read_text().strip() == str(64 * GIB) and
             (selected / 'memory.swap.max').read_text().strip() == '0' and
             (selected / 'cpuset.cpus.effective').read_text().strip() == '0-31', 'actual current boot/cgroup identity/limits differ')
    finally:
        os.close(fd)
    return value


def names(descriptor, guard, maximum):
    """Use a new open description; never inherit a held directory's cursor.

    This preserves the r3 correction: dup() shares the old directory offset.
    Independent readers retain exact stat/name comparisons without relying on
    CPython or a filesystem to rewind somebody else's open description.
    """
    guard()
    before = os.fstat(descriptor)
    reader = os.open('.', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=descriptor)
    try:
        need(facts(before) == facts(os.fstat(reader)), 'directory reader differs from held inode')
        result = []
        with os.scandir(reader) as entries:
            for row in entries:
                guard()
                need(len(result) < maximum and 0 < len(row.name) <= 255 and row.name not in ('.', '..') and
                     not any(c in row.name for c in ('\0', '\n', '\r')), 'bounded unambiguous directory names required')
                result.append(row.name)
        need(facts(before) == facts(os.fstat(reader)) == facts(os.fstat(descriptor)), 'directory changed while enumerating')
        return sorted(result)
    finally:
        os.close(reader)


def snapshot(root, guard, *, maximum, manifest=None, expected_files=None, single_links=False):
    """Inventory every directory and file; refuse links, mounts and surprises.

    Frozen-source bodies are checked against every complete MF35 entry. Small
    DEB trees retain their rows for the durable before/after catalog. Full
    source trees retain only a streaming digest, a count, and their root facts.
    No skipped sibling, empty directory, or special-file branch is allowed.
    """
    root = Path(root)
    rootfd = directory(root)
    initial = os.fstat(rootfd)
    digest = hashlib.sha256()
    rows, files, directories, total = [], set(), set(), 0

    def record(row):
        nonlocal total
        total += 1
        need(total <= maximum, 'tree entry count exceeded bound')
        digest.update((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
        if manifest is None:
            rows.append(row)

    def walk(fd, relative, depth):
        need(depth <= 64, 'bounded tree depth required')
        before = os.fstat(fd)
        need(stat.S_ISDIR(before.st_mode) and before.st_dev == initial.st_dev, 'ordinary same-filesystem tree directory required')
        directories.add(relative)
        record({'relative': relative, 'kind': 'directory', 'facts': list(facts(before))})
        before_names = names(fd, guard, maximum)
        for name in before_names:
            guard()
            rel = name if not relative else relative + '/' + name
            path = root / rel
            info = os.stat(name, dir_fd=fd, follow_symlinks=False)
            need(info.st_dev == initial.st_dev, 'nested mounted filesystem rejected')
            if stat.S_ISDIR(info.st_mode):
                child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=fd)
                try:
                    need(facts(info) == facts(os.fstat(child)), 'directory entry changed before recursion')
                    walk(child, rel, depth + 1)
                    need(facts(info) == facts(os.fstat(child)) == facts(os.stat(name, dir_fd=fd, follow_symlinks=False)), 'directory entry changed after recursion')
                finally:
                    os.close(child)
            else:
                need(stat.S_ISREG(info.st_mode) and (not single_links or info.st_nlink == 1), 'only ordinary, unambiguous files accepted')
                need(manifest is None or rel in manifest, 'unlisted frozen source file')
                need(expected_files is None or rel in expected_files, 'unlisted old/view DEB file')
                pin, _ = read(path, 512 * MIB if manifest is not None else GIB, guard)
                need(pin == fullpin(path, info, pin['sha256']) and facts(info) == facts(os.stat(name, dir_fd=fd, follow_symlinks=False)),
                     'tree opened file differs from held named entry')
                if manifest is not None:
                    need(manifest[rel] == {'bytes': pin['bytes'], 'sha256': pin['sha256']}, 'complete MF35 source bytes differ')
                if expected_files is not None:
                    need(pin == expected_files[rel], 'actual whole-tree DEB full7 differs')
                files.add(rel)
                record({'relative': rel, 'kind': 'file', 'facts': list(facts(info)), 'file': pin})
        need(before_names == names(fd, guard, maximum) and facts(before) == facts(os.fstat(fd)), 'tree directory names/facts changed')

    try:
        walk(rootfd, '', 0)
        currentfd = directory(root)
        try:
            need(facts(initial) == facts(os.fstat(rootfd)) == facts(os.fstat(currentfd)), 'absolute whole-tree root changed')
        finally:
            os.close(currentfd)
        if manifest is not None:
            need(files == set(manifest), 'complete frozen source name set differs')
        if expected_files is not None:
            need(files == set(expected_files), 'complete old/view DEB name set differs')
            need(directories == {''} | {str(PurePosixPath(name).parent) for name in expected_files}, 'unexpected/empty DEB directory')
        return {'root': str(root), 'root_facts': list(facts(initial)), 'entries': total,
                'files': len(files), 'directories': len(directories), 'sha256': digest.hexdigest(), 'rows': rows}
    finally:
        os.close(rootfd)


def publish(path, value, guard):
    """Create a bounded fresh sealed JSON file and fsync its owned directory."""
    guard()
    body = (json.dumps(value, indent=2, sort_keys=True) + '\n').encode()
    need(len(body) <= 4 * MIB, 'small cache-view receipt/catalog bound exceeded')
    parent = directory(Path(path).parent)
    descriptor = None
    try:
        descriptor = os.open(Path(path).name, os.O_RDWR | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600, dir_fd=parent)
        offset = 0
        while offset < len(body):
            guard()
            written = os.write(descriptor, body[offset:])
            need(written > 0, 'metadata write made no progress')
            offset += written
        os.fchmod(descriptor, 0o444)
        os.fsync(descriptor)
        os.fsync(parent)
    finally:
        if descriptor is not None:
            os.close(descriptor)
        os.close(parent)
    return read(path, 4 * MIB, guard)[0]


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--control', type=Path, required=True)
    parser.add_argument('--control-sha256', required=True)
    parser.add_argument('--self-sha256', required=True)
    args = parser.parse_args()
    need(sys.platform == 'linux' and os.geteuid() == 0 and os.uname().machine == 'x86_64', 'Linux x86_64 Root content preparation required')
    started = time.monotonic()

    def bootstrap_guard():
        need(time.monotonic() - started <= 1800 and resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 512 * MIB,
             'bounded30-minute/512MiB content operation required')

    self_pin, _ = read(Path(__file__), MIB, bootstrap_guard)
    need(self_pin['sha256'] == args.self_sha256, 'Root-selected preparer source SHA differs')
    control_pin, raw = read(args.control, MIB, bootstrap_guard, collect=True)
    need(control_pin['sha256'] == args.control_sha256, 'Root-selected control SHA differs')
    control = parse(raw)
    need(type(control) is dict and set(control) == {'format_version', 'kind', 'cgroup', 'source', 'manifest', 'manifest_sha256',
        'manifest_pin', 'helpers', 'source_inner_report', 'source_outer_report', 'source_cache_root', 'excluded_partial',
        'historical_signed_partial', 'owned_storage', 'out'} and control['format_version'] == 1 and
        control['kind'] == 'noetic-r6d-finite-CoW-DEB-candidate-view' and control['source'] == str(S) and
        control['manifest'] == str(MF) and control['manifest_sha256'] == MH and control['out'] == str(OUT),
        'closed finite r6d candidate-only control required')
    current(control['cgroup'])
    mf_pin, body = read(MF, 4 * MIB, bootstrap_guard, collect=True)
    need(mf_pin == validate_pin(control['manifest_pin']) and (mf_pin['bytes'], mf_pin['sha256']) == (3397553, MH), 'current complete MF35 pin differs')
    frozen = parse(body)
    need(type(frozen) is dict and len(frozen) == 17882, 'complete MF35 required')
    need(type(control['helpers']) is dict and set(control['helpers']) == set(HELPERS), 'closed two-helper selection required')
    pins = {row['path']: row for row in (self_pin, control_pin, mf_pin)}
    modules = {}
    for role, expected in HELPERS.items():
        pin = validate_pin(control['helpers'][role])
        actual, body = read(pin['path'], MIB, bootstrap_guard, collect=True)
        need(actual == pin and (pin['bytes'], pin['sha256']) == expected, 'reviewed current helper differs')
        pins[pin['path']] = pin
        module = types.ModuleType('noetic_r6d_cache_' + role)
        module.__file__ = pin['path']
        sys.modules[module.__name__] = module
        exec(compile(body, pin['path'], 'exec'), module.__dict__)
        modules[role] = module
    restart, storage = modules['restart'], modules['storage']
    probe = restart.CurrentGuard(str(S), frozen, control['cgroup'])
    probe.maximum_seconds = 1800
    pins[probe.probe_pin['path']] = probe.probe_pin
    finalizing = False

    def guard():
        bootstrap_guard()
        probe()
        if not finalizing:
            for path in (M, *M.parents):
                space = os.statvfs(path)
                need(space.f_bavail * space.f_frsize >= 8 * GIB, 'active content preparation backing reserve exhausted')

    def remember(path, sha):
        actual, _ = read(path, 2 * GIB, guard)
        need(actual['sha256'] == sha and (str(path) not in pins or pins[str(path)] == actual), 'current retained input changed')
        pins[str(path)] = actual
        return actual

    def forbidden(*_args, **_kwargs):
        raise ValueError('cache-view preparation cannot start a child, trim, or register an SDK product')

    report = {'format_version': 1, 'kind': 'noetic-r6d-finite-CoW-DEB-candidate-view', 'status': 'RUNNING',
              'control': control_pin, 'preparer': self_pin, 'cgroup_before': probe.before, 'errors': [],
              'network_executed': False, 'signature_authority_reestablished': False, 'compiler_executed': False,
              'package_extraction_executed': False, 'target_program_executed': False, 'trim_executed': False,
              'old_failed_status_promoted': False, 'old_inputs_modified': False, 'ordinary_copy_fallback': False,
              'actors_started': 0, 'cleanup': {'status': 'PASS_NO_ACTORS', 'remaining': []}}
    helper_common = types.SimpleNamespace(opened=lambda path: read(path, 2 * GIB, guard)[0])
    owned = storage.OwnedStorage(control['owned_storage'], safety_guard=guard, run_trim=forbidden, common=helper_common,
        remember=remember, product=forbidden, report=report, out=OUT)
    need(control['owned_storage']['mountpoint'] == str(M), 'candidate view must use original owned Btrfs mount')
    report['storage_before'] = owned.verify()
    need(not OUT.exists() and not OUT.is_symlink(), 'fresh r6d cache-view output required')
    parent = directory(M)
    try:
        guard()
        os.mkdir(OUT.name, 0o700, dir_fd=parent)
        os.fsync(parent)
    finally:
        os.close(parent)
    try:
        reports = {}
        for role, field in (('inner', 'source_inner_report'), ('outer', 'source_outer_report')):
            expected = validate_pin(control[field])
            need((expected['path'], expected['bytes'], expected['sha256']) == RAW[role], 'only exact genuine r6c failed raw report selected')
            pin, body = read(expected['path'], 2 * MIB, guard, collect=True)
            need(pin == expected, 'current raw failed-report full7 differs')
            reports[role] = parse(body)
            pins[pin['path']] = pin
        inner, outer = reports['inner'], reports['outer']
        need(inner['kind'] == 'authenticated-apt-sdk-acquisition' and inner['status'] == 'FAILED_ACQUISITION' and inner['packages'] == [] and
             outer['kind'] == 'noetic-r6-root-owned-fresh-all513-rebuild' and outer['status'] == 'FAIL' and
             outer['actual_terminal_or_interrupted_inner_report'] == control['source_inner_report'], 'genuine failed0-extracted-package r6c provenance required')
        for raw_report in (inner, outer):
            need(raw_report['source'] == str(S) and raw_report['source_manifest_sha256'] == MH and
                 all(raw_report[k] is True for k in ('source_preserved', 'inputs_preserved', 'products_preserved')) and
                 raw_report['cleanup'] == {'remaining': [], 'status': 'PASS'} and
                 all(raw_report[k] is False for k in ('compiler_executed', 'target_program_executed', 'compatibility_qualified')),
                 'failed provenance scope or terminal preservation/cleanup differs')
        need(control['excluded_partial'] == PARTIAL and control['historical_signed_partial'] == SIGNED_PARTIAL and
             (PARTIAL['bytes'], PARTIAL['sha256']) != (SIGNED_PARTIAL['bytes'], SIGNED_PARTIAL['sha256']), 'exact known truncated old file must remain excluded and preserved')
        selected = {}
        for candidate in inner['product_pins']:
            validate_pin(candidate)
            path = Path(candidate['path'])
            if path.is_relative_to(CACHE):
                relative = str(path.relative_to(CACHE))
                need(re.fullmatch(r'\d{5}/[^/]+\.deb', relative) and candidate['bytes'] > 0 and relative not in selected,
                     'unambiguous nonempty registered ordinary DEB paths required')
                selected[relative] = candidate
        need(len(selected) == 41 and {name.split('/')[0] for name in selected} == {'%05d' % i for i in range(330, 371)},
             'exactly the41 genuine registered r6c DEBs required')
        all_old = dict(selected)
        all_old[str(Path(PARTIAL['path']).relative_to(CACHE))] = PARTIAL
        chosen_root = control['source_cache_root']
        need(type(chosen_root) is dict and set(chosen_root) == DIR and chosen_root['path'] == str(CACHE), 'explicit current old cache directory identity required')
        rootfd = directory(CACHE)
        try:
            info = os.fstat(rootfd)
            # Root's passive six-field directory selection records permission
            # bits. O_DIRECTORY above proves the type; catalogs retain full
            # st_mode separately, so no file-type information is discarded.
            need({'path': str(CACHE), 'device': info.st_dev, 'inode': info.st_ino, 'mode': stat.S_IMODE(info.st_mode),
                  'mtime_ns': info.st_mtime_ns, 'ctime_ns': info.st_ctime_ns} == chosen_root and info.st_dev == control['owned_storage']['mount_device'],
                 'actual old cache root differs or is outside selected Btrfs')
        finally:
            os.close(rootfd)
        source_before = snapshot(S, guard, maximum=50000, manifest=frozen)
        before = snapshot(CACHE, guard, maximum=100, expected_files=all_old, single_links=True)
        need(sum(row['file']['bytes'] for row in before['rows'] if row['kind'] == 'file') == 18625844,
             'whole42 source byte count differs')
        before_pin = publish(OUT / 'source42-before.json', before, guard)
        view = OUT / 'debs'
        outfd = directory(OUT)
        clones, expected_view = [], {}
        try:
            guard()
            os.mkdir('debs', 0o700, dir_fd=outfd)
            os.fsync(outfd)
            viewfd = directory(view)
            try:
                need(os.fstat(viewfd).st_dev == info.st_dev, 'fresh view must be on the same selected Btrfs filesystem')
                for relative, expected in sorted(selected.items()):
                    guard()
                    ordinal, name = relative.split('/')
                    os.mkdir(ordinal, 0o700, dir_fd=viewfd)
                    target_parent = os.open(ordinal, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=viewfd)
                    source_parent = directory(Path(expected['path']).parent)
                    sourcefd, targetfd = None, None
                    try:
                        sourcefd = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC, dir_fd=source_parent)
                        source_info = os.fstat(sourcefd)
                        current_pin, _ = read(expected['path'], GIB, guard)
                        need(current_pin == expected and fullpin(expected['path'], source_info, expected['sha256']) == expected and source_info.st_nlink == 1,
                             'registered clone source changed before ioctl')
                        targetfd = os.open(name, os.O_RDWR | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600, dir_fd=target_parent)
                        evidence = storage.clone_descriptors(sourcefd, targetfd, guard)
                        need(evidence['sha256'] == expected['sha256'] and evidence['bytes'] == expected['bytes'] and evidence['ioctl_performed'] is True,
                             'actual complete FICLONE bytes differ from registered candidate')
                        os.fchmod(targetfd, 0o444)
                        os.fsync(targetfd)
                        after, _ = read(view / relative, GIB, guard)
                        need(after['device'] == expected['device'] and after['inode'] != expected['inode'] and
                             after['bytes'] == expected['bytes'] and after['sha256'] == expected['sha256'] and
                             facts(source_info) == facts(os.fstat(sourcefd)) and read(expected['path'], GIB, guard)[0] == expected,
                             'clone source/name/full bytes changed after sealing')
                        expected_view[relative] = after
                        clones.append({'relative': relative, 'source_current': expected, 'view_current': after, 'clone': evidence})
                        os.fchmod(target_parent, 0o555)
                        os.fsync(target_parent)
                    finally:
                        if targetfd is not None:
                            os.close(targetfd)
                        if sourcefd is not None:
                            os.close(sourcefd)
                        os.close(source_parent)
                        os.close(target_parent)
                os.fchmod(viewfd, 0o555)
                os.fsync(viewfd)
            finally:
                os.close(viewfd)
        finally:
            os.close(outfd)
        view_state = snapshot(view, guard, maximum=100, expected_files=expected_view, single_links=True)
        after = snapshot(CACHE, guard, maximum=100, expected_files=all_old, single_links=True)
        need(after == before, 'whole42 original DEB tree changed during candidate-view preparation')
        source_after = snapshot(S, guard, maximum=50000, manifest=frozen)
        need(source_after == source_before, 'complete frozen MF35 source changed during content operation')
        after_pin = publish(OUT / 'source42-after.json', after, guard)
        view_pin = publish(OUT / 'view41.json', view_state, guard)
        clones_pin = publish(OUT / 'clones41.json', clones, guard)
        report['storage_after'] = owned.verify()
        probe.force()
        for expected in pins.values():
            need(read(expected['path'], max(MIB, expected['bytes']), guard)[0] == expected, 'final current input pin changed')
        need(snapshot(view, guard, maximum=100, expected_files=expected_view, single_links=True) == view_state and
             snapshot(CACHE, guard, maximum=100, expected_files=all_old, single_links=True) == before, 'terminal whole source/view recheck differs')
        for expected in (before_pin, after_pin, view_pin, clones_pin):
            need(read(expected['path'], 4 * MIB, guard)[0] == expected, 'fresh catalog/clone-ledger product changed')
        report.update(status='PREPARED_COW_DEB_CANDIDATES_ONLY_NOT_NEW_SIGNATURE_AUTHORITY',
            root=str(view), source_cache_root=chosen_root, source_before=source_before, source_after=source_after,
            source_catalog_before=before_pin, source_catalog_after=after_pin, view_catalog=view_pin,
            clone_ledger=clones_pin, registered_candidates=41, excluded_original_files=[PARTIAL],
            historical_signed_partial=SIGNED_PARTIAL, reflinks_used=41, source_preserved=True,
            inputs_preserved=True, products_preserved=True, source_reports={k: control[v] for k, v in
                (('inner', 'source_inner_report'), ('outer', 'source_outer_report'))},
            input_pins=list(pins.values()), candidate_pins=list(expected_view.values()), cgroup_after=probe.latest,
            requirement='FRESH_SIGNED513_CLOSURE_AND_CONTROLS_AND_ALL513_EXTRACTION_AND_FINAL_TREE_REQUIRED')
        publish(OUT / 'report.json', report, guard)
        outfd = directory(OUT)
        try:
            os.fchmod(outfd, 0o555)
            os.fsync(outfd)
        finally:
            os.close(outfd)
    except BaseException as error:
        # No actor exists. Low disk must not prevent bounded read-only guard or
        # reporting; this branch performs no clone, trim, package or file removal.
        finalizing = True
        report.update(status='FAIL_CONTENT_VIEW_NOT_ADMITTED', errors=[repr(error)], traceback=traceback.format_exc())
        try:
            probe.force()
            publish(OUT / 'failure.json', report, guard)
        except BaseException as final_error:
            print('failed to publish bounded content failure: ' + repr(final_error), file=sys.stderr)
        raise


if __name__ == '__main__':
    main()
