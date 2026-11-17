#!/usr/bin/env python3
"""Build a per-package external-effect review queue from acquired source scans.

Classification is an automated hint. A file containing an API name may be a
test, comment, wrapper or unreachable path. No row produced here is a completed
semantic review, grant recommendation or successful portability test.
"""
from collections import Counter, defaultdict
from pathlib import Path, PurePosixPath
import argparse
import json
from acquire import identified_package, save


def package_owner(filename, packages):
    path = PurePosixPath(filename)
    matching = [p for p in packages if path.is_relative_to(PurePosixPath(p['path']).parent)]
    return max(matching, key=lambda p: len(PurePosixPath(p['path']).parts), default=None)


def inventory(root):
    receipts = {}
    for tree in (root, root / 'supplemental', root / 'recovery'):
        for path in (tree / 'receipts').glob('*.json'):
            row = json.loads(path.read_text())
            if row['status'] == 'ACQUIRED_AND_SCANNED':
                receipts[path.stem] = (tree, row)
    rows, unowned, total = [], [], Counter()
    for key, (tree, receipt) in sorted(receipts.items()):
        scan = json.loads((tree / 'scans' / (key + '.json')).read_text())
        packages = [p for p in scan['packages'] if identified_package(p)]
        effects = defaultdict(Counter)
        samples = defaultdict(list)
        for finding in scan['findings']:
            owner = package_owner(finding['file'], packages)
            label = (owner['name'], owner['path']) if owner else ('<unowned>', '')
            effects[label][finding['category']] += 1
            total[finding['category']] += 1
            if len(samples[label]) < 12:
                samples[label].append(finding)
        for package in packages:
            label = (package['name'], package['path'])
            rows.append(dict(source_id=key, package=package['name'], manifest=package['path'],
                             source_url=receipt['source']['url'], source_ref=receipt['source']['ref'],
                             archive_sha256=receipt['archive']['sha256'],
                             dependency_names=package['dependencies'], effects=dict(effects[label]),
                             finding_samples=samples[label], semantic_review='PENDING', portability='NOT_TESTED'))
        if effects[('<unowned>', '')]:
            unowned.append(dict(source_id=key, effects=dict(effects[('<unowned>', '')]),
                                finding_samples=samples[('<unowned>', '')]))
    summary = dict(scope='AUTOMATED_EXTERNAL_EFFECT_DISCOVERY_ONLY', acquired_snapshots=len(receipts),
                   package_source_instances=len(rows), package_names=len({r['package'] for r in rows}),
                   package_instances_with_effects=sum(bool(r['effects']) for r in rows),
                   finding_counts=dict(total), unowned_source_groups=len(unowned),
                   semantic_review='PENDING', portability='NOT_TESTED')
    save(root / 'reconciliation/package-effect-inventory.json', rows)
    save(root / 'reconciliation/unowned-source-effects.json', unowned)
    save(root / 'reconciliation/effect-summary.json', summary)
    return summary


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(inventory(args.root.resolve()), indent=2))


if __name__ == '__main__':
    main()
