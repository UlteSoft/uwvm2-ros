#!/usr/bin/env python3
"""Prepare an explicit, default-disabled industrial_msgs compatibility build.

The captured upstream ROS2 package generates six Messages but leaves six
historical Service declarations unbuilt. This source-only tool copies that
exact package into a fresh owned directory and adds an OFF-by-default CMake
option to generate those complete declarations. It never modifies upstream,
compiles code, starts a Service, registers a Runtime type, or grants authority.

Supply the original package and the reviewed catalog source-lock SHA256. Only
industrial_core commit d547cdcfdaf3bc0d46325215b8219b0a190c8e6c is admitted.
The resulting overlay retains every original definition byte. Its receipt and
unified diff identify both build-metadata changes. A separate native owner must
build/test OFF and explicit ON configurations, retain their actual products,
and bind only independently verified installed native metadata.
"""
import argparse
import difflib
import hashlib
import json
import os
from pathlib import Path
import stat

COMMIT = 'd547cdcfdaf3bc0d46325215b8219b0a190c8e6c'
TREE = '3fe453f70bd9b4dbba008968b61fa38a069f5c3a'
OPTION = 'UWVM_INDUSTRIAL_LEGACY_SERVICES'
SERVICES = ('CmdJointTrajectory', 'GetRobotInfo', 'SetDrivePower',
            'SetRemoteLoggerLevel', 'StartMotion', 'StopMotion')
BUILD_PINS = {
    'CMakeLists.txt': (917, 'e4452e1a16baef0d56a3b09b2bfcaa66623363416bff668adeb9811b16fe82d9'),
    'package.xml': (1000, '196ce19a5a0920a6c10a9363a049d011dacb34a99d771cf02f9b17d4254b846f'),
}


def need(value, reason):
    if not value:
        raise ValueError(reason)


