#!/usr/bin/env python3
"""Retire only reproducible historical ROS CMake object intermediates.

The human authorized cleanup of this task's generated build garbage. This
closed selection is restricted to October12/13 historical ROS native-temp
CMake .o files, with retained original source, compile/link metadata and the
actual final ELF executable. It never removes a source, SDK, library, Wasm,
final executable, log or receipt. Live/referenced historical roots are skipped.
Plan first; apply requires the exact retained plan hash and rechecks all bytes.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import shlex
import stat
import time


BASE=Path('/home/macromodel/.local/share/uwvm-ros-extension')


def pin(path):
    assert path.is_file()and not path.is_symlink()
    before=path.stat()
    with path.open('rb')as stream:body=hashlib.file_digest(stream,'sha256').hexdigest()
    after=path.stat()
    keys=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_nlink')
    assert all(getattr(before,k)==getattr(after,k)for k in keys)
    return dict(path=str(path),bytes=before.st_size,sha256=body,device=before.st_dev,
                inode=before.st_ino,mtime_ns=before.st_mtime_ns,ctime_ns=before.st_ctime_ns,
                links=before.st_nlink,allocated=before.st_blocks*512)


def rooted(path):
    relative=path.relative_to(BASE)
    assert re.fullmatch(r'2026101[23]-[a-z0-9-]+',relative.parts[0])
    assert relative.parts[1]=='native-temp'and 'CMakeFiles'in relative.parts
    assert path.suffix=='.o'and path.resolve()==path
    return BASE/relative.parts[0]


def candidate(path):
    root=rooted(path)
    info=path.stat();assert stat.S_ISREG(info.st_mode)and info.st_nlink==1
    assert info.st_mtime<time.time()-86400
    with path.open('rb')as stream:head=stream.read(20)
    assert head[:4]==b'\x7fELF'and head[5]in(1,2)
    assert int.from_bytes(head[16:18],'little'if head[5]==1 else'big')==1
    target=next(p for p in path.parents if p.name.endswith('.dir')and p.parent.name=='CMakeFiles')
    build=target.parent.parent
    metadata=[target/n for n in('link.txt','build.make','flags.make')]
    assert all(p.is_file()and not p.is_symlink()for p in metadata)
    link=shlex.split(metadata[0].read_text())
    assert str(path.relative_to(build))in link and link.count('-o')==1
    name=Path(link[link.index('-o')+1]);assert not name.is_absolute()and'..'not in name.parts
    binary=build/name;assert binary.is_file()and not binary.is_symlink()
    with binary.open('rb')as stream:assert stream.read(4)==b'\x7fELF'
    dep=Path(str(path)+'.d');assert dep.is_file()and not dep.is_symlink()
    names=shlex.split(dep.read_text().split(':',1)[1].replace('\\\n',' '))
    assert names and names[0].startswith('/work/')
    source=root/'source'/names[0][len('/work/'):]
    assert source.suffix in('.cc','.cpp','.c','.cxx')and source.resolve().is_relative_to(root/'source')
    assert names[0]in metadata[1].read_text()
    return dict(root=str(root),object=pin(path),source=pin(source),binary=pin(binary),
                metadata=[pin(p)for p in [*metadata,dep]])


def references(roots):
    """Observe process paths only; never signal or inspect unrelated contents."""
    roots={str(p)+'/'for p in roots};live=set()
    def observe(value):
        for root in roots:
            if value.startswith(root):live.add(root[:-1])
    for process in Path('/proc').iterdir():
        if not process.name.isdecimal():continue
        try:
            for name in('cwd','exe'):observe(os.readlink(process/name))
            for fd in(process/'fd').iterdir():
                try:observe(os.readlink(fd))
                except(FileNotFoundError,PermissionError,ProcessLookupError,OSError):pass
            for argument in(process/'cmdline').read_bytes().split(b'\0'):
                observe(argument.decode('utf-8','replace'))
            for line in(process/'maps').read_text().splitlines():
                fields=line.split(maxsplit=5)
                if len(fields)==6:observe(fields[5])
        except(FileNotFoundError,PermissionError,ProcessLookupError,OSError):continue
    return live


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--plan',type=Path,required=True)
    p.add_argument('--apply-sha256');p.add_argument('--receipt',type=Path)
    a=p.parse_args();root=a.root.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes());group=Path(inputs['cgroup']['path'])
    resource.setrlimit(resource.RLIMIT_AS,(256<<20,256<<20));resource.setrlimit(resource.RLIMIT_CPU,(600,600))
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    before_events=(group/'memory.events').read_text()
    def guard():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup']['membership']
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==inputs['boot_id']
        s=group.stat();assert[s.st_dev,s.st_ino]==inputs['cgroup']['identity']
        assert(group/'memory.max').read_text().strip()==str(64<<30)and(group/'memory.swap.max').read_text().strip()=='0'
        before=dict(line.split()for line in before_events.splitlines());after=dict(line.split()for line in(group/'memory.events').read_text().splitlines())
        assert all(before[k]==after[k]for k in('oom','oom_kill','oom_group_kill'))
    guard();assert a.plan.parent==root/'control'
    if a.apply_sha256 is None:
        assert not a.plan.exists()
        roots=[r for r in BASE.iterdir()if r.is_dir()and not r.is_symlink()and re.fullmatch(r'2026101[23]-[a-z0-9-]+',r.name)]
        live=references(roots);rows=[];skipped=[]
        for historical in roots:
            if str(historical)in live:continue
            native=historical/'native-temp'
            if not native.is_dir():continue
            for base,dirs,files in os.walk(native,followlinks=False):
                dirs[:]=[d for d in dirs if not d.startswith('uwvm-native-snapshot-')and d not in('source','sdk','sysroot','installed','unpacked','include','share')]
                for name in files:
                    if not name.endswith('.o'):continue
                    path=Path(base)/name
                    try:row=candidate(path)
                    except(AssertionError,FileNotFoundError,StopIteration,ValueError,IndexError):
                        skipped.append(str(path));continue
                    rows.append(row);guard()
        result=dict(status='REPRODUCIBLE_HISTORICAL_ROS_CMAKE_OBJECT_PLAN',objects=rows,
            selected_allocated_bytes=sum(r['object']['allocated']for r in rows),
            source_executable_sdk_or_evidence_deletions=0,live_roots_skipped=sorted(live),
            unproven_objects_skipped=skipped,default_authority=[])
        a.plan.write_text(json.dumps(result,indent=2)+'\n')
        print('PLAN',len(rows),result['selected_allocated_bytes'],pin(a.plan)['sha256'],flush=True)
    else:
        assert a.receipt is not None and a.receipt.parent==root/'control'and not a.receipt.exists()
        assert pin(a.plan)['sha256']==a.apply_sha256
        result=json.loads(a.plan.read_bytes());assert result['status']=='REPRODUCIBLE_HISTORICAL_ROS_CMAKE_OBJECT_PLAN'
        rows=result['objects'];live=references({Path(r['root'])for r in rows})
        for row in rows:
            guard();assert row['root']not in live
            assert candidate(Path(row['object']['path']))==row
        before=os.statvfs(BASE);removed=[]
        for row in rows:
            # Stop once the original 8GiB archival floor has a further1GiB
            # margin. Never remove a larger set merely because it was planned.
            fs=os.statvfs(BASE)
            if fs.f_bavail*fs.f_frsize>=9<<30:break
            guard();path=Path(row['object']['path']);assert pin(path)==row['object']
            assert not references({Path(row['root'])})
            path.unlink();removed.append(row['object'])
        after=os.statvfs(BASE);guard()
        a.receipt.write_text(json.dumps(dict(status='ONLY_PROVEN_REPRODUCIBLE_HISTORICAL_OBJECTS_REMOVED',
            plan_sha256=a.apply_sha256,removed=removed,allocated_bytes_unlinked=sum(r['allocated']for r in removed),
            free_before=before.f_bavail*before.f_frsize,free_after=after.f_bavail*after.f_frsize,
            source_executable_sdk_or_evidence_deletions=0,unrelated_processes_signalled=0,
            events_before=before_events,events_after=(group/'memory.events').read_text()),indent=2)+'\n')
        print('REMOVED',len(removed),sum(r['allocated']for r in removed),'FREE',after.f_bavail*after.f_frsize,flush=True)


if __name__=='__main__':main()
