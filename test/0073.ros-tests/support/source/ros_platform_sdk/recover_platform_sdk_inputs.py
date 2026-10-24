#!/usr/bin/env python3
"""Copy only selected SDK inputs from an admitted read-only recovery volume.

This operator utility is data restoration, never an upstream controller launch.
Run inside the original cgroup with finite process supervision. Preserve original
symlinks, modes and hardlinks. Independently compare source/destination content
and metadata before admitting the new cache; do not copy the whole sparse disk
image or mistake its nominal capacity for reclaimed/occupied storage.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import subprocess

SELECTIONS={
    'noetic':'source58-noetic-native3-r1/sdk-rootfs',
    'humble':'source39-native-gateways-physics-peer-r1/rootfs',
    'cross':'extra-cross-sdk-inputs-r3',
}


def inventory(root):
    rows=[]
    hashes={}
    groups={}
    pending=[root]
    while pending:
        path=pending.pop()
        info=path.lstat()
        role=str(path.relative_to(root))
        row=dict(path=role,mode=stat.S_IMODE(info.st_mode))
        if stat.S_ISDIR(info.st_mode):
            row['type']='directory'
            pending.extend(sorted(path.iterdir(),reverse=True))
        elif stat.S_ISLNK(info.st_mode):
            row.update(type='symlink',target=os.readlink(path))
        else:
            assert stat.S_ISREG(info.st_mode),path
            key=(info.st_dev,info.st_ino,info.st_size,info.st_mtime_ns)
            if key not in hashes:
                with path.open('rb')as stream:
                    hashes[key]=hashlib.file_digest(stream,'sha256').hexdigest()
            after=path.lstat()
            assert all(getattr(info,k)==getattr(after,k)for k in ('st_dev','st_ino','st_mode','st_size','st_mtime_ns','st_ctime_ns'))
            row.update(type='regular',bytes=info.st_size,sha256=hashes[key],
                       hardlink_group=groups.setdefault((info.st_dev,info.st_ino),role))
        rows.append(row)
    return sorted(rows,key=lambda r:r['path'])


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--source-cache',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    p.add_argument('--record',type=Path,required=True)
    a=p.parse_args()
    root=a.root.resolve(strict=True)
    cache=a.source_cache.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes())
    group=Path(inputs['cgroup']['path'])
    assert root.is_mount() and cache.is_mount() and cache.is_relative_to(root)
    assert os.statvfs(cache).f_flag&os.ST_RDONLY
    assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
    assert (group/'memory.max').read_text().strip()==str(64<<30)
    assert (group/'memory.swap.max').read_text().strip()=='0'
    assert a.out.is_absolute() and not a.out.exists() and not a.record.exists()
    fs=os.statvfs(a.out.parent)
    assert fs.f_bfree*fs.f_frsize>=16<<30
    a.out.mkdir(mode=0o700)
    report=dict(status='RUNNING',original_cache_read_only=True,selected_roots=[],
        default_authority=[],upstream_executables_run=0,source_cache=str(cache),
        scope='Selected toolchain inputs only; no platform/driver acceptance')
    def save():a.record.write_text(json.dumps(report,indent=2)+'\n')
    save()
    try:
        for name,relative in SELECTIONS.items():
            source=cache/relative
            destination=a.out/name
            before=inventory(source)
            assert max(r.get('bytes',0)for r in before)<=512<<20
            subprocess.run(['/usr/bin/cp','--archive','--sparse=always','--reflink=auto',
                            str(source),str(destination)],check=True,timeout=240)
            after=inventory(destination)
            assert before==after,(name,'source/copy content or mode difference')
            assert inventory(source)==before,(name,'original cache changed')
            receipt=a.out/(name+'-files.json')
            receipt.write_text(json.dumps(before,indent=2)+'\n')
            with receipt.open('rb')as stream:h=hashlib.file_digest(stream,'sha256').hexdigest()
            report['selected_roots'].append(dict(name=name,source=str(source),destination=str(destination),
                files=len(before),manifest_path=str(receipt),manifest_bytes=receipt.stat().st_size,manifest_sha256=h,
                regular_logical_bytes=sum(r.get('bytes',0)for r in before)))
            save()
        report['status']='SELECTED_NATIVE_AND_CROSS_SDK_INPUTS_RESTORED_AND_CONTENT_MODES_VERIFIED'
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        raise
    finally:save()
    print(report['status'],flush=True)


if __name__=='__main__':main()
