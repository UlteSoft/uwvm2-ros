#!/usr/bin/env python3
"""Run the actually compiled complete-input B2 command against original bodies.

This separate bounded check does not turn an interrupted multi-project CMake
suite into a pass. Every operation runs in a real ungranted Wasm Guest; original
native ROS source supplies the independent method IDs, argument shapes and keys.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import struct
import subprocess


def fact(path):
    path=Path(path)
    with path.open('rb') as stream:digest=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=digest)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('source','build','original','runtime','validator','checker'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--compiled-source-sha256',required=True);a=p.parse_args()
    run=Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True);assert os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD']=='1'
    out=run/'work/b2-application';out.mkdir();(out/'commands').mkdir()
    report=dict(status='RUNNING',inputs=[],commands=[],cases=[],actual_Guest_runs=0,
        whole_CMake_suite_qualified=False,native_ROS_runs=0,official_suite_runs=0,hardware_runs=0,
        QEMU_runs=0,VTune_runs=0,default_authority=[],complete_platform_support=False)
    pins={}
    def pin(path):
        path=Path(path).resolve(strict=True)
        if path not in pins:pins[path]=fact(path);report['inputs'].append(pins[path])
        return path
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def command(name,argv,expected=0,input=None):
        root=out/'commands'/name;root.mkdir();row=dict(name=name,argv=list(map(str,argv)),exit=None)
        report['commands'].append(row);save()
        with (root/'stdout').open('xb') as stdout,(root/'stderr').open('xb') as stderr:
            result=subprocess.run(row['argv'],stdin=subprocess.DEVNULL if input is None else None,input=input,
                stdout=stdout,stderr=stderr,timeout=15)
        row.update(exit=result.returncode,stdout=fact(root/'stdout'),stderr=fact(root/'stderr'));save()
        if result.returncode!=expected:raise RuntimeError((name,result.returncode,(root/'stderr').read_text()[-2500:]))
        return (root/'stdout').read_bytes()
    try:
        pin(__file__);entry=pin(a.source/'examples/b2_sport_command.cpp')
        assert fact(entry)['sha256']==a.compiled_source_sha256
        artifact=pin(a.build/'unitree_b2_sport_command.wasm')
        for path in (a.runtime,a.validator,a.checker):pin(path)
        commands=pin(a.build/'compile_commands.json');rows=json.loads(commands.read_bytes())
        compile=[row for row in rows if Path(row['file']).resolve()==entry];assert len(compile)==1
        argv=shlex.split(compile[0]['command']);assert '-fno-exceptions' in argv and any(v in argv for v in ('-std=gnu++20','-std=c++20'))
        for path in sorted(a.build.rglob('*.d')):
            if path.name!='b2_sport_command.cpp.obj.d' and path.name!='b2_sport.cpp.obj.d':continue
            pin(path)
            for word in shlex.split(path.read_text().replace('\\\n',' ').partition(':')[2]):pin(word)
        header=pin(a.original/'include/common/ros2_b2_sport_client.h').read_text()
        implementation=pin(a.original/'src/common/ros2_b2_sport_client.cpp').read_text()
        constants={name:int(value) for name,value in re.findall(r'const int32_t\s+(\w+)\s*=\s*(\d+)\s*;',header)}
        methods=list(re.finditer(r'(?:^|\n)\s*void SportClient::(\w+)\(([^)]*)\)\s*\{',implementation));assert len(methods)==21
        command('validate',[a.validator,'validate',artifact])
        command('actual-import-signatures',['/usr/bin/python3','-I','-S','-B',a.checker,artifact,'--backend','2',
            '--allow-module','wasi_snapshot_preview1','--report',out/'imports.json'])
        keep='{"keep":"λ"}'.encode();binary=bytes((0,255,1,128));identity=9007199254740993
        request=struct.pack('<qqqi?',identity,-77,-identity,-101,False)+struct.pack('<I',len(keep))+keep+struct.pack('<I',len(binary))+binary
        counted=struct.pack('<I',len(request))+request
        guest=[a.runtime,'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact]
        def decode(body):
            metadata=struct.unpack_from('<qqqi?',body);at=29
            n=struct.unpack_from('<I',body,at)[0];at+=4;parameter=body[at:at+n];at+=n
            n=struct.unpack_from('<I',body,at)[0];at+=4;data=body[at:at+n];at+=n
            assert len(data)==n and at==len(body)
            return metadata,parameter,data
        for index,method in enumerate(methods):
            name,arguments=method[1],method[2]
            body=implementation[method.end():methods[index+1].start() if index+1<len(methods) else len(implementation)]
            assigned=re.findall(r'req\.header\.identity\.api_id\s*=\s*(\w+)\s*;',body);assert len(assigned)==1
            api=constants[assigned[0]];keys=set(re.findall(r'\["([^"\n]+)"\]',body));values=[];expected=None;stdin=counted
            if 'vector<PathPoint>' in arguments:
                assert keys=={'t_from_start','x','y','yaw','vx','vy','vyaw'}
                stdin+=struct.pack('<I',30)+b''.join(struct.pack('<7f',float(i),0,-0.0,0,0,0,0) for i in range(30))
                expected=[dict(t_from_start=i,x=0,y=-0.0,yaw=0,vx=0,vy=0,vyaw=0) for i in range(30)]
            elif 'bool ' in arguments:values=['1'];expected={'data':True}
            elif re.search(r'\b(?:int32_t|int)\s+',arguments):values=['-2147483648'];expected={'data':-2147483648}
            elif arguments.count('float ')==3:
                assert keys in ({'x','y','z'},{'x','y','yaw'});values=['1.25','-0','0.5'];expected=dict(x=1.25,y=-0.0,**({'yaw':.5} if 'yaw' in keys else {'z':.5}))
            elif arguments.count('float ')==1:values=['-0.125'];expected={'data':-.125}
            else:assert not keys
            encoded=command('prepare-'+name,[*guest,'prepare',name,*values],input=stdin)
            metadata,parameter,data=decode(encoded)
            assert metadata==(identity,api,-identity,-101,False) and data==binary
            if expected is None:assert parameter==keep
            else:
                parsed=json.loads(parameter,parse_int=lambda text:-0.0 if text=='-0' else int(text));assert parsed==expected
                if isinstance(parsed,dict):assert set(parsed)==keys
                if isinstance(parsed,dict) and 'y' in parsed:assert struct.pack('<f',parsed['y'])==struct.pack('<f',-0.0)
                if isinstance(parsed,list):assert all(struct.pack('<f',point['y'])==struct.pack('<f',-0.0) for point in parsed)
                if isinstance(expected,dict) and type(expected.get('data')) is bool:assert type(parsed['data']) is bool
            assert command('denied-'+name,[*guest,'send',name,'task05.b2',*values],13)==b''
            report['cases'].append(dict(method=name,api=api,status='PASS',complete_Request_preserved=True,default_denial_before_stdin=True))
            report['actual_Guest_runs']+=2
        invalid=[('bad-id',['prepare','Damp'],struct.pack('<I',len(request))+b'\0'*8+request[8:]),
            ('truncated',['prepare','Damp'],counted[:-1]),
            ('wrong-point-count',['prepare','TrajectoryFollow'],counted+struct.pack('<I',29)),
            ('nan-point',['prepare','TrajectoryFollow'],counted+struct.pack('<I',30)+struct.pack('<7f',float('nan'),0,0,0,0,0,0)+b'\0'*(29*28)),
            ('nan-argument',['prepare','Move','nan','0','0'],counted),
            ('bool-range',['prepare','ContinuousGait','2'],counted),('unknown',['prepare','UnknownMethod'],counted)]
        for name,argv,stdin in invalid:
            assert command('invalid-'+name,[*guest,*argv],22,input=stdin)==b'';report['actual_Guest_runs']+=1
        assert all(fact(path)==row for path,row in pins.items())
        report.update(status='PASS',prepare_cases=21,default_denials=21,invalid_cases=7,passed=49,denominator=49,input_pins_preserved=True)
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
