#!/usr/bin/env python3
"""Qualify volatile cache transactions in the existing 64 GiB zero-swap gate.

Native sanitizer/GCC, optional TSan, and optional real cross-ABI execution use
one unchanged implementation. QEMU qualifies semantics, not hardware speed.
Only the known pre-main TSan unexpected-mapping startup error is retried.
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
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--recipes',type=Path)
    parser.add_argument('--thread',action='store_true')
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(4)))
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=ROOT/'test/0084.ros-runtime/output_security.cpp'
    unsupported=source.with_name("output_unsupported.cpp")
    inputs=[source,unsupported,Path(__file__).resolve()]
    inputs+=sorted((ROOT/'src/uwvm2/imported/ros/scene').glob('*.h'))
    inputs+=sorted((ROOT/'src/uwvm2/imported/ros/runtime').glob('*.h'))
    inputs+=sorted((ROOT/'src/uwvm2/utils/container').glob('*.h'))
    inputs+=sorted((ROOT/'third-parties/fast_io/include').rglob('*.h'))
    include_flags=['-I'+str(ROOT/'third-parties'/name/'include') for name in ('fast_io','bizwen','boost_unordered')]
    allocation_flags=['-fno-builtin-malloc','-fno-builtin-calloc','-fno-builtin-realloc','-Wl,--wrap=malloc','-Wl,--wrap=calloc','-Wl,--wrap=realloc']
    if args.recipes:
        for arch in ('aarch64','armhf','i686'):
            inputs.extend((args.recipes/arch/name).resolve() for name in ('host_api.default.cpp.o.argv.json','link.argv.json'))
    def hashes():return {str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in inputs}
    record=dict(complete=False,source_sha256=hashes(),commands=[],cgroup=str(GROUP),memory_max=68719476736,memory_swap_max=0,
                cpu_affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None,allow_failure=False):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=240)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(name=label,argv=argv,exit=result.returncode));save()
        if result.returncode and not allow_failure:raise RuntimeError(label+': '+result.stderr[-5000:])
        print(label+': '+str(result.returncode),flush=True)
        return result
    passed=False
    try:
        for label,compiler,flags in [('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),
                                     ('gcc13','/usr/bin/g++-13',[])]:
            binary=out/label
            run(label+'-compile',[compiler,'-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',*include_flags,*allocation_flags,*flags,str(source),'-o',str(binary)])
            result=run(label,[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
            assert 'passed 7 volatile-output safety groups' in result.stdout
            fallback=out/(label+'-unsupported')
            run(label+'-unsupported-compile',[compiler,'-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',*include_flags,'-U__linux__',*flags,str(unsupported),'-o',str(fallback)])
            run(label+'-unsupported',[str(fallback)])
        if args.thread:
            record['thread_sanitizer_allocation_injection']=False
            binary=out/'thread-sanitized'
            run('thread-compile',['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',*include_flags,*allocation_flags,
                                 '-fsanitize=thread','-fno-omit-frame-pointer',str(source),'-o',str(binary)])
            initialized=False
            for attempt in range(1,9):
                result=run('thread-run-'+str(attempt),[str(binary)],dict(os.environ,TSAN_OPTIONS='halt_on_error=1'),True)
                if not result.returncode:
                    assert 'passed 7 volatile-output safety groups' in result.stdout and 'WARNING: ThreadSanitizer:' not in result.stderr
                    initialized=True;break
                if not (result.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not result.stdout):
                    raise RuntimeError(result.stderr[-5000:])
            assert initialized,'TSan failed to initialize after eight preserved layout attempts'
        if args.recipes:
            for arch in ('aarch64','armhf','i686'):
                compile_recipe=json.loads((args.recipes/arch/'host_api.default.cpp.o.argv.json').read_text())
                link_recipe=json.loads((args.recipes/arch/'link.argv.json').read_text())
                flags=[flag for flag in compile_recipe if flag.startswith(('--target=','--sysroot=','--gcc-install-dir=',
                       '-mfloat-abi=','-march=','-mcpu=','-mfpu=')) or flag in ('-m32','-m64')]
                include=next(flag for flag in compile_recipe if '/bundled-runtimes/' in flag and flag.endswith('/include/c++/v1'))
                libraries=list(dict.fromkeys(flag for flag in link_recipe if '/bundled-runtimes/' in flag and flag.endswith('.a')))
                sysroot=next(flag.split('=',1)[1] for flag in flags if flag.startswith('--sysroot='))
                if arch=='i686':
                    prefix=[str(Path(sysroot)/'lib/ld-linux.so.2'),'--library-path',
                            ':'.join(str(Path(sysroot)/p) for p in ('lib/i386-linux-gnu','usr/lib/i386-linux-gnu'))]
                else:
                    emulator=shutil.which({'aarch64':'qemu-aarch64','armhf':'qemu-arm'}[arch]);assert emulator
                    triple={'aarch64':'aarch64-linux-gnu','armhf':'arm-linux-gnueabihf'}[arch]
                    loader=Path(sysroot)/{'aarch64':'lib/aarch64-linux-gnu/ld-linux-aarch64.so.1','armhf':'usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3'}[arch]
                    prefix=[emulator,str(loader),'--library-path',':'.join(str(Path(sysroot)/p/triple) for p in ('lib','usr/lib'))]
                binary=out/arch
                run(arch+'-compile',[compile_recipe[0],'-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread',
                    '-nostdinc++','-isystem',include,'-Isrc',*include_flags,*allocation_flags,*flags,str(source),'-o',str(binary),'-fuse-ld=lld','-nostdlib++',
                    '--unwindlib=none',*libraries,'-lpthread','-ldl','-lm','-latomic'])
                result=run(arch,[*prefix,str(binary)])
                assert 'passed 7 volatile-output safety groups' in result.stdout
        passed=True
    finally:
        record['sources_unchanged']=hashes()==record['source_sha256'];record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=passed and record['sources_unchanged'];save()
        if passed and not record['sources_unchanged']:raise RuntimeError('Implementation changed during qualification')


if __name__=='__main__':main()
