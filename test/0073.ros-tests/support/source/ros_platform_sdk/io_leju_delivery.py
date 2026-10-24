#!/usr/bin/env python3
"""Transport ordinary Leju SDK source with complete byte correspondence.

Export runs inside the bounded Linux cgroup. Receive is inert source delivery,
not a local SDK/compiler/Runtime test: it checks paths, modes, sizes and hashes,
and executes no transported content. Unknown existing local bytes are refused.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import resource
import shutil
import sys
import tarfile


def allowed(name):
    p = PurePosixPath(name)
    return (not p.is_absolute() and '..' not in p.parts and '\\' not in name and
            (name.startswith('sdk/origin/robots/humanoids/leju/') or
             len(p.parts) == 4 and p.parts[:2] == ('sdk', 'custom-types') and
             p.parts[2].startswith('leju-') and p.parts[2].endswith('-complete')))


def sha(path):
    # The inert client may use Python 3.9, before hashlib.file_digest existed.
    # Keep the transport reader portable with a finite one-MiB content buffer.
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        while block := stream.read(1 << 20):
            digest.update(block)
    return digest.hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('mode', choices=('export', 'receive'))
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--receipt', type=Path, required=True)
    a = p.parse_args()
    source = a.source.resolve(strict=True)
    if a.mode == 'export':
        resource.setrlimit(resource.RLIMIT_AS, (256 << 20, 256 << 20))
        resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        owned = source.parent
        inputs = json.loads((owned / 'inputs.json').read_bytes())
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        group = Path(inputs['cgroup']['path'])
        assert [group.stat().st_dev, group.stat().st_ino] == inputs['cgroup']['identity']
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        selected = [source / 'sdk/origin/robots/humanoids/leju',
                    *sorted((source / 'sdk/origin/custom-types').glob('leju-*-complete'))]
        rows = []
        for root in selected:
            for path in sorted(root.rglob('*')):
                assert not path.is_symlink()
                if not path.is_file() or path.name == 'Cargo.lock':
                    continue
                name = str(path.relative_to(source))
                assert allowed(name) and path.stat().st_size <= 16 << 20
                rows.append(dict(path=name, bytes=path.stat().st_size, sha256=sha(path)))
        assert len(rows) <= 10000 and sum(r['bytes'] for r in rows) <= 128 << 20
        manifest = dict(kind='leju-ordinary-source-transport-v1', files=rows,
                        logical_bytes=sum(r['bytes'] for r in rows), execution=False)
        body = (json.dumps(manifest, indent=2) + '\n').encode()
        with tarfile.open(fileobj=sys.stdout.buffer, mode='w|') as archive:
            header = tarfile.TarInfo('manifest.json')
            header.size, header.mode = len(body), 0o644
            archive.addfile(header, io.BytesIO(body))
            for row in rows:
                path = source / row['path']
                assert sha(path) == row['sha256']
                header = tarfile.TarInfo(row['path'])
                header.size, header.mode = row['bytes'], 0o644
                with path.open('rb') as stream:
                    archive.addfile(header, stream)
        with a.receipt.open('x') as stream:
            stream.write(body.decode())
    else:
        with tarfile.open(fileobj=sys.stdin.buffer, mode='r|') as archive:
            members = iter(archive)
            first = next(members)
            assert first.name == 'manifest.json' and first.isfile() and first.size <= 8 << 20
            body = archive.extractfile(first).read()
            manifest = json.loads(body)
            assert manifest['kind'] == 'leju-ordinary-source-transport-v1'
            rows = manifest['files']
            assert len(rows) <= 10000 and sum(r['bytes'] for r in rows) == manifest['logical_bytes'] <= 128 << 20
            assert shutil.disk_usage(source).free >= manifest['logical_bytes'] + (128 << 20), 'source-delivery disk margin'
            expected = {r['path']: r for r in rows}
            assert len(expected) == len(rows) and all(allowed(n) for n in expected)
            seen = set()
            for member in members:
                assert member.name in expected and member.name not in seen and member.isfile() and member.mode == 0o644
                row = expected[member.name]
                assert member.size == row['bytes'] <= 16 << 20
                data = archive.extractfile(member).read((16 << 20) + 1)
                assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
                path = source / member.name
                path.parent.mkdir(parents=True, exist_ok=True)
                assert path.parent.resolve() == path.parent and not path.is_symlink()
                if path.exists():
                    assert path.is_file() and path.read_bytes() == data, path
                else:
                    with path.open('xb') as stream:
                        stream.write(data)
                assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
                seen.add(member.name)
            assert seen == set(expected)
        with a.receipt.open('x') as stream:
            stream.write(body.decode())
        print('ORDINARY_SOURCE_DELIVERY_HASHES_MATCH', len(rows), manifest['logical_bytes'])


if __name__ == '__main__':
    main()
