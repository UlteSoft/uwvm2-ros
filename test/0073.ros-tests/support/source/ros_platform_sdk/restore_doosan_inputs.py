#!/usr/bin/env python3
"""Restore explicitly selected ordinary frozen SDK test inputs; execute none.

Only known role prefixes from closed r56 evidence are admitted. Archive paths
are metadata, never extraction targets. Original kernel symlinks are recorded
as omitted: cross tests use the actual per-architecture UAPI directories. The
Runtime executable has an additional independent fixed digest admission.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path, PurePosixPath
import resource
import tarfile


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
    group = Path(inputs['cgroup']['path'])
    assert (group / 'memory.max').read_text().strip() == str(64 << 30)
    assert (group / 'memory.swap.max').read_text().strip() == '0'
    resource.setrlimit(resource.RLIMIT_AS, (512 << 20, 512 << 20))
    resource.setrlimit(resource.RLIMIT_CPU, (120, 120))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    parent = root.parents[1] / 'recovery/ros-flexiv-r56'
    roles = []
    omitted = []
    for label in ('baseline-r2', 'kernel-r1', 'schema-r1'):
        archive_path = parent / (label + '.tar.gz')
        receipt = json.loads((parent / (label + '.json')).read_bytes())
        assert archive_path.stat().st_size == receipt['bytes']
        assert digest(archive_path) == receipt['sha256']
        with archive_path.open('rb') as raw, gzip.GzipFile(fileobj=raw, mode='rb') as zipped:
            with tarfile.open(fileobj=zipped, mode='r|') as archive:
                members = iter(archive)
                first = next(members)
                assert first.name == 'manifest.json' and first.isfile() and first.size <= 16 << 20
                manifest = json.loads(archive.extractfile(first).read())
                wanted = {}
                for row in manifest['roles']:
                    path = row['path']
                    if label == 'baseline-r2':
                        selected = path == 'tools/uwvm2-ros' or path.startswith('tools/quickjs-original/')
                        target = path
                    elif label == 'kernel-r1':
                        prefix = 'out/kernel-r1/rootfs/'
                        selected = path.startswith(prefix)
                        target = 'tools/kernel-rootfs/' + path.removeprefix(prefix)
                    else:
                        selected = path.startswith('tools/ros-schema-xsd/')
                        target = path
                    if not selected or row['type'] == 'directory-metadata':
                        continue
                    if row['type'] == 'symlink-metadata':
                        assert label == 'kernel-r1'
                        omitted.append(row)
                        continue
                    assert row['type'] == 'regular'
                    p = PurePosixPath(target)
                    assert not p.is_absolute() and '..' not in p.parts
                    wanted.setdefault(row['object'], []).append((row, target))
                pending = set(wanted)
                for member in members:
                    if member.name not in wanted:
                        continue
                    assert member.name in pending and member.isfile()
                    row = wanted[member.name][0][0]
                    assert member.size == row['bytes'] <= 32 << 20 and member.mode == row['mode']
                    body = archive.extractfile(member).read((32 << 20) + 1)
                    assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
                    for original, target in wanted[member.name]:
                        destination = root / target
                        destination.parent.mkdir(parents=True, exist_ok=True)
                        assert not destination.exists() and not destination.is_symlink()
                        with destination.open('xb') as output:
                            output.write(body)
                        destination.chmod(0o755 if target == 'tools/uwvm2-ros' else 0o644)
                        roles.append(dict(original=original, target=target, checkpoint=label,
                                          checkpoint_sha256=receipt['sha256']))
                    pending.remove(member.name)
                assert not pending
            while zipped.read(1 << 20):
                pass
    assert digest(root / 'tools/uwvm2-ros') == '719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19'
    (root / 'control/restored-inputs.json').write_text(json.dumps(dict(
        regular_roles=roles, omitted_symlink_metadata=omitted,
        archive_extraction_or_execution=False, default_authority=[]), indent=2) + '\n')
    print('EXPLICIT_FROZEN_INPUT_ROLES_RESTORED', len(roles), len(omitted))


if __name__ == '__main__':
    main()
