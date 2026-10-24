#!/usr/bin/env python3
"""Independently check complete original Doosan members and six-language products.

This source audit grants no authority and claims no native/controller result.
Original tree/blob associations are not native type identities or signatures.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re


def verify(source):
    source = Path(source).resolve(strict=True)
    kit = source / 'sdk/origin/robots/arms/doosan'
    results = []
    sha = lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    for profile in json.loads((kit / 'profiles.json').read_bytes())['profiles']:
        key, version = profile['id'], profile['ros_version']
        root = kit / 'catalogs' / key
        lock = json.loads((root / 'source-lock.json').read_bytes())
        catalog = json.loads((root / 'catalog.json').read_bytes())
        assert lock['profile'] == key and lock['ros_version'] == version
        assert lock['default_authority'] == [] and sha(root / 'catalog.json') == lock['catalog_sha256']
        trees = {}
        for row in lock['repositories']:
            original = kit / 'upstream' / row['id']
            commit = json.loads((original / 'commit.json').read_bytes())
            tree = json.loads((original / 'tree.json').read_bytes())
            assert commit['sha'] == row['commit']
            assert commit['commit']['tree']['sha'] == tree['sha'] == row['tree_git_sha1']
            assert not tree.get('truncated')
            trees[row['id']] = {r['path']:r for r in tree['tree'] if r['type']=='blob'}
        original_roots = set()
        for name in trees[key]:
            if name.startswith(profile['source_prefix']+'/') and name.endswith(('.msg','.srv','.action')):
                p = Path(name)
                relative=p.relative_to(profile['source_prefix'])
                assert relative.parts[0] in ('msg','srv','action')
                original_roots.add(profile['package'] + ('/' + relative.parts[0] + '/' if version == 2 else '/') + p.stem)
        assert original_roots == set(lock['original_custom_roots'])
        empty=set(lock['source_only_empty_topics'])
        assert empty <= original_roots
        assert original_roots-empty == {r['type'] for r in catalog['types'] if r['registered']}
        definitions = {r['generation_file']:r for r in lock['definitions']}
        assert set(definitions) == {r['file'] for r in catalog['types']}
        family = source / 'sdk/origin/custom-types' / ('doosan-' + key.replace('_','-') + '-complete')
        generation = json.loads((family / 'generation.vendor.json').read_bytes())
        preparation = json.loads((family / 'preparation.vendor.json').read_bytes())
        assert generation['default_authority'] == generation['implicit_permissions'] == []
        protocols = any(Path(r['file']).suffix in ('.srv','.action') for r in catalog['types'])
        expected_products = 14 if protocols else 10
        assert len(generation['outputs']) == expected_products and not generation['native_registry_emitted']
        assert preparation['mode'] == 'prepare' and preparation['original_catalog']['sha256'] == lock['catalog_sha256']
        expected_records = set()
        for row in catalog['types']:
            d = definitions[row['file']]
            raw = (root / row['file']).read_bytes()
            original = kit / 'upstream' / d['repository'] / 'files' / d['original_file']
            blob = trees[d['repository']][d['original_file']]
            assert raw == original.read_bytes() and len(raw) == d['bytes'] == blob['size']
            assert hashlib.sha256(raw).hexdigest() == d['sha256'] == preparation['original_definitions'][row['file']]['sha256']
            assert hashlib.sha1(b'blob '+str(len(raw)).encode()+b'\0'+raw).hexdigest() == d['git_blob_sha1'] == blob['sha']
            kind = Path(row['file']).suffix[1:]
            parts = re.split(r'^\s*---\s*(?:#[^\n]*)?$',raw.decode(),flags=re.MULTILINE)
            roles = {'msg':['message'],'srv':['request','response'],'action':['goal','result','feedback']}[kind]
            assert len(parts) == len(roles)
            if row['type'] in empty:
                assert kind=='msg' and not any(line.split('#',1)[0].strip() for line in raw.decode().splitlines())
            for role,part in zip(roles,parts):
                name = row['type'] if role == 'message' else row['type'] + ('_' if version == 2 else '') + role.title()
                expected_records.add(name)
                members,constants = set(),set()
                for line in part.splitlines():
                    line = line.split('#',1)[0].strip()
                    if not line:
                        continue
                    constant = re.match(r'^\w+\s+(\w+)\s*=',line)
                    if constant:
                        constants.add(constant.group(1))
                    else:
                        members.add(line.split()[1])
                mapping = generation['member_map'][name]
                assert set(mapping) == {'c','cpp','rust','javascript','assemblyscript','tinygo'}
                assert all(set(names) == members for names in mapping.values()),name
                assert {r['name'] for r in generation['declared_constants'].get(name,[])} == constants
        # ROS1 pseudo time/duration are represented by generated complete values.
        actual_records = {r['type'] for r in generation['records']}
        assert actual_records - expected_records <= {'time','duration'}
        assert expected_records <= actual_records
        for name,pin in generation['outputs'].items():
            p = family / name
            assert len(Path(name).parts) == 1 and not p.is_symlink()
            assert p.stat().st_size == pin['bytes'] and sha(p) == pin['sha256']
        for part in ('records','interfaces'):
            if part == 'records' or protocols:
                assert (kit / 'examples/go' / ('records_'+key) / (part+'.go')).read_bytes() == (family / ('bounded_'+part+'.go')).read_bytes()
        results.append(dict(profile=key,original_roots=len(original_roots),original_definitions=len(definitions),records=len(actual_records),products=expected_products))
    return dict(status='PASS',profiles=results,default_authority=[],native_delivery_qualified=False,original_official_cases_run=0)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    print(json.dumps(verify(p.parse_args().source),indent=2))
