#!/usr/bin/env python3
"""Actual xmake/native or SDK/Wasm execution of DOBOT application data.

The original JSON parser and both original alarm tables are real dependencies.
All service requests/responses and String inputs keep every native field. The
recording envelope is local test transport only. No native import is replaced.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess

RUNTIME_SHA256='719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19'

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def save(path,value):path.write_text(json.dumps(value,indent=2,sort_keys=True)+'\n')
def frame(kind,payload=b''):return kind.encode()+struct.pack('<I',len(payload))+payload
def string(value):
    raw=json.dumps(value,separators=(',',':'),ensure_ascii=False).encode()
    return struct.pack('<I',len(raw))+raw
def move(version,point):
    return (b'\x01' if version==4 else b'')+struct.pack('<6dI',*point,0)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--sdk',type=Path,required=True)
    p.add_argument('--families',type=Path,required=True)
    p.add_argument('--recipe',type=Path,required=True)
    p.add_argument('--xmake',type=Path,required=True)
    p.add_argument('--xmake-program',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    p.add_argument('--wasm',action='store_true',help='Use the installed SDK C++ xmake backend and actual frozen Guest Runtime')
    p.add_argument('--helpers',type=Path)
    p.add_argument('--sdk-overlay',type=Path)
    p.add_argument('--toolchain',type=Path)
    p.add_argument('--runtime',type=Path)
    p.add_argument('--read-only-sdk',action='store_true',
        help='Use an already relocated complete SDK input, redirect all xmake caches to fresh work and verify input hashes after execution')
    a=p.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD')!='1':raise RuntimeError('common finite owner required')
    if a.wasm and (not all((a.toolchain,a.runtime)) or
                   (not a.read_only_sdk and not all((a.helpers,a.sdk_overlay))) or sha(a.runtime)!=RUNTIME_SHA256):
        raise RuntimeError('actual complete SDK/helper/source/toolchain inputs and exact frozen Runtime required')
    if a.read_only_sdk and (not a.wasm or a.helpers or a.sdk_overlay):
        raise RuntimeError('read-only mode requires a pre-frozen complete SDK, with no mutation overlay')
    a.out.mkdir();project=a.out/'xmake-project';project.mkdir()
    shutil.copyfile(a.source,a.out/'application-source.cpp')
    shutil.copyfile(a.recipe,project/'xmake.lua')
    temporary=a.out/'temporary';temporary.mkdir()
    global_directory=a.out/'xmake-global';global_directory.mkdir()
    environment=dict(os.environ,XMAKE_PROGRAM_DIR=str(a.xmake_program),XMAKE_STATS='false',
        XMAKE_TMPDIR=str(temporary),TMPDIR=str(temporary),XMAKE_GLOBALDIR=str(global_directory),
        XMAKE_CONFIGDIR=str(project))
    rows=[];pins=[]
    sdk=a.sdk
    if a.wasm and not a.read_only_sdk:
        sdk=a.out/'installed-sdk'
        # A completed SDK may contain xmake's mutable cache. Never hardlink that
        # cache into a new configuration or let it rewrite historical work.
        shutil.copytree(a.sdk,sdk,copy_function=os.link,
            ignore=lambda directory,names:set(names)&{'.xmake','build'} if Path(directory)==a.sdk else set())
        overlays=[]
        for source in sorted(a.sdk_overlay.rglob('*')):
            if source.is_dir():continue
            relative=source.relative_to(a.sdk_overlay)
            if source.is_symlink() or not source.is_file() or (relative.parts[0] not in ('include','example') and relative.as_posix()!='xmake.lua'):
                raise RuntimeError('confined actual SDK source overlay required')
            target=sdk/relative;target.parent.mkdir(parents=True,exist_ok=True)
            before=sha(target) if target.is_file() else None
            if target.exists():target.unlink()
            shutil.copyfile(source,target)
            overlays.append(dict(path=relative.as_posix(),source=str(source),before_sha256=before,after_sha256=sha(target)))
        for source in sorted(a.helpers.rglob('*')):
            if source.is_symlink():raise RuntimeError('ordinary actual language helper input required')
            if not source.is_file():continue
            relative=source.relative_to(a.helpers);target=sdk/'tools'/relative
            target.parent.mkdir(parents=True,exist_ok=True)
            if target.exists():target.unlink()
            shutil.copyfile(source,target)
        if (sdk/'origin').exists() or (sdk/'test').exists():raise RuntimeError('standalone SDK must contain no origin/test tree')
    if a.wasm:
        if a.read_only_sdk:
            if (not sdk.resolve().is_relative_to(Path(os.environ['UWVM_RESOURCE_RUN']).parent.parent/'06')
                    or '/input/' not in str(sdk.resolve())):
                raise RuntimeError('explicit Task06 relocated SDK input required')
            if (sdk/'.xmake').exists() or (sdk/'origin').exists() or (sdk/'test').exists():
                raise RuntimeError('read-only SDK must have no build cache, origin or test tree')
        if sha(sdk/'example/platform/dobot/cpp/official/demo_cycle.cpp')!=sha(a.source):
            raise RuntimeError('Wasm application must match the exact supplied application source')
        for version in (3,4):
            family='dobot-ros1-demo-v'+str(version)+'-complete'
            for name in ('bounded_records.h','bounded_interfaces.h','operations.h'):
                if sha(sdk/'include/platform/dobot/c/ros1'/family/name)!=sha(a.families/family/name):
                    raise RuntimeError('Wasm must use the genuine separate complete original namespace')
        if not a.read_only_sdk:save(a.out/'sdk-source-overlay.json',overlays)
        pins += [dict(path=str(path),sha256=sha(path),bytes=path.stat().st_size) for path in sorted(sdk.rglob('*')) if path.is_file()]
        pins.append(dict(path=str(a.runtime),sha256=sha(a.runtime),bytes=a.runtime.stat().st_size))
    sources=[a.source,a.recipe,Path(__file__),a.xmake,Path('/usr/bin/clang++-22')]
    sources+=sorted(path for path in (a.source.parent/'support').rglob('*') if path.is_file())
    for backend in ('ros','ros1','ros2'):
        sources+=sorted(path for path in (a.sdk/'include/platform'/backend/'c').rglob('*') if path.is_file())
    for path in sources:pins.append(dict(path=str(path),sha256=sha(path),bytes=path.stat().st_size))

    def run(name,command,expected=0,data=None,wanted=None,stderr_check=None,layer=None):
        layer=layer or ('ACTUAL_GUEST_APPLICATION_DATA' if a.wasm else 'NATIVE_APPLICATION_DATA')
        folder=a.out/name;folder.mkdir()
        with (folder/'stdout').open('xb') as out,(folder/'stderr').open('xb') as err:
            result=subprocess.run([str(item) for item in command],input=data,stdout=out,stderr=err,
                env=environment,cwd=project,timeout=180)
        out=(folder/'stdout').read_bytes();err=(folder/'stderr').read_bytes()
        row=dict(name=name,layer=layer,command=[str(item) for item in command],exit=result.returncode,
            expected_exit=expected,stdout_sha256=hashlib.sha256(out).hexdigest(),stderr_sha256=hashlib.sha256(err).hexdigest(),
            oracle_match=wanted is None or out==wanted,stderr_oracle_match=stderr_check is None or stderr_check(err))
        row['status']='PASS' if result.returncode==expected and row['oracle_match'] and row['stderr_oracle_match'] else 'FAIL'
        rows.append(row);save(a.out/'cases.json',rows)
        print(json.dumps({key:row[key] for key in ('name','layer','exit','status')}),flush=True)
        return row['status']=='PASS'

    A=[0.0]*6;B=[0.5,0.0,0.5,0.0,0.0,0.0]
    run('actual-xmake-version',[a.xmake,'--version'],layer='ACTUAL_TOOL_IDENTITY')
    if a.wasm:
        run('actual-wasi-cpp-version',[a.toolchain/'bin/clang++','--version'],layer='ACTUAL_TOOL_IDENTITY')
    for version in (3,4):
        if a.wasm:
            output=a.out/('wasm-v'+str(version))
            entry='demo-cycle-process.ros1_demo_v'+str(version)
            config=[a.xmake,'f','-c','-P',sdk/'example','--ros-sdk-languages=cpp',
                '--sdk-toolchain='+str(a.toolchain),'--sdk-target=wasm32-wasip1','--sdk-startup=command',
                '--sdk-exceptions=n','--sdk-threads=n','--sdk-offline=y','--ros-sdk-cpp-standard=20',
                '--sdk-example-platform=dobot','--sdk-example-language=cpp','--sdk-example='+entry]
            if not run('v'+str(version)+'-configure',config,layer='XMAKE_WASM_CONFIGURATION'):continue
            if not run('v'+str(version)+'-build',[a.xmake,'sdk-example-build','-P',sdk/'example','--output='+str(output)],
                layer='XMAKE_ACTUAL_CPP_WASM_BUILD'):continue
            receipt=json.loads((output/'build-receipt.json').read_bytes())
            if receipt['status']!='BUILT_NOT_EXECUTED' or len(receipt['artifacts'])!=1:
                raise RuntimeError('one actual retained application artifact required')
            artifact=receipt['artifacts'][0];binary=Path(artifact['path']);binary_sha=sha(binary)
            if artifact['kind']!='wasm' or artifact['sha256']!=binary_sha:
                raise RuntimeError('actual Wasm receipt hash mismatch')
            if not run('v'+str(version)+'-validate',['/usr/bin/wasm-validate','--enable-all',binary],layer='WASM_VALIDATION'):continue
            run('v'+str(version)+'-imports',['/usr/bin/wasm-objdump','-x',binary],layer='ACTUAL_WASM_IMPORT_INVENTORY')
            runner=[a.runtime,'-Rint','--wasip1-global-noinherit-system-environment','--ros1-global-disable','--ros2-global-disable','--run',binary]
            run('v'+str(version)+'-live-default-denial',runner+['live','1','1',
                'task06-ungranted-feed','task06-ungranted-move','task06-ungranted-error','task06-ungranted-clock',
                'task06-ungranted-continue' if version==3 else '-',*[str(value) for value in A+B]],
                13,layer='ACTUAL_GUEST_APPLICATION_DEFAULT_DENIAL')
        else:
            generated=a.out/('generated-v'+str(version));generated.mkdir()
            pieces=['/* Exact original bytes; generated only under the common Linux owner. */\n']
            receipt=json.loads((a.source.parent/'support/application-inputs.json').read_bytes())
            for role in ('servo','controller'):
                original='rosdemo_v'+str(version)+'/alarmFile/alarm_'+role+'.json'
                fact=next(row for row in receipt['files'] if row['path']==original)
                source=a.source.parent/'support/alarms'/('v'+str(version))/('alarm_'+role+'.json')
                raw=source.read_bytes()
                if len(raw)!=fact['bytes'] or hashlib.sha256(raw).hexdigest()!=fact['sha256']:raise RuntimeError('original alarm input pin')
                pieces+=['static unsigned char const uwvm_dobot_demo_'+role+'_json[]={\n']
                for start in range(0,len(raw),1024):pieces.append(','.join(str(byte) for byte in raw[start:start+1024])+',\n')
                pieces+=['};\n']
            (generated/'dobot_demo_alarm_data.h').write_text(''.join(pieces))
            pins.append(dict(path=str(generated/'dobot_demo_alarm_data.h'),sha256=sha(generated/'dobot_demo_alarm_data.h')))
            family=a.families/('dobot-ros1-demo-v'+str(version)+'-complete')
            for name in ('bounded_records.h','bounded_interfaces.h','operations.h'):
                pins.append(dict(path=str(family/name),sha256=sha(family/name)))
            output=a.out/('native-v'+str(version));output.mkdir()
            config=[a.xmake,'f','-c','-P',project,'--task06-version='+str(version),'--task06-source='+str(a.source),
                '--task06-sdk='+str(a.sdk),'--task06-family='+str(family),'--task06-generated='+str(generated),'--task06-output='+str(output)]
            if not run('v'+str(version)+'-configure',config,layer='XMAKE_NATIVE_DATA_CONFIGURATION'):continue
            if not run('v'+str(version)+'-build',[a.xmake,'-P',project,'-j1','-vD','task06_dobot_data'],layer='XMAKE_NATIVE_DATA_BUILD'):continue
            binary=output/'task06_dobot_data';binary_sha=sha(binary);runner=[binary]
        command=runner+['process','128','1',*[str(value) for value in A+B]]
        count_index=len(runner)+1
        first=frame('M',move(version,A));second=frame('M',move(version,B))
        if version==3:
            trace=frame('M',struct.pack('<i',7))+frame('F',string(dict(EnableStatus=1,ErrorStatus=0,RunQueuedCmd=1,QactualVec=[0.001,0,0,0,0,0],unobserved={'kept':[1,2]})))
            trace+=frame('F',string(dict(QactualVec=[0.000999,0,0,0,0,0])))+frame('M',struct.pack('<i',8))+frame('F',string(dict(QactualVec=B)))
        else:
            trace=frame('F',string(dict(EnableStatus=1,ErrorStatus=0,robot_mode=4,currentCommandId=0,unobserved={'kept':[1,2]})))
            trace+=frame('M',struct.pack('<i',11))+frame('F',string(dict(robot_mode=5,currentCommandId=10)))
            trace+=frame('F',string(dict(robot_mode=4,currentCommandId=11)))+frame('F',string(dict(robot_mode=5,currentCommandId=11)))
            trace+=frame('M',struct.pack('<i',12))+frame('F',string(dict(robot_mode=5,currentCommandId=12)))
        run('v'+str(version)+'-full-A-B-cycle',command,data=trace,wanted=first+second)
        if a.wasm:
            run('v'+str(version)+'-xmake-process',[a.xmake,'sdk-example-run','-P',sdk/'example',
                '--receipt='+str(output/'build-receipt.json'),'--runtime='+str(a.runtime)],
                data=trace,wanted=b'Finite virtual-address prerequisite: 32768 MiB; physical/RSS limits are separate\n'+first+second,
                layer='ACTUAL_GUEST_XMAKE_APPLICATION')
        limited=command.copy()
        if version==3:
            limited[count_index]='2'
            boundary=frame('M',struct.pack('<i',7))+frame('F',string(dict(QactualVec=[0.001,0,0,0,0,0])))
            run('v3-strict-less-than-arrival-boundary',limited,11,boundary,first)
        else:
            limited[count_index]='3'
            boundary=frame('M',struct.pack('<i',11))+frame('F',string(dict(robot_mode=4,currentCommandId=11)))
            boundary+=frame('F',string(dict(robot_mode=5,currentCommandId=10)))
            run('v4-equal-running-and-older-id-not-complete',limited,11,boundary,first)
            limited[count_index]='2'
            boundary=frame('M',struct.pack('<i',11))+frame('F',string(dict(robot_mode=0,currentCommandId=12)))
            run('v4-newer-id-completes-independent-of-mode',limited,11,boundary,first+second)
        run('v'+str(version)+'-int32-response-not-int64',command,22,frame('M',struct.pack('<q',11)),first)
        run('v'+str(version)+'-unsolicited-response',command,22,frame('E',b''),first)
        bad=struct.pack('<I',1)+b'{'
        run('v'+str(version)+'-invalid-JSON',command,22,frame('F',bad),first)
        run('v'+str(version)+'-whole-String-trailing',command,22,frame('F',string({})+b'\0'),first)
        run('v'+str(version)+'-backwards-clock',command,22,frame('T',struct.pack('<Q',6))+frame('T',struct.pack('<Q',5)),first)
        run('v'+str(version)+'-local-cancel',command,125,frame('X'),first,
            stderr_check=lambda value:b'physical stop' not in value)
        run('v'+str(version)+'-oversize-complete-String',command,90,frame('F',struct.pack('<I',65537)+b' '*65537),first)
        run('v'+str(version)+'-invalid-point',command[:-1]+['nan'],22,b'',b'')
        if version==3:
            feed=frame('F',string(dict(EnableStatus=1,ErrorStatus=0,RunQueuedCmd=0,QactualVec=[9]*6)))
            trace=feed+frame('T',struct.pack('<Q',0))+frame('C',struct.pack('<i',42))+frame('T',struct.pack('<Q',4999999999))
            trace+=frame('T',struct.pack('<Q',5000000000))+frame('C',struct.pack('<i',43))
            limited=command.copy();limited[count_index]='6'
            run('v3-original-Continue-and-five-second-gate',limited,11,trace,first+frame('C')+frame('C'),
                stderr_check=lambda value:b'continue_response.res=42' in value and b'continue_response.res=43' in value)
            run('v3-short-QactualVec',command,22,frame('F',string(dict(QactualVec=[0]*5))),first)
        else:
            table=json.loads((a.source.parent/'support/alarms/v4/alarm_controller.json').read_bytes())
            alarm=int(table[0]['id']);other=int(table[1]['id'])
            feed=frame('F',string(dict(EnableStatus=0,ErrorStatus=1,robot_mode=4,currentCommandId=0)))
            response=struct.pack('<Iiii',2,alarm,other,-4)
            trace=feed+frame('T',struct.pack('<Q',0))+frame('E',response)
            trace+=frame('T',struct.pack('<Q',5000000000))+frame('E',struct.pack('<Iiii',2,other,alarm,-4))
            trace+=frame('T',struct.pack('<Q',10000000000))+frame('E',struct.pack('<Iiiii',3,alarm,other,alarm,-4))
            limited=command.copy();limited[count_index]='7'
            run('v4-original-error-multiset-suppression',limited,11,trace,first+frame('E')+frame('E')+frame('E'),
                stderr_check=lambda value:value.count(b'Control ErrorID : ')==5 and value.count(b'get_error_response.res=-4')==2)
        if sha(binary)!=binary_sha:raise RuntimeError('retained actual binary changed')
        pins.append(dict(path=str(binary),sha256=binary_sha))
    save(a.out/'source-pins.json',pins)
    if a.read_only_sdk:
        expected_inputs={row['path'] for row in pins if Path(row['path']).is_relative_to(sdk)}
        actual_inputs={str(path) for path in sdk.rglob('*') if path.is_file()}
        if actual_inputs!=expected_inputs:raise RuntimeError('read-only SDK file set changed during execution')
        for row in pins:
            path=Path(row['path'])
            if path.is_relative_to(sdk) and sha(path)!=row['sha256']:
                raise RuntimeError('read-only SDK source/library changed during execution')
        if (sdk/'.xmake').exists():raise RuntimeError('xmake wrote a cache into the read-only SDK input')
    save(a.out/'summary.json',dict(status=('PASS_GUEST_APPLICATION_DATA' if a.wasm else 'PASS_NATIVE_DATA_ONLY') if rows and all(row['status']=='PASS' for row in rows) else 'FAILED_RETAINED',
        layers={layer:dict(passed=sum(row['layer']==layer and row['status']=='PASS' for row in rows),
            denominator=sum(row['layer']==layer for row in rows)) for layer in sorted({row['layer'] for row in rows})},
        actual_guest=sum(row['status']=='PASS' and row['layer'].startswith('ACTUAL_GUEST_') for row in rows),
        native_ros=0,gateway=0,hardware=0,upstream_original_processes=0,
        default_authority=[],scope='Actual xmake/C++ application-data execution with original complete alarm/parser dependencies; separate native-data and Wasm Guest layers, no fake imports or native ROS transport qualification'))
    if not rows or any(row['status']!='PASS' for row in rows):raise SystemExit(1)


if __name__=='__main__':main()
