#!/usr/bin/env python3
"""Challenge explicit industrial_msgs overlay preparation before native builds.

The original owner separately admits the actual adapter, upstream package and
source-lock. These source-only refusals do not establish installed SDK identity,
ROS execution, policy authority or robot safety. Actual OFF/ON native builds and
independent protocol traffic must follow in the admitted cgroup.
"""
import argparse
import hashlib
import json
from pathlib import Path
import runpy
import shutil


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('adapter', 'original-package', 'source-lock', 'working'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    namespace = runpy.run_path(str(args.adapter))
    prepare = namespace['prepare']
    lock = args.source_lock.read_bytes()
    digest = hashlib.sha256(lock).hexdigest()
    original = {str(p.relative_to(args.original_package)): p.read_bytes()
                for p in args.original_package.rglob('*') if p.is_file()}
    working = args.working
    assert working.is_absolute() and not working.exists()
    working.mkdir(mode=0o700)
    valid = working / 'valid'
    receipt = prepare(args.original_package, args.source_lock, digest, valid)
    assert receipt['default_authority'] == [] and not receipt['default_enabled']
    assert receipt['original_definition_bytes_unchanged']
    for name, raw in original.items():
        if name.endswith(('.msg', '.srv')):
            assert (valid / name).read_bytes() == raw
    refused = []

    def reject(label, package=args.original_package, source_lock=args.source_lock,
               selected_digest=digest, output=None):
        output = output or working / ('refused-' + str(len(refused)))
        assert not output.exists()
        try:
            prepare(package, source_lock, selected_digest, output)
        except (ValueError, FileNotFoundError):
            assert not output.exists(), 'refusal wrote a partial overlay'
            refused.append(label)
        else:
            raise AssertionError('unadmitted source accepted: ' + label)

    reject('wrong reviewed source-lock digest', selected_digest='0' * 64)
    for label, change in (
        ('changed original build selection', lambda p: (p / 'CMakeLists.txt').write_bytes(b'project(unreviewed)\n')),
        ('changed original Service field', lambda p: (p / 'srv/StartMotion.srv').write_bytes(b'uint32 extra\n---\n')),
        ('unexpected upstream source', lambda p: (p / 'unreviewed.cmake').write_bytes(b'')),
    ):
        copy = working / ('copy-' + str(len(refused)))
        shutil.copytree(args.original_package, copy)
        change(copy)
        reject(label, package=copy)
    copy = working / 'symlink-package'
    copy.symlink_to(args.original_package, target_is_directory=True)
    reject('symlink package root', package=copy)
    changed_lock = working / 'duplicate-lock.json'
    changed_lock.write_bytes(b'{"format":0,"format":1}')
    reject('duplicate source-lock key', source_lock=changed_lock,
           selected_digest=hashlib.sha256(changed_lock.read_bytes()).hexdigest())
    reject('output within upstream source', output=args.original_package / 'forbidden-overlay')
    try:
        prepare(args.original_package, args.source_lock, digest, valid)
    except ValueError:
        refused.append('existing output cannot be replaced')
    else:
        raise AssertionError('existing overlay replaced')
    assert original == {str(p.relative_to(args.original_package)): p.read_bytes()
                        for p in args.original_package.rglob('*') if p.is_file()}
    print(json.dumps(dict(scope='Source-only complete-package refusal contract',
                         rejected=refused, default_authority=[]), sort_keys=True))


if __name__ == '__main__':
    main()
