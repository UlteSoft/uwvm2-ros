#!/usr/bin/env python3
"""Resolve original Leju declaration ownership without merging shadow packages.

All captured bytes are checked before analysis. This emits inert source-domain
and dependency inventories, not an executable SDK, ROS2 driver claim or native
acceptance. Original package dependency conditions are retained as text and
never evaluated as code. Run beneath the finite Linux Leju owner.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import xml.etree.ElementTree as ET

SCALARS = {'bool', 'byte', 'char', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32',
           'int64', 'uint64', 'float32', 'float64', 'string', 'time', 'duration'}
LEGACY_IK = 'src/humanoid-control/humanoid_arm_control/scripts/motion_capture_ik_packaged'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    captured = Path(inputs['captured_sources'])
    capture = json.loads((captured / 'capture.json').read_bytes())
    assert capture['status'] == 'EXACT_SELECTED_LEJU_ORIGINAL_BLOBS_CAPTURED_NO_UPSTREAM_CODE_EXECUTED'
    assert args.out.parent == root / 'out' and not args.out.exists()
    args.out.mkdir(mode=0o700)
    profiles = []
    for profile in capture['profiles']:
        directory = captured / 'upstream' / profile['id']
        lock = json.loads((directory / 'source-lock.json').read_bytes())
        files = directory / 'files'
        for row in lock['files']:
            path = files / row['path']
            assert not path.is_symlink()
            body = path.read_bytes()
            assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
            assert hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == row['git_blob']
        packages = {}
        for row in lock['packages']:
            path = PurePosixPath(row['path'])
            package = ET.fromstring((files / path).read_bytes())
            name = package.findtext('name')
            assert name and re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*', name)
            packages[str(path.parent)] = dict(name=name, path=str(path),
                buildtools=[dict(name=x.text, condition=x.get('condition')) for x in package.findall('buildtool_depend')],
                dependencies=[dict(kind=x.tag, name=x.text, condition=x.get('condition'))
                              for x in package if x.tag.endswith('depend')])
        views = dict(primary=[], legacy_packaged_ik=[])
        for row in lock['original_declarations']:
            original = PurePosixPath(row['path'])
            owners = [p for p in packages if original.is_relative_to(PurePosixPath(p))]
            assert owners, row['path']
            owner = max(owners, key=lambda p: len(PurePosixPath(p).parts))
            package = packages[owner]
            kind = original.suffix[1:]
            relative = original.relative_to(PurePosixPath(owner))
            # ROS package/type identity uses the declaration basename. The
            # unchanged easy_handeye CMake explicitly declares nested service
            # directories; retain that original path without adding it to the
            # native type name. Duplicate flattened identities still fail below.
            assert relative.parts[0] == kind, ('Declaration kind directory differs', row['path'])
            value = dict(package=package['name'], type=package['name'] + '/' + original.stem,
                         kind=kind, original_path=row['path'], package_root=owner, git_blob=row['sha'])
            views['legacy_packaged_ik' if owner == LEGACY_IK else 'primary'].append(value)
        assert len(views['primary']) == 362 and len(views['legacy_packaged_ik']) == 14
        domain_reports = []
        for domain, roots in views.items():
            seen = set()
            references = collections.defaultdict(set)
            selected_packages = {row['package_root']: packages[row['package_root']] for row in roots}
            for row in roots:
                identity = (row['type'], row['kind'])
                assert identity not in seen, (profile['id'], domain, identity)
                seen.add(identity)
                body = (files / row['original_path']).read_text()
                for original in body.splitlines():
                    line = original.split('#', 1)[0].strip()
                    if not line or line.startswith('---') or re.match(r'^\w+\s+\w+\s*=', line):
                        continue
                    field = re.match(r'^([A-Za-z][A-Za-z0-9_/]*)(?:\[[^\]]*\])?\s+\w+', line)
                    assert field, (row['original_path'], original)
                    typ = field[1]
                    if typ in SCALARS:
                        continue
                    if typ == 'Header':
                        typ = 'std_msgs/Header'
                    elif '/' not in typ:
                        typ = row['package'] + '/' + typ
                    references[typ].add(row['original_path'])
            # A legacy IK view may reach other original packages, but never
            # borrows the conflicting current IK package by name alone.
            candidates = {row['type'] for row in views['primary'] if row['kind'] == 'msg' and
                          (domain != 'legacy_packaged_ik' or row['package'] != 'motion_capture_ik')}
            candidates.update(row['type'] for row in roots if row['kind'] == 'msg')
            unresolved = {name: sorted(paths) for name, paths in sorted(references.items()) if name not in candidates}
            conditional = [p for p in selected_packages.values() if any(
                t['name'] == 'ament_cmake' for t in p['buildtools'])]
            domain_reports.append(dict(domain=domain, roots=roots, original_root_count=len(roots),
                package_count=len(selected_packages), packages=selected_packages,
                unresolved_message_references=unresolved,
                dual_build_dependency_packages=conditional,
                ros1_source_candidate=True, leju_ros2_driver_claimed=False))
        profiles.append(dict(id=profile['id'], branch=lock['branch'], commit=lock['commit'], tree=lock['tree'],
                             source_files_checked=len(lock['files']), domains=domain_reports))
    result = dict(status='EXACT_LEJU_SOURCE_OWNERSHIP_SHADOW_PACKAGE_AND_DEPENDENCY_REVIEW_PASS',
                  profiles=profiles, default_authority=[], generated_sdk_count=0,
                  original_official_cases_run=0, native_driver_qualified=False,
                  ros2_note='Conditional ROS2 builds in vendored grid_map/kindr dependencies do not establish an ROS2 Kuavo Humanoid SDK or driver.')
    (args.out / 'source-review.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], flush=True)


if __name__ == '__main__':
    main()
