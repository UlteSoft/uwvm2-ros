#!/usr/bin/env python3
"""Build the pinned, patched MTE libc into a private sysroot; never install.
Invoke this helper through the deployment's resource/cgroup wrapper.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','build','compiler','ar','ranlib','nm','builtins']:
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--target',choices=['wasm32-wasip1','wasm64-wasip1','wasm32-wasip1-threads','wasm64-wasip1-threads'],default='wasm32-wasip1')
    parser.add_argument('--jobs',type=int,default=4)
    parser.add_argument('--heap-only',action='store_true',help='Research baseline: omit compiler stack/global instrumentation of libc itself')
    args=parser.parse_args()
    if args.jobs<1:parser.error('--jobs must be positive')
    here=Path(__file__).resolve().parent
    pin=json.loads((here/'wasi-libc-mt-2.json').read_text())
    source=args.source.resolve();build=args.build.resolve()
    if source==build or source in build.parents:
        parser.error('use a separate private build directory outside the source checkout')
    revision=subprocess.check_output(['git','-C',str(source),'rev-parse','HEAD'],text=True).strip()
    if revision!=pin['revision']:parser.error('source revision differs from the maintained allocator patch pin')
    patch=here/pin['patch']
    reverse=subprocess.run(['git','-C',str(source),'apply','--reverse','--check',str(patch)],capture_output=True)
    if reverse.returncode:
        subprocess.run(['git','-C',str(source),'apply','--check',str(patch)],check=True)
        subprocess.run(['git','-C',str(source),'apply',str(patch)],check=True)
    build.mkdir(parents=True,exist_ok=True)
    receipt_path=build/'uwvm-memtag-libc-receipt.json'
    previous=json.loads(receipt_path.read_text()) if receipt_path.exists() else {}
    with args.compiler.open('rb') as binary:
        digest=hashlib.sha256()
        for chunk in iter(lambda:binary.read(1<<20),b''):digest.update(chunk)
        compiler_sha256=digest.hexdigest()
    configure=['cmake','-S',str(source),'-B',str(build),'-G','Ninja',
        '-DCMAKE_C_COMPILER='+str(args.compiler.absolute()),
        '-DCMAKE_AR='+str(args.ar.absolute()),'-DCMAKE_RANLIB='+str(args.ranlib.absolute()),
        '-DCMAKE_NM='+str(args.nm.absolute()),'-DBUILTINS_LIB='+str(args.builtins.resolve()),
        '-DTARGET_TRIPLE='+args.target,'-DENABLE_MEMTAG=ON','-DBUILD_SHARED=OFF',
        '-DSETJMP=OFF','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON','-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_INSTALL_PREFIX='+str(build/'sysroot'),
        '-DCMAKE_C_FLAGS='+('' if args.heap_only else '-fsanitize=memtag -g')]
    compile_command=['cmake','--build',str(build),'--parallel',str(args.jobs),'--target','sysroot','builtins']
    receipt=dict(repository=pin['repository'],revision=revision,patch_sha256=hashlib.sha256(patch.read_bytes()).hexdigest(),
        compiler=str(args.compiler.absolute()),compiler_sha256=compiler_sha256,compiler_version=subprocess.check_output([str(args.compiler.absolute()),'--version'],text=True),
        target=args.target,heap_only=args.heap_only,configure=configure,build=compile_command,completed=False)
    receipt_path.write_text(json.dumps(receipt,indent=2)+'\n')
    expected={'ENABLE_MEMTAG':'ON','BUILD_SHARED':'OFF','SETJMP':'OFF','TARGET_TRIPLE':args.target,
        'CMAKE_C_FLAGS':'' if args.heap_only else '-fsanitize=memtag -g','CMAKE_BUILD_TYPE':'Release'}
    def cached_settings():
        values={}
        for line in (build/'CMakeCache.txt').read_text().splitlines():
            if not line or line.startswith(('#','//')) or '=' not in line:continue
            key,value=line.split('=',1);values[key.split(':',1)[0]]=value
        return values
    # A changed compiler can make CMake discard command-line cache options while
    # returning success. Preserve compiler/ranlib argv[0] aliases, then verify
    # the effective configuration; never certify an accidentally untagged libc.
    for attempt in range(2):
        with (build/'uwvm-configure.log').open('w' if attempt==0 else 'a') as log:
            process=subprocess.run(configure,stdout=log,stderr=subprocess.STDOUT)
        if process.returncode:raise SystemExit(process.returncode)
        actual=cached_settings()
        if all(actual.get(key)==value for key,value in expected.items()):break
    else:
        raise SystemExit('CMake configuration does not match requested MTE policy: '+str(actual))
    receipt['effective_configuration']={key:actual.get(key) for key in expected}
    receipt_path.write_text(json.dumps(receipt,indent=2)+'\n')
    with (build/'uwvm-build.log').open('w') as log:
        # Ninja tracks argv, not changes to a compiler binary at the same path.
        if previous.get('compiler_sha256')!=compiler_sha256:
            clean=['cmake','--build',str(build),'--target','clean']
            process=subprocess.run(clean,stdout=log,stderr=subprocess.STDOUT)
            if process.returncode:raise SystemExit(process.returncode)
            receipt['toolchain_change_clean']=clean
        process=subprocess.run(compile_command,stdout=log,stderr=subprocess.STDOUT)
    if process.returncode:
        print((build/'uwvm-build.log').read_text()[-6000:])
        raise SystemExit(process.returncode)
    library=build/'sysroot/lib'/args.target/'libc.a'
    receipt.update(completed=True,libc_sha256=hashlib.sha256(library.read_bytes()).hexdigest(),
        sysroot=str(build/'sysroot'),resource_dir=str(build/'temp-resource-dir'))
    receipt_path.write_text(json.dumps(receipt,indent=2)+'\n')
    print(str(receipt_path))

if __name__=='__main__':main()
