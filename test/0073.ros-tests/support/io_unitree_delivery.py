#!/usr/bin/env python3
"""Export only checked public SDK/case changes as inert file data.

This transport changes no compiler result or Runtime authority. Export requires
closed passing Linux owners. The existing receiver checks all previous bytes
before installing ordinary files or confined source links. Obsolete public
files are listed separately for a checked removal; no recursive delete occurs.
"""
import argparse
import base64
import gzip
import hashlib
import json
import os
from pathlib import Path
import sys


def fact(path):
    with path.open('rb')as stream:sha=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(bytes=path.stat().st_size,sha256=sha)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,required=True)
    args=parser.parse_args();root=args.root.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes());group=Path(inputs['cgroup']['path'])
    assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
    assert [group.stat().st_dev,group.stat().st_ino]==inputs['cgroup']['identity']
    assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==inputs['boot_id']
    assert (group/'memory.max').read_text().strip()==str(64<<30)and(group/'memory.swap.max').read_text().strip()=='0'
    accepted={}
    for name,status in (
            ('sdk-layout-unitree-verify-r2','UNITREE_PUBLIC_FIVE_LANGUAGE_DEFAULT_DENIAL_AND_SPORT_PAYLOAD_CHECKS_PASS'),
            ('sdk-layout-unitree-composition-r2','SIX_WHOLE_PROFILES_ONE_TU_C_AND_CPP_RUNTIME_AND_CANONICAL_VALUES_PASS'),
            ('sdk-layout-unitree-catalog-r1','UNITREE_PUBLIC_PROJECT_TEST_NUMBERED_WITH_STABLE_PREVIOUS_CASES')):
        owner=root/'out'/(name+'.json');result=root/'out'/name/'result.json'
        value=json.loads(owner.read_bytes());assert value['status']=='PASS'and all(v['exit']==0 for v in value['commands'])
        accepted[name]=json.loads(result.read_bytes());assert accepted[name]['status']==status
    sdk=Path(json.loads((root/inputs['unitree_sdk_prepared']/'result.json').read_bytes())['sdk'])
    before=root/'out/sdk-layout-projects-prepare-r6/sdk';assert not(sdk/'origin').exists()
    assert json.loads((root/'control/unitree-public-docs-r1/receipt.json').read_bytes())['previous_entries_unchanged']
    catalog=Path(accepted['sdk-layout-unitree-catalog-r1']['test_catalog'])
    old_catalog=root/inputs['unitree_previous_catalog'];rows={};paths={};removals=[]
    manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
    old_manifest=json.loads((before/'share/distribution-manifest.json').read_bytes())
    def add(path,name,old):
        if name in rows:return
        if path.is_symlink():
            target=os.readlink(path)
            assert name.startswith('test/0073.ros-tests/platform/')and not Path(target).is_absolute()
            if old.is_symlink()and os.readlink(old)==target:return
            row=dict(path=name,kind='confined-source-link',target=target)
        else:
            assert path.is_file();current=fact(path)
            if old.is_file()and not old.is_symlink():
                previous=fact(old)
                if previous==current:return
            else:previous=None
            row=dict(path=name,kind='ordinary-file',**current)
            if previous is not None:row['before']=previous
        rows[name]=row;paths[name]=path
    for row in manifest['entries']:
        path=sdk/row['path'];assert fact(path)=={k:row[k]for k in('bytes','sha256')}
        add(path,'sdk/'+row['path'],before/row['path'])
    for name in('share/sdk-index.json','share/distribution-manifest.json','README.md','xmake.lua','CMakeLists.txt'):
        add(sdk/name,'sdk/'+name,before/name)
    for path in sorted((sdk/'tools').rglob('*')):
        if path.is_file():add(path,'sdk/'+path.relative_to(sdk).as_posix(),before/path.relative_to(sdk))
    for path in sorted(catalog.rglob('*')):
        if path.is_file()or path.is_symlink():
            add(path,'test/0073.ros-tests/'+path.relative_to(catalog).as_posix(),old_catalog/path.relative_to(catalog))
    new_names={v['path']for v in manifest['entries']}
    for row in old_manifest['entries']:
        if row['path']not in new_names:
            assert fact(before/row['path'])=={k:row[k]for k in('bytes','sha256')}
            removals.append(dict(path='sdk/'+row['path'],before={k:row[k]for k in('bytes','sha256')}))
    removal=root/'control/unitree-public-obsolete-files-r1.json';assert not removal.exists()
    removal.write_text(json.dumps(dict(status='CHECKED_REMOVALS_PLANNED_NOT_EXECUTED',files=removals),indent=2)+'\n')
    header=dict(kind='accepted-platform-first-sdk-numbered-tests-inert-transport-v1',rows=[rows[k]for k in sorted(rows)],
        linux_verification=fact(root/'out/sdk-layout-unitree-catalog-r1/result.json'),
        linux_owner=fact(root/'out/sdk-layout-unitree-catalog-r1.json'),default_authority=[],
        local_compiler_or_tests_executed=0)
    with gzip.GzipFile(fileobj=sys.stdout.buffer,mode='wb',compresslevel=1,mtime=0)as stream:
        stream.write(json.dumps(header,sort_keys=True,separators=(',',':')).encode()+b'\n')
        for row in header['rows']:
            if row['kind']!='ordinary-file':continue
            body=paths[row['path']].read_bytes()
            assert dict(bytes=len(body),sha256=hashlib.sha256(body).hexdigest())=={k:row[k]for k in('bytes','sha256')}
            stream.write(base64.b64encode(body)+b'\n')
    print('CHECKED_PROJECT_DELTA_EXPORTED',len(rows),'OBSOLETE_PLANNED',len(removals),file=sys.stderr)


if __name__=='__main__':main()
