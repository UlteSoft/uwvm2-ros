#!/usr/bin/env python3
"""Prepare complete Unitree definition catalogs from explicit external sources.

This copies ONLY caller-supplied, lock-matching definition files into a fresh
owned preparation directory.  It downloads/executes nothing, computes no native
MD5/RIHS, generates no codecs and installs no library.  A preparation receipt is
not native compatibility, license clearance, permission, or runtime validation.
Legacy definition licensing must be resolved by the external dependency owner.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat

LEGACY_CATALOG = 'uwvm-ros-custom-bounded-v2'
COMPLETE_VENDOR_CATALOG = 'uwvm-ros-vendor-complete-bounded-v1'


def read(path, maximum=4 * 1024 * 1024):
    """Open one non-symlink regular leaf and bind bytes to stable inode facts."""
    path = Path(path).absolute()
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= maximum:
            raise ValueError('bounded regular input required: ' + str(path))
        chunks, length = [], 0
        while length <= before.st_size:
            block = os.read(fd, min(65536, before.st_size + 1 - length))
            if not block:
                break
            chunks.append(block); length += len(block)
        after = os.fstat(fd)
        identity = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
        if identity(before) != identity(after) or length != before.st_size:
            raise ValueError('input changed while opening: ' + str(path))
        raw = b''.join(chunks)
        return raw, {'path': str(path), 'device': before.st_dev, 'inode': before.st_ino,
                     'bytes': length, 'mtime_ns': before.st_mtime_ns,
                     'ctime_ns': before.st_ctime_ns, 'sha256': hashlib.sha256(raw).hexdigest()}
    finally:
        os.close(fd)


def document(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result:
                raise ValueError('duplicate JSON key: ' + key)
            result[key] = value
        return result
    return json.loads(raw.decode('utf-8'), object_pairs_hook=pairs)


def write(path, raw):
    """Exclusive creation; failed preparations remain visible and are not reused."""
    with path.open('xb') as stream:
        stream.write(raw)


def prepare(upstream_root, profile_path, lock_path, output, *, catalog_format=LEGACY_CATALOG):
    """Copy a locked whole family into an explicitly selected catalog domain.

    The default preserves the existing bounded-v2 path. The complete-vendor
    choice is a distinct trusted source-generation input; it does not migrate
    native metadata, reuse a registry, or bypass full native-definition proof.
    """
    if catalog_format not in (LEGACY_CATALOG, COMPLETE_VENDOR_CATALOG):
        raise ValueError('explicit supported catalog domain required')
    profile_raw, profile_pin = read(profile_path)
    lock_raw, lock_pin = read(lock_path)
    profile, lock = document(profile_raw), document(lock_raw)
    expected = {'format', 'family', 'repository', 'commit', 'ros_version', 'protocol',
                'native_identity', 'default_authority', 'sim_defaults', 'definitions'}
    if set(profile) != expected or profile['format'] != 'uwvm-unitree-profile-v1':
        raise ValueError('closed Unitree source profile required')
    if lock.get('format') != 'uwvm-unitree-upstream-lock-v1' or profile['default_authority'] != []:
        raise ValueError('explicit lock and empty default authority required')
    if type(profile['ros_version']) is not int or profile['ros_version'] not in (1, 2):
        raise ValueError('exact ROS backend required')
    if not isinstance(profile['family'], str) or not re.fullmatch(r'[a-z][a-z0-9_]*', profile['family']):
        raise ValueError('exact native package family required')
    if profile['native_identity'] != 'REQUIRES_OFFICIAL_EXPORT':
        raise ValueError('preparation cannot supply a native identity')
    if profile['protocol'] != 'bounded-complete-topic-values' or profile['sim_defaults'] != {'domain_id': 1, 'interface': 'lo'}:
        raise ValueError('complete-topic profile and explicit loopback simulator defaults required')
    definitions = profile['definitions']
    if not isinstance(definitions, list) or not 1 <= len(definitions) <= 128:
        raise ValueError('definition count must be 1..128')
    locked = {}
    for entry in lock['files']:
        key = entry['repository'], entry['commit'], entry['path']
        if key in locked:
            raise ValueError('duplicate upstream lock entry')
        locked[key] = entry
    root = Path(upstream_root).resolve(strict=True)
    if not root.is_dir():
        raise ValueError('explicit external repository directory required')
    output = Path(output).absolute()
    if output.exists() or output.is_symlink() or output.resolve().is_relative_to(root):
        raise ValueError('fresh output outside upstream repository required')
    rows, retained, seen = [], [], set()
    # Read and verify all selected inputs BEFORE creating any output.
    for row in definitions:
        if not isinstance(row, dict) or set(row) != {'type', 'path', 'bounds'}:
            raise ValueError('closed complete definition row required')
        type_pattern = re.escape(profile['family']) + ('/msg/' if profile['ros_version'] == 2 else '/') + r'[A-Z][A-Za-z0-9]*'
        if not isinstance(row['type'], str) or not re.fullmatch(type_pattern, row['type']):
            raise ValueError('complete native type in the selected family required')
        relative = Path(row['path'])
        if relative.is_absolute() or '..' in relative.parts or relative.suffix != '.msg':
            raise ValueError('relative complete .msg path required')
        source = root / relative
        if not source.resolve(strict=True).is_relative_to(root):
            raise ValueError('definition escapes external source root')
        expected_file = locked.get((profile['repository'], profile['commit'], row['path']))
        raw, pin = read(source, 65536)
        if expected_file is None or (pin['bytes'], pin['sha256']) != (expected_file['bytes'], expected_file['sha256']):
            raise ValueError('definition does not match official lock: ' + row['path'])
        if row['type'] in seen:
            raise ValueError('duplicate complete native type')
        seen.add(row['type'])
        if row['type'].split('/')[-1] != relative.stem:
            raise ValueError('native name and definition basename differ')
        if not isinstance(row['bounds'], dict):
            raise ValueError('explicit bound map required')
        target = 'definitions/' + '/'.join(row['type'].split('/')) + '.msg'
        rows.append({'ros_version': profile['ros_version'], 'type': row['type'],
                     'file': target, 'bounds': row['bounds']})
        retained.append((target, raw, pin))
    output.mkdir(mode=0o700, parents=False)
    report = {'status': 'PREPARED_NOT_EXECUTED', 'scope': 'SOURCE_CATALOG_ONLY',
              'catalog_format': catalog_format,
              'family': profile['family'], 'repository': profile['repository'],
              'commit': profile['commit'], 'inputs': [profile_pin, lock_pin] + [p for _, _, p in retained],
              'native_identity': 'NOT_ACQUIRED', 'runtime_permissions': [],
              'upstream_licenses': 'EXTERNAL_DEPENDENCY_OWNER_RESPONSIBILITY',
              'generators_compilers_tests_executed': False, 'outputs': []}
    try:
        write(output / '.uwvm-unitree-preparation.json', (json.dumps({'format': 'uwvm-unitree-preparation-owner-v1', 'family': profile['family']}) + '\n').encode())
        for target, raw, _ in retained:
            destination = output / target
            destination.parent.mkdir(parents=True, exist_ok=True)
            write(destination, raw)
            report['outputs'].append(read(destination, 65536)[1])
        catalog = {'format': catalog_format, 'types': rows}
        write(output / 'catalog.preparation.json', (json.dumps(catalog, indent=2) + '\n').encode())
        report['outputs'].append(read(output / 'catalog.preparation.json')[1])
        for previous in report['inputs']:
            if read(previous['path'])[1] != previous:
                raise ValueError('preparation input changed before completion')
        report['inputs_preserved'] = True
    except BaseException as error:
        report.update(status='FAIL', error=repr(error), inputs_preserved=False)
        raise
    finally:
        write(output / 'report.json', (json.dumps(report, indent=2) + '\n').encode())
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--upstream-root', type=Path, required=True)
    parser.add_argument('--profile', type=Path, required=True)
    parser.add_argument('--lock', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--catalog-format', choices=(LEGACY_CATALOG, COMPLETE_VENDOR_CATALOG),
                        default=LEGACY_CATALOG,
                        help='Explicit source domain; complete-vendor needs industrial/prepare.py and fresh native binding.')
    args = parser.parse_args()
    result = prepare(args.upstream_root, args.profile, args.lock, args.out,
                     catalog_format=args.catalog_format)
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
