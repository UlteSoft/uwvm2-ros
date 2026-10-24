#!/usr/bin/env python3
"""Read exact Leju source blobs through explicitly selected bounded transport.

This is trusted operator authoring, not a Guest update/import facility. No
working tree is checked out and no upstream hook, filter, installer, build,
controller or simulator is executed. Git configuration is private, credentials
are disabled, submodules/tags are not fetched, and captured file bodies must
match both the original Git blob ID and the reviewed API size. Static codeload
mode streams a fixed-commit archive, copies only checked ordinary source, and
verifies gzip EOF without materializing unrelated payloads. Installed and
vendored definitions remain distinguishable in the output; capture alone does
not select an ROS schema, generate an SDK, or grant any ROS authority.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import subprocess
import tarfile
import urllib.request
import xml.etree.ElementTree as ET


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--transport', choices=['git', 'codeload'], default='git')
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    discovery = Path(inputs['discovery'])
    reviewed = json.loads((discovery / 'discovery.json').read_bytes())
    assert reviewed['repository'] == 'LejuRobotics/kuavo-ros-opensource'
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    mirror = root / 'mirror/leju.git'
    if args.transport == 'git':
        assert not mirror.exists(), 'A fresh mirror is required for this Git capture attempt'
    report = dict(status='RUNNING', repository=reviewed['repository'], commands=[],
                  profiles=[], source_transports=[], transport=args.transport,
                  default_authority=[], upstream_code_executed=False,
                  source_sdk_generated=False, native_driver_qualified=False)
    git = Path('/usr/bin/git')
    flags = ['-c', 'core.hooksPath=/dev/null', '-c', 'credential.helper=',
             '-c', 'protocol.version=2', '-c', 'gc.auto=0', '-c', 'maintenance.auto=false',
             '-c', 'pack.threads=1', '-c', 'fetch.writeCommitGraph=false']
    environment = dict(os.environ, GIT_CONFIG_NOSYSTEM='1', GIT_CONFIG_GLOBAL='/dev/null',
                       GIT_TERMINAL_PROMPT='0', GIT_NO_REPLACE_OBJECTS='1',
                       GIT_ASKPASS='/bin/false', SSH_ASKPASS='/bin/false')
    def save():
        (out / 'capture.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, arguments, *, data=None, deadline=300):
        directory = out / 'commands' / label
        directory.mkdir()
        argv = [str(git), *flags, *map(str, arguments)]
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            process = subprocess.run(argv, input=data, env=environment, stdout=stdout,
                                     stderr=stderr, check=False, timeout=deadline)
        report['commands'].append(dict(label=label, argv=argv, exit=process.returncode,
                                       stdout_sha256=hashlib.sha256((directory / 'stdout').read_bytes()).hexdigest(),
                                       stderr_sha256=hashlib.sha256((directory / 'stderr').read_bytes()).hexdigest()))
        save()
        assert process.returncode == 0, (label, (directory / 'stderr').read_text()[-2500:])
        return (directory / 'stdout').read_bytes()
    cache, fetched = {}, set()
    def blob(row):
        oid, size = row['sha'], row['size']
        assert re.fullmatch(r'[0-9a-f]{40}', oid) and type(size) is int and 0 <= size <= 2 << 20
        if oid not in cache:
            # cat-file without --filters returns the unchanged Git body. It
            # never applies checkout/smudge filters or evaluates file contents.
            value = run('blob-' + oid, ['--git-dir=' + str(mirror), 'cat-file', 'blob', oid])
            assert len(value) == size
            assert hashlib.sha1(b'blob ' + str(size).encode() + b'\0' + value).hexdigest() == oid
            cache[oid] = value
        assert len(cache[oid]) == size
        return cache[oid]
    def archive_blobs(profile, rows, selected):
        pending = {name for name in selected if rows[name]['sha'] not in cache}
        url = 'https://codeload.github.com/' + reviewed['repository'] + '/tar.gz/' + profile['commit']
        compressed = expanded = members = 0
        digest = hashlib.sha256()
        if pending:
            request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-source-review'})
            with urllib.request.urlopen(request, timeout=30) as response:
                class Counted:
                    def read(self, n=-1):
                        nonlocal compressed
                        assert 0 <= n <= 16 << 20
                        body = response.read(n)
                        compressed += len(body)
                        assert compressed <= 4 << 30
                        digest.update(body)
                        return body
                with gzip.GzipFile(fileobj=Counted(), mode='rb') as zipped:
                    with tarfile.open(fileobj=zipped, mode='r|') as stream:
                        for member in stream:
                            path = PurePosixPath(member.name)
                            assert not path.is_absolute() and '..' not in path.parts
                            assert path.parts[0] == 'kuavo-ros-opensource-' + profile['commit']
                            members += 1
                            expanded += member.size
                            assert members <= 65536 and expanded <= 8 << 30
                            name = '/'.join(path.parts[1:])
                            if name not in pending:
                                continue
                            row = rows[name]
                            assert member.isfile() and member.size == row['size'] <= 2 << 20
                            body = stream.extractfile(member).read((2 << 20) + 1)
                            assert len(body) == row['size']
                            assert hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == row['sha']
                            cache[row['sha']] = body
                            pending.remove(name)
                    # Verify the gzip trailer and full EOF, even after the
                    # tar end marker and all selected source blobs were found.
                    while zipped.read(1 << 20):
                        pass
                assert response.read(1) == b''
        assert not pending, sorted(pending)
        report['source_transports'].append(dict(branch=profile['branch'], url=url,
            streamed_compressed_bytes=compressed, streamed_compressed_sha256=digest.hexdigest() if compressed else None,
            expanded_member_bytes=expanded, members=members, gzip_eof_verified=bool(compressed),
            exact_cached_blobs_reused_without_new_archive=not compressed,
            compressed_limit_bytes=4 << 30, expanded_limit_bytes=8 << 30,
            archive_written_or_extracted_or_executed=False))
        save()
    try:
        if args.transport == 'git':
            run('init-private-bare-mirror', ['init', '--bare', str(mirror)])
            run('add-explicit-public-origin', ['--git-dir=' + str(mirror), 'remote', 'add', 'origin',
                                              'https://github.com/LejuRobotics/kuavo-ros-opensource.git'])
        for profile in reviewed['branches']:
            name, commit = profile['id'], profile['commit']
            assert re.fullmatch(r'[a-z0-9_]+', name) and re.fullmatch(r'[0-9a-f]{40}', commit)
            if args.transport == 'git' and commit not in fetched:
                run('fetch-' + name, ['--git-dir=' + str(mirror), 'fetch', '--quiet', '--depth=1',
                    '--filter=blob:limit=131072', '--no-tags', '--no-recurse-submodules',
                    '--no-write-fetch-head', 'origin', commit], deadline=600)
                fetched.add(commit)
            if args.transport == 'git':
                actual = run('check-tree-' + name, ['--git-dir=' + str(mirror), 'rev-parse', '--verify', commit + '^{tree}'])
                assert actual.decode().strip() == profile['tree']
                run('retain-private-review-ref-' + name, ['--git-dir=' + str(mirror), 'update-ref',
                                                         'refs/uwvm-source-review/' + name, commit])
            original = json.loads((discovery / name / 'tree.json').read_bytes())
            assert original['sha'] == profile['tree'] and not original.get('truncated')
            rows = {row['path']: row for row in original['tree'] if row['type'] == 'blob'}
            selected = {row['path'] for row in profile['declarations']}
            # Package metadata is data: it establishes actual names, duplicate
            # copies and dependencies before a later explicit catalog choice.
            selected.update(row['path'] for row in profile['packages'])
            roots = {str(PurePosixPath(row['path']).parent) for row in profile['packages']}
            for path in rows:
                p = PurePosixPath(path)
                if str(p.parent) in roots and p.name in ('CMakeLists.txt', 'CATKIN_IGNORE', 'README.md', 'README', 'readme.md'):
                    selected.add(path)
                if p.name.upper().startswith(('LICENSE', 'COPYING', 'NOTICE')):
                    selected.add(path)
                if path.startswith('src/kuavo_humanoid_sdk/') and (p.suffix in ('.py', '.md', '.toml') or p.name in ('py.typed', 'requirements.txt')):
                    selected.add(path)
                if path in ('readme.md', '.gitmodules', 'CHANGELOG.md', 'robot_setup.launch'):
                    selected.add(path)
            assert 1 <= len(selected) <= 4096
            assert sum(rows[n]['size'] for n in selected) <= 32 << 20
            assert all(rows[n]['mode'] in ('100644', '100755') and rows[n]['size'] <= 2 << 20 for n in selected)
            if args.transport == 'codeload':
                archive_blobs(profile, rows, selected)
                fetched.add(commit)
            directory = out / 'upstream' / name / 'files'
            directory.mkdir(parents=True)
            files = []
            for path in sorted(selected):
                relative = PurePosixPath(path)
                assert not relative.is_absolute() and '..' not in relative.parts and '\\' not in path
                row = rows[path]
                assert row['mode'] in ('100644', '100755'), (path, row['mode'])
                body = blob(row)
                target = directory / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(body)
                files.append(dict(path=path, bytes=len(body), git_blob=row['sha'], sha256=hashlib.sha256(body).hexdigest()))
            packages = []
            for row in profile['packages']:
                path = directory / row['path']
                try:
                    package = ET.fromstring(path.read_bytes())
                except ET.ParseError as error:
                    # Capture does not rewrite or discard an invalid original
                    # package. A later selected schema must explicitly resolve
                    # it before claiming a package name or native compatibility.
                    packages.append(dict(path=row['path'], original_xml_parse_error=str(error)))
                    continue
                packages.append(dict(path=row['path'], name=package.findtext('name'),
                    version=package.findtext('version'), buildtools=[p.text for p in package.findall('buildtool_depend')],
                    dependencies=[dict(kind=p.tag, name=p.text) for p in package if p.tag.endswith('depend')]))
            domain = dict(branch=profile['branch'], commit=commit, tree=profile['tree'], files=files,
                          packages=packages, original_declarations=profile['declarations'], ros_version=None,
                          source_catalog_selected=False, native_driver_qualified=False)
            (directory.parent / 'source-lock.json').write_text(json.dumps(domain, indent=2) + '\n')
            report['profiles'].append(dict(id=name, branch=profile['branch'], commit=commit,
                                           captured_files=len(files), original_declarations=len(profile['declarations'])))
            save()
        report.update(status='EXACT_SELECTED_LEJU_ORIGINAL_BLOBS_CAPTURED_NO_UPSTREAM_CODE_EXECUTED',
                      unique_original_blobs=len(cache), unique_commits=len(fetched),
                      private_mirror_regular_file_bytes=sum(p.stat().st_size for p in mirror.rglob('*') if p.is_file()))
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
