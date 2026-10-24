#!/usr/bin/env python3
"""Export closed Unitree native acceptance navigation with inert preimage pins."""
import argparse
import base64
import gzip
import hashlib
import json
import os
from pathlib import Path
import sys


def fact(path):
    body=path.read_bytes();return dict(bytes=len(body),sha256=hashlib.sha256(body).hexdigest())


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,required=True)
    root=p.parse_args().root.resolve(strict=True);inputs=json.loads((root/'inputs.json').read_bytes())
    g=Path(inputs['cgroup']['path'])
    assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
    assert[g.stat().st_dev,g.stat().st_ino]==inputs['cgroup']['identity']
    assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==inputs['boot_id']
    assert(g/'memory.max').read_text().strip()==str(64<<30)and(g/'memory.swap.max').read_text().strip()=='0'
    for path in(root/'out').glob('sdk-layout-*.json'):
        row=json.loads(path.read_bytes());assert row['status']!='RUNNING'and all(c['exit']is not None for c in row['commands'])
    audit=root/'out/sdk-layout-native-audit-r1';catalog=root/'out/sdk-layout-native-catalog-r2'
    assert json.loads((audit/'result.json').read_bytes())['status']=='INDEPENDENT_UNITREE_NATIVE_PACKAGE_AND_DDS_EVIDENCE_PASS'
    assert json.loads((catalog/'result.json').read_bytes())['status']=='UNITREE_NATIVE_SOURCE_CATALOG_PASS'
    for path in(audit,catalog):assert json.loads(path.with_suffix('.json').read_bytes())['status']=='PASS'
    old=root/'out/sdk-layout-sport-catalog-r1/ros-tests';new=catalog/'ros-tests';rows=[];files={}
    for path in sorted(new.rglob('*')):
        if not(path.is_file()or path.is_symlink()):continue
        relative=path.relative_to(new);previous=old/relative;name='test/0073.ros-tests/'+relative.as_posix()
        if path.is_symlink():
            target=os.readlink(path)
            if previous.is_symlink()and os.readlink(previous)==target:continue
            assert not previous.exists()and not previous.is_symlink()
            row=dict(path=name,kind='confined-source-link',target=target)
        else:
            after=fact(path);before=fact(previous)if previous.is_file()else None
            if after==before:continue
            row=dict(path=name,kind='ordinary-file',**after)
            if before:row['before']=before
        rows.append(row);files[name]=path
    tool=root/'out/sdk-layout-sport-prepare-r3/sdk/tools/project_paths.py'
    assert tool.read_bytes()==(Path(inputs['source_root'])/'sdk/tools/project_paths.py').read_bytes()
    name='sdk/tools/project_paths.py';rows.append(dict(path=name,kind='ordinary-file',**fact(tool),
        before=fact(root/'control/reproduce-path-scope-r1/project_paths-before.py')));files[name]=tool
    rows.sort(key=lambda row:row['path']);assert len(rows)==10
    header=dict(kind='accepted-platform-first-sdk-numbered-tests-inert-transport-v1',rows=rows,
        linux_verification=fact(audit/'result.json'),linux_owner=fact(audit.with_suffix('.json')),
        default_authority=[],local_compiler_or_tests_executed=0)
    plan=root/'control/native-delivery-plan-r1.json';assert not plan.exists();plan.write_text(json.dumps(header,indent=2)+'\n')
    with gzip.GzipFile(fileobj=sys.stdout.buffer,mode='wb',mtime=0,compresslevel=1)as stream:
        stream.write(json.dumps(header,separators=(',',':')).encode()+b'\n')
        for row in rows:
            if row['kind']!='ordinary-file':continue
            assert fact(files[row['path']])=={k:row[k]for k in('bytes','sha256')}
            stream.write(base64.b64encode(files[row['path']].read_bytes())+b'\n')
    print('CHECKED_NATIVE_DELTA_EXPORTED',len(rows),file=sys.stderr)


if __name__=='__main__':main()
