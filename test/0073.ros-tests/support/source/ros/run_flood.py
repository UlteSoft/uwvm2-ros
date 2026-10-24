#!/usr/bin/env python3
"""Actual malicious-Wasm Service/Graph/Log quotas with native witnesses."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import signal
import time
import run_network as network
from run_rpc import grant

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace',required=True);p.add_argument('--source',default=str(Path(__file__).resolve().parents[5]));p.add_argument('--uwvm',required=True)
    p.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''));p.add_argument('--wat2wasm',default='wat2wasm');args=p.parse_args()
    runner=network.Runner(args);original=network.policy;report={'status':'FAIL','workload':{'requests':100,'concurrency':1,'rate_hz':.001,'burst':2,'expected_allowed':2,'expected_denied':98,'expected_error':11},'results':runner.results}
    def terminated(signum,frame):
        raise RuntimeError('FAIL bounded flood runner interrupted')
    signal.signal(signal.SIGTERM,terminated)
    try:
        report['cgroup_before']=network.cgroup_snapshot();runner.prepare({1,2})
        for version in (1,2):
            for kind in ('service','graph','log'):
                name=f'ros{version}-{kind}-flood';node=None
                if kind=='service':
                    _,caps=original(version,['service-client-trigger']);cap=caps[0];cap['alias']='cap'
                    binary=f'/tmp/ros{version}-build/uwvm-ros{version}-ipc-service-probe'
                    node=runner.start(runner.native_command(version,[binary,'--observe-flood',cap['resource']]),name+'-native')
                else:
                    cap=grant(version,'graph.list-nodes' if kind=='graph' else 'log.write')
                    if kind=='graph':cap['resource']='/'
                    if kind=='graph':node=runner.start(runner.native_command(version,[f'/tmp/ros{version}-build/uwvm-ros{version}-resources-probe','--witness']),name+'-native')
                if node:
                    for _ in range(200):
                        if node.poll() is not None:raise RuntimeError('FAIL native flood witness startup')
                        if b'READY' in (runner.run/(name+'-native.log')).read_bytes():break
                        time.sleep(.025)
                    else:raise RuntimeError('FAIL native flood witness readiness')
                cap['limits'].update({'max-rate-hz':'0.001','burst':'2','max-bytes-per-second':'1048576'})
                def configured(v,role,message_type):
                    connection,_=original(v,[]);return connection,[cap]
                network.policy=configured
                wasm=runner.source/f'examples/ros{version}/{kind}_flood.wasm'
                subprocess.run([args.wat2wasm,'--enable-all',str(wasm.with_suffix('.wat')),'-o',str(wasm)],check=True,timeout=15)
                try:
                    runner.case(name,str(wasm),{version:[]},[])
                    if kind=='service':
                        if node.wait(timeout=12):raise RuntimeError('FAIL native service count differs from2')
                        effect='actual generated Trigger callbacks exactly2; denied98 never reached native server'
                    elif kind=='log':
                        log=(runner.run/(name+f'-gateway{version}.log')).read_text()
                        if log.count('allow_flood')!=2:raise RuntimeError('FAIL actual native logger did not emit exactly2 allowed messages')
                        effect='actual native logger emitted exactly2 allow_flood records for100 Guest calls'
                    else:
                        effect='2 authorized graph results;98 EAGAIN responses preserved both output sentinels; read-only actual native graph witness alive'
                        if node.poll() is not None:raise RuntimeError('FAIL native graph witness died during flood')
                    runner.results[-1].update(requests=100,allowed=2,denied=98,expected_error=11,native_effect=effect)
                    print('PASS native witness',name,flush=True)
                finally:
                    network.policy=original
                    if node:runner.stop(node)
        report['status']='PASS'
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        report.update(status='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL',error=str(error));print(report['status'],error,flush=True)
    finally:
        network.policy=original;runner.cleanup();report.update(artifacts=runner.artifacts,cgroup_after=network.cgroup_snapshot())
        def events(snapshot):return dict(line.split() for line in snapshot['memory.events'].splitlines())
        if 'cgroup_before' in report and any(events(report['cgroup_before']).get(k)!=events(report['cgroup_after']).get(k) for k in ('oom','oom_kill')):report.update(status='FAIL',error='shared target OOM counter increased')
        (runner.run/'flood.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],runner.run/'flood.json',flush=True)
    return 0 if report['status']=='PASS' else 77 if report['status']=='BLOCKED' else 1

if __name__=='__main__':raise SystemExit(main())