def read(path):
    """Bound one ordinary source read to its unchanged physical file."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path,
         'physical absolute source required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    with os.fdopen(descriptor, 'rb') as stream:
        before = os.fstat(stream.fileno())
        need(stat.S_ISREG(before.st_mode) and before.st_nlink == 1 and
             0 <= before.st_size <= 2 * 1024 * 1024, 'bounded ordinary source required')
        raw = stream.read(before.st_size + 1)
        after = os.fstat(stream.fileno())
    key = lambda s: (s.st_dev, s.st_ino, s.st_mode, s.st_nlink, s.st_size,
                     s.st_mtime_ns, s.st_ctime_ns)
    need(key(before) == key(after) == key(path.stat()) and len(raw) == before.st_size,
         'source changed during read')
    return raw


def pin(raw):
    return dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def pairs(rows):
    result = {}
    for name, value in rows:
        need(name not in result, 'duplicate source-lock member')
        result[name] = value
    return result


def prepare(original, source_lock, expected_lock, output):
    original, source_lock, output = map(Path, (original, source_lock, output))
    need(original.is_absolute() and original.resolve(strict=True) == original and
         original.is_dir(), 'physical original package directory required')
    need(output.is_absolute() and output.parent.resolve(strict=True) == output.parent and
         not output.exists() and not output.is_symlink() and
         not output.is_relative_to(original), 'fresh owned output outside upstream required')
    lock_raw = read(source_lock)
    need(hashlib.sha256(lock_raw).hexdigest() == expected_lock, 'reviewed source-lock differs')
    lock = json.loads(lock_raw, object_pairs_hook=pairs)
    need(lock['default_authority'] == [] and any(
        r['repository'] == 'ros-industrial/industrial_core' and r['commit'] == COMMIT and
        r['tree'] == TREE for r in lock['repositories']), 'exact original repository required')
    declarations = [r for r in lock['definitions'] if r['type'].startswith('industrial_msgs/')]
    need(len(declarations) == 12 and len({r['type'] for r in declarations}) == 12,
         'complete six Message/six Service declaration set required')
    sources = {}
    for row in declarations:
        need(row['repository'] == 'ros-industrial/industrial_core' and
             row['commit'] == COMMIT and row['tree'] == TREE and
             row['original_path'] == row['type'] + ('.msg' if '/msg/' in row['type'] else '.srv'),
             'original declaration association differs')
        relative = row['original_path'].removeprefix('industrial_msgs/')
        need(relative in {'msg/' + r['type'].rsplit('/', 1)[1] + '.msg'
                         for r in declarations if '/msg/' in r['type']} |
             {'srv/' + name + '.srv' for name in SERVICES}, 'unexpected original definition')
        raw = read(original / relative)
        need(pin(raw) == {k: row[k] for k in ('bytes', 'sha256')} and
             hashlib.sha1(b'blob ' + str(len(raw)).encode() + b'\0' + raw).hexdigest() ==
             row['git_blob_sha1'], 'original declaration bytes differ')
        sources[relative] = raw
    for relative, expected in BUILD_PINS.items():
        raw = read(original / relative)
        need((len(raw), hashlib.sha256(raw).hexdigest()) == expected,
             'unreviewed upstream build metadata')
        sources[relative] = raw
    actual_files = set()
    for path in original.rglob('*'):
        need(not path.is_symlink(), 'upstream package symlink refused')
        if path.is_file():
            actual_files.add(str(path.relative_to(original)))
    need(actual_files == set(sources), 'unexpected or missing upstream package source')

    cmake = sources['CMakeLists.txt'].decode('utf-8')
    selection = 'set(srv_files\n  # empty, for now\n)'
    need(cmake.count(selection) == 1 and cmake.count('    std_msgs\n') == 1,
         'reviewed upstream generation boundary differs')
    extension = (selection + '\n\n# Explicit uwvm source-compatibility overlay; no native server or grant.\n'
        'option(' + OPTION + ' "Generate the six captured historical Service interfaces" OFF)\n'
        'set(uwvm_legacy_dependencies)\nif(' + OPTION + ')\n'
        '  find_package(trajectory_msgs REQUIRED)\n  list(APPEND uwvm_legacy_dependencies trajectory_msgs)\n'
        '  list(APPEND srv_files\n' + ''.join('    srv/' + name + '.srv\n' for name in SERVICES) +
        '  )\nendif()')
    changed = dict(sources)
    changed['CMakeLists.txt'] = cmake.replace(selection, extension).replace(
        '    std_msgs\n', '    std_msgs\n    ${uwvm_legacy_dependencies}\n').encode()
    package = sources['package.xml'].decode('utf-8')
    need(package.count('  <depend>std_msgs</depend>') == 1, 'reviewed package dependency boundary differs')
    changed['package.xml'] = package.replace('  <depend>std_msgs</depend>',
        '  <depend>std_msgs</depend>\n  <!-- Explicit compatibility overlay dependency. -->\n'
        '  <depend>trajectory_msgs</depend>').encode()
    difference = ''.join(''.join(difflib.unified_diff(sources[name].decode().splitlines(True),
        changed[name].decode().splitlines(True), fromfile='upstream/' + name,
        tofile='compatibility/' + name)) for name in BUILD_PINS).encode()
    for relative, raw in sources.items():
        need(read(original / relative) == raw, 'upstream changed during preparation')
    need(read(source_lock) == lock_raw, 'source-lock changed during preparation')
    output.mkdir(mode=0o700)
    for relative, raw in changed.items():
        path = output / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open('xb') as stream:
            stream.write(raw)
    with (output / 'upstream-build.diff').open('xb') as stream:
        stream.write(difference)
    receipt = dict(format='uwvm-industrial-msgs-explicit-compatibility-source-v1',
        scope='Source preparation only; no compiler, identity, endpoint, driver, robot or authority.',
        repository='ros-industrial/industrial_core', commit=COMMIT, tree=TREE,
        source_lock=pin(lock_raw), original_files={n: pin(r) for n, r in sources.items()},
        output_files={n: pin(r) for n, r in changed.items()}, build_difference=pin(difference),
        cmake_option=OPTION, default_enabled=False, additional_services=list(SERVICES),
        original_definition_bytes_unchanged=True, default_authority=[], new_Guest_imports=[])
    with (output / 'overlay-receipt.json').open('x') as stream:
        json.dump(receipt, stream, sort_keys=True, indent=2)
        stream.write('\n')
    return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-package', type=Path, required=True)
    parser.add_argument('--source-lock', type=Path, required=True)
    parser.add_argument('--source-lock-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    prepare(args.original_package, args.source_lock, args.source_lock_sha256, args.out)


if __name__ == '__main__':
    main()
