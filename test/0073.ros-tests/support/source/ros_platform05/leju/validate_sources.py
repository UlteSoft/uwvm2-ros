#!/usr/bin/env python3
"""Independently audit Leju original bodies and complete six-language members.

Every original body is checked against its captured file, Git blob when present,
and SHA256. This audit grants no authority or native/controller qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re


def verify(source):
    source = Path(source).resolve(strict=True)
    kit = source / 'sdk/origin/robots/humanoids/leju'
    results = []
    sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
    for profile in json.loads((kit / 'profiles.json').read_bytes())['profiles']:
        key = profile['id']
        root = kit / 'catalogs' / key
        lock = json.loads((root / 'source-lock.json').read_bytes())
        catalog = json.loads((root / 'catalog.json').read_bytes())
        assert lock['profile'] == key and lock['ros_version'] == 1
        assert lock['default_authority'] == [] and sha(root / 'catalog.json') == lock['catalog_sha256']
        if profile['source_only']:
            assert not any(r['registered'] for r in catalog['types'])
        else:
            assert set(lock['original_custom_roots']) - set(lock['source_only_empty_topics']) == {r['type'] for r in catalog['types'] if r['registered']}
        family = source / 'sdk/origin/custom-types' / ('leju-' + key.replace('_', '-') + '-complete')
        generation = json.loads((family / 'generation.vendor.json').read_bytes())
        preparation = json.loads((family / 'preparation.vendor.json').read_bytes())
        assert generation['default_authority'] == generation['implicit_permissions'] == []
        assert generation['max_total_nodes'] == preparation['max_total_nodes'] == 65536
        assert preparation['mode'] == 'prepare' and not generation['native_registry_emitted']
        assert preparation['original_catalog']['sha256'] == lock['catalog_sha256']
        definitions = {r['generation_file']: r for r in lock['definitions']}
        assert set(definitions) == set(preparation['original_definitions']) == {r['file'] for r in catalog['types']}
        expected_records = set()
        for row in catalog['types']:
            d = definitions[row['file']]
            original = kit / Path(d['provenance_file']).parent / 'files' / d['original_path']
            raw = (root / row['file']).read_bytes()
            assert raw == original.read_bytes()
            assert hashlib.sha256(raw).hexdigest() == d['sha256'] == preparation['original_definitions'][row['file']]['sha256']
            if d.get('git_blob'):
                assert hashlib.sha1(b'blob ' + str(len(raw)).encode() + b'\0' + raw).hexdigest() == d['git_blob']
            kind = Path(row['file']).suffix[1:]
            roles = {'msg':['message'], 'srv':['request','response'], 'action':['goal','result','feedback']}[kind]
            parts = re.split(r'^\s*---\s*(?:#[^\n]*)?$', raw.decode(), flags=re.MULTILINE)
            assert len(parts) == len(roles)
            for role, part in zip(roles, parts):
                name = row['type'] if role == 'message' else row['type'] + role.title()
                expected_records.add(name)
                members, constants = set(), set()
                for line in part.splitlines():
                    line = line.split('#', 1)[0].strip()
                    if not line:
                        continue
                    constant = re.match(r'^\w+\s+(\w+)\s*=', line)
                    if constant:
                        constants.add(constant.group(1))
                    else:
                        members.add(line.split()[1])
                mapping = generation['member_map'][name]
                assert set(mapping) == {'c','cpp','rust','javascript','assemblyscript','tinygo'}
                assert all(set(names) == members for names in mapping.values()), name
                assert {r['name'] for r in generation['declared_constants'].get(name, [])} == constants
        actual_records = {r['type'] for r in generation['records']}
        assert all(profile['package'] + '/' + name in actual_records for name in (profile['state'], profile['command']))
        assert actual_records - expected_records <= {'time', 'duration'} and expected_records <= actual_records
        assert all(r['maximum_wire_bytes'] <= 32768 and r['expanded_nodes'] <= 4096 for r in generation['records'])
        for name, pin in generation['outputs'].items():
            path = family / name
            assert len(Path(name).parts) == 1 and not path.is_symlink()
            assert path.stat().st_size == pin['bytes'] and sha(path) == pin['sha256']
        protocols = any(Path(r['file']).suffix in ('.srv', '.action') for r in catalog['types'])
        assert len(generation['outputs']) == (14 if protocols else 10)
        for part in ('records', 'interfaces') if protocols else ('records',):
            assert (kit / 'examples/go' / ('records_' + key) / (part + '.go')).read_bytes() == (family / ('bounded_' + part + '.go')).read_bytes()
        results.append(dict(profile=key, original_definitions=len(definitions), records=len(actual_records), source_only=profile['source_only']))
    return dict(status='PASS', profiles=results, default_authority=[], native_delivery_qualified=False, original_official_cases_run=0)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    print(json.dumps(verify(parser.parse_args().source), indent=2))
