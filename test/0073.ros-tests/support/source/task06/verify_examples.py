#!/usr/bin/env python3
"""Controlled exact-source SDK-local xmake/application acceptance for task 06.

The input SDK is copied as an isolated candidate and gets the explicitly pinned
language-owner helper overlay. This is not global distribution publication.
Python is a host scheduler/oracle, never a Python Guest. Native fixture data is
not native ROS or Gateway evidence. No physical endpoint/grant is configured.
"""
import argparse
import difflib
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

RUNTIME_SHA256='719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19'
PLATFORMS=('elephant','ufactory','dobot','flexiv','realman')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path,value):
    path.write_text(json.dumps(value,indent=2,sort_keys=True)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--sdk',type=Path,required=True)
    p.add_argument('--helpers',type=Path,required=True)
    p.add_argument('--xmake',type=Path,required=True)
    p.add_argument('--xmake-program',type=Path,required=True)
    p.add_argument('--toolchain',type=Path,required=True)
    p.add_argument('--runtime',type=Path,required=True)
    p.add_argument('--fixture',type=Path,required=True)
    p.add_argument('--operations',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    p.add_argument('--quick',action='store_true')
    p.add_argument('--platform',choices=PLATFORMS,action='append',
        help='Explicitly scope a finite acceptance job; repeat for selected platforms')
    p.add_argument('--source-profile',help='One exact source profile, requiring one --platform')
    p.add_argument('--root-description-fix',action='store_true',
        help='Test only an exact private one-line SDK root candidate; never edit the shared root')
    p.add_argument('--minimal-sdk',action='store_true',
        help='Stage actual pinned command closure plus one explicit complete source profile; no dependency is stubbed')
    p.add_argument('--sdk-supplement',type=Path,
        help='Inert missing input files only; every supplement must match an existing manifest size and SHA-256')
    p.add_argument('--sdk-overlay',type=Path,
        help='Exact inert current source overlay inside a private SDK; every input/postimage is pinned separately')
    p.add_argument('--state-readers',action='store_true',
        help='Also build/validate/run denial for the real old C/C++ state applications and all explicit source/model selections')
    a=p.parse_args()
    platforms=tuple(a.platform or PLATFORMS)
    if len(set(platforms))!=len(platforms):raise RuntimeError('duplicate acceptance platform selection')
    if a.source_profile and len(platforms)!=1:raise RuntimeError('one explicit platform is required with a source profile')
    if a.quick and ('flexiv' not in platforms or a.source_profile not in (None,'ros2_humble_v1')):
        raise RuntimeError('quick acceptance is exactly the Flexiv Humble v1 source profile')
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD')!='1' or sha(a.runtime)!=RUNTIME_SHA256:
        raise RuntimeError('common owner and exact frozen Runtime required')
    if not (a.helpers/'xmake/c.lua').is_file() or not (a.helpers/'xmake/examples/register.lua').is_file():
        raise RuntimeError('exact helper package root required; do not nest source paths inside installed tools')
    a.out.mkdir()
    sdk=a.out/'installed-sdk'
    if a.minimal_sdk:
        if len(platforms)!=1 or not a.source_profile:
            raise RuntimeError('minimal SDK requires one explicit platform and exact source profile')
        platform=platforms[0]
        manifest_relative=Path('example/platform')/platform/'manifest.json'
        manifest_path=a.sdk/manifest_relative
        if a.sdk_overlay and (a.sdk_overlay/manifest_relative).is_file():manifest_path=a.sdk_overlay/manifest_relative
        profiles=json.loads(manifest_path.read_bytes())['profiles']
        matches=[row for row in profiles if row['id']==a.source_profile]
        if len(matches)!=1:raise RuntimeError('one exact installed original source profile is required')
        source_profile=matches[0];family=source_profile['family'];dialect='ros'+str(source_profile['ros_version'])
        sdk.mkdir()
        selected=set()
        pinned={}
        for row in json.loads((a.sdk/'share/wasm-profiles.json').read_bytes())['profiles']:
            if row['profile']['target']=='wasm32-wasip1' and row['profile']['startup']=='command' and not row['profile']['exceptions'] and not row['profile']['threads']:
                selected.update(fact['path'] for fact in row['files'])
                for fact in row['files']:
                    if fact['path'] in pinned and pinned[fact['path']]!=fact:
                        raise RuntimeError('contradictory actual SDK profile file pins')
                    pinned[fact['path']]=fact
        selected.update(('xmake.lua','example/xmake.lua','share/wasm-profiles.json'))
        roots=['tools','include/roswasm']
        roots += ['include/platform/'+platform+'/'+language for platform in ('ros','ros1','ros2') for language in ('c','cpp')]
        roots += ['include/platform/'+platform+'/c/'+dialect+'/'+family,
            'example/platform/'+platform+'/c/'+dialect+'/'+family]
        if source_profile.get('monitor_force_member'):roots+=['example/platform/'+platform+'/c/official']
        if a.state_readers:
            roots+=['include/platform/'+platform+'/cpp/'+dialect+'/'+family,
                'example/platform/'+platform+'/c/support','example/platform/'+platform+'/cpp/support']
            selected.update(('include/platform/'+platform+'/c/state.h','include/platform/'+platform+'/cpp/state.hpp',
                'example/platform/'+platform+'/c/guest.c','example/platform/'+platform+'/cpp/guest.cc'))
        for platform in PLATFORMS:
            for name in ('manifest.json','module.lua','xmake.lua','c/xmake.lua','cpp/xmake.lua'):
                selected.add('example/platform/'+platform+'/'+name)
            for name in ('state_reader.lua','state-reader-selections.json'):
                if (a.sdk/'example/platform'/platform/name).is_file():
                    selected.add('example/platform/'+platform+'/'+name)
        for relative in roots:
            for source in (a.sdk/relative).rglob('*'):
                if source.is_file():selected.add(source.relative_to(a.sdk).as_posix())
        supplements=[]
        for relative in sorted(selected):
            source=a.sdk/relative;target=sdk/relative
            if not source.exists() and a.sdk_overlay and (a.sdk_overlay/relative).is_file():
                source=a.sdk_overlay/relative
                if relative in pinned and (source.stat().st_size!=pinned[relative]['bytes'] or sha(source)!=pinned[relative]['sha256']):
                    raise RuntimeError('source overlay cannot change a pinned compiler/library input: '+relative)
            if not source.exists() and a.sdk_supplement:
                source=a.sdk_supplement/relative
                fact=pinned.get(relative)
                if not fact or not source.is_file() or source.is_symlink() or source.stat().st_size!=fact['bytes'] or sha(source)!=fact['sha256']:
                    raise RuntimeError('supplement is not an exact already-pinned SDK input: '+relative)
                supplements.append(dict(path=relative,source=str(source),sha256=sha(source),bytes=source.stat().st_size))
            if source.is_symlink() or not source.is_file():raise RuntimeError('actual ordinary SDK input missing: '+relative)
            target.parent.mkdir(parents=True,exist_ok=True);os.link(source,target)
        save(a.out/'minimal-sdk-selection.json',dict(scope='Exact real shared command closure plus one profile and its full application types',
            files=sorted(selected),pinned_staging_repairs=supplements,omitted_dependency_stubs=0,full_installation_acceptance=False))
    else:
        shutil.copytree(a.sdk,sdk,copy_function=os.link)
    source_overlays=[]
    if a.sdk_overlay:
        for source in sorted(a.sdk_overlay.rglob('*')):
            if source.is_dir():continue
            relative=source.relative_to(a.sdk_overlay)
            if source.is_symlink() or not source.is_file() or relative.parts[0] not in ('example','include') and relative.as_posix()!='xmake.lua':
                raise RuntimeError('only actual confined example/header/root source overlays are permitted')
            destination=sdk/relative;destination.parent.mkdir(parents=True,exist_ok=True)
            before=sha(destination) if destination.is_file() else None
            if destination.exists():destination.unlink()
            shutil.copyfile(source,destination)
            source_overlays.append(dict(path=relative.as_posix(),input=str(source),before_sha256=before,after_sha256=sha(destination)))
    save(a.out/'sdk-source-overlay.json',dict(source_candidate_only=True,files=source_overlays,global_sdk_published=False))
    overlays=[]
    for source in sorted(a.helpers.rglob('*')):
        if source.is_file() and source.suffix in ('.py','.lua','.json'):
            relative=source.relative_to(a.helpers)
            destination=sdk/'tools'/relative
            destination.parent.mkdir(parents=True,exist_ok=True)
            if destination.exists():destination.unlink()  # Break only the private snapshot link.
            shutil.copyfile(source,destination)
            overlays.append(dict(source=str(source),installed=destination.relative_to(sdk).as_posix(),sha256=sha(source)))
    if (sdk/'test').exists() or (sdk/'origin').exists():
        raise RuntimeError('installed example candidate must have no SDK test/origin tree')
    if a.root_description_fix:
        source=sdk/'xmake.lua';before=source.read_bytes()
        if hashlib.sha256(before).hexdigest()!='5e21718c07fb09e43fb9d25015dad6aedf5609e3bd39564ed02bc11c26ce6918':
            raise RuntimeError('exact shared-root candidate preimage changed; rebase required')
        anchor='                assert(c_modes[c_standard], "Unsupported C source standard")'
        replacement='                if not c_modes[c_standard] then os.raise("Unsupported C source standard") end'
        text=before.decode()
        if text.count(anchor)!=1:raise RuntimeError('exact source-standard guard anchor changed')
        after=text.replace(anchor,replacement).encode()
        source.unlink();source.write_bytes(after)  # Break the private input hardlink first.
        candidate=a.out/'shared-root-candidate';candidate.mkdir()
        (candidate/'xmake.before.lua').write_bytes(before);(candidate/'xmake.after.lua').write_bytes(after)
        (candidate/'description-guard.patch').write_text(''.join(difflib.unified_diff(
            before.decode().splitlines(True),after.decode().splitlines(True),fromfile='a/sdk/xmake.lua',tofile='b/sdk/xmake.lua')))
        save(candidate/'patch-pins.json',dict(path='sdk/xmake.lua',before_sha256=hashlib.sha256(before).hexdigest(),
            after_sha256=hashlib.sha256(after).hexdigest(),applied_only_to_private_candidate=True,
            shared_owner='01',shared_source_modified=False))
    save(a.out/'helper-overlay.json',dict(source_candidate_only=True,files=overlays,
        global_sdk_published=False,shared_sources_modified=False))
    pins=[dict(path=str(path),sha256=sha(path),bytes=path.stat().st_size) for path in
        (a.runtime,a.xmake,a.fixture,Path(__file__))]
    for path in sorted(sdk.rglob('*')):
        if path.is_file():pins.append(dict(path=str(path),sha256=sha(path),bytes=path.stat().st_size))
    save(a.out/'source-pins.json',pins)
    temporary=a.out/'temporary';temporary.mkdir()
    global_directory=a.out/'xmake-global';global_directory.mkdir()
    environment=dict(os.environ,XMAKE_PROGRAM_DIR=str(a.xmake_program),XMAKE_STATS='false',
        XMAKE_TMPDIR=str(temporary),TMPDIR=str(temporary),XMAKE_GLOBALDIR=str(global_directory))
    facts={r['family']:r for r in json.loads(a.operations.read_bytes())['platforms']}
    rows=[]

    def run(name,command,layer,expected=0,data=None,cwd=None):
        folder=a.out/name;folder.mkdir()
        with (folder/'stdout').open('xb') as out,(folder/'stderr').open('xb') as err:
            result=subprocess.run([str(x) for x in command],input=data,stdout=out,stderr=err,
                cwd=cwd or sdk/'example',env=environment,timeout=180)
        row=dict(name=name,layer=layer,command=[str(x) for x in command],exit=result.returncode,
            expected_exit=expected,status='PASS' if result.returncode==expected else 'FAIL',
            stdout_sha256=sha(folder/'stdout'),stderr_sha256=sha(folder/'stderr'))
        rows.append(row);save(a.out/'cases.json',rows)
        print(json.dumps({key:row[key] for key in ('name','layer','exit','status')}),flush=True)
        return row['status']=='PASS',folder

    def configure(tag,platform,language,entry):
        return run(tag+'-configure',[a.xmake,'f','-c','-P',sdk/'example',
            '--ros-sdk-languages=c,cpp','--sdk-toolchain='+str(a.toolchain),
            '--sdk-target=wasm32-wasip1','--sdk-startup=command','--sdk-exceptions=n','--sdk-threads=n',
            '--sdk-offline=y','--sdk-example-platform='+platform,'--sdk-example-language='+language,
            '--sdk-example='+entry,'--ros-sdk-cpp-standard=20'],'XMAKE_CONFIGURATION')[0]

    expected=("Joint positions: ["+', '.join("'"+format(value,'.3f')+"'" for value in
        (0.125,-0.0,1.0/3.0,-1.25,100.0,-100.0,2.345))+"]\n"
        "TCP Position: x=1.250, y=-2.500, z=0.125\n"
        "TCP Orientation: w=0.500, x=0.100, y=-0.200, z=0.300\n"
        "External Force Magnitude: 13.000 N\n---\n").encode()
    for platform in platforms:
        manifest=json.loads((sdk/'example/platform'/platform/'manifest.json').read_bytes())
        profiles=manifest['profiles']
        if a.source_profile:
            profiles=[row for row in profiles if row['id']==a.source_profile]
            if not profiles:raise RuntimeError('the selected actual source profile is not installed')
        if a.quick:
            profiles=[r for r in profiles if platform=='flexiv' and r['id']=='ros2_humble_v1']
        for profile in profiles:
            fixture_inputs=None
            if profile.get('monitor_force_member'):
                include=sdk/'include/platform'/platform/'c'/('ros'+str(profile['ros_version']))/profile['family']
                fixture=a.out/(profile['id']+'-fixture')
                good,_=run(profile['id']+'-fixture-build',['/usr/bin/clang-22','-std=c11','-O2',
                    '-I'+str(sdk/'include'),'-I'+str(include),
                    '-DTASK06_MONITOR_FORCE_MEMBER='+profile['monitor_force_member'],a.fixture,'-o',fixture],
                    'NATIVE_DATA_FIXTURE_BUILD')
                if good:
                    fixture_inputs={}
                    for mode in ('valid','empty-q','nan-unobserved','trailing','square-overflow'):
                        good,log=run(profile['id']+'-fixture-'+mode,[fixture,mode],'NATIVE_DATA_FIXTURE')
                        if not good:raise RuntimeError('fixture failed; no fake transport or native pass substituted')
                        fixture_inputs[mode]=(log/'stdout').read_bytes()
            for language in ('c','cpp'):
                operations=['operations','preload','libraries']
                if profile.get('monitor_force_member'):operations.append('robot-states-monitor-process')
                for operation in operations:
                    entry=operation+'.'+profile['id']
                    tag=platform+'-'+language+'-'+entry
                    if not configure(tag,platform,language,entry):continue
                    output=a.out/(tag+'-output')
                    good,_=run(tag+'-build',[a.xmake,'sdk-example-build','-P',sdk/'example','--output='+str(output)],
                        'XMAKE_ACTUAL_LANGUAGE_BUILD')
                    if not good:continue
                    receipt=json.loads((output/'build-receipt.json').read_bytes())
                    artifacts=receipt['artifacts']
                    for artifact in artifacts:
                        path=Path(artifact['path']);artifact['sha256']=sha(path)
                        run(tag+'-validate-'+artifact['name'],['/usr/bin/wasm-validate','--enable-all',path],
                            'WASM_VALIDATION')
                    if operation=='preload':
                        run(tag+'-guest',[a.xmake,'sdk-example-run','-P',sdk/'example',
                            '--receipt='+str(output/'build-receipt.json'),'--runtime='+str(a.runtime)],'ACTUAL_GUEST_PRELOAD')
                    elif operation=='operations':
                        item=next(t for t in facts[profile['family']]['types'] if t['kind'] in ('service','message'))
                        mode='call' if item['kind']=='service' else 'poll'
                        run(tag+'-denial',[a.runtime,'-Rint','--wasip1-global-noinherit-system-environment',
                            '--run',artifacts[0]['path'],mode,item['type'],'task06-ungranted-operation'],
                            'ACTUAL_GUEST_OPERATION_DEFAULT_DENIAL',13)
                    elif operation=='robot-states-monitor-process' and fixture_inputs is not None:
                        for mode,wanted in (('valid',0),('empty-q',0),('nan-unobserved',22),('trailing',22),('square-overflow',34)):
                            ok,log=run(tag+'-guest-'+mode,[a.runtime,'-Rint','--wasip1-global-noinherit-system-environment',
                                '--run',artifacts[0]['path'],'process','1'],'ACTUAL_GUEST_OFFICIAL_CALLBACK',wanted,
                                fixture_inputs[mode])
                            if mode in ('valid','empty-q') and ok:
                                wanted_stdout=expected if mode=='valid' else expected.replace(expected.split(b'\n',1)[0],b'Joint positions: []',1)
                                rows[-1]['oracle_match']=(log/'stdout').read_bytes()==wanted_stdout
                                if not rows[-1]['oracle_match']:rows[-1]['status']='FAIL'
                                save(a.out/'cases.json',rows)
                        run(tag+'-live-denial',[a.runtime,'-Rint','--wasip1-global-noinherit-system-environment',
                            '--run',artifacts[0]['path'],'live','task06-ungranted-monitor','1'],
                            'ACTUAL_GUEST_MONITOR_DEFAULT_DENIAL',13)
                        run(tag+'-xmake-process',[a.xmake,'sdk-example-run','-P',sdk/'example',
                            '--receipt='+str(output/'build-receipt.json'),'--runtime='+str(a.runtime)],
                            'ACTUAL_GUEST_XMAKE_MONITOR_PROCESS',0,fixture_inputs['valid'])
                    for artifact in artifacts:
                        if sha(Path(artifact['path']))!=artifact['sha256']:raise RuntimeError('built module changed')
    if a.state_readers:
        for platform in platforms:
            manifest=json.loads((sdk/'example/platform'/platform/'manifest.json').read_bytes())
            value=json.loads((sdk/'example/platform'/platform/'state-reader-selections.json').read_bytes())
            selections=value if isinstance(value,list) else value.get('selections',value.get('profiles'))
            for selected in selections:
                source_profile=selected.get('profile',selected.get('id'))
                if a.source_profile and source_profile!=a.source_profile:continue
                if a.quick and (platform!='flexiv' or source_profile!='ros2_humble_v1'):continue
                identifier=selected.get('selection',selected.get('id'))
                if not any(profile['id']==source_profile for profile in manifest['profiles']):
                    raise RuntimeError('old state application lost its exact source domain')
                for language in ('c','cpp'):
                    entry='state-reader.'+identifier;tag=platform+'-'+language+'-'+entry
                    if not configure(tag,platform,language,entry):continue
                    output=a.out/(tag+'-output')
                    good,_=run(tag+'-build',[a.xmake,'sdk-example-build','-P',sdk/'example','--output='+str(output)],
                        'XMAKE_RETAINED_STATE_APPLICATION_BUILD')
                    if not good:continue
                    receipt=json.loads((output/'build-receipt.json').read_bytes())
                    artifact=receipt['artifacts'][0];module=Path(artifact['path']);frozen=sha(module)
                    if len(receipt['artifacts'])!=1 or artifact['name']!='state-reader':raise RuntimeError('actual retained application artifact required')
                    run(tag+'-validate',['/usr/bin/wasm-validate','--enable-all',module],'WASM_VALIDATION')
                    run(tag+'-denial',[a.runtime,'-Rint','--wasip1-global-noinherit-system-environment',
                        '--run',module,'raw','task06-ungranted-state-reader'],'ACTUAL_GUEST_RETAINED_STATE_DEFAULT_DENIAL',13)
                    if sha(module)!=frozen:raise RuntimeError('retained state Wasm artifact changed')
    save(a.out/'summary.json',dict(status='PASS_SCOPED' if rows and all(r['status']=='PASS' for r in rows) else 'FAILED_RETAINED',
        selected_platforms=platforms,selected_source_profile=a.source_profile,quick_profile=a.quick,
        layers={layer:dict(passed=sum(r['layer']==layer and r['status']=='PASS' for r in rows),
            denominator=sum(r['layer']==layer for r in rows)) for layer in sorted({r['layer'] for r in rows})},
        default_authority=[],physical_memory_limit_changed=False,required_virtual_address_mib=49152,
        native_ros_cases=0,gateway_cases=0,hardware_cases=0,upstream_original_processes_executed=0,
        rust_status='SEPARATE_PINNED_PROVIDER_XMAKE_ACCEPTANCE_REQUIRED',
        scope='Actual C/C++20 Wasm application behavior and xmake build closures. Recorded complete native fixture data is not native ROS delivery or full official-node parity.'))
    if not rows or any(r['status']!='PASS' for r in rows):raise SystemExit(1)


if __name__=='__main__':main()
