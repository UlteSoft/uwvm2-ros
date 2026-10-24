#!/usr/bin/env python3
"""Stream actual ordinary SDK audit inputs; execute no transported source.

Packing is file transport, not a local SDK audit. Receipt output names select
ordinary input files; only the independent Linux audit compares their contents
to generation claims. Receiving requires the owned hard64GiB/swap0 test root.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import resource
import sys
import tarfile


def allowed(name):
    p = PurePosixPath(name)
    return (not p.is_absolute() and '..' not in p.parts and '\\' not in name and
            (name in ('sdk/origin/tools/audit_ros_platform_sdks.py', 'sdk/origin/robots/vendors/inventory-v2.json') or
             len(p.parts) == 4 and p.parts[:2] == ('sdk', 'custom-types')))


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=('pack', 'receive'))
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--root', type=Path)
    args = parser.parse_args()
    source = args.source.absolute()
    if args.mode == 'pack':
        names = {'sdk/origin/tools/audit_ros_platform_sdks.py', 'sdk/origin/robots/vendors/inventory-v2.json'}
        for receipt in (source / 'sdk/origin/custom-types').glob('*/generation.vendor.json'):
            assert receipt.is_file() and not receipt.is_symlink()
            names.add(str(receipt.relative_to(source)))
            for name in json.loads(receipt.read_bytes())['outputs']:
                assert len(PurePosixPath(name).parts) == 1 and name not in ('.', '..')
                names.add(str((receipt.parent / name).relative_to(source)))
        rows = []
        for name in sorted(names):
            assert allowed(name)
            path = source / name
            assert path.is_file() and not path.is_symlink() and path.stat().st_size <= 16 << 20
            rows.append(dict(path=name, bytes=path.stat().st_size, sha256=sha(path)))
        assert len(rows) <= 10000 and sum(r['bytes'] for r in rows) <= 512 << 20
        body = (json.dumps(rows, indent=2) + '\n').encode()
        with tarfile.open(fileobj=sys.stdout.buffer, mode='w|') as archive:
            member = tarfile.TarInfo('source-io.json')
            member.size, member.mode = len(body), 0o644
            archive.addfile(member, io.BytesIO(body))
            for row in rows:
                path = source / row['path']
                assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
                member = tarfile.TarInfo(row['path'])
                member.size, member.mode = row['bytes'], 0o644
                with path.open('rb') as stream:
                    archive.addfile(member, stream)
                assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
    else:
        root = args.root.resolve(strict=True)
        assert root.is_mount() and source.is_relative_to(root / 'out') and not source.exists()
        inputs = json.loads((root / 'inputs.json').read_bytes())
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        group = Path(inputs['cgroup']['path'])
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        resource.setrlimit(resource.RLIMIT_AS, (256 << 20, 256 << 20))
        resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        source.mkdir(parents=True, mode=0o700)
        with tarfile.open(fileobj=sys.stdin.buffer, mode='r|') as archive:
            members = iter(archive)
            member = next(members)
            assert member.name == 'source-io.json' and member.isfile() and member.size <= 8 << 20
            body = archive.extractfile(member).read()
            rows = json.loads(body)
            assert len(rows) <= 10000 and sum(r['bytes'] for r in rows) <= 512 << 20
            expected = {r['path']: r for r in rows}
            assert len(expected) == len(rows) and all(allowed(n) for n in expected)
            seen = set()
            for member in members:
                assert member.name in expected and member.name not in seen and member.isfile() and member.mode == 0o644
                row = expected[member.name]
                assert member.size == row['bytes'] <= 16 << 20
                data = archive.extractfile(member).read((16 << 20) + 1)
                assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
                path = source / row['path']
                path.parent.mkdir(parents=True, exist_ok=True)
                assert not path.exists() and path.parent.resolve() == path.parent
                with path.open('xb') as stream:
                    stream.write(data)
                assert sha(path) == row['sha256']
                seen.add(member.name)
            assert seen == set(expected)
        with (source / 'source-io.json').open('xb') as stream:
            stream.write(body)
        print('ACTUAL_PLATFORM_SOURCE_AUDIT_INPUTS_TRANSPORTED', len(rows), sum(r['bytes'] for r in rows))


if __name__ == '__main__':
    main()
