#!/usr/bin/env python3
"""Compare the unchanged upstream XML serializer across reviewed native ABIs.

This checks the same 56 bounded-output cases on AArch64/ARM32/i686 against the
prepared x86_64 reference. QEMU/native-IA32 correctness is not target hardware
performance, ROS executor coverage or a substitute for actual Wasm execution.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(10,16)))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepared',type=Path,required=True)
    parser.add_argument('--recipes',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard()
    prepared=args.prepared.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    assert not (out/'results.json').exists(),'Use a fresh output directory'
    qualification=json.loads((prepared/'results.json').read_text());assert qualification['preparation_complete']
    inputs={str(prepared/name):value for name,value in qualification['input_sha256'].items()}
    inputs[str(prepared/'native.stdout')]=sha(prepared/'native.stdout')
    for name,digest in inputs.items():assert sha(name)==digest
    baseline=(prepared/'native.stdout').read_bytes();assert baseline.endswith(b'checks:56\n')
    parser_dir=next((prepared/'sources').glob('*/3rdparty/tinyxml2'))
    report=dict(complete=False,scope=__doc__,cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,
        swap_max=0,events_before=(GATE/'memory.events').read_text(),inputs=inputs,recipes={},toolchain={},commands=[],matches=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,command):
        guard();start=time.monotonic();result=subprocess.run([str(x) for x in command],cwd=ROOT,capture_output=True,timeout=180)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(name=name,argv=[str(x) for x in command],exit=result.returncode,seconds=time.monotonic()-start));save()
        assert result.returncode==0,(name,result.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
        return result.stdout
    for architecture in ['aarch64','armhf','i686']:
        compile_path=args.recipes/architecture/'host_api.default.cpp.o.argv.json';link_path=args.recipes/architecture/'link.argv.json'
        compile_recipe=json.loads(compile_path.read_text());link_recipe=json.loads(link_path.read_text())
        report['recipes'][architecture]={str(compile_path.resolve()):sha(compile_path),str(link_path.resolve()):sha(link_path)}
        flags=[s for s in compile_recipe if s.startswith(('--target=','--sysroot=','--gcc-install-dir=','-mfloat-abi=','-march=','-mcpu=','-mfpu=')) or s in ('-m32','-m64')]
        include=next(s for s in compile_recipe if '/bundled-runtimes/' in s and s.endswith('/include/c++/v1'))
        libs=list(dict.fromkeys(s for s in link_recipe if '/bundled-runtimes/' in s and s.endswith('.a')))
        sysroot=Path(next(s.split('=',1)[1] for s in flags if s.startswith('--sysroot=')))
        assert libs and Path(include).is_dir()
        compiler=Path(compile_recipe[0]);report['toolchain'][str(compiler)]=sha(compiler)
        for lib in libs:report['toolchain'][lib]=sha(lib)
        binary=out/architecture
        command=[compiler,'-std=c++17','-O1','-g','-fno-exceptions','-fno-rtti','-ffunction-sections','-fdata-sections',
            '-nostdinc++','-isystem',include,*flags,'-I'+str(parser_dir),'-I'+str(prepared),
            parser_dir/'tinyxml2.cpp',prepared/'upstream_artifact_output.cpp','-o',binary,'-fuse-ld=lld','-nostdlib++','--unwindlib=none',
            '-Wl,--gc-sections',*libs,'-lpthread','-ldl','-lm','-latomic']
        run('compile-'+architecture,command)
        if architecture in ('aarch64','armhf'):
            emulator=shutil.which({'aarch64':'qemu-aarch64','armhf':'qemu-arm'}[architecture]);assert emulator
            report['toolchain'][str(Path(emulator).resolve())]=sha(emulator)
            prefix=[emulator,'-L',sysroot]
        else:
            prefix=[sysroot/'lib/ld-linux.so.2','--library-path',':'.join(str(sysroot/p) for p in ['lib/i386-linux-gnu','usr/lib/i386-linux-gnu'])]
        actual=run('execute-'+architecture,[*prefix,binary]);assert actual==baseline,architecture+' output differs'
        report['matches'].append(architecture);save()
    for name,digest in {**inputs,**report['toolchain']}.items():assert sha(name)==digest
    report['complete']=True;report['events_after']=(GATE/'memory.events').read_text();save()


if __name__=='__main__':main()
