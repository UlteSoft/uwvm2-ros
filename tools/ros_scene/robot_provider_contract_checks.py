#!/usr/bin/env python3
"""Verify remaining robot provider designs and allocation-free canonical decoders.

This qualifies 22 designed request/result contracts and their 26-domain boundary map.
It never registers imports or claims native provider, VM, physical safety or
platform implementation. All builds use -O3 -g inside the existing memory gate.
"""
import argparse,hashlib,json,os,re,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--thread',action='store_true');p.add_argument('--cross',action='store_true');a=p.parse_args();guard()
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    names=['src/uwvm2/imported/ros/scene/wire.h','src/uwvm2/imported/ros/scene/schema_contract.h',
        'src/uwvm2/imported/ros/scene/robot_provider_contract.h','test/0084.ros-runtime/robot_provider_contract.cpp',
        'tools/ros_scene/robot-provider-contracts.json','tools/ros_scene/interface-catalogue.json',
        'tools/ros_scene/generate_schema_contract.py','tools/ros_scene/robot_provider_contract_checks.py',
        'src/uwvm2/imported/ros/scene/robot_provider_response.h','src/uwvm2/imported/ros/scene/operation_design.h',
        'tools/ros_scene/operation-contracts.json','tools/ros_scene/check_operation_design.py',
        'test/0084.ros-runtime/test_operation_design.py','documents/runtime/ros-operation-reference.md',
        'src/uwvm2/imported/ros/scene/component_fault.h']
    pins={name:sha(ROOT/name)for name in names};snapshot=out/'source'
    for name in names:
        target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,target)
    report=dict(complete=False,scope=__doc__,inputs=pins,source_root=str(snapshot),commands=[],cgroup=str(GROUP),
        inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text(),
        native_provider_qualified=False,vm_carriers_qualified=False,toolchain={})
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,timeout=240):
        guard();start=time.monotonic();r=subprocess.run([str(x)for x in argv],capture_output=True,text=True,timeout=timeout,
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (out/(label+'.stdout')).write_text(r.stdout);(out/(label+'.stderr')).write_text(r.stderr)
        report['commands'].append(dict(label=label,argv=[str(x)for x in argv],exit=r.returncode,seconds=time.monotonic()-start));save()
        assert r.returncode==0,(label,r.stderr[-5000:]);print('PASS',label,flush=True);return r.stdout
    try:
        catalogue=json.loads((snapshot/names[5]).read_text());contract=json.loads((snapshot/names[4]).read_text())
        operations={row['name']:row for row in catalogue['operations']}
        designed=contract['operations'];macros=set(re.findall(r'X\(\w+,\w+,"([^"]+)"\)',(snapshot/names[2]).read_text()))
        assert set(designed)==macros=={n for n,r in operations.items()if r['family'] in {'device','control','media','bridge','component'}}
        assert {n for n,r in operations.items()if r['implementation']=='planned_provider'} <= macros
        assert len(designed)==22 and contract['abi_module']=='uwvm_ros_v2'
        required={'device':5,'control':5,'media':4,'bridge':4,'component':4}
        for family,count in required.items():
            assert sum(x['family']==family for x in designed.values())==count
            settings=contract['families'][family]
            for category in ['profile_fields','capacity_fields','timing_fields','states','required_features']:
                assert settings[category] and len(settings[category])==len(set(settings[category]))
            assert {'stop_bytes','stop_slots','retained_bytes','workspace_bytes','workspace_calls'}<=set(settings['capacity_fields'])
        for name,rule in designed.items():
            assert rule['primary_action']==name and rule['family']==operations[name]['family']
            assert set(rule['inherits'])==set(contract['common'])
            assert all(rule[k]for k in ['secondary_admission','effect','commit_and_recovery'])
            assert operations[name]['design_contract']=='robot-provider-contracts.json#'+name
            for direction in ['request','result']:assert rule[direction+'_schema']==name.replace('.','_')+'__'+direction
        domains=contract['domains'];assert len(domains)==26 and {d['id']for d in domains}==set(range(1,27))
        assert len({d['name']for d in domains})==26
        for row in domains:assert row['operations'] and set(row['operations'])<=set(operations) and row['boundary']
        assert contract['cli_contract']['status']=='registered_bindings_with_explicit_platform_scope'
        report['design_consistency']=dict(operations=22,domains=26,families=required)
        run('generated-contract',['/usr/bin/python3',snapshot/names[6],'--check'])
        run('all-operation-design',['/usr/bin/python3',snapshot/'tools/ros_scene/check_operation_design.py'])
        run('design-adversarial',['/usr/bin/python3',snapshot/'test/0084.ros-runtime/test_operation_design.py'])
        facade=snapshot/'robot_macros.cpp'
        facade.write_text('#define UWVM_SCENE_ROBOT_CONTRACT_MACROS_ONLY\n'
            '#include <uwvm2/imported/ros/scene/robot_provider_contract.h>\n'
            '#define COUNT(symbol,operation,name) +1\nstatic_assert((0 '
            'UWVM_ROS_DEVICE_FUNCTIONS(COUNT) UWVM_ROS_CONTROL_FUNCTIONS(COUNT) '
            'UWVM_ROS_MEDIA_FUNCTIONS(COUNT) UWVM_ROS_BRIDGE_FUNCTIONS(COUNT) '
            'UWVM_ROS_COMPONENT_FUNCTIONS(COUNT))==22);\n')
        report['macro_facade_sha256']=sha(facade)
        run('macro-facade',['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror',
            '-I'+str(snapshot/'src'),'-fsyntax-only',facade])
        variants=[('clang-asan','/usr/bin/clang++-22',['-fsanitize=address,undefined']),('gcc','/usr/bin/g++-13',[])]
        if a.thread:variants.append(('clang-tsan','/usr/bin/clang++-22',['-fsanitize=thread']))
        flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-fno-omit-frame-pointer','-pthread',
            '-fno-builtin-malloc','-fno-builtin-calloc','-fno-builtin-realloc','-I'+str(snapshot/'src')]
        links=['-Wl,--wrap=malloc','-Wl,--wrap=calloc','-Wl,--wrap=realloc']
        for label,compiler,extra in variants:
            report['toolchain'][compiler]=sha(compiler)
            binary=out/label;run(label+'-compile',[compiler,*flags,*extra,snapshot/names[3],*links,'-o',binary])
            assert 'passed 44 robot provider request/result shapes and 7 safety groups'in run(label+'-run',[binary])
        if a.cross:
            # Use the already qualified local sysroots and bundled C++ runtimes,
            # not guessed cross-GCC installations or downloads during a test.
            recipes=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/output-optimized-core-final/results.json'
            prior=json.loads(recipes.read_text());assert prior['complete']
            report['cross_recipe_sha256']=sha(recipes)
            for arch in ['aarch64','armhf','i686']:
                recipe=next(c['argv'] for c in prior['commands'] if c['name']==arch+'-compile')
                execute=next(c['argv'] for c in prior['commands'] if c['name']==arch)[:-1]
                at=next(i for i,x in enumerate(recipe) if x.endswith('/output_security.cpp'))
                cross=[x for x in recipe[:at] if not x.startswith('-O') and x not in ['-Isrc','-g']]
                tail=recipe[at+3:];assert recipe[at+1]=='-o'
                cross+=['-O3','-g','-I'+str(snapshot/'src'),'-fno-builtin-malloc','-fno-builtin-calloc','-fno-builtin-realloc']
                for item in [cross[0],*execute,*tail]:
                    if Path(item).is_file():report['toolchain'][item]=sha(item)
                binary=out/arch;run(arch+'-compile',[*cross,snapshot/names[3],'-o',binary,*tail,*links])
                assert 'passed 44 robot provider request/result shapes and 7 safety groups'in run(arch+'-run',[*execute,binary])
        report['complete']=True
    finally:
        report['sources_unchanged']=all(sha(ROOT/n)==h and sha(snapshot/n)==h for n,h in pins.items())
        report['complete']&=report['sources_unchanged'];report['events_after']=(GROUP/'memory.events').read_text();save()
    assert report['complete']
if __name__=='__main__':main()
