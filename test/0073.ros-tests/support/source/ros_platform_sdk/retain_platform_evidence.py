#!/usr/bin/env python3
"""Retain closed Linux platform-test data in two independently read back copies.

Run only after an owner has joined all of its actors. Select explicit paths
relative to the owned tmpfs. The archive contains inert content-addressed regular
objects and a role manifest; symlinks are metadata, never archive links. This
tool neither extracts nor executes archived content. It changes no cgroup limit.
Each checkpoint is immutable; its replica is outside Documents so a subsequent
operator removal of the test directory does not remove both copies.
"""
import argparse
import gzip
import hashlib
import io
import json
import os
from pathlib import Path, PurePosixPath
import re
import resource
import shutil
import stat
import tarfile
import time


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream,'sha256').hexdigest()


def verify(path, expected):
    """Stream every member and then the entire gzip trailer, without extraction."""
    seen=set()
    with path.open('rb') as raw,gzip.GzipFile(fileobj=raw,mode='rb') as zipped:
        with tarfile.open(fileobj=zipped,mode='r|') as archive:
            members=iter(archive)
            first=next(members)
            assert first.name=='manifest.json' and first.isfile() and first.size<=16<<20
            assert json.loads(archive.extractfile(first).read())==expected
            objects={r['object']:r for r in expected['roles'] if r['type']=='regular'}
            for member in members:
                assert member.name in objects and member.name not in seen and member.isfile()
                row=objects[member.name]
                assert member.size==row['bytes'] and member.mode==row['mode']
                body=archive.extractfile(member)
                assert hashlib.file_digest(body,'sha256').hexdigest()==row['sha256']
                seen.add(member.name)
            assert seen==set(objects)
        # tar's end marker precedes gzip's checksum. Drain through verified EOF.
        while zipped.read(1<<20):
            pass
    return dict(bytes=path.stat().st_size,sha256=digest(path),objects=len(seen))


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--label',required=True)
    p.add_argument('--destination',type=Path,required=True)
    p.add_argument('--replica',type=Path,required=True)
    p.add_argument('--include',action='append',required=True)
    p.add_argument('--small-checkpoint-byte-budget',type=int,
                   help='Explicit1..64MiB per-copy cap with a1GiB disk floor; normal checkpoints retain8GiB')
    a=p.parse_args()
    if a.small_checkpoint_byte_budget is not None:
        assert 1<<20<=a.small_checkpoint_byte_budget<=64<<20
    assert re.fullmatch(r'[a-z][a-z0-9-]{0,63}',a.label)
    root=a.root.resolve(strict=True)
    assert root.is_mount()
    inputs=json.loads((root/'inputs.json').read_bytes())
    group=Path(inputs['cgroup']['path'])
    events=(group/'memory.events').read_text()
    started=time.monotonic()
    resource.setrlimit(resource.RLIMIT_CPU,(300,300))
    resource.setrlimit(resource.RLIMIT_AS,(512<<20,512<<20))
    file_limit=a.small_checkpoint_byte_budget or 2<<30
    resource.setrlimit(resource.RLIMIT_FSIZE,(file_limit,file_limit))
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    os.sched_setaffinity(0,{20})
    def guard():
        assert time.monotonic()-started<900
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==inputs['boot_id']
        assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
        s=group.stat()
        assert [s.st_dev,s.st_ino]==inputs['cgroup']['identity']
        assert (group/'memory.max').read_text().strip()==str(64<<30)
        assert (group/'memory.swap.max').read_text().strip()=='0'
        before=dict(line.split()for line in events.splitlines())
        after=dict(line.split()for line in (group/'memory.events').read_text().splitlines())
        assert all(before[k]==after[k] for k in ('oom','oom_kill','oom_group_kill'))
        if a.small_checkpoint_byte_budget is not None:
            for parent in(a.destination,a.replica):
                if parent.exists():
                    fs=os.statvfs(parent)
                    assert fs.f_bavail*fs.f_frsize>=1<<30
    guard()
    # Refuse live or incompletely closed owners instead of archiving moving logs.
    for report in sorted({p for pattern in ('flexiv-*.json','realman-*.json','doosan-*.json','leju-*.json','remaining-*.json','sdk-layout-*.json')
                          for p in (root/'out').glob(pattern)}):
        row=json.loads(report.read_bytes())
        assert row['status']!='RUNNING'
        assert all(c['exit'] is not None for c in row['commands'])
    selected={}
    for name in a.include:
        relative=PurePosixPath(name)
        assert not relative.is_absolute() and '..'not in relative.parts and str(relative)!='.'
        path=root/relative
        assert path.exists() or path.is_symlink(),path
        pending=[path]
        while pending:
            item=pending.pop()
            role=str(item.relative_to(root))
            info=item.lstat()
            selected[role]=(item,info)
            if stat.S_ISDIR(info.st_mode):
                pending.extend(item.iterdir())
    rows=[]
    objects={}
    for role,(path,info) in sorted(selected.items()):
        guard()
        row=dict(path=role,mode=stat.S_IMODE(info.st_mode))
        if stat.S_ISREG(info.st_mode):
            assert info.st_size<=128<<20
            h=digest(path)
            row.update(type='regular',bytes=info.st_size,sha256=h,
                       object='objects/'+h+'-'+oct(row['mode'])[2:])
            objects.setdefault(row['object'],(path,info,row))
        elif stat.S_ISLNK(info.st_mode):
            row.update(type='symlink-metadata',target=os.readlink(path))
        else:
            assert stat.S_ISDIR(info.st_mode),role
            row.update(type='directory-metadata')
        rows.append(row)
    manifest=dict(format='uwvm-owned-inert-objects-v1',original_root=str(root),
                  boot_id=inputs['boot_id'],label=a.label,roles=rows)
    body=(json.dumps(manifest,indent=2)+'\n').encode()
    assert len(body)<=16<<20
    for parent in (a.destination,a.replica):
        parent.mkdir(parents=True,exist_ok=True)
        assert parent.resolve()==parent and not parent.is_relative_to(root)
        fs=os.statvfs(parent)
        minimum=(1<<30)+2*a.small_checkpoint_byte_budget if a.small_checkpoint_byte_budget is not None else 8<<30
        assert fs.f_bavail*fs.f_frsize>=minimum
    target=a.destination/(a.label+'.tar.gz')
    replica=a.replica/target.name
    assert not target.exists() and not replica.exists()
    with target.open('xb')as raw:
        with gzip.GzipFile(filename='',fileobj=raw,mode='wb',mtime=0,compresslevel=1)as zipped:
            with tarfile.open(fileobj=zipped,mode='w|',format=tarfile.USTAR_FORMAT)as archive:
                member=tarfile.TarInfo('manifest.json');member.size=len(body);member.mode=0o644
                archive.addfile(member,io.BytesIO(body))
                for name,(path,before,row)in sorted(objects.items()):
                    guard()
                    member=tarfile.TarInfo(name);member.size=row['bytes'];member.mode=row['mode']
                    with path.open('rb')as stream:
                        archive.addfile(member,stream)
                    after=path.lstat()
                    keys=('st_dev','st_ino','st_mode','st_size','st_mtime_ns','st_ctime_ns')
                    assert all(getattr(before,k)==getattr(after,k)for k in keys)
                    assert digest(path)==row['sha256']
        raw.flush();os.fsync(raw.fileno())
    first=verify(target,manifest)
    with target.open('rb')as src,replica.open('xb')as dst:
        shutil.copyfileobj(src,dst,1<<20);dst.flush();os.fsync(dst.fileno())
    second=verify(replica,manifest)
    assert first==second
    guard()
    receipt=dict(status='TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS',
                 primary=str(target),replica=str(replica),**first,roles=len(rows),
                 manifest_sha256=hashlib.sha256(body).hexdigest(),
                 default_authority=[],events_before=events,events_after=(group/'memory.events').read_text())
    if a.small_checkpoint_byte_budget is not None:
        assert first['bytes']<=a.small_checkpoint_byte_budget
        receipt.update(storage_profile='EXPLICIT_SMALL_CHECKPOINT_WITH_HARD_PER_COPY_BUDGET',
            per_copy_byte_budget=a.small_checkpoint_byte_budget,minimum_free_bytes=1<<30,
            standard_checkpoint_floor_bytes=8<<30)
    for directory in (a.destination,a.replica):
        with (directory/(a.label+'.json')).open('x')as stream:
            stream.write(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps(receipt),flush=True)


if __name__=='__main__':main()
