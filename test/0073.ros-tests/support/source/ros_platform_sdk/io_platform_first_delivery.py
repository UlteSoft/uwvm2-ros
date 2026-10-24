#!/usr/bin/env python3
"""Transport the accepted SDK and numbered cases using checked, inert file IO.

Export runs inside the original Linux cgroup after a closed passing owner.
Receive only hashes, stages and installs ordinary files and confined source
links. It never imports a delivered SDK, invokes a compiler or runs a test.
Existing foreign bytes are refused, including edited migration receipts.
"""
import argparse
import base64
import gzip
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile


def pin(body):return dict(bytes=len(body),sha256=hashlib.sha256(body).hexdigest())
def encoded(value):return(json.dumps(value,indent=2,sort_keys=True)+'\n').encode()


def safe(name):
    p=Path(name)
    if p.is_absolute()or '..'in p.parts or '\\'in name or any(ord(c)<32 for c in name):return False
    if p.parts[:1]==('sdk',):
        return len(p.parts)>2 and p.parts[1]in('include','lib','example','test','share','tools','origin')or name in('sdk/README.md','sdk/xmake.lua','sdk/CMakeLists.txt')
    return p.parts[:2]==('test','0073.ros-tests')and(len(p.parts)>3 and p.parts[2]=='platform'or p.parts[2:]==('index.json',))


def export(root,verification):
    root=root.resolve(strict=True);inputs=json.loads((root/'inputs.json').read_bytes())
    group=Path(inputs['cgroup']['path'])
    assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
    assert [group.stat().st_dev,group.stat().st_ino]==inputs['cgroup']['identity']
    assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==inputs['boot_id']
    assert (group/'memory.max').read_text().strip()==str(64<<30)and(group/'memory.swap.max').read_text().strip()=='0'
    owner=root/'out'/(verification+'.json');owner_value=json.loads(owner.read_bytes())
    assert owner_value['status']=='PASS'and all(c['exit']==0 for c in owner_value['commands'])
    result=root/'out'/verification/'result.json';accepted=json.loads(result.read_bytes())
    assert accepted['status']=='PLATFORM_FIRST_SDK_HEADER_SYSROOT_AND_REAL_GUEST_CHECKS_PASS'
    prepared=root/inputs['sdk_prepared'];state=json.loads((prepared/'result.json').read_bytes())
    sdk=Path(state['sdk']);catalog=Path(state['test_catalog']);source=Path(inputs['source_root'])
    manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes());rows={};files={}
    def add(path,name,before=None):
        assert safe(name)and name not in rows
        if path.is_symlink():
            assert name.startswith('test/0073.ros-tests/platform/')
            target=os.readlink(path);relative=Path(os.path.normpath(str(Path(name).parent/target)))
            assert not Path(target).is_absolute()and relative.parts[:4]==('test','0073.ros-tests','support','source')
            row=dict(path=name,kind='confined-source-link',target=target)
        else:
            row=dict(path=name,kind='ordinary-file',**pin(path.read_bytes()))
            if before is not None:row['before']=before
        rows[name]=row;files[name]=path
    for row in manifest['entries']:
        path=sdk/row['path'];assert pin(path.read_bytes())=={k:row[k]for k in('bytes','sha256')}
        add(path,'sdk/'+row['path'])
    for name in('share/sdk-index.json','share/distribution-manifest.json','README.md','xmake.lua','CMakeLists.txt'):
        add(sdk/name,'sdk/'+name)
    for path in sorted((sdk/'tools').rglob('*')):
        if path.is_file():add(path,'sdk/'+path.relative_to(sdk).as_posix())
    for path in sorted(catalog.rglob('*')):
        if path.is_file()or path.is_symlink():add(path,'test/0073.ros-tests/'+path.relative_to(catalog).as_posix())
    reconciliation=json.loads((root/'out/sdk-layout-prepare-r1/source-receipts/reconciliation.json').read_bytes())
    for row in reconciliation['changes']:
        path=source/row['path'];assert pin(path.read_bytes())==row['after']
        add(path,row['path'],row['before'])
    ordered=[rows[n]for n in sorted(rows)]
    header=dict(kind='accepted-platform-first-sdk-numbered-tests-inert-transport-v1',rows=ordered,
        linux_verification=pin(result.read_bytes()),linux_owner=pin(owner.read_bytes()),
        default_authority=[],local_compiler_or_tests_executed=0)
    with gzip.GzipFile(fileobj=sys.stdout.buffer,mode='wb',compresslevel=1,mtime=0)as stream:
        stream.write(encoded(header).replace(b'\n',b'')+b'\n')
        for row in ordered:
            if row['kind']!='ordinary-file':continue
            body=files[row['path']].read_bytes();assert pin(body)=={k:row[k]for k in('bytes','sha256')}
            stream.write(base64.b64encode(body)+b'\n')
    print('ACCEPTED_LAYOUT_EXPORTED',len(ordered),file=sys.stderr)


