#!/usr/bin/env python3
"""Verify warning policy through the actual VM parser and real native libraries.

The empty guest imports no ROS or WASI capability. Native constructors are
benign observation markers pinned by the completed native qualification.
"""
import argparse
import hashlib
import itertools
import json
import os
from pathlib import Path
import resource
import subprocess
import time
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--native',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    guard();resource.setrlimit(resource.RLIMIT_CORE,(0,0));assert os.sched_getaffinity(0).issubset({16,17,18,19})
    runtime=args.runtime.resolve();native=args.native.resolve();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    build_file=runtime.parent/'full-build-results.json';build=json.loads(build_file.read_text())
    assert build['complete'] and build['binary_sha256']==sha(runtime)
    proof=json.loads((native/'results.json').read_text());assert proof['complete']
    libraries=[native/('clang-'+name+'.so') for name in ('A','B')]
    for path in libraries:assert proof['binaries'][str(path)]==sha(path)
    record=dict(complete=False,scope=__doc__,runtime_sha256=sha(runtime),build_sha256=sha(build_file),native_receipt_sha256=sha(native/'results.json'),
        libraries={str(p):sha(p) for p in libraries},runner_sha256=sha(__file__),cases=[],commands=[],
        cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();r=subprocess.run(list(map(str,argv)),capture_output=True,text=True,timeout=40)
        (out/(label+'.stdout')).write_text(r.stdout);(out/(label+'.stderr')).write_text(r.stderr)
        record['commands'].append(dict(name=label,argv=list(map(str,argv)),exit=r.returncode,seconds=time.monotonic()-start));save();return r
    wat=out/'empty.wat';wasm=out/'empty.wasm';wat.write_text('(module (func (export "_start")))\n')
    assert run('compile-empty',['wat2wasm',wat,'-o',wasm]).returncode==0
    record['guest_sha256']=sha(wasm)
    prefix=[runtime,'--wasm-set-main-module-name','main','--runtime-int','--log-color','disable']
    modes=('default','suppressed','reenabled','fatal','suppressed-fatal','reenabled-fatal','all-reenabled','all-fatal')
    success=False
    try:
        for version,scope,mode in itertools.product(('ros1','ros2'),('global','single','group'),modes):
            label='-'.join((version,scope,mode));flags=[];category='all' if mode.startswith('all-') else 'deprecated-ros'
            suppressed=mode in ('suppressed','suppressed-fatal');fatal='fatal' in mode and not suppressed
            if mode!='default':
                flags+=['--disable-warning',category]
                if not suppressed:flags+=['--enable-warning',category]
            if 'fatal' in mode:flags+=['--warning-as-error',category]
            name='main' if scope=='single' else 'legacy'
            if scope!='global':flags+=['--'+version+'-'+scope+'-create',name]
            if scope=='group':flags+=['--'+version+'-group-add-module',name,'main']
            flags+=['--'+version+'-'+scope+'-enable']+([] if scope=='global' else [name])
            r=run(label,[*prefix,*flags,'--run',wasm]);assert (r.returncode!=0)==fatal,(label,r.stderr)
            count=0 if suppressed else 1;assert r.stderr.count('(deprecated-ros)')==count,(label,r.stderr)
            record['cases'].append(dict(name=label,passed=True,rejected=fatal,diagnostics=count));save()
        for mode in ('default','suppressed','reenabled','fatal','suppressed-fatal','second-fatal','second-reenabled-fatal','all-fatal'):
            flags=[];fatal='fatal' in mode and mode!='suppressed-fatal'
            if mode in ('suppressed','suppressed-fatal','reenabled'):flags+=['--disable-warning','untrusted-dl']
            if mode=='reenabled':flags+=['--enable-warning','untrusted-dl']
            if mode in ('fatal','suppressed-fatal'):flags+=['--warning-as-error','untrusted-dl']
            if mode=='all-fatal':flags+=['--warning-as-error','all']
            flags+=['--wasm-register-dl',libraries[0],'warning-A']
            if mode.startswith('second-'):
                if mode=='second-reenabled-fatal':flags+=['--disable-warning','untrusted-dl']
                flags+=['--warning-as-error','untrusted-dl']
                if mode=='second-reenabled-fatal':flags+=['--enable-warning','untrusted-dl']
            flags+=['--wasm-register-dl',libraries[1],'warning-B']
            label='preload-'+mode;r=run(label,[*prefix,*flags,'--run',wasm]);assert r.returncode==(126 if fatal else 0),(label,r.stderr)
            first=not fatal or mode.startswith('second-');second=not fatal
            assert r.stdout.count('CONSTRUCTOR_A\n')==int(first) and r.stdout.count('CONSTRUCTOR_B\n')==int(second),(label,r.stdout)
            count=0 if mode in ('suppressed','suppressed-fatal') else 2 if fatal else 1
            assert r.stderr.count('(untrusted-dl)')==count,(label,r.stderr)
            record['cases'].append(dict(name=label,passed=True,first_constructor=first,second_constructor=second,diagnostics=count));save()
        assert sha(runtime)==record['runtime_sha256'] and sha(build_file)==record['build_sha256']
        assert all(sha(path)==digest for path,digest in record['libraries'].items());success=True
    finally:record['complete']=success;record['events_after']=(GROUP/'memory.events').read_text();save()
    print('PASS 56 actual VM warning and constructor scenarios',flush=True)
if __name__=='__main__':main()
