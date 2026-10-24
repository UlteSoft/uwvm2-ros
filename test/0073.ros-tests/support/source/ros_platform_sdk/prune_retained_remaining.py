#!/usr/bin/env python3
"""Remove selected closed operator output after exact two-copy/client proof.

Only explicit paths below the owned tmpfs's out/ directory are eligible. Each
current object must match the immutable checkpoint manifest before anything
is removed. Source, installed SDKs, tools, cgroup settings and other processes
are never modified. Use this between closed phases to bound scratch storage;
the complete original checkpoint remains available in both retained locations.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import resource
import shutil
import stat
import tarfile


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--checkpoint', type=Path, required=True)
    parser.add_argument('--client-readback', type=Path, required=True)
    parser.add_argument('--include', action='append', required=True)
    parser.add_argument('--receipt', type=Path, required=True)
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_AS, (512 << 20, 512 << 20))
    resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    root = args.root.resolve(strict=True)
    assert root.is_mount() and root.stat().st_dev != root.parent.stat().st_dev
    inputs = json.loads((root / 'inputs.json').read_bytes())
    assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
    assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
    group = Path(inputs['cgroup']['path'])
    info = group.stat()
    assert [info.st_dev, info.st_ino] == inputs['cgroup']['identity']
    assert (group / 'memory.max').read_text().strip() == str(64 << 30)
    assert (group / 'memory.swap.max').read_text().strip() == '0'
    for path in (root / 'out').glob('remaining-*.json'):
        owner = json.loads(path.read_bytes())
        assert owner['status'] in ('PASS', 'FAILED_RETAINED')
        assert all(row['exit'] is not None for row in owner['commands'])
    for path in (root / 'control').glob('batch-*.json'):
        assert json.loads(path.read_bytes())['status'] != 'RUNNING'
    proof = json.loads(args.checkpoint.read_bytes())
    client = json.loads(args.client_readback.read_bytes())
    assert proof['status'] == 'TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
    assert client['status'] == 'CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
    assert proof['sha256'] == client['sha256'] and proof['bytes'] == client['bytes']
    for name in ('primary', 'replica'):
        archive = Path(proof[name])
        assert not archive.is_relative_to(root)
        assert archive.stat().st_size == proof['bytes'] and sha(archive) == proof['sha256']
    with gzip.open(proof['primary'], 'rb') as zipped:
        with tarfile.open(fileobj=zipped, mode='r|') as archive:
            member = next(iter(archive))
            assert member.name == 'manifest.json' and member.isfile() and member.size <= 16 << 20
            manifest = json.loads(archive.extractfile(member).read())
    assert manifest['original_root'] == str(root)
    rows = {row['path']: row for row in manifest['roles']}
    assert len(rows) == len(manifest['roles'])
    targets, selected = [], {}
    for name in args.include:
        relative = PurePosixPath(name)
        assert not relative.is_absolute() and '..' not in relative.parts
        assert len(relative.parts) >= 2 and relative.parts[0] == 'out'
        path = root / relative
        assert path.resolve(strict=True).is_relative_to(root / 'out')
        targets.append(path)
        pending = [path]
        while pending:
            current = pending.pop()
            info = current.lstat()
            assert info.st_dev == root.stat().st_dev
            row = rows[str(current.relative_to(root))]
            assert stat.S_IMODE(info.st_mode) == row['mode']
            if stat.S_ISDIR(info.st_mode):
                assert row['type'] == 'directory-metadata'
                pending.extend(current.iterdir())
            elif stat.S_ISREG(info.st_mode):
                assert row['type'] == 'regular'
                assert info.st_size == row['bytes'] and sha(current) == row['sha256']
            else:
                assert stat.S_ISLNK(info.st_mode) and row['type'] == 'symlink-metadata'
                assert os.readlink(current) == row['target']
            selected[str(current.relative_to(root))] = row
    # Refuse overlapping selections, rather than deleting a parent first and
    # accidentally treating the later missing child as successful cleanup.
    assert len(set(targets)) == len(targets)
    assert not any(a != b and a.is_relative_to(b) for a in targets for b in targets)
    assert args.receipt.parent == root / 'control' and not args.receipt.exists()
    # Every selected role has a quiet, closed owner. Refuse any remaining
    # process in the same group holding an inode through cwd, fd or mmap.
    identities = {(path.stat().st_dev, path.stat().st_ino) for target in targets
                  for path in [target, *target.rglob('*')] if not path.is_symlink()}
    for actor in Path('/proc').iterdir():
        if not actor.name.isdigit():
            continue
        try:
            if (actor / 'cgroup').read_text().strip() != '0::' + inputs['cgroup']['membership']:
                continue
            for handle in [actor / 'cwd', actor / 'root', *list((actor / 'fd').iterdir())]:
                try:
                    info = handle.stat()
                    assert (info.st_dev, info.st_ino) not in identities, ('Live inode holder', str(handle))
                except FileNotFoundError:
                    pass
            for line in (actor / 'maps').read_text().splitlines():
                fields = line.split(None, 5)
                device = [int(v, 16) for v in fields[3].split(':')]
                assert (os.makedev(*device), int(fields[4])) not in identities, ('Live mapping', actor.name)
        except (FileNotFoundError, ProcessLookupError):
            pass
    before = (os.statvfs(root).f_blocks - os.statvfs(root).f_bfree) * os.statvfs(root).f_frsize
    for target in targets:
        if target.is_dir() and not target.is_symlink():
            shutil.rmtree(target)
        else:
            target.unlink()
        assert not target.exists() and not target.is_symlink()
    after = (os.statvfs(root).f_blocks - os.statvfs(root).f_bfree) * os.statvfs(root).f_frsize
    result = dict(status='EXACT_RETAINED_OWNED_OUTPUT_PRUNED_AFTER_TWO_COPY_AND_CLIENT_READBACK',
                  checkpoint_sha256=proof['sha256'], selections=args.include,
                  checked_roles=len(selected), selected_manifest_sha256=hashlib.sha256(
                      json.dumps(selected, sort_keys=True, separators=(',', ':')).encode()).hexdigest(),
                  tmpfs_allocation_released_bytes=before-after,
                  source_sdk_tools_or_other_roots_removed=False)
    args.receipt.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result), flush=True)


if __name__ == '__main__':
    main()
