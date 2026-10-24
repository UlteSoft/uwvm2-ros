#!/usr/bin/env python3
"""Focused read-only current-alias/old-catalog diagnostic, never admission.

Root runs this metadata script under the existing current64GiB cgroup guard.
Only three explicitly named directories are enumerated, nonrecursively. Original
reports and their already named complete catalog files are streamed and hashed;
no executable, compiler, package tool, Runtime or broad filesystem walk starts.
Current alias observations are stat-only and cannot qualify content or nlink
history. Complete historical catalog rows remain necessary for union admission.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import sys


GIB = 1024**3
CG = Path('/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope')
BOOT = 'ec0d5f0f-3060-4220-bee2-2a7791ed4e72'
ROOTS = ('/usr/lib/arm-none-eabi/bin', '/usr/lib/cargo/bin', '/usr/lib/cargo/bin/coreutils')
INODES = {12327242, 12327243, 12327247, 12327248, 12327249, 12327250, 12327251, 12327252, 12327255, 12321529}
PIN = frozenset(('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'))


def need(value, reason):
    if not value:
        raise ValueError(reason)


def guard():
    need(sys.platform == 'linux' and os.geteuid() == 0 and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == BOOT and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + str(CG).removeprefix('/sys/fs/cgroup')],
         'exact current original Root cgroup/boot required')
    info = CG.stat()
    need((info.st_dev, info.st_ino) == (29, 18315) and (CG / 'memory.max').read_text().strip() == str(64 * GIB) and
         (CG / 'memory.swap.max').read_text().strip() == '0' and
         int((CG / 'memory.current').read_text()) <= 60 * GIB and
         (CG / 'cpuset.cpus.effective').read_text().strip() == '0-31' and
         resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= GIB,
         'hard64GiB/swap0 reserve or finite diagnostic RSS changed')
    for parent in (CG, *[p for p in CG.parents if p.is_relative_to('/sys/fs/cgroup')]):
        path = parent / 'memory.events'
        if path.exists():
            events = dict(line.split() for line in path.read_text().splitlines())
            need(all(int(events.get(key, '0')) == 0 for key in ('oom', 'oom_kill', 'oom_group_kill')), 'ancestor OOM invalidates diagnostic')


def directory(path):
    selected = Path(path)
    need(selected.is_absolute() and str(selected) == path and '..' not in selected.parts, 'literal absolute directory required')
    descriptor = os.open('/', os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC)
    try:
        for name in selected.parts[1:]:
            other = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW, dir_fd=descriptor)
            os.close(descriptor)
            descriptor = other
        return descriptor
    except BaseException:
        os.close(descriptor)
        raise


def facts(info):
    return info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns, info.st_mode, info.st_nlink


def read(path, maximum, *, rows=False):
    """Hash a complete ordinary file; optional JSONL filtering stays bounded."""
    selected = Path(path)
    parent = directory(str(selected.parent))
    try:
        descriptor = os.open(selected.name, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK, dir_fd=parent)
        try:
            first = os.fstat(descriptor)
            need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, 'bounded ordinary diagnostic input required')
            digest, size, blocks, matches = hashlib.sha256(), 0, [], []
            with os.fdopen(os.dup(descriptor), 'rb') as stream:
                while True:
                    guard()
                    block = stream.readline(65537) if rows else stream.read(65536)
                    if not block:
                        break
                    size += len(block)
                    need(size <= maximum, 'diagnostic input grew')
                    digest.update(block)
                    if rows:
                        need(len(block) <= 65536 and block.endswith(b'\n'), 'bounded complete catalog row required')
                        row = json.loads(block)
                        if row.get('kind') == 'file' and row.get('device') == 66307 and row.get('inode') in INODES:
                            matches.append(row)
                    else:
                        blocks.append(block)
            need(size == first.st_size and facts(first) == facts(os.fstat(descriptor)) ==
                 facts(os.stat(selected.name, dir_fd=parent, follow_symlinks=False)), 'diagnostic input changed')
            pin = {'path': str(selected), 'bytes': size, 'sha256': digest.hexdigest(), 'device': first.st_dev,
                   'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
            return pin, matches if rows else json.loads(b''.join(blocks))
        finally:
            os.close(descriptor)
    finally:
        os.close(parent)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--report', action='append', required=True, help='Explicit genuine original native/Worker producer report path.')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    guard()
    need(1 <= len(args.report) <= 8 and len(set(args.report)) == len(args.report), 'at most8 distinct selected original reports required')
    result = {'scope': 'DIAGNOSTIC_METADATA_ONLY_NO_CONTENT_OR_EXECUTION_QUALIFICATION',
              'current_stat_only_aliases': [], 'reports': [], 'historical_catalogs': [], 'errors': []}
    for path in ROOTS:
        try:
            fd = directory(path)
            try:
                before = os.fstat(fd)
                names = sorted(os.listdir(fd))
                need(len(names) <= 4096, 'focused directory name bound exceeded')
                for name in names:
                    guard()
                    info = os.stat(name, dir_fd=fd, follow_symlinks=False)
                    if stat.S_ISREG(info.st_mode) and info.st_dev == 66308 and info.st_ino in INODES:
                        result['current_stat_only_aliases'].append({'path': path + '/' + name, 'device': info.st_dev,
                            'inode': info.st_ino, 'bytes': info.st_size, 'mtime_ns': info.st_mtime_ns,
                            'ctime_ns': info.st_ctime_ns, 'mode': info.st_mode, 'nlink': info.st_nlink,
                            'content_sha256': 'NOT_READ_DIAGNOSTIC_ONLY'})
                need(names == sorted(os.listdir(fd)) and facts(before) == facts(os.fstat(fd)), 'focused directory changed')
            finally:
                os.close(fd)
        except (OSError, ValueError) as caught:
            result['errors'].append({'focused_directory': path, 'error': repr(caught)})
    inventories = {}
    for path in args.report:
        pin, report = read(path, 64 * 1024**2)
        result['reports'].append({'current_report': pin, 'historical_status_unchanged': report.get('status')})
        visited = 0
        def select(value, location):
            nonlocal visited
            visited += 1
            need(visited <= 2000000, 'finite original report JSON node bound exceeded')
            if type(value) is dict:
                if {'root', 'catalog', 'files', 'bytes', 'symlinks', 'content_sha256'} <= value.keys():
                    root = value['root']
                    if type(root) is str and any(target == root or target.startswith(root + '/') for target in ROOTS):
                        catalog = value['catalog']
                        need(type(catalog) is dict and set(catalog) == PIN, 'original complete catalog full7 required')
                        key = (root, catalog['path'], catalog['sha256'])
                        record = inventories.setdefault(key, {'root': root, 'historical_catalog': catalog, 'report_references': []})
                        record['report_references'].append({'report': pin, 'json_location': location})
                for key, child in value.items():
                    select(child, location + '/' + key)
            elif type(value) is list:
                for index, child in enumerate(value):
                    select(child, location + '/' + str(index))
        select(report, '')
        del report
    need(len(inventories) <= 128, 'bounded selected genuine catalog closure required')
    for record in inventories.values():
        old = record['historical_catalog']
        current, matches = read(old['path'], 64 * 1024**2, rows=True)
        need(current['bytes'] == old['bytes'] and current['sha256'] == old['sha256'] and
             all(current[key] == old[key] for key in PIN - {'device'}) and
             (old['device'], current['device']) in ((81, 68), (68, 68), (66307, 66308), (66308, 66308)),
             'genuine original catalog changed beyond selected persistent device transition')
        record['current_catalog'] = current
        record['matching_historical_regular_rows'] = matches
        result['historical_catalogs'].append(record)
    guard()
    need(args.out.is_absolute() and args.out.resolve() == args.out and args.out.is_relative_to('/tmp') and
         not args.out.exists() and not args.out.is_symlink(), 'fresh passive diagnostic report under/tmp required')
    with args.out.open('x', encoding='utf-8') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    guard()
    print(json.dumps({'scope': result['scope'], 'report': str(args.out),
        'current_aliases': len(result['current_stat_only_aliases']), 'genuine_catalogs': len(result['historical_catalogs'])}))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
