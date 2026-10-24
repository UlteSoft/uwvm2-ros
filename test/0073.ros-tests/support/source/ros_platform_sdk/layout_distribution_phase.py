#!/usr/bin/env python3
"""Prepare all SDK layout inputs, then independently compile and execute checks.

Source preparation and execution have separate receipts. The selected Runtime
is a pinned previous product; no current Runtime rebuild, official vendor suite,
physical controller or hardware performance qualification is claimed here.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys


def fact(path):
    path=Path(path)
    with path.open('rb')as stream:sha=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=sha)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    p.add_argument('--phase',choices=('prepare','verify','probe','package','catalog'),required=True)
    a=p.parse_args();root=a.root.resolve(strict=True);out=a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    assert not out.exists();out.mkdir();(out/'commands').mkdir()
    report=dict(status='RUNNING',phase=a.phase,commands=[],default_authority=[],official_suites_run=0,
        current_Runtime_rebuilt=False,native_ROS_or_robot_qualified=False)
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0,timeout=120):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),expected_exit=expected,exit=None)
        report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as so,(directory/'stderr').open('xb')as se:
            value=subprocess.run(row['argv'],stdout=so,stderr=se,stdin=subprocess.DEVNULL,timeout=timeout,
                env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1',XMAKE_ROOT='y'),check=False)
        row.update(exit=value.returncode,stdout=fact(directory/'stdout'),stderr=fact(directory/'stderr'));save()
        assert expected is None or value.returncode==expected,(label,value.returncode,(directory/'stderr').read_text()[-4000:])
        return(directory/'stdout').read_bytes()
    try:
        if a.phase=='catalog':
            run('final-numbered-source-index',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_test_catalog.py',
                '--source',source/'test/0073.ros-tests/support/source','--out',out/'ros-tests'])
            before=json.loads((root/inputs['sdk_prepared']/'ros-tests/index.json').read_bytes())
            current=json.loads((out/'ros-tests/index.json').read_bytes())
            old={r['source']:r for r in before['cases']};new={r['source']:r for r in current['cases']}
            removed=set(old)-set(new)
            assert removed in(set(),{'support/source/ros/pack_ros2_contracts.py'})
            assert not set(new)-set(old)
            changes=[]
            for name,row in new.items():
                previous=old[name]
                allowed={'id'}
                if name=='support/source/ros_platform_sdk/test_numbered_catalog.py':
                    allowed|={'source_bytes','source_sha256'}
                assert {k:v for k,v in row.items()if k not in allowed}=={k:v for k,v in previous.items()if k not in allowed}
                if row!=previous:changes.append(dict(source=name,before=previous,after=row))
            for row in current['cases']+current['support_entrypoints']:
                p=source/'test/0073.ros-tests'/row['source'];actual=fact(p)
                assert actual['bytes']==row['source_bytes']and actual['sha256']==row['source_sha256']
            (out/'case-placement-migration.json').write_text(json.dumps(dict(
                removed_test_entries=sorted(removed),reclassified_as_support=sorted(removed),changes=changes),indent=2)+'\n')
            report.update(status='NUMBERED_SOURCE_INDEX_REFRESHED',
                test_catalog=str(out/'ros-tests'),cases=len(current['cases']),all_test_pass_claim=False,
                removed_build_archive_tools=sorted(removed),
                changed_test_implementation='test_numbered_catalog.py adds an archive-tool exclusion fixture')
        elif a.phase=='package':
            sdk=root/inputs['sdk_prepared']/'sdk'
            core=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
            for row in core['entries']:
                actual=fact(sdk/row['path']);assert all(actual[k]==row[k]for k in('bytes','sha256'))
            for name in('CMakeLists.txt','README.md','tools/prepare_distribution.py'):
                shutil.copyfile(source/'sdk'/name,sdk/name)
            build=out/'cmake-build';installed=out/'installed'
            run('cmake-source-interfaces',['/usr/bin/cmake','-S',sdk,'-B',build,
                '-DCMAKE_INSTALL_PREFIX='+str(installed)],timeout=180)
            run('cmake-separated-install',['/usr/bin/cmake','--install',build],timeout=180)
            assert not(installed/'origin').exists()and not(installed/'example').exists()and not(installed/'test').exists()
            consumer=out/'consumer';consumer.mkdir()
            (consumer/'CMakeLists.txt').write_text('''cmake_minimum_required(VERSION 3.20)
project(installed_roswasm_interfaces LANGUAGES NONE)
find_package(roswasm CONFIG REQUIRED)
foreach(kind IN ITEMS c cpp)
  get_target_property(value roswasm::${kind} TYPE)
  if(NOT value STREQUAL "INTERFACE_LIBRARY")
    message(FATAL_ERROR "SDK consumer unexpectedly imports a binary")
  endif()
endforeach()
''')
            run('cmake-installed-consumer',['/usr/bin/cmake','-S',consumer,'-B',out/'consumer-build',
                '-DCMAKE_PREFIX_PATH='+str(installed)])
            run('xmake-installed-config',[inputs['xmake'],'f','-q','-c','-P',installed,
                '--ros-sdk-platforms=ros,ros1,ros2,unitree','--ros-sdk-languages=c,cpp,rs,py'])
            run('xmake-installed-build',[inputs['xmake'],'-q','-P',installed,'-j1'])
            report.update(status='SEPARATED_SDK_INSTALL_AND_CMAKE_XMAKE_CONSUMERS_PASS',
                header_or_Guest_implementation_bytes_changed=False,examples_installed=False,tests_installed=False,
                maintenance_origin_installed=False)
        elif a.phase=='probe':
            run('Runtime-version',[inputs['runtime'],'--version'],None)
            run('Runtime-help',[inputs['runtime'],'--help'],None)
            if Path('/usr/bin/gdb').is_file():
                run('Runtime-SIGILL-diagnosis',['/usr/bin/gdb','--batch','--nx','--nh',
                    '-iex','set auto-load off','-ex','set pagination off','-ex','run','-ex','bt',
                    '-ex','x/8i $pc-16','-ex','info registers','--args',inputs['runtime'],'-Rint',
                    '--wasip1-global-noinherit-system-environment','--run',
                    root/'out/sdk-layout-verify-r4/raw-c-ros1.wasm'],None)
            report['status']='RUNTIME_STARTUP_DIAGNOSTIC_RECORDED'
        elif a.phase=='prepare':
            run('origin-path-receipts',[sys.executable,'-I','-S','-B',source/'sdk/tools/reconcile_origin_migration.py',
                '--source',source,'--manifest',root/'control/local-source-migration.json','--out',out/'source-receipts'])
            wasi=Path(inputs['toolchains'])/'wasi-sdk-33.0-x86_64-linux'
            run('standalone-distribution',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_distribution.py',
                '--origin',source/'sdk/origin','--out',out/'sdk','--wasi-sysroot',wasi/'share/wasi-sysroot',
                '--fast-io',source/'third-parties/fast_io/include'],timeout=600)
            for n in('xmake.lua','CMakeLists.txt','README.md'):
                shutil.copyfile(source/'sdk'/n,out/'sdk'/n)
            shutil.copytree(source/'sdk/tools',out/'sdk/tools')
            run('numbered-ROS-test-catalog',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_test_catalog.py',
                '--source',source/'test/0073.ros-tests/support/source','--out',out/'ros-tests'])
            report.update(status='PLATFORM_FIRST_SDK_AND_NUMBERED_TEST_SOURCES_PREPARED',
                sdk=str(out/'sdk'),test_catalog=str(out/'ros-tests'),actual_application_tests_run=0)
        else:
            prepared=root/inputs['sdk_prepared']
            accepted=json.loads((prepared/'result.json').read_bytes())
            assert accepted['status']=='PLATFORM_FIRST_SDK_AND_NUMBERED_TEST_SOURCES_PREPARED'
            sdk=Path(accepted['sdk']);manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
            catalog_root=root/inputs['sdk_test_catalog']if inputs.get('sdk_test_catalog')else Path(accepted['test_catalog'])
            catalog=json.loads((catalog_root/'index.json').read_bytes())
            test_root=source/'test/0073.ros-tests'
            if not(test_root/'platform').exists():
                shutil.copytree(catalog_root/'platform',test_root/'platform',symlinks=True)
            else:
                for folder in sorted((test_root/'platform').glob('*/language/*/*')):
                    if folder.is_dir():shutil.rmtree(folder)
                shutil.copytree(catalog_root/'platform',test_root/'platform',symlinks=True,dirs_exist_ok=True)
            groups={}
            for case in catalog['cases']:
                folder=test_root/case['id'];original=test_root/case['source']
                assert original.is_file()and original.resolve().is_relative_to(test_root/'support/source')
                assert fact(original)['sha256']==case['source_sha256']
                assert next(folder.glob('source.*')).resolve(strict=True)==original.resolve()
                key=(case['platform'],case['language']);groups[key]=groups.get(key,0)+1
                assert folder.name.startswith('%04d.'%groups[key])
            for name,arguments in [('ros_distro/test_profiles.py',[]),
                    ('ros_platform_sdk/test_numbered_catalog.py',[]),
                    ('ros_build_options/test_platform_delivery_tools.py',['--source',source,'--out',out/'platform-tool-contracts'])]:
                rows=[c for c in catalog['cases']if c['source'].endswith('/'+name)]
                assert len(rows)==1,rows
                case=rows[0];driver=test_root/case['id']/'run.py'
                described=run('numbered-describe-'+Path(name).stem,[sys.executable,'-I','-S','-B',driver,'--describe'])
                assert json.loads(described)==case
                run('numbered-refuse-unowned-'+Path(name).stem,[sys.executable,'-I','-S','-B',driver,'--execute'],2)
                run('numbered-execute-'+Path(name).stem,[sys.executable,'-I','-S','-B',driver,
                    '--execute','--owner',root/'out'/(out.name+'.json'),'--',*arguments])
            report.update(numbered_source_cases_prepared=catalog['cases_prepared'],
                numbered_test_drivers_executed=3,catalog_is_not_all_test_pass=True);save()
            assert manifest['include_depends_on_origin']is False and manifest['default_authority']==[]
            assert not(sdk/'origin').exists()
            for row in manifest['entries']:
                p=sdk/row['path'];assert not p.is_symlink()
                actual=fact(p);assert all(actual[k]==row[k]for k in('bytes','sha256'))
            for p in(sdk/'include').rglob('*'):
                assert not p.is_symlink()
                if p.is_dir():assert p.name not in('origin','test','tests','example','examples')
            report['ordinary_include_and_source_pins_verified']=True;save()
            wasi=Path(inputs['toolchains'])/'wasi-sdk-33.0-x86_64-linux'
            common=['--target=wasm32-wasip1','--sysroot='+str(sdk),'-Os','-g0','-Werror']
            cppflags=['-std=c++20','-fno-exceptions','-fno-rtti']
            run('selected-Clang-version',[wasi/'bin/clang++','--version'])
            empty=out/'include-search.cpp';empty.write_text('')
            run('selected-Cpp-include-search',[wasi/'bin/clang++',*common,*cppflags,'-E','-v',empty])
            headers=sorted((sdk/'include/platform').glob('*/c/ros*/*/bounded_records.h'))
            cppheaders=sorted((sdk/'include/platform').glob('*/cpp/ros*/*/bounded_records.hpp'))
            sdk_index=json.loads((sdk/'share/sdk-index.json').read_bytes())
            for language,actual_headers,filename in [('c',headers,'bounded_records.h'),('cpp',cppheaders,'bounded_records.hpp')]:
                expected=set()
                for platform,value in sdk_index['platforms'].items():
                    for family in value['languages'].get(language,{}).get('typed_vendor_families',[]):
                        matches=list((sdk/'include/platform'/platform/language).glob('ros*/'+family+'/'+filename))
                        assert len(matches)==1,(platform,language,family,matches)
                        expected.add(matches[0])
                assert set(actual_headers)==expected,(language,len(actual_headers),len(expected))
            prior=root/inputs['sdk_header_verified']if inputs.get('sdk_header_verified')else None
            prior_result=json.loads((prior/'result.json').read_bytes())if prior else None
            for language,items,compiler,flags,suffix in [('cpp',cppheaders,'clang++',cppflags,'.cpp'),
                    ('c',headers,'clang',['-std=c11'],'.c')]:
                for index,p in enumerate(items):
                    label=language+'-family-%03d'%index
                    driver=(prior or out)/(label+suffix)
                    body='#include <'+p.relative_to(sdk/'include').as_posix()+'>\n'
                    argv=list(map(str,[wasi/'bin'/compiler,*common,*flags,'-fsyntax-only',driver]))
                    if prior:
                        assert driver.read_text()==body
                        rows=[r for r in prior_result['commands']if r['label']==label]
                        assert len(rows)==1 and rows[0]['exit']==0 and rows[0]['argv']==argv
                        for stream in ('stdout','stderr'):
                            actual=fact(prior/'commands'/label/stream)
                            assert all(actual[k]==rows[0][stream][k]for k in('bytes','sha256'))
                    else:
                        driver.write_text(body);run(label,argv)
            if prior:report['unchanged_header_compilations_reused']=dict(
                result=fact(prior/'result.json'),C=len(headers),CPP=len(cppheaders),
                sdk_manifest=fact(sdk/'share/distribution-manifest.json'),
                previous_overall_attempt_passed=prior_result['status']!='FAILED_RETAINED')
            report.update(C_complete_family_headers=len(headers),CPP_complete_family_headers=len(cppheaders));save()
            for lang,compiler,extension,flags in [('c','clang','c',['-std=c11']),('cpp','clang++','cpp',cppflags)]:
                app=sdk/'example/platform/ros'/lang/lang/('01_raw_imports.'+extension)
                assert app.is_file(),app
                for version in(1,2):
                    artifact=out/('raw-'+lang+'-ros'+str(version)+'.wasm')
                    run('build-'+artifact.stem,[wasi/'bin'/compiler,*common,*flags,'-DROSWASM_EXAMPLE_ROS_VERSION='+str(version),
                        app,'-Wl,--threads=1','-o',artifact])
                    run('validate-'+artifact.stem,[inputs['validator'],'validate',artifact])
                    output=run('denied-'+artifact.stem,[inputs['runtime'],'-Rint',
                        '--wasip1-global-noinherit-system-environment','--run',artifact],13)
                    assert not output
            for lang,compiler,extension,flags in [('c','clang','c',['-std=c11']),('cpp','clang++','cpp',cppflags)]:
                cases=sorted((sdk/'test/platform/ros'/lang).glob('*.golden_vectors/golden_vectors.'+extension))
                assert len(cases)==1,cases
                artifact=out/('golden-'+lang+'.wasm')
                run('build-golden-'+lang,[wasi/'bin'/compiler,*common,*flags,cases[0],'-Wl,--threads=1','-o',artifact])
                run('run-golden-'+lang,[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact])
            crc=list((sdk/'test/platform/unitree/c').glob('*.crc_contract/crc_contract.c'))
            assert len(crc)==1,crc
            artifact=out/'unitree-crc.wasm'
            run('build-unitree-crc',[wasi/'bin/clang',*common,'-std=c11',crc[0],'-Wl,--threads=1','-o',artifact])
            run('run-unitree-crc',[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact])
            # The exact existing arithmetic baseline exercises the compiled
            # case recipe. This comparison establishes runner correctness;
            # it is not a ROS transport, hardware or performance result.
            cases=[c for c in catalog['cases']if c['source']=='support/source/ros_unitree/native_math.c']
            assert len(cases)==1
            case=cases[0];driver=test_root/case['id']/'run.py';outputs={}
            for target,compiler,flags in [('wasm',wasi/'bin/clang',common),
                    ('native',Path('/usr/bin/clang-21'),['-Os','-g0','-Werror'])]:
                artifact=out/('numbered-arithmetic.'+target)
                run_argv=([inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run','{artifact}']
                    if target=='wasm'else['{artifact}'])
                recipe=out/('numbered-arithmetic-'+target+'-recipe.json')
                recipe.write_text(json.dumps(dict(kind='explicit-numbered-ros-test-recipe-v1',
                    source_sha256=case['source_sha256'],artifact=str(artifact),
                    compile_argv=[str(compiler),*flags,'-std=c11','-DROBOT_BENCHMARK_ITERATIONS=256',
                        '{source}','-o','{artifact}'],run_argv=run_argv),indent=2)+'\n')
                outputs[target]=run('numbered-compiled-'+target,[sys.executable,'-I','-S','-B',driver,
                    '--execute','--owner',root/'out'/(out.name+'.json'),'--recipe',recipe])
            assert len(outputs['wasm'])==8 and outputs['wasm']==outputs['native']
            report.update(numbered_compiled_recipes_executed=2,numbered_arithmetic_native_Wasm_bytes_equal=True);save()
            run('xmake-version',[inputs['xmake'],'--version'])
            for label,platforms,languages in [('C-Cpp-four-platforms','ros,ros1,ros2,unitree','c,cpp'),
                    ('source-modules-four-platforms','ros,ros1,ros2,unitree','rs,py,js,as,tinygo'),
                    ('all-delivered-base-languages','ros,ros1,ros2',','.join(sdk_index['delivered_languages'])),
                    ('official-WASI-headers','wasi','c,cpp')]:
                run('xmake-'+label,[inputs['xmake'],'f','-q','-c','-P',sdk,
                    '--ros-sdk-platforms='+platforms,'--ros-sdk-languages='+languages])
                run('xmake-build-'+label,[inputs['xmake'],'-q','-P',sdk,'-j1'],timeout=180)
            report.update(status='PLATFORM_FIRST_SDK_HEADER_SYSROOT_AND_REAL_GUEST_CHECKS_PASS',
                default_denials=4,golden_Guest_programs=2,Unitree_CRC_Guest_programs=1,
                xmake_headeronly_configurations=4,Runtime_reused=fact(inputs['runtime']))
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
