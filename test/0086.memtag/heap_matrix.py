#!/usr/bin/env python3
"""Run C/C++ MTE allocator regressions in real Wasm through uwvm2-ros.
All path templates may contain {bits} and {target}; artifacts go only to --out.
Use the environment's cgroup launcher for this script and its child compilers.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
from vm_diagnostics import fault_matches

def imported_functions(data):
    position=8
    def number():
        nonlocal position
        value=shift=0
        while True:
            byte=data[position];position+=1;value|=(byte&127)<<shift
            if byte<128:return value
            shift+=7
    def string():
        nonlocal position
        count=number();result=data[position:position+count].decode();position+=count;return result
    while position<len(data):
        section=data[position];position+=1;size=number();end=position+size
        if section==2:
            imports=[]
            for _ in range(number()):
                module=string();name=string();kind=data[position];position+=1
                if kind!=0 or module!='wasi_snapshot_preview1':
                    raise ValueError('fixture has an unexpected import: '+module+'.'+name)
                number();imports.append(name)
            return imports
        position=end
    return []

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',type=Path,required=True)
    parser.add_argument('--vm',type=Path,required=True)
    parser.add_argument('--sysroot',required=True)
    parser.add_argument('--resource-dir',required=True)
    parser.add_argument('--cxx-include')
    parser.add_argument('--cxx-library-directory')
    parser.add_argument('--out',type=Path,required=True)
    parser.add_argument('--widths',default='32,64')
    parser.add_argument('--engines',default='int,aot')
    args=parser.parse_args()
    if bool(args.cxx_include)!=bool(args.cxx_library_directory):
        parser.error('both C++ paths are required together')
    output=args.out.resolve();output.mkdir(parents=True,exist_ok=True)
    compiler=args.compiler.resolve();vm=args.vm.resolve();source=Path(__file__).resolve().parent
    rows=[];builds=[]
    for bits in map(int,args.widths.split(',')):
        if bits not in (32,64):parser.error('widths must contain only 32 or 64')
        target=f'wasm{bits}-wasip1';variables=dict(bits=bits,target=target)
        sysroot=Path(args.sysroot.format(**variables)).resolve()
        resource=Path(args.resource_dir.format(**variables)).resolve()
        languages=['c','cpp'] if args.cxx_include else ['c']
        for language in languages:
            fixture=source/('heap_lifecycle.c' if language=='c' else 'heap_cpp.cc')
            for case in range(10 if language=='c' else 3):
                name=f'{language}{bits}-{case}';binary=output/(name+'.wasm')
                command=[str(compiler),'--target='+target,'--sysroot='+str(sysroot),
                    '-resource-dir',str(resource),'-O2','-g','-fno-builtin','-fsanitize=memtag',
                    '-nostartfiles','-DHEAP_CASE='+str(case),str(fixture),
                    '-Wl,--no-entry','-Wl,--max-memory=2097152','-o',str(binary)]
                if language=='cpp':
                    include=Path(args.cxx_include.format(**variables)).resolve()
                    library=Path(args.cxx_library_directory.format(**variables)).resolve()
                    command+=['-std=c++20','-fno-exceptions','-isystem',str(include),
                        str(library/'libc++.a'),str(library/'libc++abi.a')]
                process=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
                (output/(name+'-compile.log')).write_bytes(process.stdout)
                build=dict(name=name,command=command,exit=process.returncode,
                    fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),
                    libc_sha256=hashlib.sha256((sysroot/'lib'/target/'libc.a').read_bytes()).hexdigest())
                builds.append(build);(output/'builds.json').write_text(json.dumps(builds,indent=2)+'\n')
                if process.returncode:raise SystemExit(process.returncode)
                build['wasm_sha256']=hashlib.sha256(binary.read_bytes()).hexdigest()
                (output/'builds.json').write_text(json.dumps(builds,indent=2)+'\n')
                apis=imported_functions(binary.read_bytes())
                for enabled in ([True,False] if case==0 else [True]):
                    for engine in args.engines.split(','):
                        if engine not in ('int','aot'):parser.error('engines must be int or aot')
                        run=[str(vm),'--runtime-'+engine,'--wasm-feature-enable-memory64',
                            '--wasm-feature-enable-table64','--wasip1-single-create',binary.name,
                            '--wasip1-single-enable',binary.name,'--wasip1-single-grant',binary.name,'stderr.write']
                        if engine=='aot':run+=['--runtime-llvm-jit-cache-path','disable']
                        for api in apis:
                            run+=['--wasip1-single-interface',binary.name,api,
                                'allow' if api in ('fd_write','fd_write_wasm64') else 'deny']
                        if not enabled:run+=['--runtime-disable-memtag']
                        run+=['--',str(binary)]
                        process=subprocess.run(run,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
                        label=f'{name}-{engine}-{int(enabled)}'
                        (output/(label+'.log')).write_bytes(process.stdout)
                        text=process.stdout.decode('utf8','replace')
                        passed=(process.returncode==0 and '[warning]' not in text) if case==0 else fault_matches(
                            process.returncode,text,122,'memory tag mismatch')
                        rows.append(dict(name=label,bits=bits,language=language,case=case,engine=engine,
                            enabled=enabled,passed=passed,exit=process.returncode,command=run))
                        (output/'report.json').write_text(json.dumps(rows,indent=2)+'\n')
                        print(label,process.returncode,'PASS' if passed else 'FAIL',flush=True)
    raise SystemExit(any(not row['passed'] for row in rows))

if __name__=='__main__':main()
