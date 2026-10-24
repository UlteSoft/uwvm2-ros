#!/usr/bin/env python3
"""Separate AgiBot original main/extra/prebuilt workspaces without aliasing.

Archive member paths, extent, CRC and SHA stay original. Prebuilt duplicate
declarations are retained but never silently substituted for source interfaces.
Main AimDK and the separate RL protocol package are independent source domains.
No native identity, installation or execution status is imported.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    a.out.mkdir(mode=0o700)
    capture = a.out / 'capture'
    shutil.copytree(inputs['remaining_capture'], capture)
    sdk_capture = Path(inputs['agibot_capture'])
    report = dict(status='RUNNING', domains=[], default_authority=[], upstream_code_executed=False)
    for original_lock in sorted(sdk_capture.glob('*/source-lock.json')):
        original = json.loads(original_lock.read_bytes())
        package_paths = [v['path'] for v in original['files'] if v['path'].endswith('/aimdk_msgs/package.xml')]
        selected = [v for v in package_paths if '/src/aimdk_msgs/' in v and '/prebuilt_' not in v or '/extra/x2_rl_deploy/aimdk_msgs/' in v]
        assert 1 <= len(selected) <= 2
        for path in selected:
            package_root = str(PurePosixPath(path).parent)
            scope = 'rl' if '/extra/' in path else 'main'
            key = original['id'] + '_' + scope
            target = capture / 'upstream/agibot' / key
            target.mkdir(parents=True)
            rows, retained_other = [], []
            for row in original['files']:
                name = row['path']
                if not (name == package_root + '/package.xml' or name.startswith(package_root + '/')):
                    continue
                if '/prebuilt_' in name:
                    retained_other.append(row)
                    continue
                body = (original_lock.parent / 'files' / name).read_bytes()
                assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
                destination = target / 'files' / name
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(body)
                rows.append(dict(row, source_kind='ORIGINAL_VENDOR_ZIP_MEMBER'))
            proof = dict(id=key, branch=key, repository='OFFICIAL_X2_AIMDK_DOWNLOAD_MANIFEST',
                commit=None, tree=None, source_kind='ORIGINAL_VENDOR_ZIP_MEMBERS', files=rows,
                source_proof=dict(label=original['label'], url=original['url'], archive_bytes=original['archive_bytes'],
                    archive_sha256=original['archive_sha256'], original_package_root=package_root,
                    scope=scope, original_zip_lock_sha256=hashlib.sha256(original_lock.read_bytes()).hexdigest()),
                retained_but_unselected_prebuilt_files=retained_other, default_authority=[], upstream_code_executed=False)
            (target / 'source-lock.json').write_text(json.dumps(proof, indent=2) + '\n')
            report['domains'].append(dict(profile=key, files=len(rows), scope=scope))
    report['status'] = 'ORIGINAL_AGIBOT_MAIN_RL_AND_PREBUILT_DOMAINS_SEPARATED_NO_NATIVE_OR_TEST_CREDIT'
    (a.out / 'normalization.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
