#!/usr/bin/env python3
"""Capture bounded public Leju branch/commit/tree metadata as inert input.

Run under owner_leju.py on SSH Linux. The complete bounded public branch
inventory is retained; source trees are selected explicitly. Installed and
third-party declarations must not silently become authored vendor APIs. ROS
version and native compatibility remain unclassified until the original
package/build/SDK closure is reviewed.
No controller, installer, simulation, Guest, or original build script is run.
"""
import argparse
import json
from pathlib import Path
import re
import time
import urllib.error
import urllib.request

REPOSITORY = 'LejuRobotics/kuavo-ros-opensource'
BASE = 'https://api.github.com/repos/' + REPOSITORY


def fetch(url):
    for attempt in range(3):
        try:
            request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-source-review'})
            with urllib.request.urlopen(request, timeout=25) as response:
                assert response.status == 200
                body = response.read((16 << 20) + 1)
            assert len(body) <= 16 << 20
            return body
        except (urllib.error.URLError, TimeoutError, ConnectionError):
            if attempt == 2:
                raise
            time.sleep(.5 * (attempt + 1))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    args.out.mkdir(mode=0o700)
    body = fetch(BASE)
    repository = json.loads(body)
    assert repository['full_name'] == REPOSITORY and not repository['private']
    (args.out / 'repository.json').write_bytes(body)
    branches = []
    for page in range(1, 6):
        body = fetch(BASE + '/branches?per_page=100&page=' + str(page))
        (args.out / ('branches-page-' + str(page) + '.json')).write_bytes(body)
        values = json.loads(body)
        assert isinstance(values, list) and len(values) <= 100
        branches.extend(values)
        if len(values) < 100:
            break
    else:
        raise AssertionError('Public branch inventory exceeds the explicit 500-branch review bound')
    assert branches and len({b['name'] for b in branches}) == len(branches)
    (args.out / 'branches.json').write_text(json.dumps(branches, indent=2) + '\n')
    # These are explicit source-review candidates, not inferred ROS support.
    # The upstream SDK describes master/beta and this mirror defaults to main.
    selected = {repository['default_branch'], 'main', 'master', 'beta'}
    selected.update(b['name'] for b in branches if re.fullmatch(
        r'(?:ros[12][-_])?(?:kinetic|melodic|noetic|foxy|galactic|humble|iron|jazzy|kilted|lyrical)(?:[-_]devel)?', b['name']))
    observed, seen = [], set()
    for branch in branches:
        if branch['name'] not in selected:
            continue
        commit_sha = branch['commit']['sha']
        assert re.fullmatch(r'[0-9a-f]{40}', commit_sha)
        safe = re.sub(r'[^a-z0-9]+', '_', branch['name'].lower()).strip('_')
        assert safe and safe not in seen
        seen.add(safe)
        directory = args.out / safe
        directory.mkdir()
        body = fetch(BASE + '/git/commits/' + commit_sha)
        commit = json.loads(body)
        assert commit['sha'] == commit_sha
        (directory / 'commit.json').write_bytes(body)
        tree_sha = commit['tree']['sha']
        assert re.fullmatch(r'[0-9a-f]{40}', tree_sha)
        body = fetch(BASE + '/git/trees/' + tree_sha + '?recursive=1')
        tree = json.loads(body)
        assert tree['sha'] == tree_sha and not tree.get('truncated')
        (directory / 'tree.json').write_bytes(body)
        declarations = [row for row in tree['tree'] if row['type'] == 'blob' and
                        Path(row['path']).suffix in ('.msg', '.srv', '.action')]
        packages = [row for row in tree['tree'] if row['type'] == 'blob' and
                    Path(row['path']).name == 'package.xml']
        row = dict(id=safe, repository=REPOSITORY, branch=branch['name'],
                   commit=commit_sha, tree=tree_sha, declarations=declarations,
                   packages=packages, ros_version=None, native_qualified=False)
        observed.append(row)
        print(json.dumps(dict(branch=branch['name'], commit=commit_sha,
                              declarations=len(declarations), packages=len(packages))), flush=True)
    report = dict(status='COMPLETE_PUBLIC_BRANCH_INVENTORY_AND_EXPLICIT_SELECTED_SOURCE_TREES_CAPTURED',
                  repository=REPOSITORY, default_branch=repository['default_branch'],
                  public_branch_count=len(branches), branches=observed,
                  unselected_branches=[b['name'] for b in branches if b['name'] not in selected],
                  default_authority=[], original_official_cases_run=0,
                  source_sdk_generated=False, native_driver_qualified=False)
    (args.out / 'discovery.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
