#!/usr/bin/env python3
"""Qualify managed lifecycle semantics inside the existing 64 GiB resource gate.

Records native sanitizers/GCC/TSan and optional reviewed cross-ABI execution.
TSan retains its allocator; other builds exercise allocation failure injection.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',required=True,type=Path)
    parser.add_argument('--recipes',type=Path)
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset({1,2,3})
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=ROOT/'test/0084.ros-runtime/managed_lifecycle.cpp'
    broker_source=source.with_name('managed_lifecycle_broker.cpp')
    cases=[('core',source,'passed 6 managed-lifecycle safety groups'),
           ('broker',broker_source,'passed 4 managed-lifecycle broker safety groups')]
    inputs=[source,broker_source,Path(__file__).resolve()]
    inputs += sorted((ROOT/'src/uwvm2/imported/ros/scene').glob('*.h'))
    inputs += sorted((ROOT/'src/uwvm2/imported/ros/runtime').glob('*.h'))
    if args.recipes:
        inputs += [(args.recipes/arch/name).resolve() for arch in ('aarch64','armhf','i686')
                   for name in ('host_api.default.cpp.o.argv.json','link.argv.json')]
    def hashes():return {str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in inputs}
    record=dict(complete=False,sources=hashes(),commands=[],cgroup=str(GROUP),memory_max=68719476736,swap_max=0,
                affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),tsan_allocation_injection=False)
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,allow_failure=False):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=240,
                              env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1',TSAN_OPTIONS='halt_on_error=1'))
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode));save()
        print(label+': '+str(result.returncode),flush=True)
        if result.returncode and not allow_failure:raise RuntimeError(result.stderr[-6000:])
        return result
    passed=False
    try:
        for name,compiler,flags in [('sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined']),
                                    ('gcc','/usr/bin/g++-13',[]),('thread','/usr/bin/clang++-22',['-fsanitize=thread'])]:
            for kind,case_source,marker in cases:
                label=name+'-'+kind;binary=out/label
                run(label+'-compile',[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',
                                     '-fno-omit-frame-pointer',*flags,str(case_source),'-o',str(binary)])
                initialized=False
                for attempt in range(1,9 if name=='thread' else 2):
                    result=run(label+'-run-'+str(attempt),[str(binary)],name=='thread')
                    if not result.returncode:
                        assert marker in result.stdout
                        assert 'WARNING: ThreadSanitizer:' not in result.stderr
                        initialized=True;break
                    if not (result.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not result.stdout):
                        raise RuntimeError(result.stderr[-6000:])
                assert initialized,'Thread sanitizer failed to initialize'
        if args.recipes:
            for arch in ('aarch64','armhf','i686'):
                compile_recipe=json.loads((args.recipes/arch/'host_api.default.cpp.o.argv.json').read_text())
                link_recipe=json.loads((args.recipes/arch/'link.argv.json').read_text())
                flags=[flag for flag in compile_recipe if flag.startswith(('--target=','--sysroot=','--gcc-install-dir=',
                       '-mfloat-abi=','-march=','-mcpu=','-mfpu=')) or flag in ('-m32','-m64')]
                include=next(flag for flag in compile_recipe if '/bundled-runtimes/' in flag and flag.endswith('/include/c++/v1'))
                libraries=list(dict.fromkeys(flag for flag in link_recipe if '/bundled-runtimes/' in flag and flag.endswith('.a')))
                sysroot=Path(next(flag.split('=',1)[1] for flag in flags if flag.startswith('--sysroot=')))
                if arch=='i686':
                    prefix=[str(sysroot/'lib/ld-linux.so.2'),'--library-path',
                            ':'.join(str(sysroot/p) for p in ('lib/i386-linux-gnu','usr/lib/i386-linux-gnu'))]
                else:
                    emulator=shutil.which({'aarch64':'qemu-aarch64','armhf':'qemu-arm'}[arch]);assert emulator
                    triple={'aarch64':'aarch64-linux-gnu','armhf':'arm-linux-gnueabihf'}[arch]
                    loader=sysroot/{'aarch64':'lib/aarch64-linux-gnu/ld-linux-aarch64.so.1','armhf':'usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3'}[arch]
                    prefix=[emulator,str(loader),'--library-path',':'.join(str(sysroot/p/triple) for p in ('lib','usr/lib'))]
                for kind,case_source,marker in cases:
                    label=arch+'-'+kind;binary=out/label
                    run(label+'-compile',[compile_recipe[0],'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread',
                        '-nostdinc++','-isystem',include,'-Isrc',*flags,str(case_source),'-o',str(binary),'-fuse-ld=lld','-nostdlib++',
                        '--unwindlib=none',*libraries,'-lpthread','-ldl','-lm','-latomic'])
                    result=run(label,[*prefix,str(binary)])
                    assert marker in result.stdout
        passed=True
    finally:
        record['sources_unchanged']=hashes()==record['sources'];record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=passed and record['sources_unchanged'];save()
        if passed and not record['sources_unchanged']:raise RuntimeError('Source changed during qualification')


if __name__=='__main__':main()
