#!/usr/bin/env python3
"""Read the actual process cgroup and effective ancestor limit. Never mutate it."""
import argparse
import errno
import json
import os
from pathlib import Path
import platform
import stat
import subprocess
import sys


def controllerless_root(path, mounts, read_errors, identity_before):
    """Qualify only an actual cgroup2 hierarchy root without memory files.

    Missing controller files below the hierarchy root are never ignored. The
    root must match one recorded cgroup2 mount whose mountinfo root is '/', keep
    its directory device/inode while the required file reads occur, and report
    ENOENT (not an unreadable/failed read) for all three required memory files.
    Returned metadata permits a later counter comparison to skip this same
    identified controllerless root without weakening nonroot counter checks.
    """
    matches = [row for row in mounts if row['root'] == '/' and row['filesystem_type'] == 'cgroup2'
               and row['mount_point'] == str(path)]
    required = ('memory.max', 'memory.current', 'memory.events')
    if (len(matches) != 1 or identity_before is None
            or any(read_errors.get(name) != errno.ENOENT for name in required)):
        return None
    try:
        identity_after = path.stat()
        mount = matches[0]
        major, minor = (int(value) for value in mount['major_minor'].split(':'))
    except (OSError, ValueError, KeyError):
        return None
    if (not stat.S_ISDIR(identity_before.st_mode) or not stat.S_ISDIR(identity_after.st_mode)
            or (identity_before.st_dev, identity_before.st_ino) != (identity_after.st_dev, identity_after.st_ino)
            or (os.major(identity_after.st_dev), os.minor(identity_after.st_dev)) != (major, minor)):
        return None
    return {'path': str(path), 'device': identity_after.st_dev, 'inode': identity_after.st_ino,
            'mount': mount, 'missing_files': {name: 'ENOENT' for name in required}}


