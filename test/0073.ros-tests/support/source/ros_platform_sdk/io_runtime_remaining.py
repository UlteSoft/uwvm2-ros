#!/usr/bin/env python3
"""Stream one consistent ordinary interpreter-source snapshot without local builds.

Packing only reads/hashes maintained source files. Receiving verifies the
original Linux cgroup and installs fresh read-only compiler inputs below the
owned tmpfs. No source, compiler or Runtime is executed by either mode.
Concurrent input changes, symlinks, unknown members and excessive extents fail.
The snapshot excludes SDKs, tests, LLVM and OpenSSL dependency trees.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import sys
import tarfile

DIRECTORIES = ('src', 'third-parties/fast_io/include',
               'third-parties/bizwen/include', 'third-parties/boost_unordered/include')


def allowed(name):
    path = PurePosixPath(name)
    return (not path.is_absolute() and '..' not in path.parts and '\\' not in name and
            any(name.startswith(directory + '/') for directory in DIRECTORIES))


def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        while block := stream.read(1 << 20):
            digest.update(block)
    return digest.hexdigest()


def identity(path):
    value = path.stat()
    return value.st_dev, value.st_ino, value.st_size, value.st_mtime_ns, value.st_ctime_ns


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=('pack', 'receive'))
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--root', type=Path)
    args = parser.parse_args()
    source = args.source.absolute()
    if args.mode == 'pack':
        def paths():
            result = []
            for directory in DIRECTORIES:
                for path in (source / directory).rglob('*'):
                    assert not path.is_symlink()
                    if path.is_file():
                        result.append(path)
            return sorted(result)
        selected = paths()
        births = {path: identity(path) for path in selected}
        rows = []
        for path in selected:
            name = str(path.relative_to(source))
            assert allowed(name) and path.stat().st_size <= 16 << 20
            rows.append(dict(path=name, bytes=path.stat().st_size, sha256=sha(path)))
        assert len(rows) <= 10000 and sum(row['bytes'] for row in rows) <= 128 << 20
        body = (json.dumps(rows, indent=2) + '\n').encode()
        with tarfile.open(fileobj=sys.stdout.buffer, mode='w|') as archive:
            member = tarfile.TarInfo('source-io.json')
            member.size, member.mode = len(body), 0o400
            archive.addfile(member, io.BytesIO(body))
            for row in rows:
                path = source / row['path']
                assert identity(path) == births[path] and sha(path) == row['sha256']
                member = tarfile.TarInfo(row['path'])
                member.size, member.mode = row['bytes'], 0o400
                with path.open('rb') as stream:
                    archive.addfile(member, stream)
                assert identity(path) == births[path] and sha(path) == row['sha256']
            assert paths() == selected and all(identity(path) == births[path] for path in selected)
    else:
        root = args.root.resolve(strict=True)
        inputs = json.loads((root / 'inputs.json').read_bytes())
        assert root.is_mount() and source.is_relative_to(root / 'out') and not source.exists()
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        group = Path(inputs['cgroup']['path'])
        assert [group.stat().st_dev, group.stat().st_ino] == inputs['cgroup']['identity']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        source.mkdir(mode=0o700)
        with tarfile.open(fileobj=sys.stdin.buffer, mode='r|') as archive:
            members = iter(archive)
            first = next(members)
            assert first.name == 'source-io.json' and first.isfile() and first.size <= 4 << 20
            body = archive.extractfile(first).read()
            rows = json.loads(body)
            assert len(rows) <= 10000 and sum(row['bytes'] for row in rows) <= 128 << 20
            expected, seen = {row['path']: row for row in rows}, set()
            assert len(expected) == len(rows) and all(allowed(name) for name in expected)
            for member in members:
                assert member.isfile() and member.mode == 0o400 and member.name in expected and member.name not in seen
                row = expected[member.name]
                assert member.size == row['bytes'] <= 16 << 20
                data = archive.extractfile(member).read((16 << 20) + 1)
                assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
                path = source / member.name
                path.parent.mkdir(parents=True, exist_ok=True)
                assert path.parent.resolve() == path.parent and not path.exists()
                with path.open('xb') as stream:
                    stream.write(data)
                path.chmod(0o400)
                seen.add(member.name)
            assert seen == set(expected)
        (source / 'source-io.json').write_bytes(body)
        (source / 'source-io.json').chmod(0o400)
        print('ORDINARY_CURRENT_INTERPRETER_SOURCE_FROZEN', len(rows), sum(row['bytes'] for row in rows))


if __name__ == '__main__':
    main()
