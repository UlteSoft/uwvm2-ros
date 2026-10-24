#!/usr/bin/env python3
"""Retain corrected Git root identities and retire untested draft source SDKs.

The initial GitHub top-level treeish was mislabeled as a root tree identity.
Original response bytes and prior authoring attempts stay retained. Every true
subtree is reconstructed before corrected metadata is emitted. All captured
source bodies remain byte-identical. Draft SDKs are renamed into this owner's
history, not deleted or counted as executed tests.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import shutil


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    spec = importlib.util.spec_from_file_location('remaining_exact_git_tree', Path(__file__).with_name('git_source_tree.py'))
    git = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(git)
    a.out.mkdir(mode=0o700)
    discovery, capture = a.out / 'discovery', a.out / 'capture'
    shutil.copytree(inputs['remaining_discovery'], discovery)
    shutil.copytree(inputs['remaining_capture'], capture)
    metadata = json.loads((discovery / 'discovery.json').read_bytes())
    changes = []
    for repository in metadata['repositories']:
        for profile in repository['selected']:
            document = json.loads((discovery / repository['id'] / profile['id'] / 'tree.json').read_bytes())
            actual = git.tree_identity(document)
            changes.append(dict(repository=repository['repository'], branch=profile['branch'],
                                requested_commit=profile['commit'], prior_mislabeled_tree=profile['tree'], actual_tree=actual))
            profile.update(api_response_treeish=document['sha'], tree=actual,
                           tree_identity_method='ALL_SUBTREES_AND_ROOT_RECONSTRUCTED_FROM_GIT_OBJECT_ENCODING')
            lock_path = capture / 'upstream' / repository['id'] / profile['id'] / 'source-lock.json'
            lock = json.loads(lock_path.read_bytes())
            lock.update(tree=actual, api_response_treeish=document['sha'],
                        tree_identity_method=profile['tree_identity_method'])
            lock_path.write_text(json.dumps(lock, indent=2) + '\n')
    (discovery / 'discovery.json').write_text(json.dumps(metadata, indent=2) + '\n')
    source = Path(inputs['source_root'])
    history = a.out / 'untested-draft-source'
    history.mkdir()
    for path in sorted((source / 'sdk/origin/robots/platforms').iterdir()):
        path.rename(history / path.name)
    for path in sorted((source / 'sdk/origin/custom-types').glob('*-complete')):
        path.rename(history / path.name)
    report = dict(status='ALL_GIT_SUBTREES_RECONSTRUCTED_AND_DRAFT_SOURCE_RETAINED_FOR_CORRECTED_REAUTHORING',
                  changes=changes, corrected_discovery=str(discovery), corrected_capture=str(capture),
                  default_authority=[], compiler_guest_or_native_runs=0)
    (a.out / 'repair.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
