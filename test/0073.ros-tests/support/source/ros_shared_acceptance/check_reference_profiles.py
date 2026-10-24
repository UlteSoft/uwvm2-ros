#!/usr/bin/env python3
"""Inspect/probe the user's eight explicit WASIp1 reference library profiles.

Directory names are inventory only. Every compile/validation/Guest result has
its own state. This does not publish an unsupported profile or infer an EH-free
library closure from a noeh filename. Memtag and WASIp2 are never selected.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream,'sha256').hexdigest()


def members(raw):
    if raw[:8]!=b'!<arch>\n':raise ValueError('ordinary static archive required; thin archives are not source closure')
    offset=8;names=b''
    while offset<len(raw):
        if offset+60>len(raw):raise ValueError('truncated static archive header')
        header=raw[offset:offset+60];offset+=60
        if header[58:]!=b'`\n':raise ValueError('invalid static archive member header')
        size=int(header[48:58].decode('ascii').strip())
        if size<0 or size>len(raw)-offset:raise ValueError('static archive extent')
        name=header[:16].decode('ascii').strip();body=raw[offset:offset+size];offset+=size+(size&1)
        if name=='//':names=body;continue
        if name in ('/','/SYM64/','__.SYMDEF','__.SYMDEF SORTED'):continue
        if name.startswith('#1/'):
            count=int(name[3:]);name=body[:count].decode('utf-8').rstrip('\0');body=body[count:]
        elif name.startswith('/')and name[1:].isdigit():
            start=int(name[1:]);end=names.find(b'\n',start)
            if start>=len(names)or end<0:raise ValueError('archive long-name extent')
            name=names[start:end].decode('utf-8').rstrip('/')
        else:name=name.rstrip('/')
        yield name,body


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,required=True);p.add_argument('--sha256',required=True)
    p.add_argument('--tools',type=Path,required=True);p.add_argument('--toolchain',type=Path,required=True)
    p.add_argument('--wasm-tools',type=Path,required=True);p.add_argument('--runtime',type=Path)
    p.add_argument('--probe-optional-runtime',action='store_true',help='Attempt explicit EH/thread features; failures remain unqualified')
    a=p.parse_args();sys.path.insert(0,str(a.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    from check_wasm_imports import Reader,inspect
    root=Path.cwd();report=dict(status='RUNNING',resource=validate_resource_owner(),routes=[],commands=[],
        source_kind='user-supplied immutable reference, not official SDK 33',default_authority=[],
        memtag_selected=False,wasip2_selected=False,native_ros=0,gateway=0,upstream=0,qemu=0,vtune_hardware=0)
    report['source_pins']=[dict(path=str(path),bytes=path.stat().st_size,sha256=sha(path))
        for path in (Path(__file__),a.archive,a.tools/'resource_contract.py',a.tools/'check_wasm_imports.py')]
    def save():(root/'reference-profiles.json').write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    def run(label,argv):
        out,err=root/(label+'.stdout'),root/(label+'.stderr')
        try:
            with out.open('xb')as stdout,err.open('xb')as stderr:
                result=subprocess.run(list(map(str,argv)),stdout=stdout,stderr=stderr,stdin=subprocess.DEVNULL,timeout=90)
            code=result.returncode
        except subprocess.TimeoutExpired:code=124
        row=dict(label=label,argv=list(map(str,argv)),exit=code,stdout_sha256=sha(out),stderr_sha256=sha(err))
        report['commands'].append(row);save();return code,out.read_bytes()
    if sha(a.archive)!=a.sha256:raise ValueError('exact transferred reference archive required')
    source=root/'reference';source.mkdir()
    with tarfile.open(a.archive,'r:')as archive:archive.extractall(source,filter='data')
    # Use actual selected Clang resources, not host C/C++ headers. The older
    # reference compiler-rt archives are pinned separately and tested with the
    # actual newer linker; compatibility is not assumed from version names.
    resource=root/'resources';shutil.copytree(a.toolchain/'lib/clang/22/include',resource/'include')
    for bits in (32,64):
        original=source/'builtins/lib/wasip1'/('libclang_rt.builtins-wasm'+str(bits)+'.a')
        for relative in ('lib/wasi/'+original.name,'lib/wasm'+str(bits)+'-unknown-wasip1/libclang_rt.builtins.a',
                'lib/wasm'+str(bits)+'-unknown-wasip1-threads/libclang_rt.builtins.a'):
            destination=resource/relative;destination.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(original,destination)
    compiler=a.toolchain/'bin/clang';cxx=a.toolchain/'bin/clang++';linker=a.toolchain/'bin/wasm-ld'
    report['tools']=[dict(path=str(path),bytes=path.stat().st_size,sha256=sha(path))for path in(compiler,cxx,linker)]
    for exceptions in(False,True):
        tree='wasm-sysroot'if exceptions else'wasm-noeh-sysroot'
        for bits in(32,64):
            for threads in(False,True):
                target='wasm'+str(bits)+'-wasip1';variant=target+('-threads'if threads else'')
                sysroot=source/tree/variant;label=('eh'if exceptions else'noeh')+'-'+variant
                row=dict(id=label,target=target,address_bits=bits,wasi='wasip1',exceptions=exceptions,threads=threads,
                    startup='command',core_baseline='3.0',source_ready=sysroot.is_dir(),compiled=[],wasm_validated=[],
                    actual_guest=[],exception_library_closure_verified=False,upstream_provenance='NOT_SUPPLIED_WITH_REFERENCE',
                    public_supported_profile=False,not_run_reasons=[])
                report['routes'].append(row);save()
                if not sysroot.is_dir():row['not_run_reasons'].append('reference directory missing');continue
                pins=[];features=collections.Counter();kinds=collections.Counter();imports=collections.Counter();issues=[]
                for path in sorted(sysroot.rglob('*')):
                    if not path.is_file()or path.name=='.DS_Store'or path.name.startswith('._'):continue
                    if not path.resolve(strict=True).is_relative_to(source):raise ValueError('reference file link escaped input')
                    pins.append(dict(path=path.relative_to(sysroot).as_posix(),bytes=path.stat().st_size,sha256=sha(path)))
                    if path.suffix!='.a':continue
                    for name,body in members(path.read_bytes()):
                        if body[:8]==b'\0asm\1\0\0\0':
                            kinds['wasm-object']+=1
                            reader=Reader(body);reader.take(8)
                            while reader.position<len(reader.data):
                                section=reader.byte();part=Reader(reader.take(reader.uint()))
                                if section==0 and part.name()=='target_features':
                                    for _ in range(part.count()):features[chr(part.byte())+part.name()]+=1
                                    part.end()
                            try:
                                for imported in inspect(body):
                                    if imported['kind']==0 and imported['module']in('wasi_snapshot_preview1','wasi'):
                                        imports[(imported['module'],imported['name'],','.join(imported['params']))]+=1
                            except ValueError as error:issues.append(dict(archive=path.name,member=name,error=str(error)))
                        elif body[:4]in(b'BC\xc0\xde',b'\xde\xc0\x17\x0b'):kinds['llvm-bitcode-uninspected']+=1
                        else:kinds['other-uninspected']+=1
                row.update(files=pins,archive_members=dict(kinds),target_features=dict(features),
                    import_signatures=[dict(module=k[0],name=k[1],params=k[2],objects=v)for k,v in sorted(imports.items())],
                    uninspected=issues,libunwind_present=any(pin['path'].endswith('/libunwind.a')for pin in pins))
                config=sysroot/'include/c++/v1/__config'
                if config.is_file():
                    import re
                    match=re.search(r'#\s*define\s+_LIBCPP_VERSION\s+(\d+)',config.read_text())
                    row['libcpp_header_version']=int(match[1])if match else None
                # Thread artifacts use their real compiler target/header ABI.
                # The external family remains wasm32/64-wasip1 plus an axis.
                compiler_target=variant if threads else target
                row['compiler_target']=compiler_target
                common=['--no-default-config','--target='+compiler_target,'--sysroot='+str(sysroot),'-resource-dir='+str(resource),
                    '-fuse-ld='+str(linker),'-Os','-g0','-Wl,--threads=1']
                if threads:common+=['-pthread']
                for language,program in(('c',compiler),('cpp',cxx)):
                    app=root/(label+'.'+('c'if language=='c'else'cpp'));artifact=root/(label+'-'+language+'.wasm')
                    body=('#include <stdio.h>\n'if language=='c'else'#include <cstdio>\n#include <vector>\n')
                    if threads:
                        body+='#include <pthread.h>\nstatic void* work(void* p){++*(int*)p;return 0;}\nint main(){int x=5;pthread_t t;'
                        body+='if(pthread_create(&t,0,work,&x)||pthread_join(t,0))return 2;printf("ref=%d\\n",x);return x!=6;}\n'
                    elif language=='cpp'and exceptions:
                        body+='int main(){int x=0;try{throw 6;}catch(int y){x=y;}std::printf("ref=%d\\n",x);return x!=6;}\n'
                    elif language=='cpp':
                        body+='int main(){std::vector<int> v={1,2,3};int x=v[0]+v[1]+v[2];std::printf("ref=%d\\n",x);return x!=6;}\n'
                    else:body+='int main(void){int x=6;printf("ref=%d\\n",x);return x!=6;}\n'
                    app.write_text(body)
                    flags=['-std=c11']if language=='c'else['-std=c++20','-fwasm-exceptions'if exceptions else'-fno-exceptions']
                    closure=['-lunwind']if language=='cpp'and exceptions else[]
                    code,_=run(label+'-'+language+'-compile',[program,*common,*flags,app,*closure,'-o',artifact])
                    row['compiled'].append(dict(language=language,status='PASS'if code==0 else'FAILED_RETAINED'))
                    if code:save();continue
                    code,_=run(label+'-'+language+'-validate',[a.wasm_tools,'validate',artifact])
                    row['wasm_validated'].append(dict(language=language,status='PASS'if code==0 else'FAILED_RETAINED',sha256=sha(artifact),bytes=artifact.stat().st_size))
                    if not code and a.runtime and (a.probe_optional_runtime or not threads and not exceptions):
                        feature_flags=[]
                        if bits==64:feature_flags+=['--wasm-feature-enable-memory64','--wasm-feature-enable-table64']
                        if exceptions:feature_flags+=['--wasm-feature-enable-exceptions']
                        if threads:feature_flags+=['--wasm-feature-enable-threads']
                        code,output=run(label+'-'+language+'-guest',[a.runtime,'-Rint',*feature_flags,'--wasip1-global-noinherit-system-environment','--run',artifact])
                        row['actual_guest'].append(dict(language=language,status='PASS'if code==0 and output==b'ref=6\n'else'FAILED_RETAINED',
                            stdout_hex=output.hex(),ros_authority=[],explicit_feature_flags=feature_flags,
                            exercise='pthread-create-join'if threads else'throw-catch'if language=='cpp'and exceptions else'basic-io'))
                    else:row['not_run_reasons'].append(language+': optional EH/thread Runtime ABI is not yet qualified; no automatic opt-in')
                    save()
    report.update(status='CENSUS_COMPLETE_NOT_FULL_PROFILE_ACCEPTANCE',route_denominator=8,
        compile_denominator=16,compiled_passed=sum(x['status']=='PASS'for r in report['routes']for x in r['compiled']),
        validation_attempts=sum(len(r['wasm_validated'])for r in report['routes']),
        validation_passed=sum(x['status']=='PASS'for r in report['routes']for x in r['wasm_validated']),
        guest_attempts=sum(len(r['actual_guest'])for r in report['routes']),
        guest_passed=sum(x['status']=='PASS'for r in report['routes']for x in r['actual_guest']))
    save();print(json.dumps({k:v for k,v in report.items()if k not in('routes','commands','resource')}),flush=True)


if __name__=='__main__':main()
