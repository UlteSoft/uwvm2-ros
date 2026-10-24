#!/usr/bin/env python3
"""Actual xmake configuration/catalog integration, explicitly not a build.

Uses actual root/header/example/helper sources. This sparse source snapshot has
no compiler libraries/provider artifacts and must not be called an installed
SDK/build/Guest qualification. It diagnoses registration/import/catalog failures
before a separately admitted complete-provider build.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess

CASES={
    'elephant':('operations.noetic','state-reader.kinetic_mycobot'),
    'ufactory':('operations.ros2_humble','state-reader.ros1_kinetic_xarm5'),
    'dobot':('operations.ros1_v4','state-reader.ros1_v3'),
    'flexiv':('operations.ros2_humble_v1','state-reader.ros2_humble_v1','robot-states-monitor-process.ros2_humble_v1'),
    'realman':('operations.ros2_humble','state-reader.ros1_main')}


def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def save(path,value):path.write_text(json.dumps(value,indent=2,sort_keys=True)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--helpers',type=Path,required=True)
    p.add_argument('--dobot-module',type=Path,required=True)
    p.add_argument('--engine-candidate',type=Path,
        help='Exact private shared-engine hash-sandbox candidate only; no live shared edit')
    p.add_argument('--xmake',type=Path,required=True)
    p.add_argument('--xmake-program',type=Path,required=True)
    p.add_argument('--toolchain',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    p.add_argument('--rust-catalog',action='store_true',help='Also verify genuine Flexiv Rust application registration through public language ID rs')
    a=p.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD')!='1':raise RuntimeError('common finite owner required')
    a.out.mkdir();sdk=a.out/'sdk-source';shutil.copytree(a.source,sdk)
    shutil.copytree(a.helpers,sdk/'tools')
    shutil.copyfile(a.dobot_module,sdk/'example/platform/dobot/cpp/demo_cycle.lua')
    if a.engine_candidate:
        engine=sdk/'tools/xmake/examples/engine.lua'
        if sha(engine)!='9b196946b369d2a273558bdef567911113cf4446f762f507aaa43cfa7c961ae6' or sha(a.engine_candidate)!='d5354bc7e5107781321ec6982f22c5b9376c71c79ddde2a6d6c1e6681f8bb212':
            raise RuntimeError('shared-engine candidate pre/post image moved; rebase required')
        shutil.copyfile(a.engine_candidate,engine)
        save(a.out/'shared-engine-candidate.json',dict(shared_sources_modified=False,
            before_sha256='9b196946b369d2a273558bdef567911113cf4446f762f507aaa43cfa7c961ae6',after_sha256=sha(engine)))
    temporary=a.out/'temporary';temporary.mkdir();global_directory=a.out/'xmake-global';global_directory.mkdir()
    environment=dict(os.environ,XMAKE_PROGRAM_DIR=str(a.xmake_program),XMAKE_STATS='false',
        XMAKE_TMPDIR=str(temporary),TMPDIR=str(temporary),XMAKE_GLOBALDIR=str(global_directory))
    pins=[dict(path=str(path),sha256=sha(path),bytes=path.stat().st_size) for path in sorted(sdk.rglob('*')) if path.is_file()]
    pins += [dict(path=str(path),sha256=sha(path),bytes=path.stat().st_size) for path in (a.xmake,Path(__file__))]
    save(a.out/'source-pins.json',pins);rows=[]

    def run(name,command,layer):
        folder=a.out/name;folder.mkdir()
        with (folder/'stdout').open('xb') as out,(folder/'stderr').open('xb') as err:
            result=subprocess.run([str(item) for item in command],cwd=sdk/'example',env=environment,stdout=out,stderr=err,timeout=60)
        row=dict(name=name,layer=layer,command=[str(item) for item in command],exit=result.returncode,
            status='PASS' if result.returncode==0 else 'FAIL',stdout_sha256=sha(folder/'stdout'),stderr_sha256=sha(folder/'stderr'))
        rows.append(row);save(a.out/'cases.json',rows)
        print(json.dumps({key:row[key] for key in ('name','exit','status')}),flush=True)
        return result.returncode==0,folder

    for platform,required in CASES.items():
        for language in (('c','cpp','rs') if a.rust_catalog and platform=='flexiv' else ('c','cpp')):
            name=platform+'-'+language
            selected_required=('robot-states-monitor.ros2_humble_v1','robot-states-monitor-process.ros2_humble_v1') if language=='rs' else required
            command=[a.xmake,'f','-c','-P',sdk/'example','--ros-sdk-languages='+('rs' if language=='rs' else 'c,cpp'),
                '--sdk-toolchain='+str(a.toolchain),'--sdk-target=wasm32-wasip1','--sdk-startup=command',
                '--sdk-exceptions=n','--sdk-threads=n','--sdk-offline=y','--ros-sdk-cpp-standard=20',
                '--sdk-example-platform='+platform,'--sdk-example-language='+language,'--sdk-example='+selected_required[0]]
            good,_=run(name+'-configure',command,'ACTUAL_XMAKE_CONFIGURATION')
            if not good:continue
            good,folder=run(name+'-catalog',[a.xmake,'sdk-example-list','-P',sdk/'example'],'ACTUAL_XMAKE_CATALOG')
            if not good:continue
            text=re.sub(r'\x1b\[[0-9;]*m','',(folder/'stdout').read_text())
            ids=re.findall(r'^([a-z][a-z0-9_.-]*)  ',text,re.M)
            expected=set(selected_required)
            if platform=='dobot' and language=='cpp':expected.update(('demo-cycle.ros1_demo_v3','demo-cycle-process.ros1_demo_v4'))
            if not expected.issubset(ids) or len(ids)!=len(set(ids)):
                rows[-1]['status']='FAIL';rows[-1]['catalog_error']='missing actual original selection/application or duplicate ID'
            rows[-1]['catalog_entries']=len(ids);rows[-1]['required_ids']=sorted(expected)
            save(a.out/'cases.json',rows)
    save(a.out/'summary.json',dict(status='PASS_CONFIGURATION_CATALOG_ONLY' if rows and all(row['status']=='PASS' for row in rows) else 'FAILED_RETAINED',
        layers={layer:dict(passed=sum(row['layer']==layer and row['status']=='PASS' for row in rows),
            denominator=sum(row['layer']==layer for row in rows)) for layer in sorted({row['layer'] for row in rows})},
        compiler_cases=0,wasm_cases=0,guest_cases=0,native_ros_cases=0,gateway_cases=0,installed_sdk_acceptance=False,
        default_authority=[],scope='Actual current source root/helper/catalog integration only; no provider artifacts or compiler/runtime fallback'))
    if not rows or any(row['status']!='PASS' for row in rows):raise SystemExit(1)


if __name__=='__main__':main()
