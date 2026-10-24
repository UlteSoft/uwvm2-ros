#!/usr/bin/env python3
"""Qualify public Unitree Sport builders with real serial producers and Guests.

The pinned C operation table and independently decoded complete Python records
are the oracles. JSON numeric spelling may vary; float32 bits, exact integers,
booleans, original metadata/binary and empty/query parameters must agree. Host
JavaScript is recorded separately, never as embedded-JS Guest qualification.
Run only through layout_distribution_owner in its original hard64GiB group.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys


def fact(path):
    path=Path(path)
    with path.open('rb')as stream:sha=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=sha)


def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec)
    sys.modules[name]=module;spec.loader.exec_module(module);return module


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,required=True);parser.add_argument('--out',type=Path,required=True)
    parser.add_argument('--phase',choices=('prepare','verify','catalog'),required=True)
    args=parser.parse_args();root=args.root.resolve(strict=True);out=args.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root']);tc=Path(inputs['toolchains'])
    out.mkdir();(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],products=[],Guest_executions=[],host_executions=[],
        default_authority=[],official_suite_credit=0,native_ROS_qualified=False,
        physical_controller_qualified=False,current_Runtime_rebuilt=False,QEMU_qualified=False,VTune_qualified=False)
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),exit=None);report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as so,(directory/'stderr').open('xb')as se:
            process=subprocess.run(row['argv'],stdin=subprocess.DEVNULL,stdout=so,stderr=se,timeout=600)
        row.update(exit=process.returncode,stdout=fact(directory/'stdout'),stderr=fact(directory/'stderr'));save()
        if process.returncode!=expected:
            nested=''
            for p in sorted((out/'projects'/label.removeprefix('build-')).glob('build-*/stderr')):nested+=p.read_text()[-2500:]
            raise AssertionError((label,process.returncode,(directory/'stderr').read_text()[-2500:],nested))
        return(directory/'stdout').read_bytes(),(directory/'stderr').read_bytes()
    try:
        if args.phase=='prepare':
            run('distribution',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_distribution.py',
                '--origin',source/'sdk/origin','--out',out/'sdk',
                '--wasi-sysroot',tc/'wasi-sdk-33.0-x86_64-linux/share/wasi-sysroot',
                '--fast-io',source/'third-parties/fast_io/include'])
            for name in('README.md','xmake.lua','CMakeLists.txt'):shutil.copyfile(source/'sdk'/name,out/'sdk'/name)
            shutil.copytree(source/'sdk/tools',out/'sdk/tools',dirs_exist_ok=True)
            report.update(status='SPORT_PUBLIC_SDK_PREPARED',sdk=str(out/'sdk'))
        elif args.phase=='catalog':
            previous=root/'out/sdk-layout-unitree-catalog-r2/ros-tests/index.json'
            run('catalog',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_test_catalog.py',
                '--source',source/'test/0073.ros-tests/support/source','--out',out/'ros-tests','--previous-index',previous])
            old={r['source']:r for r in json.loads(previous.read_bytes())['cases']}
            current=json.loads((out/'ros-tests/index.json').read_bytes())
            for row in current['cases']:
                if row['source']in old:assert row==old[row['source']]
                actual=fact(source/'test/0073.ros-tests'/row['source'])
                assert all(row['source_'+k]==actual[k]for k in('bytes','sha256'))
            added=[r for r in current['cases']if r['source']not in old]
            assert len(old)==416 and len(added)==1 and added[0]['source'].endswith('/check_unitree_sport.py')
            report.update(status='SPORT_NUMBERED_CATALOG_PASS',cases=current['cases_prepared'],added=added)
        else:
            sdk=root/'out/sdk-layout-sport-prepare-r3/sdk'
            assert json.loads((sdk.parent/'result.json').read_bytes())['status']=='SPORT_PUBLIC_SDK_PREPARED'
            bindings=json.loads((sdk/'share/project-path-rebindings.json').read_bytes())
            for row in bindings['changes']:
                if Path(row['path']).suffix in('.ts','.js','.mjs')and row['before'].startswith('.'):
                    assert row['after'].startswith('.'),row
            tools=dict(cargo=str(tc/'rust-1.90.0/bin/cargo'),rustc=str(tc/'rust-1.90.0/bin/rustc'),
                tinygo=str(tc/'tinygo/bin/tinygo'),goroot=str(tc/'go'),node='/usr/bin/node',asc=str(tc/'node_modules/assemblyscript/bin/asc.js'))
            paths={Path(v).resolve(strict=True)for k,v in tools.items()if k!='goroot'}
            paths|={tc/'go/bin/go',tc/'node_modules/assemblyscript/dist/asc.js',tc/'node_modules/assemblyscript/dist/assemblyscript.js',
                Path(inputs['runtime']),Path(inputs['validator'])}
            pins=[fact(p)for p in sorted(paths)];report['producer_files_before']=pins
            toolfile=out/'tools.json';toolfile.write_text(json.dumps(tools,indent=2)+'\n');(out/'projects').mkdir()
            for language in('rs','as','tinygo','js'):
                project=out/'projects'/language
                argv=[sys.executable,'-I','-S','-B',sdk/'tools/prepare_unitree_sport.py','--sdk',sdk,'--out',project,'--language',language]
                if language!='js':argv+=['--build','--tools',toolfile]
                run('build-'+language,argv)
                recipe=json.loads((project/'recipe.json').read_bytes())
                if language=='js':continue
                artifact=Path(recipe['artifact'])
                run('validate-'+language,[inputs['validator'],'validate',artifact])
                run('imports-'+language,[sys.executable,'-I','-S','-B',sdk/'tools/check_wasm_imports.py',artifact,
                    '--backend','2','--allow-module','wasi_snapshot_preview1','--report',project/'imports.json'])
                report['products'].append(dict(language=language,artifact=fact(artifact),recipe=fact(project/'recipe.json'),build=fact(project/'build.json')));save()
            # Every build/validator has finished before any Guest is executed.
            sys.path.insert(0,str(sdk/'include/platform/ros/py'))
            records=load('bounded_records',sdk/'include/platform/unitree/py/ros2/unitree-api-complete/bounded_records.py')
            sport=load('unitree_sport_python_oracle',sdk/'include/platform/unitree/py/sport.py')
            table=(sdk/'include/platform/unitree/c/roswasm_unitree/sport_operations.inc').read_text()
            operations=re.findall(r'^UWVM_UNITREE_SPORT\((\w+),(\d+),(\w+)\)',table,re.M)
            assert len(operations)==39 and set(sport.OPERATIONS)=={x[0]for x in operations}
            request=records.Ros2Unitree_apiMsgRequest()
            request.header.identity.id=9007199254740993;request.header.identity.api_id=-77;request.header.lease.id=-9007199254740993
            request.header.policy.priority=-101;request.header.policy.noreply=True;request.parameter='{"keep":"λ"}';request.binary=[0,255,1,128]
            expected={}
            for name,identity,kind in operations:
                arguments=(1.25,-0.0,struct.unpack('<f',b'\x01\0\0\0')[0])if kind=='xyz'else(-(1<<31),)if kind=='integer'else(True,)if kind=='boolean'else()
                value=sport.prepare(request,name,*arguments);assert value.header.identity.api_id==int(identity)
                expected[name]=value
            expected['edge-max']=sport.prepare(request,'Move',*(struct.unpack('<f',struct.pack('<I',bits))[0]for bits in(0x7f7fffff,0xff7fffff,0x00800000)))
            expected['edge-int']=sport.prepare(request,'SpeedLevel',(1<<31)-1)
            expected['edge-bool']=sport.prepare(request,'Pose',False)
            def pairs(items):
                result={}
                for k,v in items:
                    assert k not in result;result[k]=v
                return result
            def decoded_json(text):
                return json.loads(text,object_pairs_hook=pairs,parse_int=lambda s:-0.0 if s=='-0'else int(s))
            def verify_vectors(body):
                lines=[line.split(b'\t')for line in body.splitlines()if b'\t'in line];seen=set()
                assert len(lines)==42
                for parts in lines:
                    assert len(parts)==2;label=parts[0].decode();assert label in expected and label not in seen;seen.add(label)
                    raw=bytes.fromhex(parts[1].decode());value=records.Ros2Unitree_apiMsgRequest.decode(raw);assert value.encode()==raw
                    want=expected[label]
                    assert value.header==want.header and value.binary==want.binary
                    name=label if label in sport.OPERATIONS else{'edge-max':'Move','edge-int':'SpeedLevel','edge-bool':'Pose'}[label]
                    kind=sport.OPERATIONS[name][1]
                    if kind in('none','query'):assert value.parameter==request.parameter
                    else:
                        a,b=decoded_json(value.parameter),decoded_json(want.parameter);assert a.keys()==b.keys()
                        for k in a:
                            if kind=='xyz':assert type(a[k])in(int,float)and struct.pack('<f',a[k])==struct.pack('<f',b[k]),(label,k,a,b)
                            else:assert type(a[k])is type(b[k])and a[k]==b[k]
                return dict(complete_requests=42,operations=39,edge_vectors=3,input_fields_preserved=True,float32_bits_checked=True)
            for row in report['products']:
                language=row['language'];so,se=run('Guest-'+language,[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',row['artifact']['path']])
                result=verify_vectors(so);refusals=8 if language=='rs'else 9
                assert('PASS_SPORT_LOCAL_AND_DEFAULT_DENIAL refusals='+str(refusals)).encode()in so+se
                report['Guest_executions'].append(dict(language=language,default_authority=[],invalid_refusals=refusals,actual_alias_denial=13,**result));save()
            so,se=run('host-js-local',[tools['node'],out/'projects/js/run.mjs'])
            result=verify_vectors(so);assert b'PASS_SPORT_HOST_LOCAL'in so and b'"refusals":20'in so
            report['host_executions'].append(dict(language='js',Guest_qualified=False,native_qualified=False,invalid_refusals=20,**result))
            # Resolve the actual GO/HG convenience entry, rather than merely
            # syntax-checking files whose rewritten imports may be invalid.
            family_script=('import * as family from '+json.dumps((sdk/'include/platform/unitree/js/lowstate_families.mjs').as_uri())+';'
                'const codecs=[family.Ros2Unitree_goMsgLowStateCodec,family.Ros2Unitree_hgMsgLowStateCodec];'
                'if(codecs[0].codecIdentity===codecs[1].codecIdentity)throw Error("family identity collision");'
                'for(const codec of codecs){const bytes=codec.encode(codec.defaultValue());if(!codec.valid(bytes)||codec.encode(codec.decode(bytes)).length!==bytes.length)throw Error("complete family roundtrip");}'
                'console.log("PASS_ACTUAL_GO_HG_JS_MODULE_RESOLUTION");')
            so,se=run('host-js-go-hg-module-resolution',[tools['node'],'--input-type=module','--eval',family_script])
            assert b'PASS_ACTUAL_GO_HG_JS_MODULE_RESOLUTION'in so
            report['host_executions'].append(dict(language='js',Guest_qualified=False,native_qualified=False,complete_GO_HG_modules_loaded=2))
            assert [fact(p['path'])for p in pins]==pins;report['producer_files_after']=pins
            manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
            for row in manifest['entries']:
                actual=fact(sdk/row['path']);assert all(actual[k]==row[k]for k in('bytes','sha256'))
            report.update(status='SPORT_REAL_WASM_BUILDERS_AND_DEFAULT_DENIAL_PASS',sdk=str(sdk),manifest_entries=len(manifest['entries']))
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()
    print(report['status'],flush=True)


if __name__=='__main__':main()