def probe(expected_memory_max=68719476736, expected_leaf=None, expected_device=None,
          expected_inode=None, expected_boot=None):
    """Require the current explicitly selected 64 GiB, zero-swap incarnation.

    The no-argument form discovers and brackets the actual live leaf for callers
    which pin it themselves. SDK execution supplies all four expected identity
    fields; a partial identity or a different memory limit is never accepted.
    Historical 48 GiB and 64 GiB receipts remain historical; only the current
    physical inputs and explicitly bound live incarnation can qualify this run.
    """
    if expected_memory_max != 68719476736:
        return {'status': 'BLOCKED', 'expected_memory_max': expected_memory_max,
                'reason': 'current authorization requires exactly 64 GiB'}
    identity_pins = (expected_leaf, expected_device, expected_inode, expected_boot)
    if any(value is not None for value in identity_pins) and any(value is None for value in identity_pins):
        return {'status': 'BLOCKED', 'expected_memory_max': expected_memory_max,
                'reason': 'complete expected leaf/device/inode/boot identity required'}
    if platform.system() != 'Linux':
        return {'status': 'BLOCKED', 'reason': 'Linux cgroup validation requires Linux'}
    proc = Path('/proc/self/cgroup').read_text().splitlines()
    unified = [s.split(':', 2)[2] for s in proc if s.startswith('0::')]
    if len(unified) != 1:
        return {'status': 'BLOCKED', 'reason': 'cgroup v2 unified membership unavailable', 'membership': proc}
    mounts = []
    for line in Path('/proc/self/mountinfo').read_text().splitlines():
        before, after = line.split(' - ', 1)
        if after.split()[0] == 'cgroup2':
            fields = before.split()
            mounts.append({'mount_point': str(Path(fields[4])), 'root': fields[3],
                           'filesystem_type': 'cgroup2', 'mount_id': fields[0],
                           'major_minor': fields[2], 'raw_mountinfo': line})
    candidates = []
    for record in mounts:
        mount, root = Path(record['mount_point']), record['root']
        cpath = unified[0]
        if root == '/':
            candidates.append(mount / cpath.lstrip('/'))
        elif cpath == root or cpath.startswith(root.rstrip('/') + '/'):
            candidates.append(mount / cpath[len(root):].lstrip('/'))
        if cpath == '/':
            candidates.append(mount)
    actual = None
    for candidate in candidates:
        try:
            if str(os.getpid()) in (candidate / 'cgroup.procs').read_text().splitlines():
                actual = candidate
                break
        except OSError:
            pass
    if actual is None:
        return {'status': 'BLOCKED', 'reason': 'cannot prove live PID in visible actual cgroup.procs', 'membership': proc}
    try:
        leaf_before = actual.stat()
        boot_before = Path('/proc/sys/kernel/random/boot_id').read_text().strip()
    except OSError as error:
        return {'status': 'BLOCKED', 'reason': 'actual leaf/boot identity unavailable: ' + str(error), 'membership': proc}
    records = []
    current = actual
    while (current / 'cgroup.procs').exists():
        record = {'path': str(current)}
        identity_before = None
        if any(row['root'] == '/' and row['mount_point'] == str(current) for row in mounts):
            try:
                identity_before = current.stat()
            except OSError:
                pass  # An unavailable root identity cannot qualify missing counters.
        read_errors = {}
        for field in ('memory.max', 'memory.swap.max', 'memory.current', 'memory.peak', 'memory.events', 'cpu.max', 'pids.max', 'cpuset.cpus.effective'):
            try:
                record[field] = (current / field).read_text().strip()
            except OSError as error:
                record[field] = None
                read_errors[field] = error.errno
        qualified_root = controllerless_root(current, mounts, read_errors, identity_before)
        if qualified_root is not None:
            record['controllerless_hierarchy_root'] = qualified_root
        records.append(record)
        if current.parent == current:
            break
        current = current.parent
    try:
        finite = [int(r['memory.max']) for r in records if r.get('memory.max') not in (None, 'max')]
    except ValueError:
        return {'status': 'BLOCKED', 'reason': 'invalid ancestor memory.max', 'membership': proc,
                'expected_memory_max': expected_memory_max, 'cgroup_ancestors': records}
    effective = min(finite) if finite else None
    reasons = []
    if effective != expected_memory_max or not records or records[0].get('memory.max') != str(expected_memory_max):
        reasons.append('actual leaf and effective memory.max must both equal the provided 64 GiB limit')
    if not records or records[0].get('memory.swap.max') != '0':
        reasons.append('actual leaf memory.swap.max must equal zero')
    if any(any(row.get(name) is None for name in ('memory.max', 'memory.current', 'memory.events'))
           and 'controllerless_hierarchy_root' not in row for row in records):
        reasons.append('required nonroot memory/counter files are unavailable')
    try:
        leaf_after = actual.stat()
        boot_after = Path('/proc/sys/kernel/random/boot_id').read_text().strip()
        membership_after = Path('/proc/self/cgroup').read_text().splitlines()
        live_after = str(os.getpid()) in (actual/'cgroup.procs').read_text().splitlines()
        if (not boot_before or boot_before != boot_after or membership_after != proc or not live_after
                or not stat.S_ISDIR(leaf_before.st_mode) or not stat.S_ISDIR(leaf_after.st_mode)
                or (leaf_before.st_dev, leaf_before.st_ino) != (leaf_after.st_dev, leaf_after.st_ino)
                or (actual/'memory.max').read_text().strip() != str(expected_memory_max)
                or (actual/'memory.swap.max').read_text().strip() != '0'):
            reasons.append('actual live leaf/boot/membership/limit changed during the probe')
    except OSError as error:
        reasons.append('final actual leaf/boot identity unavailable: ' + str(error))
    incarnation = {'path': str(actual), 'device': leaf_before.st_dev, 'inode': leaf_before.st_ino,
                   'boot_id': boot_before}
    if expected_leaf is not None and (str(actual) != str(Path(expected_leaf))
            or (leaf_before.st_dev, leaf_before.st_ino) != (expected_device, expected_inode)
            or boot_before != expected_boot):
        reasons.append('actual cgroup incarnation differs from the complete explicit identity')
    return {'status': 'PASS' if not reasons else 'BLOCKED', 'pid': os.getpid(),
            'membership': proc, 'effective_memory_max': effective, 'expected_memory_max': expected_memory_max,
            'expected_memory_swap_max': '0', 'cgroup_incarnation': incarnation,
            'same_cgroup_incarnation_during_probe': not reasons,
            'uname': platform.uname()._asdict(), 'cpu_count': os.cpu_count(),
            'os_release': Path('/etc/os-release').read_text(), 'cgroup_ancestors': records,
            'meminfo': Path('/proc/meminfo').read_text(),
            'reason': '; '.join(reasons) if reasons else None}


if __name__ == '__main__':
    p = argparse.ArgumentParser()
    p.add_argument('--output', type=Path)
    args = p.parse_args()
    result = probe()
    encoded = json.dumps(result, indent=2, ensure_ascii=False) + '\n'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded)
    print(encoded, end='')
    sys.exit(0 if result['status'] == 'PASS' else 77)
