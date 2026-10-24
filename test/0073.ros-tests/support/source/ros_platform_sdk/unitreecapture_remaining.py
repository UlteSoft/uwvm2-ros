#!/usr/bin/env python3
"""Capture pinned Unitree example trees and ordinary sources without execution.

The four existing ROS/SDK locks are authoritative. A recursive tree must be
complete and reconstruct its pinned Git root before any source is admitted.
Only ordinary, small example/build/header/Python sources are retained. Robot
models, vendor executables, libraries and installers are never materialized.
"""
import argparse
import base64
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path, PurePosixPath
import re
import tarfile
import time
import urllib.error
import urllib.request


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    out = args.out
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    source = Path(json.loads((root / 'inputs.json').read_bytes())['source_root'])
    lock = json.loads((source / 'sdk/origin/robots/unitree/upstream-lock.json').read_bytes())
    spec = importlib.util.spec_from_file_location('unitree_git_tree', Path(__file__).with_name('git_source_tree.py'))
    git = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(git)
    selected = {'unitree_ros', 'unitree_ros2', 'unitree_sdk2', 'unitree_sdk2_python'}
    report = dict(status='RUNNING', repositories=[], default_authority=[],
                  upstream_code_executed=False, official_examples_ported=0,
                  native_driver_qualified=False)
    total = 0

    def save():
        (out / 'unitreecapture.json').write_text(json.dumps(report, indent=2) + '\n')

    def fetch(url, maximum):
        for attempt in range(3):
            try:
                request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-pinned-example-review'})
                with urllib.request.urlopen(request, timeout=30) as response:
                    assert response.geturl().startswith('https://api.github.com/')
                    body = response.read(maximum + 1)
                    assert len(body) <= maximum
                    return body
            except (urllib.error.URLError, TimeoutError, ConnectionError):
                if attempt == 2:
                    raise
                time.sleep(.5 * (attempt + 1))

    def capture(item, directory, rows):
        """Stream one pinned archive; only tree-verified source bytes are kept."""
        url = 'https://codeload.github.com/' + item['repository'] + '/tar.gz/' + item['commit']
        pending = {r['path']: r for r in rows}
        bodies = {}
        digest = hashlib.sha256()
        compressed = expanded = members = 0
        request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-pinned-example-review'})
        with urllib.request.urlopen(request, timeout=30) as response:
            assert response.geturl().startswith('https://codeload.github.com/')

            class Counted:
                def read(self, n):
                    nonlocal compressed
                    assert 0 <= n <= 16 << 20
                    data = response.read(n)
                    compressed += len(data)
                    assert compressed <= 1 << 30
                    digest.update(data)
                    return data

            with gzip.GzipFile(fileobj=Counted(), mode='rb') as zipped:
                with tarfile.open(fileobj=zipped, mode='r|') as archive:
                    for member in archive:
                        path = PurePosixPath(member.name)
                        assert not path.is_absolute() and '..' not in path.parts
                        assert path.parts[0] == item['repository'].split('/')[1] + '-' + item['commit']
                        expanded += member.size
                        members += 1
                        assert expanded <= 2 << 30 and members <= 65536
                        name = '/'.join(path.parts[1:])
                        if name not in pending:
                            continue
                        row = pending.pop(name)
                        assert member.isfile() and member.size == row['size'] <= 2 << 20
                        data = archive.extractfile(member).read((2 << 20) + 1)
                        assert len(data) == row['size']
                        assert hashlib.sha1(b'blob ' + str(len(data)).encode() + b'\0' + data).hexdigest() == row['sha']
                        bodies[name] = data
                while zipped.read(1 << 20):
                    pass
            assert response.read(1) == b''
        assert not pending, sorted(pending)
        return bodies, dict(url=url, compressed_bytes=compressed, compressed_sha256=digest.hexdigest(),
                            expanded_member_bytes=expanded, members=members, gzip_eof_verified=True,
                            archive_written_or_extracted=False)

    try:
        for item in lock['repositories']:
            name = item['repository'].split('/')[1]
            if name not in selected:
                continue
            directory = out / name
            directory.mkdir()
            url = 'https://api.github.com/repos/' + item['repository'] + '/git/trees/' + item['commit'] + '?recursive=1'
            body = fetch(url, 12 << 20)
            tree = json.loads(body)
            assert git.tree_identity(tree) == item['tree']
            (directory / 'tree.json').write_bytes(body)
            result = dict(repository=item['repository'], commit=item['commit'], tree=item['tree'],
                          tree_bytes=len(body), tree_sha256=hashlib.sha256(body).hexdigest(),
                          complete_tree_verified=True, files=[])
            report['repositories'].append(result)
            rows = []
            for row in tree['tree']:
                path = PurePosixPath(row['path'])
                # Retain the complete ROS2 example source package and legacy
                # application sources. SDK2 Go2 declarations clarify the exact
                # RPC payloads; they do not establish firmware acceptance.
                example = name == 'unitree_ros2' and ('example' in path.parts or 'examples' in path.parts)
                sport = ('go2' in path.parts and 'sport' in str(path) and
                         name in ('unitree_sdk2', 'unitree_sdk2_python'))
                metadata = path.name.upper().startswith(('LICENSE', 'COPYING', 'NOTICE')) and len(path.parts) == 1
                if not (metadata or (example or sport) and
                        (path.suffix in ('.c', '.cpp', '.cc', '.h', '.hpp', '.py', '.md', '.msg', '.srv', '.action', '.xml') or
                         path.name == 'CMakeLists.txt')):
                    continue
                if row['type'] != 'blob' or row['mode'] not in ('100644', '100755'):
                    continue
                assert 0 <= row['size'] <= 2 << 20
                rows.append(row)
            assert len(rows) <= 256 and sum(r['size'] for r in rows) <= 12 << 20
            if name == 'unitree_ros2':
                bodies, result['transport'] = capture(item, directory, rows)
            else:
                # Legacy/model repositories carry large meshes and native
                # binaries. Their complete trees remain reviewable, but only
                # license/Sport dependencies are needed for this new lane.
                # A few exact Git objects avoid downloading those assets.
                bodies = {}
                for row in rows:
                    url = 'https://api.github.com/repos/' + item['repository'] + '/git/blobs/' + row['sha']
                    blob = json.loads(fetch(url, 4 << 20))
                    assert blob['encoding'] == 'base64' and blob['size'] == row['size']
                    data = base64.b64decode(blob['content'])
                    assert len(data) == row['size']
                    assert hashlib.sha1(b'blob ' + str(len(data)).encode() + b'\0' + data).hexdigest() == row['sha']
                    bodies[row['path']] = data
                result['transport'] = dict(exact_individual_git_blobs=len(bodies), archive_written_or_extracted=False)
            for row in rows:
                path = PurePosixPath(row['path'])
                url = 'https://raw.githubusercontent.com/' + item['repository'] + '/' + item['commit'] + '/' + str(path)
                data = bodies[row['path']]
                total += len(data)
                assert total <= 24 << 20 and len(result['files']) < 256
                target = directory / 'files' / Path(*path.parts)
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(data)
                # A textual entry signature is only a source-review candidate;
                # CMake/source inspection must decide actual runnable targets.
                signature = (re.search(rb'\bint\s+main\s*\(', data) is not None if path.suffix in ('.cpp', '.cc', '.c')
                             else b'__name__' in data and b'__main__' in data if path.suffix == '.py' else False)
                result['files'].append(dict(path=str(path), bytes=len(data), sha256=hashlib.sha256(data).hexdigest(),
                                           git_blob=row['sha'], source_url=url, entry_signature_candidate=signature))
                save()
            print(name, len(result['files']), flush=True)
        assert len(report['repositories']) == 4
        report.update(status='FOUR_COMPLETE_PINNED_TREES_AND_SELECTED_ORIGINAL_EXAMPLE_SOURCES_CAPTURED',
                      selected_source_scope='Complete ROS2 example package, SDK2 Sport dependencies and root licenses; other tree entries remain inventory only',
                      ordinary_source_bytes=total)
        print(report['status'], total, flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
