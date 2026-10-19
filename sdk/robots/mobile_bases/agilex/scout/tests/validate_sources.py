#!/usr/bin/env python3
"""Verify original SCOUT source closure and exact six-language products.

This read-only source check grants no authority and executes no generator,
compiler, ROS process or robot command. Git object association is provenance,
not a signature, installed Native TypeIdentity or physical compatibility proof.
"""
import argparse
import hashlib
import json
from pathlib import Path


def verify(source):
    source = Path(source).resolve(strict=True)
    kit = source / 'sdk/robots/mobile_bases/agilex/scout'
    results = []
    for version, count, records in ((1, 16, 17), (2, 15, 15)):
        root = kit / 'catalogs' / ('ros' + str(version))
        raw_catalog = (root / 'catalog.json').read_bytes()
        catalog = json.loads(raw_catalog)
        lock = json.loads((root / 'source-lock.json').read_text())
        assert lock['ros_version'] == version and lock['default_authority'] == []
        assert hashlib.sha256(raw_catalog).hexdigest() == lock['catalog_sha256']
        repos = {row['id']: row for row in lock['repositories']}
        assert len(repos) == len(lock['repositories'])
        trees = {}
        for key, row in repos.items():
            upstream = kit / 'upstream' / key
            commit = json.loads((upstream / 'commit.json').read_text())
            tree = json.loads((upstream / 'tree.json').read_text())
            capture = json.loads((upstream / 'capture.json').read_text())
            assert row['repository'] == capture['repository']
            assert commit['sha'] == capture['commit'] == row['commit']
            assert commit['commit']['tree']['sha'] == tree['sha'] == row['tree_git_sha1']
            assert not tree.get('truncated')
            trees[key] = {entry['path']: entry for entry in tree['tree'] if entry['type'] == 'blob'}
        definitions = {row['generation_file']: row for row in lock['definitions']}
        assert len(definitions) == count == len(lock['definitions']) == len(catalog['types'])
        assert {row['file'] for row in catalog['types']} == set(definitions)
        for row in catalog['types']:
            definition = definitions[row['file']]
            assert row['ros_version'] == version and row['type'] == definition['type']
            assert not Path(row['file']).is_absolute() and '..' not in Path(row['file']).parts
            body = (root / row['file']).read_bytes()
            original = trees[definition['repository']][definition['original_file']]
            assert len(body) == original['size'] == definition['bytes']
            assert hashlib.sha256(body).hexdigest() == definition['sha256']
            git = hashlib.sha1(b'blob ' + str(len(body)).encode('ascii') + b'\0' + body).hexdigest()
            assert git == original['sha'] == definition['git_blob_sha1']
            if row['type'].endswith('/Twist'):
                assert row['registered'] is False  # Existing builtin, never a second vendor endpoint.
        vendor = 'scout_ros' + str(version)
        all_custom = {name for name in trees[vendor] if name.startswith('scout_msgs/msg/') and name.endswith('.msg')}
        actual_custom = {row['original_file'] for row in definitions.values() if row['repository'] == vendor}
        assert actual_custom == all_custom
        generated = source / 'sdk/custom-types' / ('agilex-scout-ros' + str(version) + '-complete')
        proof = json.loads((generated / 'generation.vendor.json').read_text())
        assert proof['default_authority'] == proof['implicit_permissions'] == []
        assert proof['native_registry_emitted'] is False
        assert len(proof['records']) == records
        assert sum(map(len, proof['declared_constants'].values())) == 10
        # The preparer retains the original catalog and gives the generator a
        # sorted canonical JSON copy. Verify both identities independently.
        preparation = json.loads((generated / 'preparation.vendor.json').read_text())
        assert preparation['original_catalog']['sha256'] == lock['catalog_sha256']
        canonical = (json.dumps(catalog, indent=2, sort_keys=True, ensure_ascii=True,
                                allow_nan=False) + '\n').encode('ascii')
        assert proof['source_inputs']['catalog']['sha256'] == hashlib.sha256(canonical).hexdigest()
        assert {row['file'] for row in proof['source_inputs']['definitions']} == set(definitions)
        for row in proof['source_inputs']['definitions']:
            original = definitions[row['file']]
            assert row['bytes'] == original['bytes'] and row['sha256'] == original['sha256']
        for name, pin in proof['outputs'].items():
            assert not Path(name).is_absolute() and '..' not in Path(name).parts
            path = generated / name
            assert path.is_file() and not path.is_symlink()
            body = path.read_bytes()
            assert len(body) == pin['bytes'] and hashlib.sha256(body).hexdigest() == pin['sha256']
        for suffix in ('h', 'hpp', 'rs', 'mjs', 'ts', 'go'):
            assert 'bounded_records.' + suffix in proof['outputs']
        assert (kit / 'examples/go/customtypes' / ('records_ros' + str(version) + '.go')).read_bytes() == (generated / 'bounded_records.go').read_bytes()
        results.append(dict(ros=version,original_definitions=count,complete_records=records,
                            original_custom_messages=len(all_custom),constants=10,verified_products=len(proof['outputs'])))
    return dict(status='PASS',backends=results,default_authority=[],
                scope='Exact original full-message source and generated product integrity only; no execution or Native/hardware support is inferred.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    print(json.dumps(verify(parser.parse_args().source), indent=2))
