#!/usr/bin/env python3
"""Execute bounded policy, messaging or receiver-control cores on five Linux ABIs. No hardware qualification."""
import argparse
import json
from pathlib import Path
import shlex
import shutil
import subprocess
import time

from device_dispatch_checks import ROOT, GROUP, guard, sha
from pack_communication_worker import inspect_elf


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--recipes',type=Path,required=True)
    parser.add_argument('--case',choices=['policy','agilex','bounded-message','module-exchange','receiver-control'],default='policy');args=parser.parse_args();guard()
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    previous=json.loads(args.recipes.read_text());assert previous['complete']
    source=Path('test/0084.ros-runtime')/{'policy':'platform_extension_core.cpp','agilex':'platform_agilex.cpp','bounded-message':'bounded_message.cpp','module-exchange':'module_exchange.cpp','receiver-control':'driver_control.cpp'}[args.case];snapshot=out/'source'
    marker={'policy':'PASS cross-ABI child ownership and live parent revocation','agilex':'PASS AgileX exact motion codec and common authority fencing','bounded-message':'PASS bounded ROS messages:','module-exchange':'PASS registered messages:','receiver-control':'PASS 7 receiver ownership security groups'}[args.case]
    report=dict(complete=False,scope=__doc__,cgroup=str(GROUP),memory_max=68719476736,swap_max=0,
        case=args.case,recipe_sha256=sha(args.recipes),commands=[],inputs={},snapshots={},elf={},events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();done=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=240)
        (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=done.returncode,seconds=time.monotonic()-start));save()
        print(label+': '+str(done.returncode),flush=True);assert done.returncode==0,(label,done.stderr[-6000:]);return done
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    variants=[('x86_64',['/usr/bin/clang++-22',*flags],[],[])]
    for architecture in ('aarch64','armhf','i686'):
        recipe=next(c['argv'] for c in previous['commands'] if c['name']==architecture+'-compile')
        index=next(i for i,x in enumerate(recipe) if x.endswith('/output_security.cpp'));assert recipe[index+1]=='-o'
        before=[p for p in recipe[:index] if not p.startswith('-I')]
        after=[p for p in recipe[index+3:] if not p.startswith(('-I','-Wl,--wrap','-fno-builtin'))]
        execute=next(c['argv'] for c in previous['commands'] if c['name']==architecture)[:-1]
        variants.append((architecture,before,after,execute))
    # ARM base PCS uses its actual paired runtime and GCC/sysroot layout. It
    # must never be linked with ARM hard-float archives or that interpreter.
    runtime=ROOT/'.tmp/github-push-build/work/arm/bundled-runtimes/341b776ba8f2988688d3c8b0268e89410ec0e1879ec9ba52dffb8bf28fc169c2'
    sysroot=ROOT/'.tmp/github-push-build/sysroots/arm'
    metadata=json.loads((runtime/'uwvm-runtimes.json').read_text());assert metadata['compiler_target']=='arm-linux-gnueabi'
    compiler=variants[1][1][0]
    variants.append(('arm',[compiler,*flags,'-nostdinc++','-isystem',str(runtime/'include/c++/v1'),
        '--target=arm-linux-gnueabi','--sysroot='+str(sysroot),'--gcc-install-dir='+str(sysroot/'usr/lib/gcc/arm-linux-gnueabi/12'),
        '-march=armv7-a','-mfloat-abi=soft'],['-fuse-ld=lld','-nostdlib++','--unwindlib=none',
        *[str(runtime/'lib'/name) for name in ('libc++.a','libc++abi.a','libunwind.a')],'-lpthread','-ldl','-lm','-latomic'],
        ['/home/macromodel/.local/bin/qemu-arm',str(sysroot/'lib/arm-linux-gnueabi/ld-linux.so.3'),'--library-path',
         str(sysroot/'lib/arm-linux-gnueabi')+':'+str(sysroot/'usr/lib/arm-linux-gnueabi')]))
    success=False
    try:
        dependencies=set()
        for arch,before,after,execute in variants:
            report['inputs'][before[0]]=sha(before[0])
            for item in [*after,*execute,str(runtime/'uwvm-runtimes.json')]:
                if Path(item).is_file():report['inputs'][str(Path(item).resolve())]=sha(item)
            result=run(arch+'-dependencies',[*before,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),'-MM','-MT','policy',str(ROOT/source)])
            words=shlex.split(result.stdout.replace('\\\n',' '));assert words[0]=='policy:'
            dependencies.update(Path(p).resolve() for p in words[1:])
        for original in sorted(dependencies):
            assert original.is_relative_to(ROOT),original
            relative=original.relative_to(ROOT);target=snapshot/relative;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(original,target);report['inputs'][str(original)]=sha(original);report['snapshots'][str(relative)]=sha(target)
        for arch,before,after,execute in variants:
            binary=out/arch
            run(arch+'-compile',[*before,'-I'+str(snapshot/'src'),'-I'+str(snapshot/'third-parties/fast_io/include'),str(snapshot/source),'-o',str(binary),*after])
            report['elf'][arch]=inspect_elf(binary.read_bytes(),arch);save()
            assert marker in run(arch,[*execute,str(binary)]).stdout
        success=True
    finally:
        report['inputs_unchanged']=all(sha(p)==h for p,h in report['inputs'].items())
        report['snapshots_unchanged']=all(sha(snapshot/p)==h for p,h in report['snapshots'].items())
        report['runner_sha256']=sha(__file__);report['events_after']=(GROUP/'memory.events').read_text()
        report['complete']=success and report['inputs_unchanged'] and report['snapshots_unchanged'];save()
    assert report['complete']


if __name__=='__main__':main()
