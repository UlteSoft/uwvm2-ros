#!/usr/bin/env python3
"""Execute a recording suite on reviewed AArch64, ARM32 and i686 ABIs.

Reuse only the architecture/toolchain portions of a retained successful output
qualification recipe. Discover and copy each actual dependency before compiling
the new source closure at -O3 -g. ARM execution uses the recorded QEMU/loader;
i686 uses its real loader. These are core semantics, not full VM qualifications.
"""
import argparse, hashlib, json, os, shlex, shutil, subprocess, time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--recipes',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--suite',choices=('store','contract','dispatch'),default='store')
    args=parser.parse_args();guard();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    prior=json.loads(args.recipes.read_text());assert prior['complete']
    source=Path('test/0084.ros-runtime/recording_'+args.suite+'.cpp');retained=out/'source'
    marker={'store':'9 recording store','contract':'3 recording carrier','dispatch':'8 recording dispatch'}[args.suite]
    report=dict(complete=False,scope=__doc__,suite=args.suite,recipe=str(args.recipes.resolve()),recipe_sha256=sha(args.recipes),
                cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),
                commands=[],inputs={},snapshots={},toolchain={})
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,argv):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=300)
        (out/(name+'.stdout')).write_text(result.stdout);(out/(name+'.stderr')).write_text(result.stderr)
        report['commands'].append(dict(name=name,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save()
        assert not result.returncode,(name,result.stderr[-6000:]);print('PASS '+name,flush=True);return result.stdout
    success=False
    try:
        dependencies=set();variants=[]
        for arch in ('aarch64','armhf','i686'):
            recipe=next(c['argv'] for c in prior['commands'] if c['name']==arch+'-compile')
            execute=next(c['argv'] for c in prior['commands'] if c['name']==arch)[:-1]
            index=next(i for i,x in enumerate(recipe) if x.endswith('/output_security.cpp'))
            flags=[x for x in recipe[:index] if not x.startswith('-O') and x not in ('-Isrc','-g')]
            flags+=['-O3','-g','-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include')]
            tail=recipe[index+3:];assert recipe[index+1]=='-o'
            report['toolchain'][flags[0]]=sha(flags[0])
            for path in [*execute,*tail]:
                if Path(path).is_file():report['toolchain'][path]=sha(path)
            result=run(arch+'-dependencies',[*flags,'-MM','-MT','recording',str(ROOT/source)])
            words=shlex.split(result.replace('\\\n',' '));assert words[0]=='recording:'
            dependencies.update(Path(p).resolve() for p in words[1:]);variants.append((arch,flags,tail,execute))
        for original in sorted(dependencies):
            assert original.is_relative_to(ROOT),original
            relative=original.relative_to(ROOT);target=retained/relative;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(original,target);report['inputs'][str(original)]=sha(original);report['snapshots'][str(relative)]=sha(target)
        runner=retained/'tools/ros_scene/recording_cross_checks.py';runner.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(__file__,runner)
        report['inputs'][str(Path(__file__).resolve())]=sha(__file__);report['snapshots'][str(runner.relative_to(retained))]=sha(runner)
        for arch,flags,tail,execute in variants:
            flags=[x.replace('-I'+str(ROOT/'src'),'-I'+str(retained/'src')).replace('-I'+str(ROOT/'third-parties/fast_io/include'),'-I'+str(retained/'third-parties/fast_io/include')) for x in flags]
            binary=out/arch
            run(arch+'-compile',[*flags,str(retained/source),'-o',str(binary),*tail,'-Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc'])
            result=run(arch,[*execute,str(binary)]);assert 'passed '+marker+' security groups' in result
        success=True
    finally:
        report['inputs_unchanged']=all(sha(p)==v for p,v in report['inputs'].items())
        report['snapshots_unchanged']=all(sha(retained/p)==v for p,v in report['snapshots'].items())
        report['events_after']=(GROUP/'memory.events').read_text()
        report['complete']=success and report['inputs_unchanged'] and report['snapshots_unchanged'];save()
    assert report['complete']
if __name__=='__main__':main()
