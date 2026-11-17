#!/usr/bin/env python3
"""Qualify register-guarded artifact wrappers under hostile native callbacks.

Real wrappers/providers with controlled runtime identity/memory APIs. Both
optimized native and ASan/UBSan builds retain full debug information. The 48
control profiles do not replace actual Wasm/MTE or imported-memory tests.
"""
import argparse,json,os,resource,subprocess,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,sha,verify_gate

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compile-argv',type=Path,required=True);p.add_argument('--link-argv',type=Path,required=True)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--fixtures-root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--fixture',choices=['artifact_wrapper_fp','module_wrapper_fp','observation_wrapper_fp'],default='artifact_wrapper_fp');a=p.parse_args();verify_gate()
    assert os.sched_getaffinity(0).issubset({12,13});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    source=a.source_root.resolve();origin=source.with_name('transform-source-central')
    flags=[f.replace(str(origin),str(source)) for f in json.loads(a.compile_argv.read_text()) if not f.startswith('-fprebuilt-module-path=')]
    for i,flag in enumerate(flags):
        if flag.startswith('-I') and flag.endswith('/src'):flags[i]='-I'+str(source/'src')
        elif flag.startswith('-I') and flag.endswith('/third-parties/fast_io/include'):flags[i]='-I'+str(source/'third-parties/fast_io/include')
    flags+=['-UNDEBUG','-O3','-g','-fexceptions','-Werror']
    fixtures=a.fixtures_root.resolve();link=json.loads(a.link_argv.read_text());fixture=fixtures/'test/0084.ros-runtime'/(a.fixture+'.cpp');policy=fixtures/'test/0084.ros-runtime/host_fp_policy.cpp'
    manifest=json.loads((source/'source-manifest.json').read_text());assert all(sha(source/n)==v for n,v in manifest.items())
    paths=[Path(__file__).resolve(),fixture,policy,a.compile_argv.resolve(),a.link_argv.resolve(),source/'source-manifest.json',fixtures/'test/0008.imported/wasi/wasip1/func/fp_control_probe.h']
    pins=lambda:{str(p):sha(p)for p in paths}
    report=dict(complete=False,scope=__doc__,inputs=pins(),source_root=str(source),production_files=len(manifest),cgroup=str(GATE),inode=GATE.stat().st_ino,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,cmd):
        verify_gate();start=time.monotonic();r=subprocess.run(cmd,cwd=ROOT,capture_output=True,timeout=300)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        report['commands'].append(dict(name=name,argv=cmd,exit=r.returncode,seconds=time.monotonic()-start));save()
        assert not r.returncode,(name,r.returncode,r.stdout.decode(errors='replace')[-2000:],r.stderr.decode(errors='replace')[-5000:]);print('PASS '+name,flush=True)
    try:
        run('policy',[*flags,'-c',str(policy),'-o',str(out/'policy.o')])
        for name,extra in [('asan',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('native',[])]:
            obj=out/(name+'.o');binary=out/name
            run(name+'-compile',[*flags,*extra,'-c',str(fixture),'-o',str(obj)])
            wrapped=['malloc','calloc','realloc']+(['pread'] if a.fixture=='artifact_wrapper_fp' else ['free'] if a.fixture=='observation_wrapper_fp' else [])
            run(name+'-link',[link[0],*extra,str(obj),*link[1:],'-lcrypto','-lm',*['-Wl,--wrap='+n for n in wrapped],'-o',str(binary)])
            run(name,[str(binary)]);report[name+'_sha256']=sha(binary)
            if a.fixture=='module_wrapper_fp':run(name+'-retained-input',[str(binary),'--retain-input'])
        report['source_unchanged']=all(sha(source/n)==v for n,v in manifest.items());assert report['source_unchanged'] and pins()==report['inputs'];report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
