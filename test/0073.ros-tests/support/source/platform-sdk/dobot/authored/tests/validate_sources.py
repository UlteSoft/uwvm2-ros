#!/usr/bin/env python3
"""Audit complete original DOBOT schemas and six-language product correspondence.

This read-only check is separate from compiler/Guest/native/official acceptance.
No source hash is adopted as a native identity or authority grant.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET

PROFILES = [('ros1_v3', 'ros1', 'dobot_bringup', 1, 100, 198),
            ('ros1_v4', 'ros1', 'dobot_v4_bringup', 1, 96, 190),
            ('ros2_v4', 'ros2', 'dobot_msgs_v4', 2, 126, 250)]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify(source):
    source = Path(source).resolve(strict=True)
    kit = source / 'sdk/origin/robots/arms/dobot'
    results = []
    for profile, repository, package, version, count, records in PROFILES:
        root = kit / 'catalogs' / profile
        lock = json.loads((root / 'source-lock.json').read_bytes())
        catalog = json.loads((root / 'catalog.json').read_bytes())
        original = kit / 'upstream' / repository
        tree = json.loads((original / 'tree.json').read_bytes())
        commit = json.loads((original / 'commit.json').read_bytes())
        capture = json.loads((original / 'capture.json').read_bytes())
        assert lock['profile'] == profile and lock['vendor'] == 'dobot'
        assert lock['ros_version'] == version and lock['default_authority'] == []
        assert commit['sha'] == capture['commit'] == lock['repositories'][0]['commit']
        assert commit['commit']['tree']['sha'] == tree['sha'] == lock['repositories'][0]['tree_git_sha1']
        assert not tree.get('truncated')
        blobs = {row['path']: row for row in tree['tree'] if row['type'] == 'blob'}
        assert ET.fromstring((original / 'files' / package / 'package.xml').read_bytes()).findtext('name') == package
        expected = {name for name in blobs if len(Path(name).parts) == 3 and
                    name.startswith(package + '/') and name.endswith(('.msg', '.srv', '.action'))}
        definitions = {row['generation_file']: row for row in lock['definitions']}
        assert set(definitions) == expected and len(definitions) == len(catalog['types']) == count
        assert sha(root / 'catalog.json') == lock['catalog_sha256']
        fields, constants = {}, {}
        for row in catalog['types']:
            name = row['file']
            assert name in definitions and not Path(name).is_absolute() and '..' not in Path(name).parts
            definition = definitions[name]
            raw = (root / name).read_bytes()
            blob = blobs[definition['original_file']]
            assert raw == (original / 'files' / definition['original_file']).read_bytes()
            assert len(raw) == definition['bytes'] == blob['size']
            assert hashlib.sha256(raw).hexdigest() == definition['sha256']
            assert hashlib.sha1(b'blob ' + str(len(raw)).encode() + b'\0' + raw).hexdigest() == definition['git_blob_sha1'] == blob['sha']
            kind = Path(name).parent.name
            typ = package + ('/' + kind + '/' if version == 2 else '/') + Path(name).stem
            assert row['type'] == definition['type'] == typ and row['ros_version'] == version and row['registered'] is True
            sections = re.split(r'^\s*---\s*(?:#[^\n]*)?$', raw.decode(), flags=re.MULTILINE)
            roles = {'msg': [''], 'srv': ['Request', 'Response'], 'action': ['Goal', 'Result', 'Feedback']}[kind]
            assert len(sections) == len(roles)
            for role, section in zip(roles, sections):
                record = typ + (('_' if version == 2 else '') + role if role else '')
                members, declared = set(), set()
                for line in section.splitlines():
                    line = line.split('#', 1)[0].strip()
                    if not line:
                        continue
                    constant = re.match(r'^\w+\s+(\w+)\s*=', line)
                    if constant:
                        declared.add(constant[1])
                    else:
                        member = line.split()[1]
                        assert member not in members
                        members.add(member)
                assert record not in fields
                fields[record], constants[record] = members, declared
        family = source / 'sdk/origin/custom-types' / ('dobot-' + profile.replace('_', '-') + '-complete')
        gen = json.loads((family / 'generation.vendor.json').read_bytes())
        prep = json.loads((family / 'preparation.vendor.json').read_bytes())
        assert gen['default_authority'] == gen['implicit_permissions'] == []
        assert gen['native_registry_emitted'] is False and len(gen['outputs']) == 14
        assert {r['type'] for r in gen['records']} == set(fields) and len(fields) == records
        assert prep['original_catalog']['sha256'] == lock['catalog_sha256']
        assert set(prep['original_definitions']) == expected
        for name, definition in definitions.items():
            assert all(prep['original_definitions'][name][k] == definition[k] for k in ('bytes', 'sha256'))
        for name, members in fields.items():
            mapping = gen['member_map'][name]
            assert set(mapping) == {'c', 'cpp', 'rust', 'javascript', 'assemblyscript', 'tinygo'}
            assert all(set(value) == members for value in mapping.values())
            assert {r['name'] for r in gen['declared_constants'].get(name, [])} == constants[name]
        for name, pin in gen['outputs'].items():
            p = family / name
            assert len(Path(name).parts) == 1 and p.is_file() and not p.is_symlink()
            assert p.stat().st_size == pin['bytes'] and sha(p) == pin['sha256']
        for part in ('records', 'interfaces'):
            assert (kit / 'examples/go' / ('records_' + profile) / (part + '.go')).read_bytes() == (family / ('bounded_' + part + '.go')).read_bytes()
        results.append(dict(profile=profile, original_definitions=count, complete_records=records,
                            complete_original_member_sets=sum(map(len, fields.values())), source_products=14))
    return dict(status='DOBOT_ORIGINAL_FULL_SCHEMA_AND_SIX_LANGUAGE_SOURCE_PASS',
                profiles=results, default_authority=[], actual_guest_or_native_runs=0,
                scope='Source correspondence only; no native identity, driver/controller, original official-suite or performance acceptance')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    print(json.dumps(verify(parser.parse_args().source), indent=2))
