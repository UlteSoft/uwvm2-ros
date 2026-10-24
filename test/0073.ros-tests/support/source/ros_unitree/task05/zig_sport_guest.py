#!/usr/bin/env python3
"""Real Zig SDK/application builds and packet values through actual providers.

Finite original-owner workload only. Uses the actual complete C records and
successful packet objects, exact WASI libc/no-EH C++ archives and existing Zig
ROS2/client modules. A previously successful real Nim/C++ oracle verifies all
60 complete command outputs independently; no schema/response is fabricated.
This is private integration, not public installed xmake or native ROS.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import shutil
import subprocess


def pin(path):
    body=path.read_bytes();return {'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('zig','foundation','zig-format-run','packet-run','nim-oracle-run','wasi',
                 'runtime','validator','checker','memory-profile'):
        parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();work=Path.cwd();captured={};commands=[]
    def observe(path):
        path=path.absolute();path.resolve(strict=True);captured[str(path)]=pin(path);return path
    zig=observe(args.zig);runtime=observe(args.runtime);validator=observe(args.validator);checker=observe(args.checker)
    observe(checker.parent.parent/'abi/abi-v1.json')
    observe(zig.parent/'lib/std/zig/LibCInstallation.zig')
    profile=observe(args.memory_profile);memory=json.loads(profile.read_text())
    required={'format':'uwvm-unitree-wasm32-command-memory-v1','target':'wasm32-wasip1',
        'memory_index':0,'owned_memories':1,'shared':False,'memory64':False,'default_authority':[],
        'stack_bytes':1048576,'initial_memory_bytes':16777216,'maximum_memory_bytes':67108864}
    if any(type(memory.get(k)) is not type(v) or memory[k]!=v for k,v in required.items()):
        raise ValueError('exact reviewed complete-record command memory profile required')
    packet=json.loads(observe(args.packet_run/'work/result.json').read_text())
    if packet['status']!='PASS_CORE_PACKET_NATIVE_AND_WASM_VALUES_ONLY':
        raise ValueError('genuine successful original packet build required')
    for path,before in packet['source_and_tool_inputs'].items():
        if pin(observe(Path(path)))!=before:raise ValueError('original packet/provider pair changed')
    nim=json.loads(observe(args.nim_oracle_run/'work/result.json').read_text())
    if nim['status']!='PASS_PRIVATE_NIM_WASI_VALUES_COMMANDS_AND_DEFAULT_DENIAL':
        raise ValueError('actual successful complete-record Nim/C++ oracle required')
    oracle=observe(Path(nim['artifacts']['nim_sport_values']['path']))
    if any(pin(oracle)[k]!=nim['artifacts']['nim_sport_values'][k] for k in ('bytes','sha256')):
        raise ValueError('actual compiled oracle artifact changed')
    formatted=json.loads(observe(args.zig_format_run/'work/result.json').read_text())
    if formatted['status']!='PASS_ZIG_FORMAT_ONLY' or formatted['tool']['sha256']!=pin(zig)['sha256']:
        raise ValueError('actual matching Zig formatter/source receipt required')
    source=work/'source';source.mkdir();leaves={
        'sdk/origin/robots/unitree/language/zig/unitree.zig':'unitree.zig',
        'sdk/origin/robots/unitree/examples/zig/sport_topic_command.zig':'sport_topic_command.zig',
        'test/0073.ros-tests/support/source/ros_platform05/unitree/zig_sport_values.zig':'zig_sport_values.zig'}
    for name,destination in leaves.items():
        path=observe(args.zig_format_run/'work/formatted'/name)
        expected=next(row['after'] for row in formatted['sources'] if row['path']==name)
        if pin(path)!=expected:raise ValueError('actual Zig source postimage changed')
        shutil.copyfile(path,source/destination)
    foundation=args.foundation.resolve(strict=True)
    for path in sorted(foundation.glob('*.zig')):observe(path)
    for path in sorted((args.packet_run/'work/include').rglob('*')):
        if path.is_file():observe(path)
    objects=[observe(args.packet_run/('work/wasm-'+str(i)+'.o')) for i in range(3)]
    sysroot=args.wasi/'share/wasi-sysroot';library=sysroot/'lib/wasm32-wasip1'
    archives=[observe(library/'noeh'/name) for name in ('libc++.a','libc++abi.a')]
    for name in ('libc.a','crt1-command.o'):observe(library/name)
    for name in ('stdint.h','string.h'):observe(sysroot/'include/wasm32-wasip1'/name)
    observe(zig.parent/'lib/include/stddef.h')
    libc=work/'actual-wasi-libc.txt'
    libc.write_text('include_dir='+str(sysroot/'include/wasm32-wasip1')+'\n'+
        'sys_include_dir='+str(sysroot/'include/wasm32-wasip1')+'\ncrt_dir='+str(library)+
        '\nmsvc_lib_dir=\nkernel32_lib_dir=\ngcc_dir=\n')
    def run(name,argv,data=b'',expected=0):
        out=work/(name+'.stdout');err=work/(name+'.stderr')
        with out.open('xb') as stdout,err.open('xb') as stderr:
            result=subprocess.run(list(map(str,argv)),input=data,stdout=stdout,stderr=stderr,check=False)
        commands.append({'name':name,'argv':list(map(str,argv)),'exit':result.returncode,'expected_exit':expected,
            'stdin':{'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()},'stdout':pin(out),'stderr':pin(err)})
        if result.returncode!=expected:raise ValueError(name+' failed; actual compiler/runtime diagnostics retained')
        return out.read_bytes()
    spec=importlib.util.spec_from_file_location('task05_zig_actual_inspector',checker)
    inspector=importlib.util.module_from_spec(spec);spec.loader.exec_module(inspector)
    artifacts={}
    for name in ('zig_sport_values','sport_topic_command'):
        binary=work/(name+'.wasm')
        include=args.packet_run/'work/include';family=include/'complete-api'
        run('build-'+name,[zig,'build-exe','-target','wasm32-wasi','-O','ReleaseSmall','-lc',
            '--libc',libc,'--cache-dir',work/'zig-cache','--global-cache-dir',work/'zig-global',
            '--stack',str(memory['stack_bytes']),'--initial-memory='+str(memory['initial_memory_bytes']),
            '--max-memory='+str(memory['maximum_memory_bytes']),'-femit-bin='+str(binary),
            *objects,*archives,'-I',include,'-I',family,'--dep','unitree','-Mroot='+str(source/(name+'.zig')),
            '-I',include,'-I',family,'--dep','ros','--dep','client','-Munitree='+str(source/'unitree.zig'),
            '--dep','ros','-Mclient='+str(foundation/'client.zig'),'-Mros='+str(foundation/'ros2.zig')])
        run('validate-'+name,[validator,'validate',binary])
        image=binary.read_bytes()
        imported=inspector.inspect(image)
        if any(row['module'].startswith('uwvm_ros') for row in imported):
            run('imports-'+name,['/usr/bin/python3','-I','-S','-B',checker,binary,'--backend','2',
                '--allow-module','wasi_snapshot_preview1','--report',work/(name+'-imports.json')])
        elif name!='zig_sport_values' or any(row['module']!='wasi_snapshot_preview1' or row['kind']!=0 for row in imported):
            raise ValueError('the application must retain actual ROS imports; pure values only admit actual WASI functions')
        else:
            (work/(name+'-imports.json')).write_text(json.dumps({
                'status':'PURE_VALUE_IMPORTS_ONLY','ROS_functions_checked':0,'imports':imported,
                'ABI_coverage_qualified':False,'default_authority':[]},indent=2)+'\n')
        if any(row['kind']==2 for row in imported):raise ValueError('imported memory not selected')
        reader=inspector.Reader(image);reader.take(8);memories=[]
        while reader.position<len(reader.data):
            kind=reader.byte();section=inspector.Reader(reader.take(reader.uint()))
            if kind==5:
                if section.uint()!=1 or section.uint()!=1:raise ValueError('owned nonshared memory32 with a maximum required')
                minimum,maximum=section.uint(),section.uint()
                if section.position!=len(section.data):raise ValueError('trailing memory section bytes')
                memories.append({'initial_pages':minimum,'maximum_pages':maximum})
        if memories!=[{'initial_pages':256,'maximum_pages':1024}]:raise ValueError('actual binary memory limits differ')
        artifacts[name]={'path':str(binary),**pin(binary),'owned_memory':memories[0]}
    fixture=work/'zig_sport_values.wasm';app=work/'sport_topic_command.wasm'
    if b'UNITREE_ZIG_VALUES_OK operations=60 fixed_slots=16' not in run('sdk-values',[runtime,'--run',fixture]):
        raise ValueError('actual Zig/C++ all60-method/16-slot assertions did not pass')
    request=run('oracle-request',[runtime,'--run',oracle,'emit-request'])
    methods=run('oracle-list',[runtime,'--run',oracle,'list']).decode('ascii').splitlines()
    if len(methods)!=60:raise ValueError('complete actual method set required')
    arguments={0:[],1:['1.25','-0.0','0.5'],2:[str(-(1<<31))],3:['1'],4:['-0.125'],6:['1.25','-0.0','0.5'],
        5:[value for i in range(30) for value in (str(i),'0','-0.0','0','0','0','0')]}
    prepared=[]
    for i,row in enumerate(methods):
        family,name,shape=row.split()
        prepared.append(run('prepare-'+str(i),[runtime,'--run',app,'prepare',family,name,*arguments[int(shape)]],request))
    if b'UNITREE_NIM_COMMANDS_OK operations=60 complete_records=1' not in run(
            'actual-independent-command-values',[runtime,'--run',oracle,'check-commands'],b''.join(prepared)):
        raise ValueError('actual complete command outputs differ from the genuine C++ oracle')
    invalid=[('prepare','wrong','Move'),('prepare','go2','Missing'),('prepare','go2','Move','0','0'),
        ('prepare','go2','Move','nan','0','0'),('prepare','b2','ContinuousGait','2'),
        ('prepare','go2','SpeedLevel','2147483648')]
    for i,argv in enumerate(invalid):run('refuse-'+str(i),[runtime,'--run',app,*argv],expected=22)
    for family in ('go2','b2'):
        output=run('default-deny-'+family,[runtime,'--run',app,'send',family,'Move','unitree-pub','unitree-sub',
            '10','100','1','0','0','0'],expected=13)
        if b'DONE primary=13 subscription=0 publisher=0 native_valid=0 native_code=0' not in output:
            raise ValueError('actual ungranted pair must fail before stdin')
    for path,before in captured.items():
        if pin(Path(path))!=before:raise ValueError('actual source/provider/oracle input changed')
    result={'status':'PASS_PRIVATE_ZIG_VALUES_COMMANDS_AND_DEFAULT_DENIAL','source_and_tool_inputs':captured,
        'commands':commands,'artifacts':artifacts,'payload_methods':60,'command_preparations':60,'pending_slots':16,
        'invalid_argument_refusals':6,'ungranted_pair_refusals':2,'default_authority':[],
        'stack_bytes_from_compiler_argv':memory['stack_bytes'],'native_ROS_runs':0,'public_installed_SDK_qualified':False}
    (work/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])


if __name__=='__main__':main()
