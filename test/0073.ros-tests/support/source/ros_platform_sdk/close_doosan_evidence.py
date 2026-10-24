#!/usr/bin/env python3
"""Retire the closed Doosan tmpfs after full two-copy/client retention.

This trusted operator tool runs inside the original hard64GiB cgroup. It
requires a final checkpoint covering every current file, independently checks
the retained client receipt, and refuses any process still using the device.
It never sends signals, performs a lazy/forced unmount, changes a resource
limit, or removes shared toolchains, native SDKs, or another task's files.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import subprocess
import tarfile


def read(path):
    return json.loads(path.read_bytes())


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--checkpoint', type=Path, required=True)
    parser.add_argument('--client-readback', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_AS, (256 << 20, 256 << 20))
    resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    root = args.root.resolve(strict=True)
    assert root.is_mount() and root.stat().st_dev != root.parent.stat().st_dev
    assert args.out.is_absolute() and not args.out.is_relative_to(root) and not args.out.exists()
    inputs = read(root / 'inputs.json')
    group = Path(inputs['cgroup']['path'])
    events = (group / 'memory.events').read_text()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
    assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
    info = group.stat()
    assert [info.st_dev, info.st_ino] == inputs['cgroup']['identity']
    assert (group / 'memory.max').read_text().strip() == str(64 << 30)
    assert (group / 'memory.swap.max').read_text().strip() == '0'
    assert all(dict(line.split() for line in events.splitlines())[k] == '0'
               for k in ('oom', 'oom_kill', 'oom_group_kill'))
    checkpoint, client = read(args.checkpoint), read(args.client_readback)
    assert checkpoint['status'] == 'TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
    assert client['status'] == 'CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
    assert client['sha256'] == checkpoint['sha256'] and client['bytes'] == checkpoint['bytes']
    for name in ('primary', 'replica'):
        archive = Path(checkpoint[name])
        assert not archive.is_relative_to(root) and archive.stat().st_size == checkpoint['bytes']
        assert sha(archive) == checkpoint['sha256']
    with gzip.open(checkpoint['primary'], 'rb') as zipped:
        with tarfile.open(fileobj=zipped, mode='r|') as archive:
            member = next(iter(archive))
            assert member.name == 'manifest.json' and member.isfile() and member.size <= 16 << 20
            manifest = json.loads(archive.extractfile(member).read())
    assert manifest['original_root'] == str(root)
    roles = {r['path']: r for r in manifest['roles']}
    assert len(roles) == len(manifest['roles'])
    device = root.stat().st_dev
    pending, seen, actual = list(root.iterdir()), set(), set()
    allocated = logical = 0
    while pending:
        path = pending.pop()
        info = path.lstat()
        assert info.st_dev == device
        name = str(path.relative_to(root))
        actual.add(name)
        row = roles[name]
        assert stat.S_IMODE(info.st_mode) == row['mode']
        key = (info.st_dev, info.st_ino)
        if key not in seen:
            allocated += info.st_blocks * 512
            seen.add(key)
        if stat.S_ISDIR(info.st_mode):
            assert row['type'] == 'directory-metadata'
            pending.extend(path.iterdir())
        elif stat.S_ISREG(info.st_mode):
            assert row['type'] == 'regular'
            assert info.st_size == row['bytes'] and sha(path) == row['sha256']
            logical += info.st_size
        else:
            assert stat.S_ISLNK(info.st_mode) and row['type'] == 'symlink-metadata'
            assert os.readlink(path) == row['target']
    assert actual == set(roles), 'Final checkpoint must cover the entire current tmpfs'
    phases = []
    for path in sorted((root / 'out').glob('doosan-*.json')):
        row = read(path)
        assert row['status'] in ('PASS', 'FAILED_RETAINED')
        assert all(c['exit'] is not None for c in row['commands'])
        phases.append(dict(phase=row['phase'], attempt=row['attempt'], status=row['status']))
    for path, expected in [
        ('out/verify-r1/verification.json', 'INDEPENDENT_DOOSAN_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS'),
        ('out/nativecheck-r1/native-verification.json', 'INDEPENDENT_DOOSAN_SELECTED_ORIGINAL_PACKAGE_BUILDS_VERIFIED'),
        ('out/audit-r1/audit.json', 'DELIVERED_SOURCE_AUDIT_AND_EXPECTED_INCOMPLETE_CI_REFUSAL_PASS'),
        ('out/doosan-registry-r4.json', 'PASS'),
    ]:
        assert read(root / path)['status'] == expected
    active = []
    for process in Path('/proc').iterdir():
        if not process.name.isdecimal() or int(process.name) == os.getpid():
            continue
        try:
            for name in ('cwd', 'root'):
                if (process / name).stat().st_dev == device:
                    active.append(str(process / name))
            for fd in (process / 'fd').iterdir():
                try:
                    if fd.stat().st_dev == device:
                        active.append(str(fd))
                except (FileNotFoundError, ProcessLookupError):
                    pass
            for line in (process / 'maps').read_text().splitlines():
                fields = line.split(maxsplit=5)
                if len(fields) >= 5 and fields[4] != '0':
                    major, minor = (int(s, 16) for s in fields[3].split(':'))
                    if (major, minor) == (os.major(device), os.minor(device)):
                        active.append(str(process / 'maps'))
        except (FileNotFoundError, ProcessLookupError, PermissionError):
            pass
    assert not active, active
    fs = os.statvfs(root)
    used = (fs.f_blocks - fs.f_bfree) * fs.f_frsize
    command = ['/usr/bin/umount', str(root)]
    subprocess.run(command, check=True, timeout=15)
    assert not root.is_mount() and not list(root.iterdir())
    root.rmdir()
    result = dict(status='OWNED_DOOSAN_TMPFS_NORMALLY_UNMOUNTED_AFTER_FULL_RETAINED_CLIENT_READBACK',
                  original_root=str(root), root_exists=root.exists(), commands=[command],
                  tmpfs_data_allocation_before_unmount=used,
                  unique_inode_allocated_bytes_before=allocated, regular_logical_bytes_before=logical,
                  retained_roles=len(actual), unique_inodes=len(seen), phases=phases,
                  checkpoint_sha256=checkpoint['sha256'],
                  persistent_sdk_inputs_preserved=True, borrowed_loop_detached=False,
                  other_roots_or_limits_modified=False, events_before=events,
                  events_after=(group / 'memory.events').read_text())
    args.out.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result), flush=True)


if __name__ == '__main__':
    main()
