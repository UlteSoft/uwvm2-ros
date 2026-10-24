#!/usr/bin/env python3
"""Copy a complete closed implementation view before serial build/test stages.

SourceA contains the maintained implementation, official custom definitions,
all fixtures and the verbatim original request. SourceB may additionally pin
the *new* generated composite providers and native-generation receipt. Neither
freeze runs a compiler, generator, model or ROS process. Each file is copied
through a bounded opened regular descriptor and checked again after the entire
copy, so a concurrent author edit fails rather than silently mixing revisions.
Original worktrees and earlier freezes are never changed. Outputs are new
owned directories; all source bytes are made read-only after publication.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys

sys.dont_write_bytecode=True
DIRECTORIES=("src","ros-tests","test","sdk","examples","xmake",
             "third-parties/fast_io/include","third-parties/fast_io/share/fast_io","third-parties/bizwen/include",
             "third-parties/boost_unordered/include",".github/workflows",
             "documents/command-line","documents/how-to-build")
FILES=("README.md","LICENSE.md","xmake.lua","third-parties/fast_io/license.txt",
       "third-parties/bizwen/LICENSE","third-parties/boost_unordered/LICENSE",
       "documents/ros/original-task-audit-20261013-evidence/original-task.txt")
EXCLUDED=frozenset((".git","__pycache__","node_modules","target",".cache",".pytest_cache"))
LIMIT=64*1024**2


def copy_file(origin,destination):
    """Check unchanged bytes/inode/time through an opened no-follow FD."""
    fd=os.open(origin,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        before=os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 0<=before.st_size<=LIMIT:
            raise ValueError("freeze input must be bounded regular file: "+str(origin))
        destination.parent.mkdir(parents=True,exist_ok=True)
        hasher=hashlib.sha256();used=0
        with destination.open("xb") as output:
            while True:
                data=os.read(fd,min(65536,LIMIT+1-used))
                if not data:break
                used+=len(data)
                if used>LIMIT:raise ValueError("freeze input exceeds bound")
                output.write(data);hasher.update(data)
            output.flush();os.fsync(output.fileno())
        after=os.fstat(fd);now=origin.stat(follow_symlinks=False)
        fields=("st_dev","st_ino","st_size","st_mtime_ns","st_ctime_ns")
        if used!=before.st_size or any(getattr(before,key)!=getattr(after,key) or
                                     getattr(before,key)!=getattr(now,key) for key in fields):
            raise ValueError("freeze input changed: "+str(origin))
        destination.chmod(0o500 if before.st_mode&0o111 else 0o400)
        return {"sha256":hasher.hexdigest(),"bytes":used},tuple(getattr(before,key) for key in fields)
    finally:os.close(fd)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source",type=Path,required=True)
    parser.add_argument("--out",type=Path,required=True)
    parser.add_argument("--extra",action="append",default=[],metavar="RELATIVE=ABSOLUTE",
                        help="Explicit generated tree/receipt appended only in the new SourceB freeze")
    args=parser.parse_args();source=args.source.resolve(strict=True);out=args.out.resolve()
    if out.exists() or out==source or source.is_relative_to(out) or out.is_relative_to(source):
        parser.error("fresh output outside input source required")
    selected={}
    def tree(prefix,root):
        if root.is_symlink():raise ValueError("freeze source root symlink requires explicit resolution: "+str(root))
        if not root.is_dir():raise ValueError("required freeze directory absent: "+str(root))
        for path in sorted(root.rglob("*")):
            relative=path.relative_to(root)
            if EXCLUDED.intersection(relative.parts):continue
            if path.is_symlink():raise ValueError("freeze source symlink requires explicit resolution: "+str(path))
            if path.is_file():
                name=str(Path(prefix)/relative)
                if name in selected:raise ValueError("overlapping source input: "+name)
                selected[name]=path
    for name in DIRECTORIES:tree(name,source/name)
    # Current authored ROS guides/tables are build material. The nested dated
    # evidence archives are preserved in the worktree and are intentionally
    # separate from this code freeze, avoiding gigabytes of historical products.
    for path in sorted((source/"documents/ros").iterdir()):
        if path.is_file() and path.suffix in (".md",".json",".txt"):
            if path.is_symlink():raise ValueError("ROS material symlink requires explicit resolution")
            selected[str(path.relative_to(source))]=path
    for name in FILES:
        if not (source/name).is_file():raise ValueError("required source input absent: "+name)
        selected[name]=source/name
    extras=[]
    for value in args.extra:
        name,separator,path=value.partition("=");relative=Path(name);origin=Path(path)
        if separator!="=" or relative.is_absolute() or ".." in relative.parts or not origin.is_absolute():
            parser.error("extra requires safe relative name and absolute existing input")
        origin=origin.resolve(strict=True)
        if origin.is_dir():tree(relative,origin)
        else:
            if name in selected:raise ValueError("extra shadows maintained source: "+name)
            selected[name]=origin
        extras.append({"relative":name,"origin":str(origin)})
    out.mkdir(mode=0o700,parents=True);destination=out/"source";destination.mkdir(mode=0o700)
    manifest={};births={}
    for name,path in sorted(selected.items()):manifest[name],births[name]=copy_file(path,destination/name)
    fields=("st_dev","st_ino","st_size","st_mtime_ns","st_ctime_ns")
    for name,path in selected.items():
        now=path.stat(follow_symlinks=False)
        if tuple(getattr(now,key) for key in fields)!=births[name]:raise ValueError("author modified source during freeze: "+name)
    body=json.dumps(manifest,indent=2)+"\n";(out/"source-manifest.json").write_text(body)
    (out/"freeze.json").write_text(json.dumps({"kind":"ros-complete-source-freeze","source":str(source),
        "destination":str(destination),"files":len(manifest),"bytes":sum(row["bytes"] for row in manifest.values()),
        "source_manifest_sha256":hashlib.sha256(body.encode()).hexdigest(),"extras":extras,
        "concurrent_input_changes":False,"tests_executed":False},indent=2)+"\n")
    for path in sorted((path for path in destination.rglob("*") if path.is_dir()),key=lambda path:len(path.parts),reverse=True):path.chmod(0o500)
    destination.chmod(0o500)
    print(json.dumps({"status":"FROZEN_SOURCE_ONLY","files":len(manifest),"bytes":sum(row["bytes"] for row in manifest.values())}))
    return 0
if __name__=="__main__":raise SystemExit(main())
