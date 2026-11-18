#!/usr/bin/env python3
"""Build O3/full-debug OpenSSL and execute mutual-TLS checks on five Linux ABIs.

Reuse pinned target compiler/sysroot recipes; perform all work serially inside
the existing 64 GiB cgroup. A userspace emulator is not hardware qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
from performance_build_policy import compile_policy


def sha(p):
    with Path(p).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('source-root','baseline-tls','x86-openssl','output'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args()
    for k,v in vars(a).items():setattr(a,k,v.resolve())
    group=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(group.relative_to('/sys/fs/cgroup'))
    assert (group/'memory.max').read_text().strip()=='68719476736' and (group/'memory.swap.max').read_text().strip()=='0'
    assert '.tmp' in a.output.parts;a.output.mkdir(parents=True,exist_ok=False)
    prior=json.loads(a.baseline_tls.read_text());assert prior['complete']
    inputs=[a.baseline_tls,Path(__file__).resolve(),a.source_root/'tools/ros_scene/performance_build_policy.py',a.source_root/'tools/dependencies/build_openssl.py',
        a.source_root/'third-parties/openssl/sources.sha256',a.source_root/'test/0084.ros-runtime/attachment_tls.cpp',
        *sorted((a.source_root/'src/uwvm2/imported/ros/scene').glob('*.h'))]
    result=dict(complete=False,scope=__doc__,inputs={str(f):sha(f) for f in inputs},runs=[],dependencies={},events_before=(group/'memory.events').read_text())
    receipt=a.output/'results.json'
    def save():receipt.write_text(json.dumps(result,indent=2)+'\n')
    def run(abi,action,cmd):
        cmd=list(map(str,cmd));start=time.monotonic()
        with (a.output/(abi+'-'+action+'.log')).open('wb') as log:r=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=600)
        result['runs'].append(dict(abi=abi,action=action,argv=cmd,exit=r.returncode,seconds=time.monotonic()-start));save()
        assert r.returncode==0,(abi,action);print(abi,action,'passed',flush=True)
    for abi in ('x86_64','i686','aarch64','armhf','arm'):
        original=next(r['argv'] for r in prior['runs'] if r['abi']==abi and r['action']=='compile')
        execute=next(r['argv'] for r in prior['runs'] if r['abi']==abi and r['action']=='run')
        old=Path(next(x for x in original if x.endswith('/libcrypto.a'))).parent
        if abi=='x86_64':prefix=a.x86_openssl
        else:
            prefix=a.output/abi/'openssl';prefix.mkdir(parents=True)
            spec=json.loads((old/'spec.json').read_text());spec['jobs']=1;spec['keep_work']=True
            spec['cflags']=['-O3','-g',*[x for x in spec['cflags'] if not re.fullmatch(r'-O(?:[0-3szg]|fast)|-g(?:[0-3])?',x)]]
            path=prefix/'spec.json';path.write_text(json.dumps(spec,indent=2)+'\n')
            run(abi,'dependency',['/usr/bin/python3',a.source_root/'tools/dependencies/build_openssl.py','--source-root',a.source_root/'third-parties/openssl','--spec',path,'--output',prefix])
        dependency=json.loads((prefix/'receipt.json').read_text());policy=compile_policy(dependency['identity']['spec']['cflags'])
        assert all(sha(prefix/k)==v for k,v in dependency['artifacts'].items())
        result['dependencies'][abi]=dict(prefix=str(prefix),receipt_sha256=sha(prefix/'receipt.json'),policy=policy,archive_abi=dependency['actual_archive_abi'])
        previous=Path(next(x for x in original if x.endswith('/test/0084.ros-runtime/attachment_tls.cpp'))).parents[2]
        oldbinary=original[original.index('-o')+1];binary=a.output/(abi+'-tls')
        compile=[x.replace(str(previous),str(a.source_root)).replace(str(old),str(prefix)) for x in original]
        compile[compile.index('-o')+1]=str(binary);compile_policy(compile)
        run(abi,'compile',compile);run(abi,'run',[str(binary) if x==oldbinary else x for x in execute])
        result['runs'][-1]['binary_sha256']=sha(binary);save()
    result['inputs_unchanged']=all(sha(k)==v for k,v in result['inputs'].items())
    result['events_after']=(group/'memory.events').read_text();result['complete']=result['inputs_unchanged'];save();assert result['complete']


if __name__=='__main__':main()
