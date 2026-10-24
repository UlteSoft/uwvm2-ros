#!/usr/bin/env python3
"""Capture public source metadata for the six outstanding vendor families.

Run only through the bounded Linux owner. Inventories preserve every public
branch within a declared bound; selection is explicit and never implies a
ROS release, working driver or controller acceptance. No downloaded program,
installer, model or SDK binary is executed. Original HTTP bodies are retained.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import time
import urllib.error
import urllib.request

REPOSITORIES = [
    ('deep', 'DeepRoboticsLab/sdk_deploy'),
    ('pal', 'pal-robotics/pal_msgs'),
    ('pal_motion', 'pal-robotics/play_motion_msgs'),
    ('pal_tiago', 'pal-robotics/tiago_robot'),
    ('interbotix', 'Interbotix/interbotix_ros_core'),
    ('interbotix_models', 'Interbotix/interbotix_ros_manipulators'),
    ('ubtech', 'UBTECH-Robot/tk_sdk_demo'),
    ('fourier', 'FFTAI/fourier_aurora_sdk'),
]
DOCS = [
    ('agibot_sdk', 'https://x2-aimdk.agibot.com/en/latest/get_sdk/index.html'),
    ('agibot_interfaces', 'https://x2-aimdk.agibot.com/en/latest/Interface/index.html'),
]


def fetch(url, bound=16 << 20):
    for attempt in range(3):
        try:
            req = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-selected-source-review'})
            with urllib.request.urlopen(req, timeout=25) as response:
                assert response.status == 200
                body = response.read(bound + 1)
                headers = dict(response.headers)
            assert len(body) <= bound, ('HTTP object bound', url)
            return body, headers
        except (urllib.error.URLError, TimeoutError, ConnectionError):
            if attempt == 2:
                raise
            time.sleep(.5 * (attempt + 1))


def save(path, body):
    path.write_bytes(body)
    return dict(path=str(path), bytes=len(body), sha256=hashlib.sha256(body).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location('remaining_git_tree', Path(__file__).with_name('git_source_tree.py'))
    git = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(git)
    out = args.out
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', repositories=[], documents=[], default_authority=[], original_official_cases_run=0)
    inputs = json.loads((args.root / 'inputs.json').read_bytes())
    prior_dir = Path(inputs['remaining_discovery_reuse']) if inputs.get('remaining_discovery_reuse') else None
    prior = json.loads((prior_dir / 'discovery.json').read_bytes()) if prior_dir else {}
    try:
        for key, repository in REPOSITORIES:
            base = 'https://api.github.com/repos/' + repository
            directory = out / key
            directory.mkdir()
            old = next((v for v in prior.get('repositories', []) if v['id'] == key and v.get('selected')), None)
            if old:
                for path in (prior_dir / key).rglob('*'):
                    if path.is_file():
                        target = directory / path.relative_to(prior_dir / key)
                        target.parent.mkdir(parents=True, exist_ok=True)
                        target.write_bytes(path.read_bytes())
                old['reused_exact_metadata_from'] = str(prior_dir / key)
                for profile in old['selected']:
                    tree = json.loads((directory / profile['id'] / 'tree.json').read_bytes())
                    profile.update(tree=git.tree_identity(tree), api_response_treeish=tree['sha'],
                                   tree_identity_method='ALL_SUBTREES_AND_ROOT_RECONSTRUCTED_FROM_GIT_OBJECT_ENCODING')
                report['repositories'].append(old)
                continue
            try:
                body, headers = fetch(base)
            except urllib.error.HTTPError as error:
                if error.code != 404:
                    raise
                report['repositories'].append(dict(id=key, repository=repository, unavailable_http_status=404, selected=[]))
                print(json.dumps(dict(repository=repository, unavailable_http_status=404)), flush=True)
                continue
            repo = json.loads(body)
            assert repo['full_name'].lower() == repository.lower() and not repo['private']
            save(directory / 'repository.json', body)
            branches = []
            for page in range(1, 6):
                body, headers = fetch(base + '/branches?per_page=100&page=' + str(page))
                save(directory / ('branches-' + str(page) + '.json'), body)
                page_rows = json.loads(body)
                assert isinstance(page_rows, list) and len(page_rows) <= 100
                branches.extend(page_rows)
                if len(page_rows) < 100:
                    break
            else:
                raise AssertionError('Explicit 500-branch public inventory bound')
            assert branches and len({b['name'] for b in branches}) == len(branches)
            selected = {repo['default_branch'], 'master', 'main', 'ros1', 'ros2'}
            selected.update(b['name'] for b in branches if re.fullmatch(
                r'(?:ros[12][-_])?(?:indigo|kinetic|melodic|noetic|foxy|galactic|humble|iron|jazzy|kilted|lyrical)(?:[-_]devel)?', b['name']))
            row = dict(id=key, repository=repo['full_name'], default_branch=repo['default_branch'],
                       branches=branches, selected=[], unselected=[b['name'] for b in branches if b['name'] not in selected])
            for branch in branches:
                if branch['name'] not in selected:
                    continue
                commit = branch['commit']['sha']
                assert re.fullmatch('[a-f0-9]{40}', commit)
                safe = re.sub('[^a-z0-9]+', '_', branch['name'].lower()).strip('_')
                branch_dir = directory / safe
                branch_dir.mkdir()
                body, headers = fetch(base + '/git/trees/' + commit + '?recursive=1')
                tree = json.loads(body)
                assert re.fullmatch('[a-f0-9]{40}', tree['sha']) and not tree.get('truncated')
                save(branch_dir / 'tree.json', body)
                declarations = [v for v in tree['tree'] if v['type'] == 'blob' and Path(v['path']).suffix in ('.msg', '.srv', '.action', '.idl', '.proto')]
                branch_row = dict(id=safe, branch=branch['name'], commit=commit, tree=git.tree_identity(tree),
                                  api_response_treeish=tree['sha'], declarations=declarations,
                                  tree_identity_method='ALL_SUBTREES_AND_ROOT_RECONSTRUCTED_FROM_GIT_OBJECT_ENCODING')
                row['selected'].append(branch_row)
                print(json.dumps(dict(repository=repository, branch=branch['name'], declarations=len(declarations),
                                      api_remaining=headers.get('X-RateLimit-Remaining'))), flush=True)
            report['repositories'].append(row)
            (out / 'discovery.json').write_text(json.dumps(report, indent=2) + '\n')
        for key, url in DOCS:
            try:
                body, headers = fetch(url, 4 << 20)
                report['documents'].append(dict(id=key, url=url, **save(out / (key + '.html'), body)))
            except (urllib.error.URLError, TimeoutError, ConnectionError) as error:
                report['documents'].append(dict(id=key, url=url, unavailable=repr(error)))
        report['status'] = 'BOUNDED_PUBLIC_INVENTORY_AND_SELECTED_SOURCE_TREES_CAPTURED_NO_SDK_OR_NATIVE_CREDIT'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'discovery.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
