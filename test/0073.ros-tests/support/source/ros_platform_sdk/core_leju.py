#!/usr/bin/env python3
"""Freeze explicit ROS1 dependency declarations for Leju source preparation.

Installed declarations retain their actual Noetic SDK/dpkg/file provenance;
they are never assigned invented Git commits. The absent geographic_msgs input
comes from an explicitly pinned official geographic_info/master source tree.
Only inert declarations/build metadata/licenses are read. No original setup,
build, controller, import hook or installer executes, and no rights are granted.
"""
import argparse
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path, PurePosixPath
import re
import tarfile
import urllib.request
import xml.etree.ElementTree as ET


def pin(path):
    body = path.read_bytes()
    return dict(path=str(path), bytes=len(body), sha256=hashlib.sha256(body).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    sdk = Path(inputs['native_dependency_root']).resolve(strict=True)
    manifest_path = Path(inputs['native_dependency_manifest']).resolve(strict=True)
    manifest_pin = pin(manifest_path)
    manifest_rows = json.loads(manifest_path.read_bytes())
    manifest = {row['path']: row for row in manifest_rows}
    assert len(manifest) == len(manifest_rows)
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    source = Path(inputs['source_root'])
    spec = importlib.util.spec_from_file_location('leju_public_metadata_reader', source / 'test/0073.ros-tests/support/source/ros_platform_sdk/discover_leju.py')
    reader = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(reader)
    identities = [pin(sdk / name) for name in ('etc/os-release', 'var/lib/dpkg/status', 'opt/ros/noetic/setup.sh') if (sdk / name).is_file()]
    assert (sdk / 'etc/os-release').is_file() and (sdk / 'opt/ros/noetic/setup.sh').is_file()
    packages, files = [], []
    registry = sdk / 'var/lib/dpkg/status'
    status_text = registry.read_text() if registry.is_file() else None
    for name in ('std_msgs', 'geometry_msgs', 'nav_msgs', 'sensor_msgs', 'trajectory_msgs', 'std_srvs', 'actionlib_msgs', 'diagnostic_msgs'):
        original = sdk / 'opt/ros/noetic/share' / name
        package = ET.fromstring((original / 'package.xml').read_bytes())
        assert package.findtext('name') == name
        debian_name = 'ros-noetic-' + name.replace('_', '-')
        paragraphs = [p for p in status_text.split('\n\n') if ('Package: ' + debian_name + '\n') in p + '\n'] if status_text is not None else []
        assert len(paragraphs) <= 1
        version = re.search(r'^Version: (.+)$', paragraphs[0], re.MULTILINE) if paragraphs else None
        selected = [(original / 'package.xml')]
        for kind in ('msg', 'srv', 'action'):
            selected += sorted((original / kind).glob('*.' + kind))
        for path in selected:
            assert path.is_file() and not path.is_symlink() and path.stat().st_size <= 2 << 20
            before = pin(path)
            expected = manifest[str(path.relative_to(sdk))]
            assert expected['type'] == 'regular' and expected['bytes'] == before['bytes']
            assert expected['sha256'] == before['sha256']
            body = path.read_bytes()
            assert pin(path) == before
            target = out / 'installed/files' / name / path.relative_to(original)
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(body)
            files.append(dict(path=str(target.relative_to(out / 'installed/files')), bytes=len(body),
                              sha256=hashlib.sha256(body).hexdigest(), installed_source=before,
                              original_git_commit=None))
        packages.append(dict(name=name, xml_version=package.findtext('version'),
                             debian_package_candidate=debian_name, debian_version=version[1] if version else None,
                             debian_registry_available=status_text is not None,
                             origin_note='Exact SDK files and package.xml version; a missing dpkg registry is not reconstructed or inferred.'))
    installed = dict(source_kind='ACTUAL_INSTALLED_NOETIC_DEPENDENCY_DECLARATIONS', sdk_root=str(sdk),
                     identities=identities, recovery_manifest=manifest_pin,
                     all_copied_files_match_restored_sdk_manifest=True,
                     packages=packages, files=files, default_authority=[])
    (out / 'installed/capture.json').write_text(json.dumps(installed, indent=2) + '\n')
    repository = 'ros-geographic-info/geographic_info'
    base = 'https://api.github.com/repos/' + repository
    directory = out / 'geographic'
    directory.mkdir()
    body = reader.fetch(base + '/git/ref/heads/master')
    (directory / 'branch.json').write_bytes(body)
    branch = json.loads(body)
    assert branch['object']['type'] == 'commit'
    commit = branch['object']['sha']
    assert re.fullmatch(r'[0-9a-f]{40}', commit)
    body = reader.fetch(base + '/git/commits/' + commit)
    (directory / 'commit.json').write_bytes(body)
    original = json.loads(body)
    assert original['sha'] == commit
    tree_sha = original['tree']['sha']
    body = reader.fetch(base + '/git/trees/' + tree_sha + '?recursive=1')
    (directory / 'tree.json').write_bytes(body)
    tree = json.loads(body)
    assert tree['sha'] == tree_sha and not tree.get('truncated')
    rows = {r['path']: r for r in tree['tree'] if r['type'] == 'blob'}
    selected = {name for name in rows if name.startswith('geographic_msgs/') and
                (PurePosixPath(name).suffix in ('.msg', '.srv') or PurePosixPath(name).name in ('CMakeLists.txt', 'package.xml'))}
    selected.update(name for name in rows if PurePosixPath(name).name.upper().startswith(('LICENSE', 'COPYING', 'NOTICE')))
    assert selected and all(rows[n]['mode'] in ('100644', '100755') and rows[n]['size'] <= 2 << 20 for n in selected)
    pending, captured = set(selected), []
    compressed = expanded = members = 0
    digest = hashlib.sha256()
    url = 'https://codeload.github.com/' + repository + '/tar.gz/' + commit
    request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-source-review'})
    with urllib.request.urlopen(request, timeout=30) as response:
        class Counted:
            def read(self, n=-1):
                nonlocal compressed
                assert 0 <= n <= 16 << 20
                body = response.read(n)
                compressed += len(body)
                assert compressed <= 16 << 20
                digest.update(body)
                return body
        with gzip.GzipFile(fileobj=Counted(), mode='rb') as zipped:
            with tarfile.open(fileobj=zipped, mode='r|') as archive:
                for member in archive:
                    path = PurePosixPath(member.name)
                    assert not path.is_absolute() and '..' not in path.parts
                    assert path.parts[0] == 'geographic_info-' + commit
                    expanded += member.size
                    members += 1
                    assert expanded <= 64 << 20 and members <= 4096
                    name = '/'.join(path.parts[1:])
                    if name not in pending:
                        continue
                    row = rows[name]
                    assert member.isfile() and member.size == row['size']
                    value = archive.extractfile(member).read((2 << 20) + 1)
                    assert len(value) == row['size']
                    assert hashlib.sha1(b'blob ' + str(len(value)).encode() + b'\0' + value).hexdigest() == row['sha']
                    target = directory / 'files' / name
                    target.parent.mkdir(parents=True, exist_ok=True)
                    target.write_bytes(value)
                    captured.append(dict(path=name, bytes=len(value), git_blob=row['sha'], sha256=hashlib.sha256(value).hexdigest()))
                    pending.remove(name)
            while zipped.read(1 << 20):
                pass
        assert response.read(1) == b''
    assert not pending
    xml = ET.fromstring((directory / 'files/geographic_msgs/package.xml').read_bytes())
    assert xml.findtext('name') == 'geographic_msgs' and any(p.text == 'catkin' for p in xml.findall('buildtool_depend'))
    geographic = dict(source_kind='EXACT_OFFICIAL_ROS1_SOURCE_BLOBS', repository=repository,
                      branch='master', commit=commit, tree=tree_sha, files=captured,
                      compressed_bytes=compressed, compressed_sha256=digest.hexdigest(),
                      expanded_member_bytes=expanded, archive_members=members, gzip_eof_verified=True,
                      archive_written_or_extracted_or_executed=False, default_authority=[])
    (directory / 'capture.json').write_text(json.dumps(geographic, indent=2) + '\n')
    for row in identities:
        assert pin(Path(row['path'])) == row
    assert pin(manifest_path) == manifest_pin
    report = dict(status='EXPLICIT_INSTALLED_NOETIC_AND_ORIGINAL_GEOGRAPHIC_DEPENDENCIES_CAPTURED',
                  installed_packages=len(packages), installed_files=len(files), geographic_files=len(captured),
                  geographic_commit=commit, default_authority=[], native_code_or_guest_executed=False)
    (out / 'core.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
