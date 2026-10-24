#!/usr/bin/env python3
"""Exercise actual Unitree source projects from a standalone public SDK.

All producers and Guests run serially under layout_distribution_owner. No
native robot, ROS peer, update/debugger bridge, simulated import or implicit
grant is supplied. Default denial, stale-owner checks and local Sport payloads
are separate from native ROS/controller and official-example qualification.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys


def fact(path):
    with Path(path).open('rb')as stream:sha=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=Path(path).stat().st_size,sha256=sha)


def load_module(name,path):
    spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec)
    sys.modules[name]=module;spec.loader.exec_module(module);return module


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,required=True);parser.add_argument('--out',type=Path,required=True)
    parser.add_argument('--phase',choices=('prepare','verify','catalog','composition','views'),required=True)
    args=parser.parse_args();root=args.root.resolve(strict=True);out=args.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root']);toolchain=Path(inputs['toolchains'])
    assert not out.exists();out.mkdir();(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],products=[],executions=[],default_authority=[],
        official_example_ports_credited=0,official_suite_credit=0,native_ROS_qualified=False,
        QEMU_or_hardware_performance_qualified=False)
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0,cwd=None):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),cwd=str(cwd)if cwd else None,exit=None)
        report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as so,(directory/'stderr').open('xb')as se:
            process=subprocess.run(row['argv'],cwd=cwd,stdin=subprocess.DEVNULL,stdout=so,stderr=se,check=False,timeout=600)
        row.update(exit=process.returncode,stdout=fact(directory/'stdout'),stderr=fact(directory/'stderr'));save()
        if process.returncode!=expected:
            nested=''
            if label.startswith('build-'):
                for path in sorted((out/'projects'/label[6:]).glob('build-*/stderr')):
                    nested+=str(path)+'\n'+path.read_text()[-2400:]+'\n'
            raise AssertionError((label,process.returncode,(directory/'stderr').read_text()[-2400:],nested))
        return(directory/'stdout').read_bytes(),(directory/'stderr').read_bytes()
    try:
        if args.phase=='prepare':
            run('standalone-sdk',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_distribution.py',
                '--origin',source/'sdk/origin','--out',out/'sdk',
                '--wasi-sysroot',toolchain/'wasi-sdk-33.0-x86_64-linux/share/wasi-sysroot',
                '--fast-io',source/'third-parties/fast_io/include'])
            for name in('README.md','xmake.lua','CMakeLists.txt'):shutil.copyfile(source/'sdk'/name,out/'sdk'/name)
            shutil.copytree(source/'sdk/tools',out/'sdk/tools',dirs_exist_ok=True)
            report.update(status='UNITREE_PUBLIC_PROJECT_INPUTS_PREPARED',sdk=str(out/'sdk'),applications_executed=0)
        elif args.phase=='views':
            state=json.loads((root/inputs['unitree_sdk_prepared']/'result.json').read_bytes());sdk=Path(state['sdk'])
            for backend,family,typename in((1,'unitree-ros1-complete','Ros1Unitree_legged_msgsLowCmd'),
                    (2,'unitree-go-complete','Ros2Unitree_goMsgLowCmd')):
                relative=lambda p:json.dumps(os.path.relpath(p.with_suffix(''),out))
                code='/** Mix the public backend view, common SDK and a complete vendor codec. */\n'
                code+='import {Node,NativeApi} from '+relative(sdk/('include/platform/ros'+str(backend)+'/as/ros'+str(backend)+'/roswasm.ts'))+';\n'
                code+='import {GeneratedCodec} from '+relative(sdk/'include/platform/unitree/as/roswasm_unitree/generated_codec.ts')+';\n'
                code+='import {Codec as ViewCodec} from '+relative(sdk/('include/platform/ros'+str(backend)+'/as/codecs.ts'))+';\n'
                code+='import {Codec as CommonCodec} from '+relative(sdk/'include/platform/ros/as/codecs.ts')+';\n'
                code+='import {'+typename+' as Value,'+typename+'Codec as ActualCodec} from '+relative(sdk/('include/platform/unitree/as/ros'+str(backend)+'/'+family+'/bounded_records.ts'))+';\n'
                code+='const codec:CommonCodec<Value>=new GeneratedCodec<Value,ActualCodec>(new ActualCodec());\n'
                code+='const view:ViewCodec<Value>=codec;\n'
                code+='export function _start():void{const node=new Node(new NativeApi(),"public-view");const owner=node.createPublisher<Value>("ungranted-view",view);if(owner.capability.openStatus!=13||owner.capability.isOpen())unreachable();const closed=node.close();for(let i=0;i<closed.length;++i)if(closed[i])unreachable();}\n'
                name='view-'+str(backend);entry=out/(name+'.ts');entry.write_text(code);artifact=out/(name+'.wasm')
                abort=os.path.relpath(sdk/'include/platform/ros/as/api',out)+'/abortGuest'
                run('compile-'+name,['/usr/bin/node','--max-old-space-size=256','--disable-wasm-trap-handler',
                    toolchain/'node_modules/assemblyscript/bin/asc.js',entry.name,'--outFile',artifact,'--runtime','incremental',
                    '--initialMemory','128','--maximumMemory','512','--optimizeLevel','0','--shrinkLevel','0','--use','abort='+abort],cwd=out)
                run('validate-'+name,[inputs['validator'],'validate',artifact])
                run('imports-'+name,[sys.executable,'-I','-S','-B',sdk/'tools/check_wasm_imports.py',artifact,
                    '--backend',str(backend),'--allow-module','wasi_snapshot_preview1','--report',out/(name+'-imports.json')])
                run('execute-'+name,[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact])
                report['products'].append(dict(backend=backend,artifact=fact(artifact)))
                report['executions'].append(dict(backend=backend,default_authority=[],exit=0));save()
            report.update(status='ASSEMBLYSCRIPT_BACKEND_AND_COMMON_VIEWS_SHARE_REAL_VENDOR_CODEC_IDENTITY_PASS')
        elif args.phase=='composition':
            state=json.loads((root/inputs['unitree_sdk_prepared']/'result.json').read_bytes());sdk=Path(state['sdk'])
            profiles=[('unitree/ros2/unitree-go-complete','Ros2Unitree_goMsgLowCmd','2_unitree_go_msg_LowCmd',2),
                ('unitree/ros2/unitree-hg-complete','Ros2Unitree_hgMsgLowCmd','2_unitree_hg_msg_LowCmd',2),
                ('unitree/ros2/unitree-api-complete','Ros2Unitree_apiMsgRequest','2_unitree_api_msg_Request',2),
                ('unitree/ros1/unitree-ros1-complete','Ros1Unitree_legged_msgsLowCmd','1_unitree_legged_msgs_LowCmd',1),
                ('fanuc/ros2/fanuc-ros2-complete','Ros2Sensor_msgsMsgJointState','2_sensor_msgs_msg_JointState',2),
                ('kuka/ros2/kuka-ros2-complete','Ros2Sensor_msgsMsgJointState','2_sensor_msgs_msg_JointState',2)]
            program=sdk/'tools/compose_profiles.py';composed=out/'composed'
            run('compose-whole-profiles',[sys.executable,'-I','-S','-B',program,'--sdk',sdk,'--out',composed,
                *[arg for row in profiles for arg in('--profile',row[0])]])
            composition=json.loads((composed/'composition.json').read_bytes())
            assert composition['status']=='WHOLE_PROFILE_SOURCE_VIEWS_PREPARED_NOT_COMPILED'
            for index,names in enumerate(([profiles[0][0],profiles[0][0]],['unitree/ros2/../outside'],['unitree/ros2/missing-family'])):
                target=out/('refused-'+str(index))
                run('compose-refuses-'+str(index),[sys.executable,'-I','-S','-B',program,'--sdk',sdk,'--out',target,
                    *[arg for name in names for arg in('--profile',name)]],1)
                assert not target.exists()
            outputs=[]
            for language,extension in(('c','.c'),('cpp','.cpp')):
                body=['/** Real whole profiles in one TU; only generated identifiers differ. */',
                    '#include <platform/ros1/c/roswasm/ros1.h>','#include <platform/ros2/c/roswasm/ros2.h>',
                    '#include <stdint.h>','#include <stddef.h>','#include <string.h>','#include <stdio.h>']
                if language=='cpp':body+=['#include <fast_io.h>']
                for row in composition['profiles']:
                    include=row['include']if language=='cpp'else row['include'][:-3]+'h'
                    body.append('#include <'+include+'>')
                body+=['int main(void){','static unsigned char a[32768],b[32768];']
                for index,(profile,cpp_type,c_type,backend)in enumerate(profiles):
                    row=composition['profiles'][index];assert row['profile']==profile
                    prefix=row['C_identifier_prefix'];symbol=prefix+'roswasmc_bounded_'+c_type
                    body+=['{','roswasmc_context context={roswasmc_ros'+str(backend)+'_backend()};']
                    if language=='cpp':
                        typename=row['cpp_namespace']+'::'+cpp_type
                        body+=['using Value='+typename+';Value value{},decoded{};roswasmcpp::wire::bytes wire,again;',
                            'if(!roswasmcpp::Codec<Value>::encode(value,wire)||!roswasmcpp::Codec<Value>::decode({wire.data(),wire.size()},decoded)||!roswasmcpp::Codec<Value>::encode(decoded,again)||wire!=again)return 22;',
                            'roswasmcpp::Publisher<Value> owner{context,"composed-ungranted-'+str(index)+'"};if(owner.error()!=13||owner.is_open())return 22;',
                            'size_t n=wire.size();if(n>sizeof a)return 22;memcpy(a,wire.data(),n);']
                    else:
                        body+=['static '+symbol+' value,decoded;size_t n=0,m=0;',
                            'if('+symbol+'_encode(&value,a,sizeof a,&n)||'+symbol+'_decode(a,n,&decoded)||'+symbol+'_encode(&decoded,b,sizeof b,&m)||n!=m||memcmp(a,b,n))return 22;',
                            'roswasmc_capability owner;roswasmc_capability_init(&owner,&context);char const alias[]="composed-ungranted-'+str(index)+'";',
                            'if(roswasmc_open(&owner,alias,sizeof alias-1)!=13||roswasmc_is_open(&owner))return 22;']
                    body+=['unsigned char extent[4]={(unsigned char)n,(unsigned char)(n>>8),(unsigned char)(n>>16),(unsigned char)(n>>24)};']
                    if language=='cpp':body+=['fast_io::operations::write_all(fast_io::out(),reinterpret_cast<char const*>(extent),reinterpret_cast<char const*>(extent)+4);',
                        'fast_io::operations::write_all(fast_io::out(),reinterpret_cast<char const*>(a),reinterpret_cast<char const*>(a)+n);']
                    else:body+=['if(fwrite(extent,1,4,stdout)!=4||fwrite(a,1,n,stdout)!=n)return 5;']
                    body+=['}']
                body+=['return 0;}']
                entry=out/('composed'+extension);entry.write_text('\n'.join(body)+'\n');artifact=entry.with_suffix('.wasm')
                compiler=toolchain/'wasi-sdk-33.0-x86_64-linux/bin'/('clang++'if language=='cpp'else'clang')
                flags=['--target=wasm32-wasip1','--sysroot='+str(sdk),'-std=c++20'if language=='cpp'else'-std=c11',
                    '-Os','-g0','-I'+str(sdk/'include'),'-I'+str(composed/'include')]
                if language=='cpp':flags+=['-fno-exceptions','-fno-rtti']
                run('compile-composed-'+language,[compiler,*flags,entry,'-Wl,--threads=1,-z,stack-size=1048576','-o',artifact])
                run('validate-composed-'+language,[inputs['validator'],'validate',artifact])
                run('imports-composed-'+language,[sys.executable,'-I','-S','-B',sdk/'tools/check_wasm_imports.py',artifact,
                    '--backend','dual','--allow-module','wasi_snapshot_preview1','--report',out/('imports-'+language+'.json')])
                wire,se=run('execute-composed-'+language,[inputs['runtime'],'-Rint',
                    '--wasip1-global-noinherit-system-environment','--run',artifact]);outputs.append(wire)
                report['products'].append(dict(language=language,artifact=fact(artifact),composition=fact(composed/'composition.json')))
                report['executions'].append(dict(language=language,profiles=len(profiles),default_authority=[],exit=0));save()
            assert outputs[0]==outputs[1]and len(outputs[0])>24
            report.update(status='SIX_WHOLE_PROFILES_ONE_TU_C_AND_CPP_RUNTIME_AND_CANONICAL_VALUES_PASS',
                complete_profiles=len(profiles),C_CPP_canonical_outputs_identical=True,bytes=len(outputs[0]))
        elif args.phase=='catalog':
            previous=root/inputs['unitree_previous_catalog']/'index.json'
            run('numbered-catalog',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_test_catalog.py',
                '--source',source/'test/0073.ros-tests/support/source','--out',out/'ros-tests','--previous-index',previous])
            old={x['source']:x for x in json.loads(previous.read_bytes())['cases']}
            current=json.loads((out/'ros-tests/index.json').read_bytes())
            for row in current['cases']:
                if row['source']in old:assert row==old[row['source']],row['source']
                actual=fact(source/'test/0073.ros-tests'/row['source'])
                assert all(row['source_'+k]==actual[k]for k in('bytes','sha256'))
            added=[row for row in current['cases']if row['source']not in old]
            assert len(added)==1 and added[0]['source'].endswith('/check_unitree_public.py')
            report.update(status='UNITREE_PUBLIC_PROJECT_TEST_NUMBERED_WITH_STABLE_PREVIOUS_CASES',
                cases=current['cases_prepared'],test_catalog=str(out/'ros-tests'),added=added)
        else:
            state=json.loads((root/inputs['unitree_sdk_prepared']/'result.json').read_bytes())
            assert state['status']=='UNITREE_PUBLIC_PROJECT_INPUTS_PREPARED';sdk=Path(state['sdk'])
            assert not(sdk/'origin').exists()and not(sdk/'example/platform/unitree/go').exists()
            manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
            def check_sources():
                for row in manifest['entries']:
                    p=sdk/row['path'];assert p.is_file()and not p.is_symlink()
                    assert all(fact(p)[k]==row[k]for k in('bytes','sha256')),row['path']
            check_sources()
            tools=dict(cc=str(toolchain/'wasi-sdk-33.0-x86_64-linux/bin/clang'),
                cxx=str(toolchain/'wasi-sdk-33.0-x86_64-linux/bin/clang++'),cargo=str(toolchain/'rust-1.90.0/bin/cargo'),
                rustc=str(toolchain/'rust-1.90.0/bin/rustc'),tinygo=str(toolchain/'tinygo/bin/tinygo'),
                goroot=str(toolchain/'go'),node='/usr/bin/node',asc=str(toolchain/'node_modules/assemblyscript/bin/asc.js'))
            tools_file=out/'producers.json';tools_file.write_text(json.dumps(tools,indent=2)+'\n')
            producer_paths={Path(v).resolve(strict=True)for k,v in tools.items()if k!='goroot'}
            producer_paths|={toolchain/'go/bin/go',Path(inputs['validator']),Path(inputs['runtime']),
                toolchain/'node_modules/assemblyscript/dist/asc.js',toolchain/'node_modules/assemblyscript/dist/assemblyscript.js'}
            producer_pins=[fact(p)for p in sorted(producer_paths)];report['producer_files_before']=producer_pins;save()
            program=sdk/'tools/prepare_unitree_example.py'
            listing,_=run('selection-inventory',[sys.executable,'-I','-S','-B',program,'--list'])
            selections=json.loads(listing);assert len(selections)==33
            (out/'projects').mkdir()
            # Complete all selected builds before running any Guest.
            for selected in selections:
                language,role,family=[selected[k]for k in('language','role','family')]
                name=language+'-'+role+'-'+family;directory=out/'projects'/name
                run('build-'+name,[sys.executable,'-I','-S','-B',program,'--sdk',sdk,'--out',directory,
                    '--language',language,'--role',role,'--family',family,'--build','--tools',tools_file])
                recipe=json.loads((directory/'recipe.json').read_bytes());artifact=Path(recipe['artifact'])
                run('validate-'+name,[inputs['validator'],'validate',artifact])
                run('imports-'+name,[sys.executable,'-I','-S','-B',sdk/'tools/check_wasm_imports.py',artifact,
                    '--backend',str(recipe['backend']),'--allow-module','wasi_snapshot_preview1',
                    '--report',directory/'imports.json'])
                report['products'].append(dict(**selected,backend=recipe['backend'],artifact=fact(artifact),
                    recipe=fact(directory/'recipe.json'),build=fact(directory/'build.json')));save()
            for row in report['products']:
                name=row['language']+'-'+row['role']+'-'+row['family'];directory=out/'projects'/name
                recipe=json.loads((directory/'recipe.json').read_bytes())
                argv=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment',
                    '--run',row['artifact']['path'],*recipe['example_arguments']]
                so,se=run('default-denial-'+name,argv,recipe['expected_exit'])
                if row['language']in('rs','tinygo')or row['role']in('rpc','legacy')and row['language']in('c','cpp'):
                    assert b'PASS_UNITREE_PUBLIC_DEFAULT_DENIED'in so+se
                report['executions'].append(dict(name=name,scope='actual-default-denial-and-checked-owner-state',
                    default_authority=[],exit=recipe['expected_exit']));save()
            # Independently decode complete C++-produced payloads with the actual
            # public Python codec/builder. This is a host codec oracle, not a
            # Python Wasm or native-controller result.
            sys.path.insert(0,str(sdk/'include/platform/ros/py'))
            records=load_module('bounded_records',sdk/'include/platform/unitree/py/ros2/unitree-api-complete/bounded_records.py')
            sport=load_module('public_unitree_sport',sdk/'include/platform/unitree/py/sport.py')
            rows=[x for x in report['products']if x['role']=='sport'];assert len(rows)==1;artifact=rows[0]['artifact']['path']
            table=(sdk/'include/platform/unitree/c/roswasm_unitree/sport_operations.inc').read_text()
            operations=re.findall(r'^UWVM_UNITREE_SPORT\((\w+),(\d+),(\w+)\)',table,re.M)
            assert len(operations)==39 and set(sport.OPERATIONS)=={row[0]for row in operations}
            for name,identity,kind in operations:
                strings=['1.25','-2.5','0.5']if kind=='xyz'else['2']if kind=='integer'else['1']if kind=='boolean'else[]
                values=[1.25,-2.5,0.5]if kind=='xyz'else[2]if kind=='integer'else[True]if kind=='boolean'else[]
                argv=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact]
                wire,se=run('sport-prepare-'+name,[*argv,'prepare',name,'7',*strings])
                reference=records.Ros2Unitree_apiMsgRequest();reference.header.identity.id=7
                expected=sport.prepare(reference,name,*values)
                actual=records.Ros2Unitree_apiMsgRequest.decode(wire)
                assert actual.header.identity.api_id==int(identity)and wire==expected.encode()and actual.encode()==wire
                run('sport-denied-'+name,[*argv,'send',name,'unitree-api-request','7',*strings],13)
                report['executions'].extend([dict(name='sport-prepare-'+name,scope='complete-canonical-payload-matches-public-Python-oracle',
                    bytes=len(wire),default_authority=[],exit=0),dict(name='sport-denied-'+name,scope='actual-open-default-denial',default_authority=[],exit=13)]);save()
            for index,arguments in enumerate((['prepare','Unknown','7'],['prepare','Move','7','nan','0','0'],
                    ['prepare','Pose','7','2'],['prepare','StandDown','0'],['prepare','StandDown','7','extra'])):
                run('sport-invalid-'+str(index),[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment',
                    '--run',artifact,*arguments],22)
                report['executions'].append(dict(name='sport-invalid-'+str(index),scope='invalid-input-refusal',default_authority=[],exit=22));save()
            check_sources()
            assert producer_pins==[fact(p)for p in sorted(producer_paths)]
            report.update(status='UNITREE_PUBLIC_FIVE_LANGUAGE_DEFAULT_DENIAL_AND_SPORT_PAYLOAD_CHECKS_PASS',
                compiled_products=len(report['products']),actual_Guest_executions=len(report['executions']),
                complete_Sport_operations_checked=39,Python_Wasm_execution_qualified=False,
                producer_and_public_manifest_files_unchanged=True)
        print(report['status'],flush=True)
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
