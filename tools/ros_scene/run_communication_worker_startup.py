#!/usr/bin/env python3
"""Run the real namespace/seccomp startup against a complete pinned image pack.

This only proves readiness/attachment and cleanup. A real RPC/ROS communication
test remains required for all permitted endpoint and codec operations.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pack',type=Path,required=True)
    parser.add_argument('--threads',type=int,required=True)
    parser.add_argument('--request-bytes',type=int,required=True)
    parser.add_argument('--response-bytes',type=int,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset({12,13}) and 1<=args.threads<=256
    assert 1<=args.request_bytes<=4*1024*1024 and 1<=args.response_bytes<=4*1024*1024
    output=args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    output.mkdir(parents=True,exist_ok=False)
    packed=json.loads(args.pack.read_text())
    assert packed['complete']
    files=[packed['launcher'],*packed['files']]
    assert all(Path(x['path']).stat().st_size==x['bytes'] and digest(x['path'])==x['sha256'] for x in files)
    fixture='test/0084.ros-runtime/communication_worker_startup.cpp'
    dep=subprocess.run(['/usr/bin/clang++-22','-std=c++23','-Isrc','-isystem','third-parties/fast_io/include','-MM',fixture],cwd=ROOT,capture_output=True,text=True,check=True,timeout=60)
    paths={(ROOT/p).resolve() for p in shlex.split(dep.stdout.replace('\\\n','').split(':',1)[1])}
    paths.update(p.resolve() for p in (ROOT/'third-parties/fast_io/include').rglob('*') if p.is_file())
    paths.add(Path(__file__).resolve())
    pins={str(p.relative_to(ROOT)):digest(p) for p in paths}
    source=output/'source'
    for path in paths:
        dest=source/path.relative_to(ROOT)
        dest.parent.mkdir(parents=True,exist_ok=True)
        dest.write_bytes(path.read_bytes())
        assert digest(dest)==pins[str(path.relative_to(ROOT))]
    (source/'source-manifest.json').write_text(json.dumps(pins,indent=2)+'\n')
    record=dict(complete=False,scope='real worker startup/attachment only; no endpoint data-path qualification',commands=[],sources=pins,pack=str(args.pack.resolve()),pack_sha256=digest(args.pack),images={x['path']:x['sha256'] for x in files},events_before=(GROUP/'memory.events').read_text(),cgroup=str(GROUP),memory_max=68719476736,swap_max=0)
    def run(label,argv):
        done=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=180)
        (output/(label+'.stdout')).write_text(done.stdout)
        (output/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=done.returncode))
        (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
        print(label,done.returncode,flush=True)
        if done.returncode:raise RuntimeError(done.stderr[-10000:])
    try:
        binary=output/'startup'
        run('compile',['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(source/'src'),'-isystem',str(source/'third-parties/fast_io/include'),str(source/fixture),'-ldl','-o',str(binary)])
        launcher=packed['launcher']
        argv=[str(binary),launcher['path'],launcher['sha256'],str(packed['required_image_bytes']),str(args.threads),str(args.request_bytes),str(args.response_bytes)]
        for entry in packed['files']:
            argv.extend([entry['role'],entry['destination'],entry['path'],entry['sha256']])
        run('startup',argv)
        record['binary_sha256']=digest(binary)
        record['complete']=all(digest(source/name)==value for name,value in pins.items()) and all(digest(path)==value for path,value in record['images'].items()) and digest(args.pack)==record['pack_sha256']
        assert record['complete']
    finally:
        record['events_after']=(GROUP/'memory.events').read_text()
        (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')


if __name__=='__main__':
    main()
