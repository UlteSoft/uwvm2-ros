#!/usr/bin/env python3
"""Recheck real LLVM imported-memory/provider guards using pinned VM runtime objects.

This runs the existing hostile imported-memory/global/signature callback test,
including generated-bridge reentry rejection in a child process. It qualifies
the independent runtime boundary; artifact wrapper tests remain separate.
"""
import argparse,json,os,re,resource,shutil,subprocess,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,sha,verify_gate

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime-build',type=Path,required=True);p.add_argument('--compile-argv',type=Path,required=True)
    p.add_argument('--fixtures-root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True);a=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({12,13})
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    build=a.runtime_build.resolve();receipt=json.loads((build/'full-build-results.json').read_text());assert receipt['complete'];source=Path(receipt['source_root'])
    manifests=json.loads((source/'source-manifest.json').read_text());assert all(sha(source/n)==v for n,v in manifests.items())
    origin=a.fixtures_root.resolve();fixtures=out/'fixtures';todo=[origin/'test/0014.llvm_jit/llvm_jit_local_imported_provider_callback_guard.cc'];pinned={}
    while todo:
        path=todo.pop().resolve()
        if path in pinned:continue
        assert path.is_relative_to(origin/'test');pinned[path]=sha(path);target=fixtures/path.relative_to(origin);target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(path,target)
        for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(),re.M):
            child=(path.parent/name).resolve()
            if child.is_file():todo.append(child)
    fixture=fixtures/'test/0014.llvm_jit/llvm_jit_local_imported_provider_callback_guard.cc'
    paths=[Path(__file__).resolve(),a.compile_argv.resolve(),build/'full-build-results.json',build/'link.argv.json',source/'source-manifest.json',
        build/'host_api.default.cpp.o',build/'uwvm_runtime.default.cpp.o',*[fixtures/p.relative_to(origin) for p in pinned]]
    pins=lambda:{str(p):sha(p) for p in paths}
    report=dict(complete=False,scope=__doc__,inputs=pins(),production_source=str(source),production_files=len(manifests),cgroup=str(GATE),inode=GATE.stat().st_ino,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,cmd):
        verify_gate();started=time.monotonic();r=subprocess.run(cmd,cwd=ROOT,capture_output=True,timeout=300)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        report['commands'].append(dict(name=name,argv=cmd,exit=r.returncode,seconds=time.monotonic()-started));save()
        assert not r.returncode,(name,r.returncode,r.stderr.decode(errors='replace')[-6000:]);print('PASS '+name,flush=True)
    try:
        flags=[f for f in json.loads(a.compile_argv.read_text()) if not f.startswith('-fprebuilt-module-path=')];flags+=['-UNDEBUG','-O3','-g','-fexceptions']
        obj=out/'provider.o';run('compile',[*flags,'-c',str(fixture),'-o',str(obj)])
        raw=json.loads((build/'link.argv.json').read_text());link=[];skip=False
        for f in raw:
            if skip:skip=False;continue
            if f=='-o':skip=True;continue
            if f.endswith('/main.default.cpp.o'):continue
            link.append(f)
        binary=out/'provider';run('link',[*link,str(obj),'-o',str(binary)]);run('execute',[str(binary)])
        report['binary_sha256']=sha(binary);report['source_unchanged']=all(sha(source/n)==v for n,v in manifests.items())
        assert report['source_unchanged'] and pins()==report['inputs'];report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
