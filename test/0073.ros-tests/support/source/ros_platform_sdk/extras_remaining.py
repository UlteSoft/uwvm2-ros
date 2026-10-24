#!/usr/bin/env python3
"""Capture narrowly selected official source references as inert bytes.

JavaScript is inspected as text and is never evaluated. A download URL is not
an SDK acceptance or a license grant. Extra robot archives/installers are not
run. Original ROS action parser bytes establish grammar rather than a guessed
normalization of the vendor's declarations.
"""
import argparse
import base64
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import re
import tarfile


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    a.out.mkdir(mode=0o700)
    spec = importlib.util.spec_from_file_location('remaining_extra_fetch', Path(__file__).with_name('discover_remaining.py'))
    fetcher = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(fetcher)
    spec = importlib.util.spec_from_file_location('remaining_extra_git_tree', Path(__file__).with_name('git_source_tree.py'))
    git = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(git)
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    prior = Path(inputs['remaining_extras_reuse']) if inputs.get('remaining_extras_reuse') else None
    report = dict(status='RUNNING', objects=[], urls=[], default_authority=[], upstream_code_executed=False)
    def save(name, url):
        if prior and (prior / name).is_file():
            body = (prior / name).read_bytes()
            expected = next(v for v in json.loads((prior / 'extras.json').read_bytes())['objects'] if v['path'] == name)
            assert expected['url'] == url and len(body) == expected['bytes'] and hashlib.sha256(body).hexdigest() == expected['sha256']
        else:
            body, headers = fetcher.fetch(url, 4 << 20)
        (a.out / name).write_bytes(body)
        report['objects'].append(dict(path=name, url=url, bytes=len(body), sha256=hashlib.sha256(body).hexdigest()))
        return body
    try:
        body = save('agibot-lang-ver-nav.js', 'https://x2-aimdk.agibot.com/lang-ver-nav.js')
        assert "'/sdk-downloads.json'" in body.decode()
        save('agibot-sdk-downloads.json', 'https://x2-aimdk.agibot.com/sdk-downloads.json')
        report['urls'] = sorted(set(re.findall(r'https?://[^\s\x22\x27<>`]+', body.decode())))
        base = 'https://api.github.com/repos/ros/common_msgs'
        ref = json.loads(save('ros-action-branch.json', base + '/git/ref/heads/noetic-devel'))
        assert ref['object']['type'] == 'commit'
        commit = ref['object']['sha']
        tree = json.loads(save('ros-action-tree.json', base + '/git/trees/' + commit + '?recursive=1'))
        assert not tree.get('truncated')
        row = next(v for v in tree['tree'] if v['path'] == 'actionlib_msgs/scripts/genaction.py')
        original = json.loads(save('ros-action-blob.json', base + '/git/blobs/' + row['sha']))
        assert original['encoding'] == 'base64'
        raw = base64.b64decode(original['content'])
        assert len(raw) == row['size']
        assert hashlib.sha1(b'blob ' + str(len(raw)).encode() + b'\0' + raw).hexdigest() == row['sha']
        (a.out / 'genaction.py.source').write_bytes(raw)
        report['action_parser'] = dict(repository='ros/common_msgs', branch='noetic-devel', commit=commit,
            tree=git.tree_identity(tree), api_response_treeish=tree['sha'], original_path=row['path'], git_blob=row['sha'],
            sha256=hashlib.sha256(raw).hexdigest(), bytes=len(raw), upstream_code_executed=False)
        base = 'https://api.github.com/repos/ahornung/humanoid_msgs'
        ref = json.loads(save('humanoid-nav-branch.json', base + '/git/ref/heads/devel'))
        assert ref['object']['type'] == 'commit'
        commit = ref['object']['sha']
        tree = json.loads(save('humanoid-nav-tree.json', base + '/git/trees/' + commit + '?recursive=1'))
        actual_tree = git.tree_identity(tree)
        rows = {v['path']: v for v in tree['tree'] if v['type'] == 'blob'}
        selected = {name for name in rows if name.startswith('humanoid_nav_msgs/') and
                    (name.endswith('.msg') or Path(name).name in ('package.xml', 'CMakeLists.txt'))}
        selected.update(name for name in rows if Path(name).name.upper().startswith(('LICENSE', 'COPYING', 'NOTICE')))
        assert selected and all(rows[n]['mode'] in ('100644', '100755') and rows[n]['size'] <= 1 << 20 for n in selected)
        archive, headers = fetcher.fetch('https://codeload.github.com/ahornung/humanoid_msgs/tar.gz/' + commit, 8 << 20)
        files = []
        with tarfile.open(fileobj=io.BytesIO(archive), mode='r:gz') as stream:
            for member in stream:
                parts = Path(member.name).parts
                assert parts[0] == 'humanoid_msgs-' + commit and '..' not in parts
                name = '/'.join(parts[1:])
                if name not in selected:
                    continue
                expected = rows[name]
                assert member.isfile() and member.size == expected['size']
                body = stream.extractfile(member).read((1 << 20) + 1)
                assert len(body) == expected['size'] and hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == expected['sha']
                target = a.out / 'humanoid-nav/files' / name
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(body)
                files.append(dict(path=name, bytes=len(body), sha256=hashlib.sha256(body).hexdigest(), git_blob=expected['sha']))
        assert {v['path'] for v in files} == selected
        report['humanoid_nav_dependency'] = dict(repository='ahornung/humanoid_msgs', branch='devel', commit=commit,
            tree=actual_tree, api_response_treeish=tree['sha'], files=files,
            archive_sha256=hashlib.sha256(archive).hexdigest(), archive_bytes=len(archive), upstream_code_executed=False)
        report['status'] = 'OFFICIAL_EXTRA_SOURCE_BYTES_CAPTURED_WITHOUT_EVALUATION'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (a.out / 'extras.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
