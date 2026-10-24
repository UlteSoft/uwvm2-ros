#!/usr/bin/env python3
"""Author separate, exact selected ROS source domains for outstanding vendors.

ROS family comes from each original package.xml, not a branch label. Installed
core declarations retain their restored-file manifest proof. Missing and
unparseable original dependencies remain explicit exclusions; no similarly
named type, ROS2 migration or unavailable vendor SDK is invented. The finite
canonical bounds below are storage choices, never robot motion limits.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path, PurePosixPath
import shutil

CORE_PACKAGES = ('builtin_interfaces', 'std_msgs', 'geometry_msgs', 'nav_msgs', 'sensor_msgs',
                 'trajectory_msgs', 'std_srvs', 'actionlib_msgs', 'diagnostic_msgs', 'shape_msgs')
VENDORS = {'deep': ('deep_robotics', 'DEEP Robotics'), 'pal': ('pal', 'PAL Robotics'),
           'interbotix': ('trossen', 'Interbotix'), 'interbotix_models': ('trossen', 'Interbotix'),
           'ubtech': ('ubtech', 'UBTECH'), 'agibot': ('agibot', 'AgiBot X2 AimDK'),
           'fourier_bridge': ('fourier', 'Fourier Aurora native SDK bridge')}


def canonical(value):
    return (json.dumps(value, indent=2, sort_keys=True) + '\n').encode()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    capture = Path(inputs['remaining_capture'])
    review = json.loads(Path(inputs['remaining_review']).read_bytes())
    assert review['status'].startswith('EXACT_SOURCE_PACKAGE_OWNERSHIP_REVIEWED_')
    spec = importlib.util.spec_from_file_location('remaining_original_field_reader', Path(__file__).with_name('review_remaining.py'))
    reader = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(reader)
    out.mkdir(mode=0o700)
    core = {}
    for version, key, distribution in [(1, 'noetic', 'noetic'), (2, 'humble', 'humble')]:
        sdk = Path(inputs[key + '_source_root'])
        manifest_path = Path(inputs[key + '_source_manifest'])
        manifest = {v['path']: v for v in json.loads(manifest_path.read_bytes())}
        candidates = {}
        for package in CORE_PACKAGES:
            directory = sdk / 'opt/ros' / distribution / 'share' / package
            for path in sorted((directory / 'msg').glob('*.msg')):
                relative = str(path.relative_to(sdk))
                pin = manifest[relative]
                body = path.read_bytes()
                assert pin['type'] == 'regular' and not path.is_symlink()
                assert len(body) == pin['bytes'] and hashlib.sha256(body).hexdigest() == pin['sha256']
                name = package + ('/msg/' if version == 2 else '/') + path.stem
                candidates[name] = dict(type=name, kind='msg', body=body, ros_version=version,
                    original_path=relative, bytes=len(body), sha256=pin['sha256'],
                    origin='actual_restored_' + distribution + '_manifest',
                    manifest_sha256=hashlib.sha256(manifest_path.read_bytes()).hexdigest(), registered=False)
        core[version] = candidates
    extras = Path(inputs['remaining_extras'])
    extra_proof = json.loads((extras / 'extras.json').read_bytes())
    dependency = extra_proof['humanoid_nav_dependency']
    for row in dependency['files']:
        p = PurePosixPath(row['path'])
        if p.suffix != '.msg':
            continue
        body = (extras / 'humanoid-nav/files' / p).read_bytes()
        assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
        assert hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == row['git_blob']
        name = p.parts[-3] + '/' + p.stem
        assert name not in core[1]
        core[1][name] = dict(type=name, kind='msg', body=body, ros_version=1,
            original_path=row['path'], bytes=len(body), sha256=row['sha256'], git_blob=row['git_blob'],
            repository=dependency['repository'], commit=dependency['commit'], tree=dependency['tree'],
            origin='exact_original_humanoid_nav_dependency', registered=False)
    report = dict(status='RUNNING', profiles=[], exclusions=[], default_authority=[], original_official_cases_run=0)
    try:
        for original in review['profiles']:
            if original['source_key'] not in VENDORS or not original['declarations']:
                continue
            vendor, display = VENDORS[original['source_key']]
            for version in sorted({v['ros_version'] for v in original['declarations']}):
                roots = [v for v in original['declarations'] if v['ros_version'] == version]
                candidates = dict(core[version])
                for row in roots:
                    assert row['type'] not in candidates
                    body = (capture / 'upstream' / original['source_key'] / original['branch'].replace('-', '_') / 'files' / row['path']).read_bytes()
                    candidates[row['type']] = dict(row, body=body, original_path=row['path'],
                        origin='exact_' + original['source_key'] + '_original', registered=True)
                def reference(package, name):
                    if name == 'Header':
                        name = 'std_msgs/Header'
                    parts = name.split('/')
                    if len(parts) == 1:
                        return package + ('/msg/' if version == 2 else '/') + name
                    if len(parts) == 2 and version == 2:
                        return parts[0] + '/msg/' + parts[1]
                    return name
                def closure(name, seen):
                    if name not in candidates and version == 1:
                        # ROS1 actions generate their Goal/Result/Feedback
                        # messages. Retain the original action, not a made-up
                        # stand-alone message or a renamed similar declaration.
                        actions = [key for key, value in candidates.items() if value['kind'] == 'action'
                                   and name in {key + suffix for suffix in ('Goal', 'Result', 'Feedback')}]
                        if len(actions) == 1:
                            closure(actions[0], seen)
                            return
                    assert name in candidates, ('Unavailable exact original dependency', name)
                    if name in seen:
                        return
                    value = candidates[name]
                    seen.add(name)
                    for role, native, array, member, string_bound, bounded in reader.fields(value['body'], value['kind'], version):
                        if native not in reader.SCALARS:
                            closure(reference(name.split('/')[0], native), seen)
                admitted, exclusions = [], []
                for row in roots:
                    try:
                        seen = set()
                        closure(row['type'], seen)
                        admitted.append(row)
                    except AssertionError as error:
                        exclusions.append(dict(type=row['type'], original_path=row['path'], reason=repr(error)))
                if not admitted:
                    report['exclusions'].append(dict(source=original['id'], ros_version=version, roots=exclusions))
                    continue
                scope = 'manipulators_' if original['source_key'] == 'interbotix_models' else ''
                key = 'ros' + str(version) + '_' + scope + original['branch'].replace('-', '_')
                kit = out / 'sdk/origin/robots/platforms' / vendor
                directory = kit / 'catalogs' / key
                directory.mkdir(parents=True)
                included = set()
                for row in admitted:
                    closure(row['type'], included)
                rows, definitions, empty = [], [], []
                for name in sorted(included):
                    value = candidates[name]
                    limits = {}
                    members = list(reader.fields(value['body'], value['kind'], version))
                    for role, native, array, member, string_bound, bounded in members:
                        limit = {}
                        if native in ('string', 'wstring') and string_bound is None:
                            limit['string'] = 128
                        if array == '' or bounded:
                            limit['sequence'] = 16 if native in reader.SCALARS else 4
                            if native in ('byte', 'char', 'uint8', 'int8'):
                                limit['sequence'] = 256
                            if name.split('/')[-1] == 'JointState':
                                limit['sequence'] = 64
                                if member == 'name':
                                    limit['string'] = 64
                            if name.split('/')[-1] in ('JointGroupCommand', 'JointTemps', 'RobotInfo', 'JointsDataCmdValue', 'JointsDataValue'):
                                limit['sequence'] = 64
                            if member == 'data' and name.split('/')[-1] in ('Image', 'CompressedImage', 'PointCloud2'):
                                limit['sequence'] = 4096
                            if vendor == 'agibot':
                                short = name.split('/')[-1]
                                if short in ('ModuleChannelInfo', 'ModuleRpcInfo') and member == 'backends':
                                    limit.update(sequence=4, string=64)
                                if short == 'ModuleResourceInfo' or short == 'ProcessInfo' and member == 'modules':
                                    limit['sequence'] = 2
                                if short in ('JointStateArray', 'JointCommandArray') and member == 'joints':
                                    limit['sequence'] = 64
                            if vendor == 'fourier':
                                limit['sequence'] = 64 if native in ('float64',) else 8
                        if limit:
                            limits[role + member] = limit
                    registered = name in {v['type'] for v in admitted}
                    if value['kind'] == 'msg' and not members:
                        empty.append(name)
                        registered = False
                    package, stem = name.split('/')[0], name.split('/')[-1]
                    logical = str(PurePosixPath(package) / value['kind'] / (stem + '.' + value['kind']))
                    path = directory / logical
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_bytes(value['body'])
                    rows.append(dict(ros_version=version, type=name, file=logical, registered=registered, bounds=limits))
                    definitions.append({k: v for k, v in value.items() if k not in ('body', 'dependencies', 'original_parse_error', 'package_root')} | dict(generation_file=logical))
                catalog = canonical(dict(format='uwvm-ros-vendor-complete-bounded-v1', types=rows))
                (directory / 'catalog.json').write_bytes(catalog)
                lock = dict(kind='explicit-original-ros-platform-source-v1', profile=key, vendor=vendor,
                            ros_version=version, repository=original['repository'], branch=original['branch'],
                            commit=original['commit'], tree=original['tree'], definitions=definitions,
                            source_proof=original.get('source_proof'),
                            original_custom_roots=sorted(v['type'] for v in roots),
                            admitted_custom_roots=sorted(v['type'] for v in admitted),
                            excluded_original_roots=exclusions, source_only_empty_topics=empty,
                            catalog_sha256=hashlib.sha256(catalog).hexdigest(), default_authority=[],
                            native_driver_qualified=False, original_official_cases_run=0)
                (directory / 'source-lock.json').write_bytes(canonical(lock))
                messages = [v['type'] for v in admitted if v['kind'] == 'msg' and v['type'] not in empty]
                if vendor == 'deep_robotics':
                    state, command = 'drdds/msg/RobotStatus', 'drdds/msg/JointsDataCmd'
                elif vendor == 'fourier':
                    prefix = 'uwvm_fourier_bridge' + ('/msg/' if version == 2 else '/')
                    state, command = prefix + 'Status', prefix + 'VelocityCommand'
                elif original['source_key'] == 'interbotix':
                    prefix = 'interbotix_xs_msgs' + ('/msg/' if version == 2 else '/')
                    state, command = prefix + 'JointTemps', prefix + 'JointGroupCommand'
                elif vendor == 'pal' and any(n.endswith('/BatteryState') for n in messages):
                    state = next(n for n in messages if n.endswith('/BatteryState'))
                    command = next(n for n in messages if n.endswith('/LedEffectParams'))
                elif vendor == 'pal':
                    state, command = 'pal_common_msgs/JointCurrent', 'pal_control_msgs/ActuatorCurrentLimit'
                else:
                    state, command = messages[0], messages[-1]
                assert state in included and command in included
                profile = dict(id=key, vendor=vendor, display=display, ros_version=version,
                    package=state.split('/')[0], state=state.split('/')[-1], command=command.split('/')[-1],
                    state_type=state, command_type=command, branch=original['branch'],
                    complete_definitions=len(rows), original_roots=len(roots), admitted_roots=len(admitted),
                    excluded_roots=len(exclusions), has_protocols=any(v['kind'] != 'msg' for v in admitted),
                    source_only=False, source_max_total_nodes=65536,
                    family=vendor.replace('_', '-') + '-' + key.replace('_', '-') + '-complete')
                report['profiles'].append(profile)
                profile_path = kit / 'profiles.json'
                profile_path.write_bytes(canonical(dict(vendor=vendor, default_authority=[],
                    profiles=[v for v in report['profiles'] if v['vendor'] == vendor])))
                print(json.dumps(profile), flush=True)
        report['status'] = 'SEPARATE_EXACT_ROS_SOURCE_CATALOGS_AUTHORED_WITH_EXPLICIT_UNRESOLVED_ORIGINAL_ROOTS'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'catalogs.json').write_bytes(canonical(report))


if __name__ == '__main__':
    main()
