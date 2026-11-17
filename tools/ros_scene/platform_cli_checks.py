#!/usr/bin/env python3
"""Exercise actual ROSv2 CLI child refusal and exhausted-quota cleanup in Wasm.

The fixture module is deliberately not registered in production. No physical
provider is selected. Both interpreter/AOT and memory32/memory64 paths execute
the same real guard cleanup and ownership assertions inside the existing gate.
"""
import argparse
import json
from pathlib import Path
import resource
import subprocess
import sys
import time
from device_dispatch_checks import ROOT, GROUP, guard, sha

sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import Guest, policy, grant


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    runtime=args.runtime.resolve();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    build=runtime.parent/'full-build-results.json';proof=json.loads(build.read_text())
    assert proof['complete'] and sha(runtime)==proof['binary_sha256']
    inputs={str(p):sha(p) for p in [runtime,build,Path(__file__),ROOT/'test/0084.ros-runtime/run_scene_vm.py']}
    record=dict(complete=False,scope=__doc__,inputs=inputs,cases=[],cgroup=str(GROUP),
        memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text(),production_platforms_registered=0)
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(name,argv,success=True):
        guard();start=time.monotonic();result=subprocess.run([str(x) for x in argv],capture_output=True,text=True,timeout=60)
        (out/(name+'.stdout')).write_text(result.stdout);(out/(name+'.stderr')).write_text(result.stderr)
        passed=(result.returncode==0) if success else (result.returncode>0)
        record['cases'].append(dict(name=name,argv=[str(x) for x in argv],exit=result.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,result.returncode,result.stderr[-5000:]);print('PASS',name,flush=True);return result
    prefix=[runtime,'--log-color','disable','--wasm-set-main-module-name','main',
            '--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable']
    help_result=run('help',[runtime,'--log-color','disable','--help','rosv2'])
    assert 'platform_extension.bind' in help_result.stdout+help_result.stderr
    child='uwvm_ros_v2.platform_extension.fixture.v1'
    for name,flags in [
        ('missing-parent',['--ros-scene','robot','platform_extension',child,'enable']),
        ('unknown-module',['--ros-scene-create','robot','--ros-scene','robot','platform_extension',child,'enable']),
        ('unknown-disable',['--ros-scene-create','robot','--ros-scene','robot','platform_extension',child,'disable']),
        ('unknown-bind',['--ros-scene-create','robot','--ros-scene','robot','platform_extension.bind',child,'drive']),
        ('no-registration-escape',['--ros-scene-create','robot','--ros-scene','robot','platform_extension.register',child,'enable']),
        ('no-peer-policy',['--platform-extension',child,'enable']),
        ('no-wildcard',['--ros-scene-create','robot','--ros-scene','robot','platform_extension','uwvm_ros_v2.platform_extension.*.v1','enable']),
        ('no-version-alias',['--ros-scene-create','robot','--ros-scene','robot','platform_extension','uwvm_ros_v2.platform_extension.fixture.v01','enable'])]:
        run(name,[*prefix,*flags,'--ros-scene-check'],False)
    operations=[dict(name='guard.create',words=0,bytes=8),dict(name='guard.trigger',words=1,bytes=0),
                dict(name='guard.take',words=1,bytes=8),dict(name='resource.close',words=1,bytes=0)]
    flags=policy(operations)
    for alias in ['signal','other']:
        flags+=grant(alias,'guard',[r['name'] for r in operations])
        for key,value in [('delivery','may-reject'),('calls-per-window','1'),('bytes-per-window','8'),('window-ns','3600000000000')]:
            flags+=['--ros-scene-limit','robot',alias,key,value]
    for width in [32,64]:
        guest=Guest(operations,width,1);guest.check('call $abi',2)
        signal=guest.open('signal');other=guest.open('other');guest.call('guard.create',signal);obj=guest.save('object')
        guest.call('guard.create',signal,expected=11);guest.call('guard.trigger',signal,[obj],expected=11)
        guest.call('guard.take',signal,[obj],expected=11)
        guest.call('resource.close',other,[obj],expected=9)
        guest.call('resource.close',signal,[obj],output=65535,expected=22)
        guest.call('resource.close',signal,[obj]);guest.call('resource.close',signal,[obj],expected=9)
        guest.check(signal+' call $close');signal=guest.open('signal');guest.call('guard.create',signal,expected=11)
        wat=out/f'cleanup-{width}.wat';wasm=wat.with_suffix('.wasm');wat.write_text(guest.render())
        run(f'compile-{width}',['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',wat,'-o',wasm])
        record['inputs'][str(wasm)]=sha(wasm)
        for engine in ['--runtime-int','--runtime-aot']:
            run(f'cleanup-{width}-{engine[2:]}',[*prefix,engine,*flags,'--run',wasm])
    record['inputs_unchanged']=all(sha(p)==h for p,h in record['inputs'].items())
    record['events_after']=(GROUP/'memory.events').read_text();record['complete']=record['inputs_unchanged'];save();assert record['complete']


if __name__=='__main__':main()
