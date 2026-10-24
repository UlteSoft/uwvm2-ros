#!/usr/bin/env python3
"""Restore exact old tool bytes; never adopt archived execution status.

The immutable Leju closure is read as data under the current finite cgroup
owner. Its complete archive SHA and selected ordinary object identities are
checked before tools enter this root. No previous owner/result/configuration is
restored. Current tests must produce their own command/product evidence.
"""
import argparse
import hashlib
import json
from pathlib import Path
import tarfile

ARCHIVE_SHA = '07b1f3adef8cecba3724a3882d941224a9f786d2ab1a82ec407ab220e83cb7bc'
RUNTIME_SHA = '719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19'


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    archive = Path('/home/macromodel/Documents/uwvm-test-261018/recovery/ros-leju-r59/closure-r1.tar.gz')
    assert archive.stat().st_size == 127740456 and sha(archive) == ARCHIVE_SHA
    a.out.mkdir(mode=0o700)
    restored, runtime = [], None
    with tarfile.open(archive, 'r|gz') as stream:
        first = stream.next()
        assert first.name == 'manifest.json' and first.size <= 16 << 20
        manifest = json.loads(stream.extractfile(first).read())
        roles = [v for v in manifest['roles'] if v.get('type') == 'regular' and
                 (v['path'].startswith(('tools/quickjs-original/', 'tools/kernel-rootfs/')) or v.get('sha256') == RUNTIME_SHA)]
        assert roles and sum(v['sha256'] == RUNTIME_SHA for v in roles) == 1
        objects = {}
        for role in roles:
            assert role['path'].startswith('tools/') and '..' not in Path(role['path']).parts
            objects.setdefault(role['object'], []).append(role)
        for member in stream:
            if member.name not in objects:
                continue
            assert member.isfile() and member.size <= 32 << 20
            body = stream.extractfile(member).read((32 << 20) + 1)
            for role in objects.pop(member.name):
                assert len(body) == role['bytes'] and hashlib.sha256(body).hexdigest() == role['sha256']
                destination = a.root / role['path']
                destination.parent.mkdir(parents=True, exist_ok=True)
                with destination.open('xb') as output:
                    output.write(body)
                destination.chmod(role['mode'])
                assert sha(destination) == role['sha256']
                restored.append(dict(path=str(destination), bytes=role['bytes'], sha256=role['sha256']))
                if role['sha256'] == RUNTIME_SHA:
                    assert role['bytes'] == 15442000
                    runtime = str(destination)
        assert not objects
    assert sha(archive) == ARCHIVE_SHA and runtime
    report = dict(status='EXACT_TOOL_BYTES_RESTORED_NO_PRIOR_EXECUTION_STATUS_ADOPTED', runtime=runtime,
                  source_archive_sha256=ARCHIVE_SHA, files=restored, default_authority=[], previous_test_credit=0)
    (a.out / 'restoration.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