def receive(source,receipt):
    source=source.resolve(strict=True);receipt=receipt.absolute();assert not receipt.exists()
    stage=Path(tempfile.mkdtemp(prefix='uwvm-platform-first-delivery-',dir='/tmp'))
    report=dict(status='STAGING',stage=str(stage),local_compiler_or_tests_executed=0,default_authority=[])
    def save():receipt.parent.mkdir(parents=True,exist_ok=True);receipt.write_bytes(encoded(report))
    try:
        with gzip.GzipFile(fileobj=sys.stdin.buffer,mode='rb')as stream:
            raw=stream.readline((16<<20)+1);assert len(raw)<=16<<20
            header=json.loads(raw);rows=header['rows'];names=set();total=0
            assert header['kind']=='accepted-platform-first-sdk-numbered-tests-inert-transport-v1'
            assert header['default_authority']==[]and len(rows)<40000
            for row in rows:
                name=row['path'];assert safe(name)and name not in names;names.add(name)
                destination=source/name
                for parent in destination.parents:
                    if parent==source:break
                    assert not parent.is_symlink()
                else:raise ValueError('destination escaped source')
                staged=stage/'after'/name;staged.parent.mkdir(parents=True,exist_ok=True)
                if row['kind']=='confined-source-link':
                    target=row['target'];relative=Path(os.path.normpath(str(Path(name).parent/target)))
                    assert not Path(target).is_absolute()and relative.parts[:4]==('test','0073.ros-tests','support','source')
                    assert (source/relative).resolve(strict=True).is_relative_to(source/'test/0073.ros-tests/support/source')
                    if destination.exists()or destination.is_symlink():
                        assert destination.is_symlink()and os.readlink(destination)==target
                    staged.symlink_to(target);continue
                assert row['kind']=='ordinary-file'and 0<=row['bytes']<=24<<20
                total+=row['bytes'];assert total<=2<<30
                line=stream.readline((32<<20)+1);assert len(line)<=32<<20 and line.endswith(b'\n')
                body=base64.b64decode(line.rstrip(b'\n'),validate=True)
                assert pin(body)=={k:row[k]for k in('bytes','sha256')}
                staged.write_bytes(body)
                if destination.exists()or destination.is_symlink():
                    assert destination.is_file()and not destination.is_symlink()
                    previous=destination.read_bytes();expected={k:row[k]for k in('bytes','sha256')}
                    assert pin(previous)in(expected,row.get('before')),('foreign destination bytes',name)
                    if previous!=body:
                        backup=stage/'before'/name;backup.parent.mkdir(parents=True,exist_ok=True);backup.write_bytes(previous)
                elif 'before'in row:raise ValueError('required pre-migration receipt is absent: '+name)
            assert not stream.read(1)
        report.update(status='STAGED_AND_PREIMAGES_CHECKED',rows=len(rows),bytes=total,
            transport_header=pin(raw),linux_verification=header['linux_verification'],linux_owner=header['linux_owner'])
        save()
        for row in rows:
            destination=source/row['path'];destination.parent.mkdir(parents=True,exist_ok=True)
            staged=stage/'after'/row['path']
            if row['kind']=='confined-source-link':
                if not destination.is_symlink():os.replace(staged,destination)
                assert os.readlink(destination)==row['target']
            else:
                os.replace(staged,destination)
                assert pin(destination.read_bytes())=={k:row[k]for k in('bytes','sha256')}
        report['status']='CHECKED_INERT_LAYOUT_DELIVERY_PASS';save()
        print(report['status'],len(rows),total)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));save();raise


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('mode',choices=('export','receive'))
    parser.add_argument('--root',type=Path);parser.add_argument('--verification')
    parser.add_argument('--source',type=Path);parser.add_argument('--receipt',type=Path)
    args=parser.parse_args()
    if args.mode=='export':export(args.root,args.verification)
    else:receive(args.source,args.receipt)


if __name__=='__main__':main()
