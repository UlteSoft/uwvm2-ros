#!/usr/bin/env python3
"""Qualify complete Unitree Sport payloads and actual default-denied Wasm.

The independent oracle reads the pinned original C++ methods, constants and
parameter assignments. It decodes the complete canonical frame with Python
struct/json, never the SDK generator/codec. No native controller, middleware
or official suite is substituted by this representation/refusal lane.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys


def pin(path):
    p=Path(path)
    return dict(path=str(p),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())


def original_operations(header,implementation):
    constants={n:int(v)for n,v in re.findall(r'const int32_t (ROBOT_SPORT_API_ID_\w+) = (\d+);',header)}
    functions=re.findall(r'void SportClient::(\w+)\((.*?)\)\s*\{(.*?)^\}',implementation,re.S|re.M)
    result={}
    declarations=set(re.findall(r'\bvoid (\w+)\(',header))
    for name,parameters,body in functions:
        ids=re.findall(r'req\.header\.identity\.api_id\s*=\s*(ROBOT_SPORT_API_ID_\w+)\s*;',body)
        assert len(ids)==1 and ids[0]in constants and name not in result
        if 'bool &flag'in parameters:
            assert 'Call<'in body and 'js["data"].get<bool>()'in body
            kind='query'
        elif 'float'in parameters:
            assert re.findall(r'js\["(\w+)"\] = \w+;',body)==['x','y','z']
            kind='xyz'
        elif 'int level'in parameters:
            assert 'js["data"] = level;'in body
            kind='integer'
        elif 'bool flag'in parameters:
            assert 'js["data"] = flag;'in body
            kind='boolean'
        else:
            assert 'js['not in body and 'req.parameter'not in body
            kind='none'
        assert ('req_puber_->publish(req);'in body)==(kind!='query')
        result[name]=dict(id=constants[ids[0]],kind=kind)
    assert set(result)==declarations and len(result)==len(constants)==39
    return result


def check_frames(raw,ordered):
    cursor=0
    for variant in range(2):
        for name,operation in ordered:
            assert len(raw)-cursor>=4
            size=struct.unpack_from('<I',raw,cursor)[0];cursor+=4
            assert 37<=size<=6181 and size<=len(raw)-cursor
            frame=raw[cursor:cursor+size];cursor+=size
            identity,api,lease,priority,noreply=struct.unpack_from('<qqqiB',frame)
            assert (identity,api,lease,priority,noreply)==(0x123456789ab,operation['id'],0x23456789abc,-13,1)
            n=struct.unpack_from('<I',frame,29)[0];assert n<=4096
            parameter=frame[33:33+n].decode('utf-8','strict')
            position=33+n
            binary_size=struct.unpack_from('<I',frame,position)[0];position+=4
            assert binary_size==3 and frame[position:]==bytes((17,128,255))
            assert position+binary_size==len(frame)
            body=json.loads(parameter)
            kind=operation['kind']
            if kind in ('none','query'):
                assert parameter=='{"original":19}'
            elif kind=='xyz':
                values=[struct.unpack('<f',struct.pack('<I',0x7f7fffff))[0],
                        struct.unpack('<f',struct.pack('<I',1))[0],-0.0]if variant else[1.25,-2.5,0.0625]
                assert set(body)=={'x','y','z'}
                assert all(struct.pack('<f',body[key])==struct.pack('<f',value)for key,value in zip(('x','y','z'),values))
            elif kind=='integer':
                assert body=={'data':-2147483648 if variant else-7}and type(body['data'])is int
            else:
                assert set(body)=={'data'}and type(body['data'])is bool and body['data']is (not variant)
    assert cursor==len(raw)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    assert out.parent==root/'out'and not out.exists()
    out.mkdir(mode=0o700);(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],artifacts=[],default_authority=[],
                native_controller_or_transport_qualified=False,official_suite_cases_run=0)
    def save():(out/'unitreesport.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),expected_exit=expected,exit=None)
        report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
            result=subprocess.run(row['argv'],cwd=source,env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1'),
                stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=180,check=False)
        row.update(exit=result.returncode,stdout=pin(directory/'stdout'),stderr=pin(directory/'stderr'));save()
        assert result.returncode==expected,(label,result.returncode,(directory/'stderr').read_text()[-2800:])
        return(directory/'stdout').read_bytes()
    try:
        captured=root/inputs['unitree_source_capture'];capture=json.loads((captured/'unitreecapture.json').read_bytes())
        assert capture['status']=='FOUR_COMPLETE_PINNED_TREES_AND_SELECTED_ORIGINAL_EXAMPLE_SOURCES_CAPTURED'
        original=next(r for r in capture['repositories']if r['repository']=='unitreerobotics/unitree_ros2')
        upstream=captured/'unitree_ros2/files'
        for row in original['files']:
            actual=pin(upstream/row['path']);assert actual['bytes']==row['bytes']and actual['sha256']==row['sha256']
        header=upstream/'example/src/include/common/ros2_sport_client.h'
        implementation=upstream/'example/src/src/common/ros2_sport_client.cpp'
        operations=original_operations(header.read_text(),implementation.read_text())
        kit=source/'sdk/origin/robots/unitree';inc=kit/'include/roswasm_unitree/sport_operations.inc'
        table=re.findall(r'^UWVM_UNITREE_SPORT\((\w+),(\d+),(\w+)\)$',inc.read_text(),re.M)
        assert len(table)==39 and {n:dict(id=int(i),kind=k)for n,i,k in table}==operations
        ordered=[(n,operations[n])for n,_,_ in table]
        report['operations']=operations
        report['original_source']=[pin(header),pin(implementation),pin(inc)]
        family=source/'sdk/origin/custom-types/unitree-api-complete'
        fastio=Path(inputs['runtime_source_root'])/'third-parties/fast_io/include'
        assert(fastio/'fast_io.h').is_file()
        includes=['-I'+str(source/'sdk/origin/include'),'-I'+str(family),'-I'+str(kit/'include'),'-I'+str(fastio)]
        wasi=Path(inputs['toolchains'])/'wasi-sdk-33.0-x86_64-linux/bin'
        memory=['-Wl,--threads=1','-Wl,-z,stack-size=1048576','-Wl,--initial-memory=16777216','-Wl,--max-memory=67108864']
        prefix=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run']
        for target in ('native','wasm'):
            compiler=str(wasi/'clang++')if target=='wasm'else'/usr/bin/clang++-21'
            obj=out/(target+'-sport.o')
            run(target+'-formatter-build',[compiler,'-std=c++20','-Os','-g0','-fno-exceptions','-fno-rtti',*includes,'-c',kit/'sport.cpp','-o',obj])
            outputs=[]
            for language,extension in(('c','c'),('cpp','cpp')):
                entry=out/(target+'-'+language+'.o')
                cc=str(wasi/('clang'if language=='c'else'clang++'))if target=='wasm'else'/usr/bin/'+('clang-21'if language=='c'else'clang++-21')
                run(target+'-'+language+'-compile',[cc,'-std='+('c11'if language=='c'else'c++20'),'-Os','-g0',*includes,
                    '-c',kit/'tests'/('sport_values.'+extension),'-o',entry])
                artifact=out/(target+'-'+language+('.wasm'if target=='wasm'else''))
                run(target+'-'+language+'-link',[compiler,*([*memory]if target=='wasm'else[]),entry,obj,'-o',artifact])
                if target=='wasm':run(target+'-'+language+'-validate',[inputs['validator'],'validate',artifact])
                raw=run(target+'-'+language+'-values',prefix+[artifact]if target=='wasm'else[artifact])
                check_frames(raw,ordered);outputs.append(raw)
                report['artifacts'].append(dict(pin(artifact),value_frames=78,transactional_refusals=14))
            assert outputs[0]==outputs[1]
        artifact=out/'sport-operations.wasm'
        run('application-build',[wasi/'clang++','-std=c++20','-Os','-g0','-fno-exceptions','-fno-rtti',*memory,*includes,
            kit/'examples/sport_operations.cpp',out/'wasm-sport.o','-o',artifact])
        run('application-validate',[inputs['validator'],'validate',artifact])
        run('application-imports',[sys.executable,'-I','-S','-B',source/'sdk/origin/tools/check_wasm_imports.py',artifact,
            '--backend','2','--allow-module','wasi_snapshot_preview1','--report',out/'imports.json'])
        for name,operation in ordered:
            arguments=['1.25','-2.5','0.0625']if operation['kind']=='xyz'else['-7']if operation['kind']=='integer'else['1']if operation['kind']=='boolean'else[]
            body=run('prepare-'+name,prefix+[artifact,'prepare',name,'41',*arguments])
            assert struct.unpack_from('<qq',body)==(41,operation['id'])
            assert run('default-denied-'+name,prefix+[artifact,'send',name,'unitree.sport.request','41',*arguments],13)==b''
            assert report['commands'][-1]['stderr']['bytes']==0
        invalid=[[],['prepare','Unknown','1'],['prepare','Move','1','nan','0','0'],
            ['prepare','Move','1','inf','0','0'],['prepare','Move','1','1','2'],
            ['prepare','Damp','0'],['prepare','Damp','9223372036854775808'],
            ['prepare','Damp','-1'],['prepare','SpeedLevel','1','2147483648'],
            ['prepare','Pose','1','2'],['prepare','Move','1','1x','2','3']]
        for index,args in enumerate(invalid):
            assert run('invalid-'+str(index),prefix+[artifact,*args],22)==b''
        # The independent full-frame oracle itself must reject an altered API
        # identity. A validation-only invocation cannot satisfy this check.
        corrupt=bytearray(outputs[0]);corrupt[12]^=1
        try:check_frames(corrupt,ordered)
        except AssertionError:report['independent_corrupt_control_refused']=True
        else:raise AssertionError('corrupt API reference was accepted')
        report['artifacts'].append(pin(artifact))
        report.update(status='ALL_THIRTY_NINE_SPORT_PAYLOAD_OPERATIONS_C_CPP_AND_REAL_WASM_DEFAULT_REFUSALS_PASS',
            original_operations=39,complete_nonzero_frames_per_product=78,complete_value_products=4,
            actual_default_denials=39,invalid_application_refusals=len(invalid),
            blocking_query_compatibility='EXPLICIT_ASYNCHRONOUS_MIGRATION_REQUIRED',
            runtime_rebuilt=False,runtime_reused=pin(inputs['runtime']))
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
