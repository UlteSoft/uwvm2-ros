#!/usr/bin/env python3
"""Finite installed-layout Task05 xmake checks with genuine Wasm producers.

This is a selected private source overlay over frozen actual SDK artifacts.
It does not publish a distribution, change shared generators, run native ROS,
claim the other language providers or execute any original official suite.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess


KITS={'unitree':'robots/unitree','leju':'robots/humanoids/leju',
    'deep_robotics':'robots/platforms/deep_robotics','agibot':'robots/platforms/agibot',
    'ubtech':'robots/platforms/ubtech','fourier':'robots/platforms/fourier',
    'booster':'robots/humanoids/booster','limx':'robots/humanoids/limx'}
CASES=[
    ('unitree','cpp','b2_sport_command.api',['send','Damp','task05.b2'],13,None),
    ('unitree','cpp','sport_roundtrip_gated.api',['Damp','task05.request','task05.response','0','10','1'],13,b'G'),
    ('unitree','c','complete_telemetry.api',[],0,None),
    ('leju','cpp','whole_value.ros1_beta_unregistered_dtos',['state','task05.leju'],13,None),
    ('deep_robotics','cpp','lite3_service_once.ros2_main',['sdk-mode','task05.lite3','200'],13,None),
    ('agibot','cpp','whole_value.ros2_sdk_v1_1_0_rl',['state','task05.agibot'],13,None),
    ('ubtech','cpp','whole_value.ros2_main',['state','task05.ubtech'],13,None),
    ('fourier','cpp','whole_value.ros2_aurora_bridge_v1',['state','task05.fourier'],13,None),
    ('booster','cpp','original_telemetry.official_ros2',[],0,None),
    ('booster','cpp','telemetry.t1_sdk_bridge',[],0,None),
    ('booster','cpp','complete_command.t1_sdk_bridge',[],0,None),
    ('limx','cpp','telemetry.pf_p441a6',[],0,None),
    ('limx','cpp','complete_command.pf_p441a6',[],0,None)]


def fact(path):
    path=Path(path)
    with path.open('rb') as stream:digest=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=digest)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('snapshot','artifact-sdk','tools','fast-io','xmake','wasi','runtime','validator'):
        parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    run=Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True)
    assert os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD']=='1'
    assert Path('/proc/self/cgroup').read_text().strip().endswith('/'+Path(os.environ['UWVM_RESOURCE_SCOPE']).name)
    out=run/'work/installed-example';out.mkdir();logs=out/'commands';logs.mkdir()
    origin=args.snapshot/'sdk/origin';public=args.snapshot/'sdk'
    report=dict(status='RUNNING',scope='selected-private-installed-layout-overlay',inputs=[],commands=[],cases=[],catalogs=[],
        actual_xmake_builds=0,actual_Wasm_validations=0,actual_Guest_runs=0,
        actual_Guest_denials=0,actual_Guest_zero_exits=0,native_ROS_runs=0,
        official_suite_runs=0,QEMU_runs=0,VTune_runs=0,full_distribution_qualified=False,
        complete_platform_support=False,default_authority=[])
    pins={}
    def pin(path):
        path=Path(path).resolve(strict=True)
        if path not in pins:pins[path]=fact(path);report['inputs'].append(pins[path])
        return path
    def tree(source,destination):
        assert source.is_dir()
        for path in sorted(source.rglob('*')):
            if path.is_file():copy(path,destination/path.relative_to(source))
    def copy(source,destination):
        source=pin(source);destination.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(source,destination)
        assert fact(source)['sha256']==fact(destination)['sha256']
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def command(label,argv,*,cwd=out,expected=0,input=None):
        folder=logs/label;folder.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),cwd=str(cwd),exit=None);report['commands'].append(row);save()
        with (folder/'stdout').open('xb') as stdout,(folder/'stderr').open('xb') as stderr:
            child=subprocess.run(row['argv'],cwd=cwd,stdin=subprocess.DEVNULL if input is None else None,input=input,
                stdout=stdout,stderr=stderr,timeout=180,env=dict(os.environ,XMAKE_STATS='false',XMAKE_SKIP_HISTORY='y'))
        row.update(exit=child.returncode,stdout=fact(folder/'stdout'),stderr=fact(folder/'stderr'));save()
        if child.returncode!=expected:raise RuntimeError((label,child.returncode,(folder/'stderr').read_text()[-4500:],(folder/'stdout').read_text()[-4500:]))
        return (folder/'stdout').read_bytes()
    try:
        pin(__file__)
        for path in (args.xmake,args.runtime,args.validator,args.wasi/'bin/clang',args.wasi/'bin/clang++'):pin(path)
        sdk=out/'sdk';tree(args.artifact_sdk,sdk)
        tree(args.tools,sdk/'tools')
        for language in ('c','cpp'):
            copy(origin/'tools/xmake'/(language+'.lua'),sdk/'tools/xmake'/(language+'.lua'))
            for platform in ('ros','ros1','ros2'):
                tree(public/'include/platform'/platform/language,sdk/'include/platform'/platform/language)
        tree(origin/'tools/xmake/examples',sdk/'tools/xmake/examples')
        tree(args.fast_io,sdk/'include/wasm/language/c++/include')
        copy(origin/'include/roswasm/cpp_profile.hpp',
            sdk/'include/platform/ros/cpp/roswasm/cpp_profile.hpp')
        copy(public/'share/distribution-manifest.json',sdk/'share/distribution-manifest.json')
        common=origin/'robots/humanoids/leju/tools'
        for platform,relative in KITS.items():
            kit=origin/relative;prefix=sdk/'example/platform'/platform
            model=json.loads((kit/'examples/catalog.json').read_bytes())
            assert model['platform']==platform and model['default_authority']==[]
            unique=set();selections=0
            for entry in model['applications']:
                key=(entry['language'],entry['id']);assert key not in unique;unique.add(key)
                assert entry['startup'] in ('command','reactor')
                profiles=model['profiles'] if entry['profiles']==['*'] else entry['profiles']
                assert all(profile in model['profiles'] for profile in profiles)
                selections+=len(profiles)
            report['catalogs'].append(dict(platform=platform,application_definitions=len(unique),source_profile_selections=selections,
                scope='static catalog consistency only; not build/official-example qualification'))
            selected_profiles={example.rsplit('.',1)[1] for name,_,example,*_ in CASES if name==platform}
            selected_families={model['profiles'][name]['family'] for name in selected_profiles}
            copy(kit/'examples/catalog.json',prefix/'catalog.json')
            if platform=='unitree':
                copy(kit/'recipes/wasm32-command-memory.json',sdk/'share/platform/unitree/wasm32-command-memory.json')
            copy(common/'platform_examples.lua',prefix/'build.lua')
            copy(common/'prepare_application.py',prefix/'prepare.py')
            copy(common/'XMAKE_EXAMPLES.md',prefix/'XMAKE_EXAMPLES.md')
            for language in ('c','cpp'):
                folder=public/'example/platform'/platform/language
                if folder.is_dir():tree(folder,prefix/language)
                module=kit/'examples/xmake'/(language+'.lua')
                if module.is_file():copy(module,prefix/language/'xmake.lua')
                generated=public/'include/platform'/platform/language
                if generated.is_dir():
                    for path in sorted(generated.rglob('*')):
                        if not path.is_file():continue
                        relative=path.relative_to(generated)
                        if relative.parts[0] in ('ros1','ros2') and relative.parts[1] not in selected_families:continue
                        copy(path,sdk/'include/platform'/platform/language/relative)
            # Preserve current owned command bodies/support, including the
            # newly fixed EH-off branches. Public headers remain exact frozen
            # installed products, with separately recorded owned additions.
            for path in sorted((kit/'examples').rglob('*')):
                if not path.is_file() or path.suffix not in ('.c','.cc','.cpp','.h','.hpp'):
                    continue
                languages=('c','cpp') if path.suffix=='.h' else ('c',) if path.suffix=='.c' else ('cpp',)
                for language in languages:
                    copy(path,prefix/language/path.relative_to(kit/'examples'))
            for path in sorted((kit/'include').rglob('*')):
                if path.is_file() and path.suffix in ('.h','.hpp','.inc'):
                    language='cpp' if path.suffix=='.hpp' else 'c'
                    copy(path,sdk/'include/platform'/platform/language/path.relative_to(kit/'include'))
            if platform=='deep_robotics':copy(kit/'examples/lite3_service_once.cpp',prefix/'cpp/lite3_service_once.cpp')
            if platform=='unitree':
                for name in ('b2_sport_command.cpp','sport_roundtrip.cpp'):copy(kit/'examples'/name,prefix/'cpp'/name)
                for name in ('sport.cpp','b2_sport.cpp'):copy(kit/name,sdk/'lib/platform/unitree/cpp/source'/name)
                for path in sorted((kit/'include').rglob('*')):
                    if path.is_file() and path.suffix in ('.h','.hpp','.inc'):
                        language='cpp' if path.suffix=='.hpp' else 'c'
                        copy(path,sdk/'include/platform/unitree'/language/path.relative_to(kit/'include'))
        project=out/'project';project.mkdir()
        lua='set_project("task05-installed-layout-acceptance")\nset_version("0.1.0")\n'
        lua+='includes('+json.dumps(str(sdk/'tools/xmake/common/profiles.lua'))+')\nlocal common=uwvm_sdk_common()\ncommon.register_options()\n'
        lua+='option("ros-sdk-cpp-standard") set_default("20") option_end()\n'
        lua+='includes('+json.dumps(str(sdk/'tools/xmake/examples/register.lua'))+')\nuwvm_sdk_example_tasks('+json.dumps(str(sdk))+',common)\n'
        (project/'xmake.lua').write_text(lua)
        command('xmake-version',[args.xmake,'--version'])
        for index,(platform,language,example,argv,expected,stdin) in enumerate(CASES):
            label=str(index)+'-'+platform+'-'+example
            command(label+'-configure',[args.xmake,'f','-y','-c','--sdk-example-platform='+platform,
                '--sdk-example-language='+language,'--sdk-example='+example,'--sdk-target=wasm32-wasip1',
                '--sdk-startup=command','--sdk-exceptions=n','--sdk-threads=n','--sdk-python=/usr/bin/python3',
                '--sdk-toolchain='+str(args.wasi)],cwd=project)
            listing=command(label+'-list',[args.xmake,'sdk-example-list'],cwd=project)
            assert example.encode()+b'  ' in listing
            destination=out/('build-'+str(index))
            command(label+'-build',[args.xmake,'sdk-example-build','--output='+str(destination)],cwd=project)
            receipt=json.loads((destination/'build-receipt.json').read_bytes())
            assert receipt['status']=='BUILT_NOT_EXECUTED' and receipt['default_authority']==[] and len(receipt['artifacts'])==1
            artifact=Path(receipt['artifacts'][0]['path']);assert artifact.is_relative_to(destination)
            command(label+'-validate',[args.validator,'validate',artifact])
            body=command(label+'-Guest',[args.runtime,'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact,*argv],expected=expected,input=stdin)
            if example=='sport_roundtrip_gated.api':assert body==b'READY_UNITREE_SPORT\nDONE primary=13 subscription=0 publisher=0\n'
            if example=='lite3_service_once.ros2_main':assert body==b'DONE transport=13 native_valid=0 native_result=0 cleanup=0\n'
            report['cases'].append(dict(platform=platform,language=language,example=example,status='PASS',
                actual_xmake_build=True,artifact=fact(artifact),actual_Wasm_Guest=True,expected_exit=expected,
                native_ROS_qualified=False,official_suite_credit=0))
            report['actual_xmake_builds']+=1;report['actual_Wasm_validations']+=1
            report['actual_Guest_runs']+=1
            report['actual_Guest_denials' if expected==13 else 'actual_Guest_zero_exits']+=1;save()
        assert all(fact(path)==row for path,row in pins.items())
        report.update(status='PASS',passed=len(CASES),denominator=len(CASES),input_pins_preserved=True)
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
