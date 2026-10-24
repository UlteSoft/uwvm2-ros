#!/usr/bin/env python3
"""Author explicit, closed Leju source catalogs without inventing old messages.

Current coherent declarations, complete packaged legacy IK, and complete
unregistered DTOs are separate domains. Three stale kuavo_sdk declarations
remain explicitly unresolved. No type is guessed from a similarly named
kuavo_msgs record. Original bytes and provenance remain in the capture tree.
Finite canonical bounds are trusted source configuration, never native/model
limits, permission grants, truncation rules or a whole-platform acceptance.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re

SCALARS = {'bool', 'byte', 'char', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32',
           'int64', 'uint64', 'float32', 'float64', 'string', 'time', 'duration'}
FORMAT = 'uwvm-ros-vendor-complete-bounded-v1'


def canonical(value):
    return (json.dumps(value, indent=2, sort_keys=True) + '\n').encode()


def fields(body, kind):
    roles = {'msg': [''], 'srv': ['request.', 'response.'], 'action': ['goal.', 'result.', 'feedback.']}[kind]
    parts = re.split(r'^\s*---\s*(?:#[^\n]*)?$', body.decode(), flags=re.MULTILINE)
    assert len(parts) == len(roles)
    for role, part in zip(roles, parts):
        for original in part.splitlines():
            line = original.split('#', 1)[0].strip()
            if not line or re.match(r'^\w+\s+\w+\s*=', line):
                continue
            match = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:\[([0-9]*)\])?\s+([A-Za-z][A-Za-z0-9_]*)(?:\s+.*)?', line)
            assert match, original
            yield role, match.groups()


def reference(package, name):
    return 'std_msgs/Header' if name == 'Header' else name if '/' in name else package + '/' + name


def quotas(typ, role, native, array, member):
    result = {}
    if native == 'string':
        result['string'] = 64 if typ == 'std_msgs/MultiArrayDimension' and member == 'label' else 256
        if typ == 'sensor_msgs/JointState' and member == 'name':
            result['string'] = 64
    if array == '':
        result['sequence'] = 16 if native in SCALARS else 8
        if native in ('byte', 'char', 'uint8', 'int8'):
            result['sequence'] = 256 if native in ('char', 'uint8') else 16
        if typ in ('sensor_msgs/Image', 'sensor_msgs/PointCloud2') and member == 'data':
            result['sequence'] = 4096
        if member in {'joint_q', 'joint_v', 'joint_vd', 'joint_torque', 'tau', 'tau_max', 'tau_ratio', 'joint_kp', 'joint_kd', 'control_modes'}:
            result['sequence'] = 128
        if typ == 'sensor_msgs/JointState':
            result['sequence'] = 128
        if typ in ('std_msgs/Float32MultiArray', 'std_msgs/Float64MultiArray') and member == 'data':
            result['sequence'] = 128
        if typ == 'std_msgs/MultiArrayLayout' and member == 'dim':
            result['sequence'] = 3
        if native.split('/')[-1] in ('Float32MultiArray', 'Float64MultiArray'):
            result['sequence'] = 4
        if native.split('/')[-1] in ('LaserScan', 'Image', 'PointCloud2', 'JointState'):
            result['sequence'] = 2
        if typ == 'sensor_msgs/LaserScan' and member in ('ranges', 'intensities'):
            result['sequence'] = 1024
        if typ.split('/')[-1] in ('mpc_state', 'mpc_input') and member == 'value':
            result['sequence'] = 128
        if typ in ('kuavo_msgs/qv', 'kuavo_sdk/qv') and member in ('q', 'v'):
            result['sequence'] = 128
        if typ == 'kuavo_msgs/gestureInfo' and member == 'alias':
            result['string'] = 64
        if typ == 'leju_mobile_base_msgs/HardwareStatus' and native == 'string':
            result['string'] = 128 if member.endswith('message') else 64
        if typ in ('ocs2_msgs/mpc_target_trajectories', 'ocs2_msgs/mpc_flattened_controller') and native not in SCALARS:
            result['sequence'] = 4
        if typ == 'robot_calibration_msgs/CalibrationData' and member == 'observations':
            result['sequence'] = 1
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    captured, core = Path(inputs['captured_sources']), Path(inputs['core_sources'])
    review = json.loads(Path(inputs['source_review']).read_bytes())
    assert review['status'] == 'EXACT_LEJU_SOURCE_OWNERSHIP_SHADOW_PACKAGE_AND_DEPENDENCY_REVIEW_PASS'
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    dependencies = {}
    for base, receipt, origin in [(core / 'installed/files', core / 'installed/capture.json', 'actual_installed_noetic'),
                                   (core / 'geographic/files', core / 'geographic/capture.json', 'official_geographic_master')]:
        proof = json.loads(receipt.read_bytes())
        for row in proof['files']:
            path = PurePosixPath(row['path'])
            if path.suffix != '.msg':
                continue
            assert path.parts[-2] == 'msg'
            package = path.parts[-3]
            body = (base / path).read_bytes()
            assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
            name = package + '/' + path.stem
            assert name not in dependencies
            dependencies[name] = dict(type=name, kind='msg', body=body, origin=origin, original_path=row['path'],
                                      sha256=row['sha256'], git_blob=row.get('git_blob'),
                                      provenance_file='upstream/core/' + ('installed' if origin == 'actual_installed_noetic' else 'geographic') + '/capture.json')
    profiles = []
    for original in review['profiles']:
        branch = original['id']
        base = captured / 'upstream' / branch / 'files'
        primary = next(d for d in original['domains'] if d['domain'] == 'primary')
        legacy = next(d for d in original['domains'] if d['domain'] == 'legacy_packaged_ik')
        unresolved = [row for row in primary['roots'] if row['package'] == 'kuavo_sdk' and
                      PurePosixPath(row['original_path']).stem not in ('qv', 'fullBodyTargetTrajectories')]
        assert len(unresolved) == 3
        current = [row for row in primary['roots'] if row['package'] not in ('kuavo_sdk', 'motion_capture_ik')]
        dtos = [row for row in primary['roots'] if row['package'] == 'kuavo_sdk' and
                PurePosixPath(row['original_path']).stem in ('qv', 'fullBodyTargetTrajectories')]
        assert len(current) == 351 and len(dtos) == 2
        for scope, roots, registered in [('platform', current, True), ('packaged_ik', legacy['roots'], False), ('unregistered_dtos', dtos, False)]:
            key = 'ros1_' + branch + '_' + scope
            directory = out / 'catalogs' / key
            directory.mkdir(parents=True)
            candidates = dict(dependencies)
            for row in roots:
                body = (base / row['original_path']).read_bytes()
                assert hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == row['git_blob']
                assert row['type'] not in candidates
                candidates[row['type']] = dict(row, body=body, sha256=hashlib.sha256(body).hexdigest(),
                                               origin='exact_leju_original', provenance_file='upstream/' + branch + '/source-lock.json')
            # All current packages are candidates for genuine dependencies of
            # a selected root. Conflicting legacy IK names are never borrowed.
            for row in current:
                if row['kind'] == 'msg' and row['type'] not in candidates:
                    body = (base / row['original_path']).read_bytes()
                    candidates[row['type']] = dict(row, body=body, sha256=hashlib.sha256(body).hexdigest(),
                                                   origin='exact_leju_original', provenance_file='upstream/' + branch + '/source-lock.json')
            pending = [row['type'] for row in roots]
            root_names, seen, rows, definitions, empty = set(pending), set(), [], [], []
            while pending:
                name = pending.pop(0)
                if name in seen:
                    continue
                assert name in candidates, (key, 'Original message dependency unavailable', name)
                seen.add(name)
                value = candidates[name]
                limits, members = {}, list(fields(value['body'], value['kind']))
                for role, (native, array, member) in members:
                    bound = quotas(name, role, native, array, member)
                    if bound:
                        limits[role + member] = bound
                    if native not in SCALARS:
                        dependency = reference(name.split('/')[0], native)
                        assert dependency in candidates, (key, name, dependency)
                        pending.append(dependency)
                active = registered and name in root_names
                if value['kind'] == 'msg' and not members:
                    empty.append(name)
                    active = False
                logical = PurePosixPath(name.split('/')[0]) / value['kind'] / (name.split('/')[-1] + '.' + value['kind'])
                path = directory / logical
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(value['body'])
                rows.append(dict(ros_version=1, type=name, file=str(logical), registered=active, bounds=limits))
                definitions.append({k: v for k, v in value.items() if k not in ('body', 'package_root', 'package')} | dict(generation_file=str(logical)))
            catalog = canonical(dict(format=FORMAT, types=sorted(rows, key=lambda row: row['type'])))
            (directory / 'catalog.json').write_bytes(catalog)
            lock = dict(kind='leju-explicit-complete-selected-source-lock-v1', profile=key, branch=original['branch'],
                        commit=original['commit'], tree=original['tree'], ros_version=1, scope=scope,
                        original_custom_roots=sorted(root_names), definitions=sorted(definitions, key=lambda row: row['type']),
                        default_authority=[], source_only_empty_topics=empty,
                        source_only_profile=not registered, catalog_sha256=hashlib.sha256(catalog).hexdigest(),
                        unresolved_old_kuavo_sdk_declarations=unresolved,
                        current_unregistered_ik_files=[row for row in primary['roots'] if row['package'] == 'motion_capture_ik'],
                        unsupported_not_renamed=True, native_driver_qualified=False)
            (directory / 'source-lock.json').write_bytes(canonical(lock))
            profiles.append(dict(id=key, ros_version=1, scope=scope, branch=original['branch'],
                                 commit=original['commit'], tree=original['tree'], original_roots=len(root_names),
                                 complete_definitions=len(rows), source_only=not registered,
                                 package={'platform':'kuavo_msgs','packaged_ik':'motion_capture_ik','unregistered_dtos':'kuavo_sdk'}[scope],
                                 state={'platform':'sensorsData','packaged_ik':'robotArmQVVD','unregistered_dtos':'qv'}[scope],
                                 command={'platform':'jointCmd','packaged_ik':'twoArmHandPoseCmdFree','unregistered_dtos':'fullBodyTargetTrajectories'}[scope]))
    (out / 'profiles.json').write_bytes(canonical(dict(vendor='leju', profiles=profiles, default_authority=[])))
    report = dict(status='EXPLICIT_CLOSED_LEJU_SOURCE_CATALOGS_AUTHORED', profiles=profiles,
                  generated_sdk_count=0, original_official_cases_run=0, default_authority=[],
                  unresolved_files_preserved=True, native_driver_qualified=False)
    (out / 'catalogs.json').write_bytes(canonical(report))
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
