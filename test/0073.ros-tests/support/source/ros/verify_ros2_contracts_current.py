#!/usr/bin/env python3
"""Bind current scoped files to the frozen main run and explicit tested revisions.

This is a read-only join of independently checked evidence. It never rebuilds,
runs ROS, repairs a report, or treats a post-freeze fixture as a main build input.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path, PurePosixPath
import re


def digest(path):
    h = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1048576), b''):
            h.update(block)
    return h.hexdigest()


def production(name):
    return name.startswith(('src/ros/gateway/', 'sdk/origin/common/', 'src/uwvm2/imported/ros/',
                            'src/uwvm2/uwvm/imported/ros/'))


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--main-report', type=Path, required=True)
    p.add_argument('--main-verification', type=Path, required=True)
    p.add_argument('--revisions', type=Path, required=True)
    p.add_argument('--task-files', type=Path, required=True)
    p.add_argument('--protected', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    if a.out.exists():
        p.error('fresh output required')
    counts = collections.Counter()
    files = {}
    root = a.root.resolve()

    def check(value, category, message):
        counts[category] += 1
        if not value:
            raise RuntimeError(message)

    def read(path):
        path = Path(path)
        files[str(path)] = digest(path)
        return json.loads(path.read_bytes())

    def safe(name):
        check(isinstance(name, str) and name and not name.startswith('/')
              and '\\' not in name and all(x not in ('', '.', '..') for x in name.split('/'))
              and str(PurePosixPath(name)) == name, 'paths', 'unsafe scoped path')

    def sha(value):
        check(isinstance(value, str) and re.fullmatch('[0-9a-f]{64}', value),
              'hashes', 'invalid SHA256')

    main_report, proof = read(a.main_report), read(a.main_verification)
    check(main_report['status'] == proof['status'] == 'PASS', 'main', 'main evidence not PASS')
    check(proof['source_report_sha256'] == digest(a.main_report), 'main', 'main proof/report differs')
    check(proof['worker_dependency_files'] == 12 and proof['native_targets'] == 18
          and proof['a15_suites'] == 15, 'main', 'complete main scope missing')
    expected = {n: h for n, h in proof['compiled_project_inputs'].items() if production(n)}
    expected.update(main_report['overlay_inputs'])
    revisions = read(a.revisions)
    check(revisions['status'] == 'TESTED_REVISIONS', 'revisions', 'revision inventory not closed')
    check(revisions['main_report_sha256'] == digest(a.main_report), 'revisions', 'revision main binding differs')
    accepted = []
    used = set()
    for row in revisions['files']:
        name = row['path'];safe(name)
        check(name not in used and name.startswith('test/0073.ros-tests/support/source/ros'), 'revisions', 'duplicate or non-test revision')
        used.add(name);sha(row['sha256'])
        old = expected.get(name)
        check(row['main_frozen_sha256'] == old, 'revisions', 'revision hides another main input')
        independent = read(row['verification_file'])
        check(digest(row['verification_file']) == row['verification_sha256']
              and independent['status'] == 'PASS', 'revisions', 'independent revision proof differs')
        inputs = independent['current_scoped_inputs']
        check(inputs.get(name) == row['sha256'], 'revisions', 'revision is not covered by actual evidence')
        expected[name] = row['sha256']
        accepted.append({'path': name, 'main_frozen_sha256': old,
                         'current_sha256': row['sha256'], 'verification_sha256': row['verification_sha256']})

    for name, value in sorted(expected.items()):
        safe(name);sha(value)
        check(digest(root/name) == value, 'current', 'current tested source differs: '+name)
    manifest = read(a.task_files)
    rows = manifest['files']
    check(len({v['path'] for v in rows}) == len(rows), 'manifest', 'duplicate task manifest path')
    current = {}
    for row in rows:
        name = row['path'];safe(name);sha(row['sha256'])
        check((root/name).stat().st_size == row['bytes'] and digest(root/name) == row['sha256'],
              'manifest', 'task file bytes differ: '+name)
        current[name] = row['sha256']
    check(all(current.get(n) == value for n, value in expected.items()), 'manifest', 'tested current inputs omitted')
    protected = read(a.protected)
    check(len(protected) == 39, 'preservation', 'protected prior delivery count differs')
    for name, value in protected.items():
        safe(name);sha(value)
        check(digest(root/name) == value, 'preservation', 'prior delivery changed: '+name)
    result = {'status': 'PASS', 'scope': 'Current scoped ROS files, immutable full main run, explicit tested post-freeze test revisions and 39 prior deliveries; no extra runtime execution claim',
              'executed_verifier_sha256': digest(__file__), 'checks': sum(counts.values()),
              'checks_by_category': dict(counts), 'main_report_sha256': digest(a.main_report),
              'main_verification_sha256': digest(a.main_verification),
              'current_scoped_inputs': expected, 'tested_post_freeze_revisions': accepted,
              'current_task_files': len(rows), 'protected_prior_files': len(protected),
              'retained_file_sha256': files}
    a.out.write_text(json.dumps(result, ensure_ascii=False, indent=2)+'\n')
    print(json.dumps({'status': 'PASS', 'checks': result['checks'], 'files': len(expected)}))


if __name__ == '__main__':
    main()
