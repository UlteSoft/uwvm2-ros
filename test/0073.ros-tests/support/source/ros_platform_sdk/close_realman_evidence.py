#!/usr/bin/env python3
"""Normally unmount one closed owned test tmpfs after retained client readback.

This operator cleanup never signals another actor, lazily/forcibly unmounts a
busy tree, detaches a borrowed loop device, removes persistent SDK inputs or
attributes another task's memory changes to this one. The exact released tmpfs
allocation and physical checkpoint sizes remain separate measurements.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import subprocess
import time


def sha(path):
    with path.open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--checkpoint',type=Path,required=True)
    p.add_argument('--client-readback',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args()
    resource.setrlimit(resource.RLIMIT_AS,(256<<20,256<<20))
    resource.setrlimit(resource.RLIMIT_CPU,(120,120))
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    root=a.root.resolve(strict=True)
    assert root.is_mount() and root.stat().st_dev!=root.parent.stat().st_dev
    inputs=json.loads((root/'inputs.json').read_bytes())
    group=Path(inputs['cgroup']['path'])
    events=(group/'memory.events').read_text()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
    assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==inputs['boot_id']
    info=group.stat()
    assert [info.st_dev,info.st_ino]==inputs['cgroup']['identity']
    assert (group/'memory.max').read_text().strip()==str(64<<30)
    assert (group/'memory.swap.max').read_text().strip()=='0'
    checkpoint=json.loads(a.checkpoint.read_bytes())
    client=json.loads(a.client_readback.read_bytes())
    assert checkpoint['status']=='TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
    assert client['status']=='CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
    assert client['sha256']==checkpoint['sha256'] and client['bytes']==checkpoint['bytes']
    for name in ('primary','replica'):
        archive=Path(checkpoint[name])
        assert not archive.is_relative_to(root)and archive.stat().st_size==checkpoint['bytes']
        assert sha(archive)==checkpoint['sha256']
    assert a.out.is_absolute()and not a.out.is_relative_to(root)and not a.out.exists()
    phases=[]
    for path in sorted((root/'out').glob('realman-*.json')):
        row=json.loads(path.read_bytes())
        assert row['status']in ('PASS','FAILED_RETAINED')
        assert all(c['exit']is not None for c in row['commands'])
        phases.append(dict(phase=row['phase'],attempt=row.get('attempt',path.stem.rsplit('-',1)[-1]),status=row['status']))
    for path,expected in [('out/verify-r1/verification.json','INDEPENDENT_REALMAN_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS'),
                          ('out/nativecheck-r1/native-verification.json','INDEPENDENT_REALMAN_ORIGINAL_FAILURE_AND_FORMAT_ONLY_REBUILD_VERIFIED'),
                          ('out/audit-r1/audit.json','DELIVERED_SOURCE_AUDIT_AND_EXPECTED_INCOMPLETE_CI_REFUSAL_PASS')]:
        assert json.loads((root/path).read_bytes())['status']==expected
    maintenance=root/'out/local-wat-owner.json'
    if maintenance.exists():
        row=json.loads(maintenance.read_bytes())
        assert row['status']=='PASS'
        assert all(c['exit']==0 and c['forced_signal']is None for c in row['commands'])
    device=root.stat().st_dev
    active=[]
    # Descriptor/cwd device checks also catch aliases in a native SDK's private
    # /tmp bind, where a pathname-only search would miss an active actor.
    for proc in Path('/proc').iterdir():
        if not proc.name.isdecimal()or int(proc.name)==os.getpid():continue
        try:
            for name in ('cwd','root'):
                if (proc/name).stat().st_dev==device:active.append(str(proc/name))
            for fd in (proc/'fd').iterdir():
                try:
                    if fd.stat().st_dev==device:active.append(str(fd))
                except(FileNotFoundError,ProcessLookupError):pass
            for line in (proc/'maps').read_text().splitlines():
                fields=line.split(maxsplit=5)
                if len(fields)>=5 and fields[4]!='0':
                    major,minor=(int(s,16)for s in fields[3].split(':'))
                    if (major,minor)==(os.major(device),os.minor(device)):active.append(str(proc/'maps'))
        except(FileNotFoundError,ProcessLookupError,PermissionError):pass
    assert not active,active
    commands=[]
    borrowed=root/'recovered-cache'
    for target in ([borrowed]if borrowed.is_mount()else[]):
        assert os.statvfs(target).f_flag&os.ST_RDONLY
        argv=['/usr/bin/umount',str(target)]
        subprocess.run(argv,check=True,timeout=15);commands.append(argv)
        assert not target.is_mount()
    paths=[root]
    seen=set();allocated=0;logical=0;entries=0
    while paths:
        path=paths.pop();info=path.lstat();assert info.st_dev==device
        entries+=1;key=(info.st_dev,info.st_ino)
        if key not in seen:allocated+=info.st_blocks*512;seen.add(key)
        if stat.S_ISDIR(info.st_mode):paths.extend(path.iterdir())
        elif stat.S_ISREG(info.st_mode):logical+=info.st_size
        else:assert stat.S_ISLNK(info.st_mode)
    fs=os.statvfs(root)
    used=(fs.f_blocks-fs.f_bfree)*fs.f_frsize
    argv=['/usr/bin/umount',str(root)]
    subprocess.run(argv,check=True,timeout=15);commands.append(argv)
    assert not root.is_mount()and not list(root.iterdir())
    root.rmdir()
    result=dict(status='OWNED_TEST_TMPFS_NORMALLY_UNMOUNTED_AFTER_RETAINED_CLIENT_READBACK',
        original_root=str(root),root_exists=root.exists(),commands=commands,
        tmpfs_data_allocation_before_unmount=used,unique_inode_allocated_bytes_before=allocated,
        regular_logical_bytes_before=logical,entries=entries,unique_inodes=len(seen),
        phases=phases,checkpoint_sha256=checkpoint['sha256'],
        preserved_sdk_inputs=inputs['sdk_recovery_output'],borrowed_loop_detached=False,
        other_roots_or_limits_modified=False,events_before=events,events_after=(group/'memory.events').read_text())
    a.out.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result),flush=True)


if __name__=='__main__':main()
