#!/usr/bin/env python3
"""Retain exact ordinary upstream source without installing or executing it.

Declaration, package, license and small authored integration sources are
selected from the captured original Git trees. Raw HTTPS bodies must match
their tree size and Git blob identity. Models, binaries and symlinks are never
materialized. An identical blob cache saves bandwidth across historic branches;
it conveys no historical execution or compatibility status.
"""
import argparse
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path, PurePosixPath
import tarfile
import urllib.request


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    discovery = Path(inputs['remaining_discovery'])
    metadata = json.loads((discovery / 'discovery.json').read_bytes())
    assert metadata['status'].startswith('BOUNDED_PUBLIC_INVENTORY_')
    spec = importlib.util.spec_from_file_location('remaining_fetch', Path(__file__).with_name('discover_remaining.py'))
    helper = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(helper)
    spec = importlib.util.spec_from_file_location('remaining_capture_git_tree', Path(__file__).with_name('git_source_tree.py'))
    git = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(git)
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', sources=[], default_authority=[], upstream_code_executed=False,
                  source_sdk_generated=False, native_driver_qualified=False)
    cache, total = {}, 0
    objects = out / 'objects'
    objects.mkdir()
    def capture_archive(repository, profile, selected):
        pending = {v['path']: v for v in selected if v['sha'] not in cache}
        if not pending:
            return dict(exact_git_blobs_reused=True, archive_streamed=False)
        url = 'https://codeload.github.com/' + repository + '/tar.gz/' + profile['commit']
        digest = hashlib.sha256()
        compressed = expanded = members = 0
        request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-selected-source-review'})
        with urllib.request.urlopen(request, timeout=30) as response:
            class Counted:
                def read(self, n=-1):
                    nonlocal compressed
                    assert 0 <= n <= 16 << 20
                    body = response.read(n)
                    compressed += len(body)
                    assert compressed <= 1 << 30
                    digest.update(body)
                    return body
            with gzip.GzipFile(fileobj=Counted(), mode='rb') as zipped:
                with tarfile.open(fileobj=zipped, mode='r|') as archive:
                    for member in archive:
                        path = PurePosixPath(member.name)
                        assert not path.is_absolute() and '..' not in path.parts
                        assert path.parts[0] == repository.split('/')[1] + '-' + profile['commit']
                        members += 1
                        expanded += member.size
                        assert members <= 65536 and expanded <= 2 << 30
                        name = '/'.join(path.parts[1:])
                        if name not in pending:
                            continue
                        row = pending.pop(name)
                        assert member.isfile() and member.size == row['size'] <= 2 << 20
                        body = archive.extractfile(member).read((2 << 20) + 1)
                        assert len(body) == row['size']
                        assert hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == row['sha']
                        destination = objects / row['sha']
                        if destination.exists():
                            assert destination.read_bytes() == body
                        else:
                            destination.write_bytes(body)
                        cache[row['sha']] = destination
                while zipped.read(1 << 20):
                    pass
            assert response.read(1) == b''
        assert not pending, sorted(pending)
        return dict(url=url, compressed_bytes=compressed, compressed_sha256=digest.hexdigest(),
                    expanded_member_bytes=expanded, members=members, gzip_eof_verified=True,
                    archive_written_extracted_or_executed=False)
    try:
        for repository in metadata['repositories']:
            for profile in repository['selected']:
                tree = json.loads((discovery / repository['id'] / profile['id'] / 'tree.json').read_bytes())
                assert git.tree_identity(tree) == profile['tree'] and not tree.get('truncated')
                rows = {v['path']: v for v in tree['tree'] if v['type'] == 'blob'}
                selected = []
                for name, row in rows.items():
                    p = PurePosixPath(name)
                    declaration = p.suffix in ('.msg', '.srv', '.action', '.idl', '.proto')
                    metadata_file = p.name in ('package.xml', 'CMakeLists.txt', '.gitmodules', 'VERSION', 'CHANGELOG', 'setup.py', 'setup.cfg', 'pyproject.toml')
                    document = p.suffix.lower() in ('.md', '.rst') or p.name.upper().startswith(('LICENSE', 'COPYING', 'NOTICE', 'README'))
                    integration = p.suffix in ('.py', '.h', '.hpp', '.cpp', '.cc', '.launch', '.yaml', '.yml', '.repos')
                    if not profile['declarations'] and repository['id'] not in ('ubtech', 'fourier'):
                        integration = p.suffix == '.repos'
                    if not (declaration or metadata_file or document or integration):
                        continue
                    if any(part in {'third_party', 'thirdparty', 'vendor', 'venv', '.venv', 'node_modules', 'mujoco', 'eigen3', 'eigen'} for part in p.parts) and not (declaration or metadata_file or document):
                        continue
                    if row['mode'] not in ('100644', '100755'):
                        continue
                    assert not p.is_absolute() and '..' not in p.parts
                    assert row['size'] <= 2 << 20, ('Selected source object exceeds bound', name)
                    selected.append(row)
                assert len(selected) <= 4096 and sum(r['size'] for r in selected) <= 32 << 20
                transport = capture_archive(repository['repository'], profile, selected)
                directory = out / 'upstream' / repository['id'] / profile['id']
                files = directory / 'files'
                files.mkdir(parents=True)
                captured = []
                for row in selected:
                    oid, size, name = row['sha'], row['size'], row['path']
                    body = cache[oid].read_bytes()
                    assert len(body) == size
                    assert hashlib.sha1(b'blob ' + str(size).encode() + b'\0' + body).hexdigest() == oid
                    destination = files / name
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    destination.write_bytes(body)
                    cache[oid] = destination
                    total += size
                    assert total <= 192 << 20
                    captured.append(dict(path=name, bytes=size, git_blob=oid, original_mode=row['mode'],
                                         sha256=hashlib.sha256(body).hexdigest()))
                lock = dict(repository=repository['repository'], id=profile['id'], branch=profile['branch'],
                            commit=profile['commit'], tree=profile['tree'], files=captured,
                            upstream_code_executed=False, transport=transport, default_authority=[])
                (directory / 'source-lock.json').write_text(json.dumps(lock, indent=2) + '\n')
                report['sources'].append(dict(vendor=repository['id'], profile=profile['id'], files=len(captured),
                                              bytes=sum(v['bytes'] for v in captured), unique_blobs=len(cache)))
                print(json.dumps(report['sources'][-1]), flush=True)
                (out / 'capture.json').write_text(json.dumps(report, indent=2) + '\n')
        report['status'] = 'EXACT_SELECTED_ORDINARY_SOURCES_CAPTURED_NO_INSTALLER_OR_NATIVE_EXECUTION'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'capture.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
