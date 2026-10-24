#!/usr/bin/env python3
"""Independently compare all six vendors' original fields with delivered SDKs.

This inspection imports no codec generator. It re-reads original package-owned
declarations, installed dependency files and ZIP/source proofs, then compares
every emitted language member/constant map and every product hash. A source
pass is not compiler, native ROS/controller, official-suite or performance
qualification. The original zero-authority configuration remains unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import xml.etree.ElementTree as ET

VENDORS = ('agibot', 'deep_robotics', 'fourier', 'pal', 'trossen', 'ubtech')
SOURCE_KEYS = {'agibot': 'agibot', 'deep_robotics': 'deep', 'fourier': 'fourier_bridge',
               'pal': 'pal', 'trossen': 'interbotix', 'ubtech': 'ubtech'}
LANGUAGES = {'c', 'cpp', 'rust', 'javascript', 'assemblyscript', 'tinygo'}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def split(raw, version, kind):
    text = raw.decode()
    if version == 1 and kind in ('srv', 'action'):
        pieces = [[]]
        for line in text.split('\n'):
            if line.startswith('---'):
                if kind == 'action' or len(pieces) == 1:
                    pieces.append([])
            else:
                pieces[-1].append(line + '\n')
        if kind == 'srv' and len(pieces) == 1:
            pieces.append([])
        return [''.join(v) for v in pieces]
    return re.split(r'^\s*---\s*(?:#[^\n]*)?$', text, flags=re.MULTILINE)


def verify(source, inputs):
    source = Path(source).resolve(strict=True)
    results = []
    manifests, manifest_hashes = {}, {}
    for backend in ('noetic', 'humble'):
        path = Path(inputs[backend + '_source_manifest'])
        manifest_hashes[backend] = sha(path)
        manifests[backend] = {v['path']: v for v in json.loads(path.read_bytes())}
    for vendor in VENDORS:
        kit = source / 'sdk/origin/robots/platforms' / vendor
        profiles = json.loads((kit / 'profiles.json').read_bytes())['profiles']
        for profile in profiles:
            key, version = profile['id'], profile['ros_version']
            root = kit / 'catalogs' / key
            lock = json.loads((root / 'source-lock.json').read_bytes())
            catalog = json.loads((root / 'catalog.json').read_bytes())
            assert lock['profile'] == key and lock['ros_version'] == version
            assert lock['default_authority'] == [] and sha(root / 'catalog.json') == lock['catalog_sha256']
            assert lock['excluded_original_roots'] == [], ('Unexpected missing original interface', vendor, key)
            source_key = 'interbotix_models' if key.startswith('ros1_manipulators_') else SOURCE_KEYS[vendor]
            original = kit / 'upstream' / source_key / lock['branch'].replace('-', '_')
            capture = json.loads((original / 'source-lock.json').read_bytes())
            assert capture['commit'] == lock['commit'] and capture['tree'] == lock['tree']
            captures = {v['path']: v for v in capture['files']}
            package_names = {}
            for name, pin in captures.items():
                if PurePosixPath(name).name != 'package.xml':
                    continue
                xml = ET.fromstring((original / 'files' / name).read_bytes())
                family = {1 if v.text == 'catkin' else 2 for v in xml.findall('buildtool_depend')
                          if v.text in ('catkin', 'ament_cmake', 'ament_python')}
                assert len(family) <= 1
                package_names[str(PurePosixPath(name).parent)] = (xml.findtext('name'), next(iter(family), None))
            original_roots = {}
            for name, pin in captures.items():
                path = PurePosixPath(name)
                if path.suffix not in ('.msg', '.srv', '.action'):
                    continue
                package = next(package_names[str(v)] for v in path.parents if str(v) in package_names)
                if package[1] != version:
                    continue
                typ = package[0] + ('/' + path.suffix[1:] if version == 2 else '') + '/' + path.stem
                assert typ not in original_roots
                original_roots[typ] = name
            assert set(original_roots) == set(lock['original_custom_roots']) == set(lock['admitted_custom_roots'])
            family = source / 'sdk/origin/custom-types' / profile['family']
            gen = json.loads((family / 'generation.vendor.json').read_bytes())
            prep = json.loads((family / 'preparation.vendor.json').read_bytes())
            assert gen['default_authority'] == gen['implicit_permissions'] == []
            assert not gen['native_registry_emitted'] and prep['mode'] == 'prepare'
            assert gen['max_total_nodes'] == prep['max_total_nodes'] == profile['source_max_total_nodes']
            assert prep['original_catalog']['sha256'] == lock['catalog_sha256']
            definitions = {v['generation_file']: v for v in lock['definitions']}
            assert set(definitions) == set(prep['original_definitions']) == {v['file'] for v in catalog['types']}
            expected, primitives, field_count = set(), set(), 0
            for row in catalog['types']:
                d = definitions[row['file']]
                raw = (root / row['file']).read_bytes()
                assert hashlib.sha256(raw).hexdigest() == d['sha256'] == prep['original_definitions'][row['file']]['sha256']
                if d['origin'].startswith('exact_') and d['origin'].endswith('_original'):
                    assert raw == (original / 'files' / d['original_path']).read_bytes()
                    assert d['original_path'] == original_roots[row['type']]
                    assert captures[d['original_path']]['sha256'] == d['sha256']
                elif d['origin'].startswith('actual_restored_'):
                    backend = 'noetic' if version == 1 else 'humble'
                    physical = Path(inputs[backend + '_source_root']) / d['original_path']
                    assert raw == physical.read_bytes()
                    assert manifest_hashes[backend] == d['manifest_sha256']
                    installed = manifests[backend][d['original_path']]
                    assert installed['type'] == 'regular' and installed['bytes'] == len(raw) and installed['sha256'] == d['sha256']
                elif d['origin'] == 'exact_original_humanoid_nav_dependency':
                    assert raw == (kit / 'upstream/core-source-references/humanoid-nav/files' / d['original_path']).read_bytes()
                else:
                    raise AssertionError(('Unknown original source role', d['origin']))
                if d.get('git_blob'):
                    assert hashlib.sha1(b'blob ' + str(len(raw)).encode() + b'\0' + raw).hexdigest() == d['git_blob']
                kind = Path(row['file']).suffix[1:]
                roles = {'msg': ['message'], 'srv': ['request', 'response'], 'action': ['goal', 'result', 'feedback']}[kind]
                sections = split(raw, version, kind)
                assert len(sections) == len(roles)
                for role, section in zip(roles, sections):
                    name = row['type'] if role == 'message' else row['type'] + ('_' if version == 2 else '') + role.title()
                    expected.add(name)
                    members, constants = set(), set()
                    for original_line in section.splitlines():
                        line = original_line.split('#', 1)[0].strip()
                        if not line:
                            continue
                        const = re.match(r'^\w+\s+(\w+)\s*=', line)
                        if const:
                            constants.add(const[1])
                        else:
                            tokens = line.split()
                            assert len(tokens) >= 2
                            members.add(tokens[1])
                            if version == 1 and tokens[0].split('[')[0] in ('time', 'duration'):
                                primitives.add(tokens[0].split('[')[0])
                    mapping = gen['member_map'][name]
                    assert set(mapping) == LANGUAGES and all(set(v) == members for v in mapping.values())
                    assert {v['name'] for v in gen['declared_constants'].get(name, [])} == constants
                    field_count += len(members)
            assert {v['type'] for v in gen['records']} == expected | primitives
            assert all(v['maximum_wire_bytes'] <= 32768 and v['expanded_nodes'] <= 4096 for v in gen['records'])
            assert profile['state_type'] in expected and profile['command_type'] in expected
            assert len(gen['outputs']) == (14 if profile['has_protocols'] else 10)
            for name, pin in gen['outputs'].items():
                path = family / name
                assert len(Path(name).parts) == 1 and not path.is_symlink()
                assert path.stat().st_size == pin['bytes'] and sha(path) == pin['sha256']
            results.append(dict(vendor=vendor, profile=key, original_roots=len(original_roots),
                                complete_records=len(expected | primitives), original_declared_fields=field_count))
    assert all(sha(Path(inputs[b + '_source_manifest'])) == manifest_hashes[b] for b in manifests)
    return dict(status='ALL_SIX_SELECTED_VENDOR_ORIGINAL_SOURCES_AND_SIX_LANGUAGE_MEMBER_MAPS_PASS',
                profiles=results, default_authority=[], original_official_cases_run=0,
                compiler_guest_or_native_runs=0, native_driver_qualified=False)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    a.out.mkdir(mode=0o700)
    report = verify(inputs['source_root'], inputs)
    (a.out / 'source-audit.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
