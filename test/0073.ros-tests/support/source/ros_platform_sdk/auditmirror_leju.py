#!/usr/bin/env python3
"""Restore only archived ordinary SDK source matching current client byte facts.

This is bounded inert source transport, not acceptance of archived test status.
Every selected archive role must match the independently captured current
pathname, extent and SHA256. Existing unknown bytes and missing new revisions
are refused before copying. No archived object is executed or imported.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path, PurePosixPath
import tarfile


ARCHIVE_SHA = '6d4917fa49ff7de4431b2150ae92d3eb10b52fa6ba25f287736512381d08b5c1'


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    facts_path = root / 'control/current-sdk-source-file-facts.json'
    facts = json.loads(facts_path.read_bytes())['files']
    expected = {}
    for row in facts:
        p = PurePosixPath(row['path'])
        assert len(p.parts) == 4 and p.parts[:2] == ('sdk', 'custom-types')
        assert p.parts[2].endswith('-complete') and '..' not in p.parts
        assert not p.is_absolute() and row['path'] not in expected
        assert type(row['bytes']) is int and 0 <= row['bytes'] <= 16 << 20
        expected[row['path']] = row
    assert len(expected) <= 4096
    archive_path = Path('/home/macromodel/Documents/uwvm-test-261018/recovery/ros-doosan-r58/audit-r1.tar.gz')
    if not archive_path.is_file():
        archive_path = Path('/home/macromodel/.local/share/uwvm-ros-extension/ros-platform-evidence-20261020-r58/audit-r1.tar.gz')
    assert sha(archive_path) == ARCHIVE_SHA
    result = dict(status='RUNNING', facts_sha256=sha(facts_path), archive=str(archive_path),
                  archive_sha256=ARCHIVE_SHA, reused_exact_current_files=[], restored=[],
                  needs_current_transport=[], archived_execution_status_adopted=False,
                  compiler_or_guest_or_native_runs=0, default_authority=[])
    try:
        with archive_path.open('rb') as raw, gzip.GzipFile(fileobj=raw) as zipped:
            with tarfile.open(fileobj=zipped, mode='r|') as archive:
                members = iter(archive)
                first = next(members)
                assert first.name == 'manifest.json' and first.isfile() and first.size <= 16 << 20
                manifest = json.load(archive.extractfile(first))
                candidates = {}
                for role in manifest['roles']:
                    marker = 'sdk/origin/custom-types/'
                    if role['type'] != 'regular' or marker not in role['path']:
                        continue
                    relative = marker + role['path'].split(marker, 1)[1]
                    if relative in expected and all(role[k] == expected[relative][k] for k in ('bytes', 'sha256')):
                        # The cohort may contain two logical source copies of
                        # one exact body. Only the current client's complete
                        # extent/hash pair can select either archived object.
                        candidates.setdefault(relative, role)
                needed = {}
                for relative, row in expected.items():
                    target = source / relative
                    if target.exists():
                        assert target.is_file() and not target.is_symlink()
                        assert target.stat().st_size == row['bytes'] and sha(target) == row['sha256'], relative
                        result['reused_exact_current_files'].append(relative)
                        continue
                    role = candidates.get(relative)
                    if role is None or any(role[k] != row[k] for k in ('bytes', 'sha256')):
                        result['needs_current_transport'].append(row)
                    else:
                        needed.setdefault(role['object'], []).append(row)
                assert not result['needs_current_transport'], 'Current source revisions require explicit ordinary transport; archived bytes are not substituted'
                seen = set()
                for member in members:
                    if member.name not in needed:
                        continue
                    rows = needed[member.name]
                    assert member.isfile() and member.size == rows[0]['bytes'] <= 16 << 20
                    body = archive.extractfile(member).read((16 << 20) + 1)
                    assert len(body) == rows[0]['bytes'] and hashlib.sha256(body).hexdigest() == rows[0]['sha256']
                    for row in rows:
                        target = source / row['path']
                        target.parent.mkdir(parents=True, exist_ok=True)
                        assert target.parent.resolve() == target.parent and not target.is_symlink()
                        with target.open('xb') as stream:
                            stream.write(body)
                        assert sha(target) == row['sha256']
                        result['restored'].append(dict(row, archive_role=candidates[row['path']]['path'],
                                                       archive_object=member.name))
                    seen.add(member.name)
                assert seen == set(needed)
            while zipped.read(1 << 20):
                pass
        for relative, row in expected.items():
            target = source / relative
            assert target.stat().st_size == row['bytes'] and sha(target) == row['sha256']
        result['status'] = 'CURRENT_ORDINARY_SDK_SOURCE_BYTES_MATCH_NO_EXECUTION_STATUS_ADOPTED'
        print(result['status'], len(expected), flush=True)
    except BaseException as error:
        result.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'source-mirror.json').write_text(json.dumps(result, indent=2) + '\n')


if __name__ == '__main__':
    main()
