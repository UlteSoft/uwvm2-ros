#!/usr/bin/env python3
"""Qualify pointer-mmap denial with actual native filters and cross-ABI BPF.

All builds retain -O3 -g. Native i386 executes on this kernel, not QEMU; ARM
and AArch64 interpret their generated filters under QEMU without claiming
kernel seccomp support. A retained pre-fix i386 negative control must fail.
"""
import argparse,hashlib,json,os,resource,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--old-source',type=Path,required=True)
    p.add_argument('--recipes',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    inode=GATE.stat().st_ino
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
        assert GATE.stat().st_ino==inode and (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    gate();assert os.sched_getaffinity(0).issubset({12,13,14,15});out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    source=a.source_root.resolve();old=a.old_source.resolve();fixture=ROOT/'test/0084.ros-runtime/model_sandbox_mmap.cpp'
    manifests={base:json.loads((base/'source-manifest.json').read_text())for base in [source,old]}
    for base,manifest in manifests.items():assert all(sha(base/n)==v for n,v in manifest.items())
    paths=[Path(__file__).resolve(),fixture,*[base/'source-manifest.json'for base in manifests],*[a.recipes/arch/name for arch in ['aarch64','armhf','i686']for name in ['host_api.default.cpp.o.argv.json','link.argv.json']]]
    pins=lambda:{str(p):sha(p)for p in paths}
    record=dict(complete=False,scope=__doc__,inputs=pins(),source=str(source),old_source=str(old),commands=[],cgroup=str(GATE),inode=inode,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(name,cmd,negative=False):
        gate();started=time.monotonic();r=subprocess.run([str(x)for x in cmd],capture_output=True,text=True,timeout=300)
        (out/(name+'.stdout')).write_text(r.stdout);(out/(name+'.stderr')).write_text(r.stderr)
        record['commands'].append(dict(name=name,argv=[str(x)for x in cmd],exit=r.returncode,expected_failure=negative,seconds=time.monotonic()-started));save()
        assert (r.returncode!=0 if negative else r.returncode==0),(name,r.returncode,r.stderr[-5000:])
        if negative:assert 'kernel child status=2816' in r.stderr and 'legacy executable mapping admitted' in r.stderr,r.stderr
        print('PASS '+name,flush=True)
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    def include(base):return ['-I'+str(base/'src'),'-isystem',str(base/'third-parties/fast_io/include')]
    try:
        for name,compiler,extra in [('asan','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc','/usr/bin/g++-13',[])]:
            binary=out/name;run(name+'-compile',[compiler,*flags,*extra,*include(source),fixture,'-o',binary]);run(name,[binary])
        for arch in ['aarch64','armhf','i686']:
            recipe=json.loads((a.recipes/arch/'host_api.default.cpp.o.argv.json').read_text());link=json.loads((a.recipes/arch/'link.argv.json').read_text())
            target=[f for f in recipe if f.startswith(('--target=','--sysroot=','--gcc-install-dir=','-mfloat-abi=','-march=','-mcpu=','-mfpu='))or f in ('-m32','-m64')]
            cxx=next(f for f in recipe if '/bundled-runtimes/'in f and f.endswith('/include/c++/v1'))
            libs=list(dict.fromkeys(f for f in link if '/bundled-runtimes/'in f and f.endswith('.a')))
            sysroot=Path(next(f.split('=',1)[1]for f in target if f.startswith('--sysroot=')))
            if arch=='i686':prefix=[sysroot/'lib/ld-linux.so.2','--library-path',':'.join(str(sysroot/p)for p in ['lib/i386-linux-gnu','usr/lib/i386-linux-gnu'])]
            else:
                triple={'aarch64':'aarch64-linux-gnu','armhf':'arm-linux-gnueabihf'}[arch]
                loader=sysroot/({'aarch64':'lib/aarch64-linux-gnu/ld-linux-aarch64.so.1','armhf':'usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3'}[arch])
                prefix=[shutil.which({'aarch64':'qemu-aarch64','armhf':'qemu-arm'}[arch]),loader,'--library-path',':'.join(str(sysroot/p/triple)for p in ['lib','usr/lib'])]
            for name,base in [(arch,source)]+([(arch+'-negative',old)]if arch=='i686'else[]):
                binary=out/name
                run(name+'-compile',[recipe[0],*flags,'-nostdinc++','-isystem',cxx,*target,*include(base),fixture,'-o',binary,'-fuse-ld=lld','-nostdlib++','--unwindlib=none',*libs,'-lpthread','-ldl','-lm','-latomic'])
                run(name,[*prefix,binary,*(['--kernel-only']if name.endswith('negative')else ['--interpret-only']if arch!='i686'else [])],name.endswith('negative'))
        record['sources_unchanged']=all(sha(base/n)==v for base,manifest in manifests.items()for n,v in manifest.items())
        assert record['sources_unchanged'] and pins()==record['inputs'];record['complete']=True
    finally:record['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
