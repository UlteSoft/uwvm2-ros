#!/usr/bin/env python3
"""Relink genuine packet objects with the actual application memory profile.

This is a link/profile/actual Guest value scope. It is not a CMake build,
installed xmake product, new-language build or positive native ROS receipt.
The existing compiled fixture's own 60-method assertions must actually run.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import subprocess


def pin(path):
    body=path.read_bytes()
    return {"bytes":len(body),"sha256":hashlib.sha256(body).hexdigest()}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ("packet-run","memory-profile","wasi","runtime","validator","checker"):
        parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();work=Path.cwd();captured={};commands=[]
    def observe(path):
        path=path.absolute();path.resolve(strict=True);captured[str(path)]=pin(path);return path
    previous=json.loads(observe(args.packet_run/"work/result.json").read_text())
    if previous['status']!='PASS_CORE_PACKET_NATIVE_AND_WASM_VALUES_ONLY':
        raise ValueError('actual original successful native/Wasm packet receipt required')
    profile=observe(args.memory_profile);memory=json.loads(profile.read_text())
    values={'format':'uwvm-unitree-wasm32-command-memory-v1','target':'wasm32-wasip1',
        'stack_bytes':1048576,'initial_memory_bytes':16777216,'maximum_memory_bytes':67108864,
        'memory_index':0,'owned_memories':1,'shared':False,'memory64':False,'default_authority':[]}
    if any(type(memory.get(k)) is not type(v) or memory[k]!=v for k,v in values.items()):
        raise ValueError('reviewed actual complete-record command memory profile required')
    cxx=observe(args.wasi/'bin/clang++');runtime=observe(args.runtime);validator=observe(args.validator)
    checker=observe(args.checker)
    if previous['source_and_tool_inputs'][str(cxx)]!=pin(cxx):
        raise ValueError('actual object/linker provider pair differs')
    for path,before in previous['source_and_tool_inputs'].items():
        original=observe(Path(path))
        if pin(original)!=before:raise ValueError('original packet/provider input changed')
    objects=[observe(args.packet_run/('work/wasm-'+str(i)+'.o')) for i in range(4)]
    def run(name,argv):
        out=work/(name+'.stdout');err=work/(name+'.stderr')
        with out.open('xb') as stdout,err.open('xb') as stderr:
            result=subprocess.run(list(map(str,argv)),stdout=stdout,stderr=stderr,check=False)
        commands.append({'name':name,'argv':list(map(str,argv)),'exit':result.returncode,
                         'stdout':pin(out),'stderr':pin(err)})
        if result.returncode:raise ValueError(name+' failed; original actual diagnostics retained')
        return out.read_bytes()
    binary=work/'packet-product-memory.wasm'
    run('actual-profile-link',[cxx,*objects,'-fno-exceptions','-fno-rtti',
        '-Wl,--threads=1',
        '-Wl,-z,stack-size='+str(memory['stack_bytes']),
        '-Wl,--initial-memory='+str(memory['initial_memory_bytes']),
        '-Wl,--max-memory='+str(memory['maximum_memory_bytes']),'-o',binary])
    run('actual-full-validation',[validator,'validate',binary])
    spec=importlib.util.spec_from_file_location('task05_actual_product_memory_inspector',checker)
    inspector=importlib.util.module_from_spec(spec);spec.loader.exec_module(inspector)
    body=binary.read_bytes();imports=inspector.inspect(body)
    if any(row['kind']==2 for row in imports):raise ValueError('imported memory not selected')
    reader=inspector.Reader(body);reader.take(8);memories=[]
    while reader.position<len(reader.data):
        kind=reader.byte();section=inspector.Reader(reader.take(reader.uint()))
        if kind==5:
            if section.uint()!=1 or section.uint()!=1:raise ValueError('one owned maximum-bounded memory32 required')
            minimum,maximum=section.uint(),section.uint()
            if section.position!=len(section.data):raise ValueError('trailing memory section bytes')
            memories.append({'initial_pages':minimum,'maximum_pages':maximum})
    if memories!=[{'initial_pages':256,'maximum_pages':1024}]:
        raise ValueError('actual binary differs from the real application memory profile')
    output=run('actual-profile-values',[runtime,'--run',binary])
    if b'UNITREE_PACKET_LOCAL_OK operations=60 fixed_slots=16' not in output:
        raise ValueError('actual original all60-method/16-slot assertions did not pass')
    for path,before in captured.items():
        if pin(Path(path))!=before:raise ValueError('original link/profile input changed')
    report={'status':'PASS_GENUINE_PACKET_LINK_WITH_APPLICATION_MEMORY_PROFILE_ONLY',
        'source_and_tool_inputs':captured,'commands':commands,'artifact':pin(binary),
        'memory_profile':{'path':str(profile),**pin(profile)},'actual_owned_memory':memories[0],
        'stack_bytes_from_executed_link_argv':memory['stack_bytes'],'stack_size_binary_proof':False,
        'actual_Wasm_Guest_runs':1,'payload_methods':60,'pending_slots':16,
        'new_language_SDKs_qualified':0,'CMake_product_builds':0,'installed_xmake_builds':0,
        'native_ROS_runs':0,'default_authority':[]}
    (work/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'])


if __name__=='__main__':main()
