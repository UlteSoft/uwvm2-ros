#!/usr/bin/env python3
"""Check the retained maintenance view, truthful coverage and confinement.

These are authored navigation/refusal tests, not upstream driver test cases.
Actual compiler/import/default-denial tests are owned by layout_remaining.py.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    sdk = args.source.resolve(strict=True) / 'sdk/origin'
    assert not args.out.exists()
    args.out.mkdir()
    spec = importlib.util.spec_from_file_location('layout_under_test', sdk / 'tools/sdk_layout.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    checks = []

    def checked(name):
        checks.append(name)

    def refused(name, call):
        try:
            call()
        except (ValueError, AssertionError, FileNotFoundError, RuntimeError):
            checked(name)
        else:
            raise AssertionError('Invalid operation was accepted: ' + name)

    index = json.loads((sdk / 'platform/index.json').read_bytes())
    assert index['default_authority'] == [] and len(index['platforms']) == 31
    for vendor, row in index['platforms'].items():
        assert not row['complete_platform_support']
        assert set(row['languages']) == set(module.LANGUAGES)
        for lang, entry in row['languages'].items():
            assert entry['base_ros_sdk'] and not entry['complete_upstream_examples']
            assert entry['official_suite_cases_run'] == 0
            base = sdk / 'platform' / vendor / lang
            assert (base / 'include/base').samefile(sdk / module.LANGUAGES[lang][1])
            assert (base / 'example/base').samefile(sdk / 'examples' / module.LANGUAGES[lang][2])
            assert (base / 'test/base').samefile(sdk / 'language' / lang / 'test')
            for version in (1, 2):
                for family in row['ros'+str(version)+'_families']:
                    path = base / 'include' / ('ros'+str(version)) / family
                    if family in entry['typed_vendor_families']:
                        if lang=='python':
                            assert path.is_dir() and not path.is_symlink()
                            assert (path/'generation.python.json').is_file()
                        else:
                            assert path.samefile(sdk / 'custom-types' / family)
                    else:
                        assert not path.exists()
            checked(vendor + '/' + lang + '/source-identity-and-scope')
        coverage = json.loads((sdk / 'platform' / vendor / 'example-coverage.json').read_bytes())
        assert coverage['default_authority'] == [] and coverage['official_example_total'] is None
        assert coverage['candidate_source_files'] == len(coverage['candidates'])
        for tree in coverage['captured_trees']:
            raw = (sdk / tree['path']).read_bytes()
            assert hashlib.sha256(raw).hexdigest() == tree['sha256']
        for candidate in coverage['candidates']:
            assert set(candidate['languages']) == set(module.LANGUAGES)
            assert all(v == dict(status='NOT_MAPPED', wasm_source=None, test_receipt=None)
                       for v in candidate['languages'].values())
        checked(vendor + '/no-invented-example-or-official-test-credit')
    for name in ('/etc/passwd', '../outside', 'language/../../../outside', '', 'language/evil\x00name'):
        refused('unsafe-relative-path-'+repr(name), lambda: module.relative_path(name))
    fixture = args.out / 'fixture'
    fixture.mkdir()
    (fixture / 'library').mkdir()
    (fixture / 'library/source.txt').write_text('stable source\n')
    plan = module.Plan(fixture)
    plan.file('platform/index.json', '{}\n')
    plan.file('language/README.md', 'owned\n')
    plan.link('language/sample/include', 'library')
    plan.apply()
    plan.apply()
    assert plan.check()['copied_implementation_bytes'] == 0
    checked('relative-link-idempotence-and-zero-implementation-copy')
    prior=dict(files={name:dict(bytes=len(body),sha256=hashlib.sha256(body).hexdigest())
                      for name,body in plan.files.items()},links=plan.links)
    (fixture / 'language/README.md').write_text('user edited\n')
    refused('preserve-user-edited-source', plan.apply)
    refused('reviewed-update-still-refuses-user-edited-source',lambda:plan.apply(prior))
    refused('changed-source-check-fails', plan.check)
    (fixture / 'language/README.md').write_text('owned\n')
    link = fixture / 'language/sample/include'
    link.unlink()
    link.symlink_to('/etc')
    refused('refuse-foreign-link-overwrite', plan.apply)
    refused('refuse-escaping-link', plan.check)
    link.unlink()
    plan.apply()
    foreign = fixture / 'platform/foreign'
    foreign.symlink_to(args.out.resolve(), target_is_directory=True)
    bad = module.Plan(fixture)
    bad.file('platform/foreign/would-write.txt', 'no\n')
    refused('refuse-symlink-write-parent', bad.apply)
    assert not (args.out / 'would-write.txt').exists()
    foreign.unlink()
    (fixture / 'platform/extra.txt').write_text('untracked\n')
    refused('refuse-untracked-public-layout-file', plan.check)
    (fixture / 'platform/extra.txt').unlink()
    target = sdk / 'language/c/include'
    result = subprocess.run([sys.executable, '-I', '-S', '-B', sdk/'tools/sdk_layout.py',
        'resolve', '--sdk', sdk, '--path', 'language/c/include'], check=True, capture_output=True)
    assert Path(result.stdout.decode().strip()).samefile(target)
    checked('physical-project-resolution')
    # A candidate is not counted as a main, a port, a test or hardware coverage.
    kit = fixture / 'kit/upstream'
    kit.mkdir(parents=True)
    (kit / 'tree.json').write_text(json.dumps(dict(sha='1'*40, truncated=True, tree=[
        dict(path='examples/motion/main.cpp', type='blob', sha='2'*40, size=12),
        dict(path='examples/motion/helper.py', type='blob', sha='3'*40, size=14),
        dict(path='include/driver.h', type='blob', sha='4'*40, size=20)])))
    census = module.example_candidates(fixture, 'kit')
    assert census['candidate_source_files'] == 2 and census['official_example_total'] is None
    assert census['captured_trees'][0]['truncated'] is True
    assert all(v['role'] == 'UNREVIEWED_ENTRY_HELPER_OR_LAUNCH_CANDIDATE' for v in census['candidates'])
    checked('truncated-tree-and-helper-census-not-completion')
    # An API response can echo a commit ID rather than the root tree. Rebuild
    # the actual subtree/root objects; never relabel that echo as a Git tree.
    leaf=hashlib.sha1(b'tree 36\0'+b'100644 main.cpp\0'+bytes.fromhex('2'*40)).hexdigest()
    nodes=[dict(path='example',type='tree',mode='040000',sha=leaf),
           dict(path='example/main.cpp',type='blob',mode='100644',sha='2'*40,size=12)]
    (kit/'tree.json').write_text(json.dumps(dict(sha='1'*40,truncated=False,tree=nodes)))
    complete=module.example_candidates(fixture,'kit')['captured_trees'][0]
    assert complete['git_tree_reconstructed'] and complete['git_tree']!='1'*40
    assert complete['api_response_treeish']=='1'*40
    checked('reconstructed-root-not-api-commit-echo')
    nodes[0]['sha']='3'*40
    (kit/'tree.json').write_text(json.dumps(dict(sha='1'*40,truncated=False,tree=nodes)))
    refused('refuse-incomplete-or-altered-original-subtree',lambda:module.example_candidates(fixture,'kit'))
    report = dict(status='PUBLIC_LAYOUT_SOURCE_IDENTITY_AND_BOUNDARIES_PASS',
                  checks=checks, check_count=len(checks), default_authority=[],
                  original_official_cases_run=0)
    (args.out / 'models.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], len(checks))


if __name__ == '__main__':
    main()
