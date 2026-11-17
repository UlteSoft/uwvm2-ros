#!/usr/bin/env python3
"""Qualify ROSv2 child policy, actual CLI callbacks and debugger adapters.

Source-authored fixture routes exercise the management boundary only. They do
not register a shipping vendor driver or prove physical robot safety. All
compilers and descendants remain in the existing 64 GiB zero-swap cgroup.
"""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time

from device_dispatch_checks import ROOT, GROUP, guard, sha


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--source-root',type=Path,help='Frozen repository with matching production and debugger sources')
    args=parser.parse_args();guard();out=args.output.resolve()
    source_root=args.source_root.resolve() if args.source_root else ROOT
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True,exist_ok=False);snapshot=out/'source'
    sources=['test/0084.ros-runtime/platform_extension.cpp','test/0084.ros-runtime/scene_debug.cc',
             'test/0084.ros-runtime/deployment_attenuation.cpp']
    variants=[('clang','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc','/usr/bin/g++-13',[])]
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread',
           '-DUWVM=2','-DUWVM_IMPORT_ROSV2','-DUWVM_DISABLE_JIT','-DUWVM_USE_UWVM_INT']
    includes=['src','third-parties/fast_io/include','third-parties/bizwen/include','third-parties/boost_unordered/include']
    report=dict(complete=False,scope=__doc__,cgroup=str(GROUP),memory_max=68719476736,swap_max=0,
        events_before=(GROUP/'memory.events').read_text(),affinity=sorted(os.sched_getaffinity(0)),
        inputs={},snapshots={},commands=[],production_platforms_registered=0,source_root=str(source_root))
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,cwd=ROOT,env=None):
        guard();start=time.monotonic();done=subprocess.run(argv,cwd=cwd,capture_output=True,text=True,timeout=360,env=env)
        (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=done.returncode,seconds=time.monotonic()-start));save()
        print(label+': '+str(done.returncode),flush=True)
        assert done.returncode==0,(label,done.stderr[-7000:]);return done
    success=False
    try:
        deps=set()
        for kind,compiler,extra in variants:
            for source in sources:
                parsed=run(kind+'-'+Path(source).stem+'-dependencies',[compiler,*flags,*extra,
                    *['-I'+str(source_root/p) for p in includes],'-MM','-MT','child',str(source_root/source)])
                words=shlex.split(parsed.stdout.replace('\\\n',' '));assert words[0]=='child:'
                deps.update(Path(p).resolve() for p in words[1:])
        # Freeze the adapters, imported helper modules and their actual editor
        # regressions as well, so a running workspace edit cannot change tests.
        for pattern in ('*.py','*.js','*.json'):
            deps.update((source_root/'tools/debug').glob(pattern))
        deps.update((source_root/'tools/debug/media').glob('*.js'))
        deps.update((source_root/'tools/debug/media').glob('*.css'))
        deps.add(source_root/'tools/debug/package.json')
        deps.update(source_root/p for p in ['test/0084.ros-runtime/test_scene_dap.py','test/0084.ros-runtime/test_scene_ui.cjs',
            'test/0018.debugger/test_inspector_views.cjs','test/0018.debugger/test_inspector_lifecycle.cjs',
            'tools/ros_scene/platform_extension_checks.py','tools/ros_scene/device_dispatch_checks.py'])
        for source in sorted(deps):
            assert source.is_relative_to(source_root),source
            relative=source.relative_to(source_root);dest=snapshot/relative;dest.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(source,dest);report['inputs'][str(source)]=sha(source);report['snapshots'][str(relative)]=sha(dest)
        save()
        for kind,compiler,extra in variants:
            for source in sources:
                label=kind+'-'+Path(source).stem;binary=out/label
                run(label+'-compile',[compiler,*flags,*extra,*['-I'+str(snapshot/p) for p in includes],str(snapshot/source),'-o',str(binary)])
                result=run(label,[str(binary)],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
                assert 'PASS ' in result.stdout
        run('dap',['/usr/bin/python3',str(snapshot/'test/0084.ros-runtime/test_scene_dap.py'),'--formatter',str(out/'clang-scene_debug')],snapshot)
        node=shutil.which('node');assert node
        for source in ['test/0084.ros-runtime/test_scene_ui.cjs','test/0018.debugger/test_inspector_views.cjs','test/0018.debugger/test_inspector_lifecycle.cjs']:
            run(Path(source).stem,[node,str(snapshot/source)],snapshot)
        success=True
    finally:
        report['inputs_unchanged']=all(Path(p).is_file() and sha(p)==h for p,h in report['inputs'].items())
        report['snapshots_unchanged']=all(sha(snapshot/p)==h for p,h in report['snapshots'].items())
        report['events_after']=(GROUP/'memory.events').read_text()
        report['complete']=success and report['inputs_unchanged'] and report['snapshots_unchanged'];save()
    assert report['complete']


if __name__=='__main__':main()
