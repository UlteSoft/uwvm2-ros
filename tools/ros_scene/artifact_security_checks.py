#!/usr/bin/env python3
"""Qualify artifact authority/publication safety within the existing 64 GiB gate.

Every guest-like operation goes through the real broker and sealed snapshot
backend. Both sanitizer and GCC runs must pass against unchanged source bytes.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--recipes',type=Path,help='Also run AArch64, ARM32 and i686 using reviewed cross-build recipes')
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.geteuid()!=0 and os.sched_getaffinity(0).issubset(set(range(0,4)))
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=ROOT/'test/0084.ros-runtime/artifact_security.cpp'
    inputs=[source,Path(__file__).resolve(),ROOT/'src/uwvm2/uwvm/imported/scene/input.h']
    inputs+=list(sorted((ROOT/'src/uwvm2/imported/ros/scene').glob('*.h')))
    inputs+=list(sorted((ROOT/'src/uwvm2/imported/ros/runtime').glob('*.h')))
    if args.recipes:
        for arch in ('aarch64','armhf','i686'):
            inputs.extend((args.recipes/arch/name).resolve() for name in ('host_api.default.cpp.o.argv.json','link.argv.json'))
    def hashes():return {str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in inputs}
    record={'complete':False,'source_sha256':hashes(),'commands':[],'cgroup':str(GROUP),'memory_max':68719476736,
            'memory_swap_max':0,'cpu_affinity':sorted(os.sched_getaffinity(0)),'events_before':(GROUP/'memory.events').read_text()}
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=180)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append({'name':label,'argv':argv,'exit':result.returncode});save()
        if result.returncode:raise RuntimeError(label+': '+result.stderr[-5000:])
        print(label+': '+result.stdout.strip(),flush=True)
        return result.stdout
    passed=False
    try:
        with tempfile.TemporaryDirectory(prefix='uwvm-artifact-security-',dir='/dev/shm') as temporary:
            environment=dict(os.environ,UWVM_INPUT_TEST_TMP=temporary,ASAN_OPTIONS='detect_leaks=1')
            for label,compiler,flags in [('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),
                                         ('gcc13','/usr/bin/g++-13',[])]:
                binary=out/label
                run(label+'-compile',[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',
                    '-Ithird-parties/fast_io/include',*flags,str(source),'-o',str(binary)])
                run(label,[str(binary)],environment)
            if args.recipes:
                failures=[]
                for arch in ('aarch64','armhf','i686'):
                    compile_recipe=json.loads((args.recipes/arch/'host_api.default.cpp.o.argv.json').read_text())
                    link_recipe=json.loads((args.recipes/arch/'link.argv.json').read_text())
                    flags=[flag for flag in compile_recipe if flag.startswith(('--target=','--sysroot=','--gcc-install-dir=',
                           '-mfloat-abi=','-march=','-mcpu=','-mfpu=')) or flag in ('-m32','-m64')]
                    include=next(flag for flag in compile_recipe if '/bundled-runtimes/' in flag and flag.endswith('/include/c++/v1'))
                    libraries=list(dict.fromkeys(flag for flag in link_recipe if '/bundled-runtimes/' in flag and flag.endswith('.a')))
                    sysroot=next(flag.split('=',1)[1] for flag in flags if flag.startswith('--sysroot='))
                    assert libraries and Path(include).is_dir()
                    if arch=='i686':
                        prefix=[str(Path(sysroot)/'lib/ld-linux.so.2'),'--library-path',
                                ':'.join(str(Path(sysroot)/path) for path in ('lib/i386-linux-gnu','usr/lib/i386-linux-gnu'))]
                    else:
                        emulator=shutil.which({'aarch64':'qemu-aarch64','armhf':'qemu-arm'}[arch]);assert emulator
                        triple={'aarch64':'aarch64-linux-gnu','armhf':'arm-linux-gnueabihf'}[arch]
                        loader=Path(sysroot)/{'aarch64':'lib/aarch64-linux-gnu/ld-linux-aarch64.so.1',
                                             'armhf':'usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3'}[arch]
                        assert loader.is_file()
                        # QEMU -L redirects open("/") to its sysroot, while
                        # absolute /dev/shm paths may fall back to the host.
                        # An explicit loader preserves one real directory tree
                        # for the production component-by-component resolver.
                        prefix=[emulator,str(loader),'--library-path',
                                ':'.join(str(Path(sysroot)/path/triple) for path in ('lib','usr/lib'))]
                    for width in (64,) if arch=='aarch64' else (32,64):
                        label=arch+'-off'+str(width);binary=out/label
                        command=[compile_recipe[0],'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread',
                                 '-nostdinc++','-isystem',include,'-Isrc','-Ithird-parties/fast_io/include',*flags,
                                 '-D_FILE_OFFSET_BITS='+str(width),str(source),'-o',str(binary),'-fuse-ld=lld','-nostdlib++',
                                 '--unwindlib=none',*libraries,'-lpthread','-ldl','-lm','-latomic']
                        try:
                            run(label+'-compile',command)
                            output=run(label,[*prefix,str(binary)],environment)
                            assert 'native widths off_t='+str(width)+' ' in output,'requested file ABI was not selected'
                        except (RuntimeError,AssertionError,subprocess.TimeoutExpired) as error:
                            failures.append({'label':label,'reason':str(error)})
                            record['failures']=failures;save()
                if failures:raise RuntimeError('Cross-architecture qualification failed; inspect recorded failures')
        passed=True
    finally:
        record['sources_unchanged']=hashes()==record['source_sha256']
        record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=passed and record['sources_unchanged'];save()
        if passed and not record['sources_unchanged']:raise RuntimeError('Source changed during qualification')


if __name__=='__main__':main()
