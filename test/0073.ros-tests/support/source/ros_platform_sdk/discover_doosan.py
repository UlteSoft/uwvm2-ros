#!/usr/bin/env python3
"""Record public Doosan branch/commit/tree metadata as bounded inert input.

Run beneath a finite Linux owner in the original 64 GiB cgroup. This operator
downloads JSON data only. It executes no upstream installer, controller or
build instruction and grants no Guest authority. Every observed branch is
listed, including branches that cannot yet be admitted as source SDK domains.
"""
import argparse
import json
from pathlib import Path
import time
import urllib.error
import urllib.request


def fetch(url):
    for attempt in range(3):
        try:
            request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-source-review'})
            with urllib.request.urlopen(request, timeout=25) as response:
                body = response.read((8 << 20) + 1)
            assert len(body) <= 8 << 20
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
    observed = []
    for version, repository in [(1, 'DoosanRobotics/doosan-robot'),
                                (2, 'DoosanRobotics/doosan-robot2')]:
        base = 'https://api.github.com/repos/' + repository
        body = fetch(base + '/branches?per_page=100')
        branches = json.loads(body)
        assert isinstance(branches, list) and len(branches) < 100
        (args.out / ('branches-ros' + str(version) + '.json')).write_bytes(body)
        for branch in branches:
            key = 'ros' + str(version) + '_' + branch['name'].replace('/', '_').replace('-', '_').lower()
            directory = args.out / key
            directory.mkdir()
            body = fetch(base + '/commits/' + branch['commit']['sha'])
            commit = json.loads(body)
            (directory / 'commit.json').write_bytes(body)
            body = fetch(base + '/git/trees/' + commit['commit']['tree']['sha'] + '?recursive=1')
            tree = json.loads(body)
            (directory / 'tree.json').write_bytes(body)
            assert not tree.get('truncated')
            definitions = [r['path'] for r in tree['tree']
                           if r['type'] == 'blob' and Path(r['path']).suffix in ('.msg', '.srv', '.action')]
            row = dict(id=key, repository=repository, branch=branch['name'], ros_version=version,
                       commit=commit['sha'], tree=tree['sha'], definitions=definitions)
            observed.append(row)
            print(json.dumps(dict(row, definitions=len(definitions))), flush=True)
    (args.out / 'discovery.json').write_text(json.dumps(observed, indent=2) + '\n')


if __name__ == '__main__':
    main()
