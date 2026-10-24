#!/usr/bin/env python3
"""Native four-thread manager/IPC admission race, observed over real ROS networks."""
import argparse
import json
import os
from pathlib import Path
import signal
import shlex
import subprocess
import time
import run_network as network


class RaceRunner(network.Runner):
    def dual_revoke(self,revoked):
        name=f'dual-wasm-ros{revoked}-revoke-independent'
        directory=self.run/name;directory.mkdir(mode=0o700)
        guest=directory/'guest.wasm'
        self.snapshot(self.source/f'examples/ros_dual/revoke_{revoked}_independent.wasm',guest,name+'/guest')
        cli=[];policies={}
        for version in (1,2):
            connection,grants=network.policy(version,['publish']);cap=grants[0];cap['resource']='/uwvm_ros_test/revoke_race'
            policies[version]=(connection,cap)
            cli += [f'--ros{version}-global-enable']
            for key,value in {**connection,'gateway-socket':str(directory/f'ros{version}.sock'),
                    'gateway-uid':str(os.getuid()),'connect-timeout-ms':'1000','audit-deny-max-rate-hz':'100',
                    'audit-deny-burst':'100','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}.items():
                cli += [f'--ros{version}-global-'+key,value]
            cli += [f'--ros{version}-global-grant',*[cap[key] for key in ('alias','operation','resource','type','identity')]]
            for key,value in cap['limits'].items():cli += [f'--ros{version}-global-set','pub',key,value]
        command=[self.product,*cli,'--wasip1-global-noinherit-system-environment','--wasip1-global-grant','monotonic-clock','--wasm-set-main-module-name','main','--run',str(guest)]
        (directory/'uwvm-argv.json').write_text(json.dumps(command,indent=2)+'\n')
        worker=self.start(['/bin/bash','-c','kill -STOP $$; exec "$@"','uwvm-dual-revoke',*command],name+'-uwvm')
        gateways=[];nodes=[]
        try:
            for version,(connection,cap) in policies.items():
                native_dir=directory/f'ros{version}';native_dir.mkdir()
                lines=[f'backend ros{version}',f'worker-uid {os.getuid()}',f'worker-pid {worker.pid}',
                    f'socket /uwvm-ros-test/{name}/ros{version}.sock','timeout-ms 10000','max-sessions 16',
                    'audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
                lines += [f'connection {key} {value}' for key,value in connection.items()]
                lines.append('grant '+' '.join(cap[key] for key in ('alias','operation','resource','type','identity')))
                lines += [f"set {cap['alias']} {key} {value}" for key,value in cap['limits'].items()]
                config=directory/f'ros{version}.txt';config.write_text('\n'.join(lines)+'\n');os.chmod(config,0o600)
                gateway=self.start(self.native_command(version,[f'/tmp/ros{version}-build/uwvm-ros{version}-gateway',
                                  f'/uwvm-ros-test/{name}/ros{version}.txt']),name+f'-gateway{version}');gateways.append(gateway)
                deadline=time.monotonic()+4
                while not (directory/f'ros{version}.sock').exists():
                    if gateway.poll() is not None or time.monotonic()>deadline:raise RuntimeError('FAIL dual revoke gateway startup')
                    time.sleep(.02)
                native=self.start(self.native_command(version,[f'/tmp/ros{version}-build/uwvm-ros{version}-native-probe',
                                  '--observe-revoke-race',f'/uwvm-ros-test/{name}/ros{version}']),name+f'-native{version}');nodes.append(native)
                log=self.run/(name+f'-native{version}.log');deadline=time.monotonic()+5
                while b'READY' not in log.read_bytes():
                    if native.poll() is not None or time.monotonic()>deadline:raise RuntimeError('FAIL dual revoke native readiness')
                    time.sleep(.02)
            os.kill(worker.pid,signal.SIGCONT)
            output=self.run/(name+'-uwvm.log');deadline=time.monotonic()+10
            while b'READY_REVOKE_OBSERVE' not in output.read_bytes():
                if worker.poll() is not None or time.monotonic()>deadline:
                    raise RuntimeError('FAIL dual Wasm revoke observation readiness')
                time.sleep(.02)
            for version in (1,2):(directory/f'ros{version}/revoked.count').write_text(('1' if version==revoked else '3')+'\n')
            for native in nodes:
                if native.wait(timeout=12):raise RuntimeError('FAIL dual revoke actual native count witness')
            if any(gateway.poll() is not None for gateway in gateways):raise RuntimeError('FAIL dual revoke gateway exited before verification')
            if worker.poll() is not None:raise RuntimeError('FAIL dual revoke worker exited before native frozen-count verification')
            code=worker.wait(timeout=12)
            if code:raise RuntimeError(f'FAIL dual Wasm revoke boundary exit{code}')
            self.results.append({'name':name,'status':'PASS','wasm_exit':code,'revoked_backend':revoked,
                'surviving_backend':3-revoked,'retired_handle_error':9,'reopen_revoked_alias_error':125,
                'native_unique_before_revocation':1,'native_unique_revoked_total':1,'native_unique_survivor_total':3,
                'native_post_revoke_count_frozen_samples':30,'gateway_alive_during_observation':True,
                'guest_sha256':self.artifacts[name+'/guest']['sha256'],'product_sha256':self.artifacts['uwvm']['sha256'],
                'gateway_sha256':{str(v):self.artifacts[f'ros{v}/uwvm-ros{v}-gateway']['sha256'] for v in (1,2)}})
            print('PASS '+name,flush=True)
        finally:
            for process in [worker,*nodes,*gateways]:self.stop(process)

    def race(self,version,bridge):
        name=f'ros{version}-native-admission-revoke-race'
        directory=self.run/name;directory.mkdir(mode=0o700)
        connection,grants=network.policy(version,['publish']);cap=grants[0]
        cap['resource']='/uwvm_ros_test/revoke_race'
        cap['limits'].update({'max-rate-hz':'1000','burst':'256','max-queue-bytes':'1048576','max-bytes-per-second':'4096000'})
        cap['limits']['queue-depth' if version==1 else 'qos.depth']='256'
        socket_path=directory/f'ros{version}.sock';config=directory/f'ros{version}.txt'
        command=[str(bridge),str(config),str(socket_path),str(directory)]
        (directory/'bridge-argv.json').write_text(json.dumps(command,indent=2)+'\n')
        worker=self.start(['/bin/bash','-c','kill -STOP $$; exec "$@"','uwvm-native-manager-race',*command],name+'-bridge')
        gateway=native=None
        try:
            lines=[f'backend ros{version}',f'worker-uid {os.getuid()}',f'worker-pid {worker.pid}',
                   f'socket /uwvm-ros-test/{name}/ros{version}.sock','timeout-ms 10000','max-sessions 16',
                   'audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
            lines += [f'connection {key} {value}' for key,value in connection.items()]
            lines.append('grant '+' '.join(cap[key] for key in ('alias','operation','resource','type','identity')))
            lines += [f"set {cap['alias']} {key} {value}" for key,value in cap['limits'].items()]
            config.write_text('\n'.join(lines)+'\n');os.chmod(config,0o600)
            gateway=self.start(self.native_command(version,[f'/tmp/ros{version}-build/uwvm-ros{version}-gateway',
                               f'/uwvm-ros-test/{name}/ros{version}.txt']),name+'-gateway')
            deadline=time.monotonic()+4
            while not socket_path.exists():
                if gateway.poll() is not None or time.monotonic()>deadline:raise RuntimeError('FAIL race gateway startup')
                time.sleep(.02)
            native=self.start(self.native_command(version,[f'/tmp/ros{version}-build/uwvm-ros{version}-native-probe',
                              '--observe-revoke-race',f'/uwvm-ros-test/{name}']),name+'-native')
            log=self.run/(name+'-native.log');deadline=time.monotonic()+5
            while b'READY' not in log.read_bytes():
                if native.poll() is not None or time.monotonic()>deadline:raise RuntimeError('FAIL race native readiness')
                time.sleep(.02)
            os.kill(worker.pid,signal.SIGCONT)
            deadline=time.monotonic()+15
            while not (directory/'bridge.json').exists():
                if worker.poll() is not None or time.monotonic()>deadline:
                    raise RuntimeError('FAIL native manager/IPC race bridge readiness')
                time.sleep(.02)
            if native.wait(timeout=15):raise RuntimeError('FAIL actual native revoke race observer')
            if gateway.poll() is not None:raise RuntimeError('FAIL race gateway exited before native count verification')
            if worker.poll() is not None:raise RuntimeError('FAIL race bridge exited before native count verification')
            text=log.read_text(errors='replace')
            if 'native-count-frozen=30 samples' not in text:raise RuntimeError('FAIL missing native frozen-count evidence')
            (directory/'observation.done').write_text('actual native count verified\n')
            code=worker.wait(timeout=5)
            if code:raise RuntimeError(f'FAIL native manager/IPC race bridge exit{code}')
            row=json.loads((directory/'bridge.json').read_text())
            row.update({'name':name,'status':'PASS','concurrency':'native C++ threads; no Wasm thread claim',
                'native_received_unique_messages':row['admitted'],'native_post_revoke_count_frozen_samples':30,
                'gateway_alive_during_observation':True,
                'bridge_sha256':self.artifacts['native-revoke-bridge']['sha256'],
                'gateway_sha256':self.artifacts[f'ros{version}/uwvm-ros{version}-gateway']['sha256'],
                'observer_sha256':self.artifacts[f'ros{version}/uwvm-ros{version}-native-probe']['sha256']})
            self.results.append(row);print('PASS '+name,flush=True)
        finally:
            for process in (worker,native,gateway):
                if process:self.stop(process)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace',default='/tmp/uwvm-ros-extension-20261012')
    parser.add_argument('--source',default='/tmp/uwvm-ros-extension-20261012/source')
    parser.add_argument('--uwvm',default='/tmp/uwvm-ros-extension-20261012/product-final-v6-event/dual/uwvm2-ros')
    parser.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''))
    parser.add_argument('--version',choices=['1','2','both'],default='both')
    parser.add_argument('--mode',choices=['native','dual','all'],default='all')
    parser.add_argument('--cxx',default=os.environ.get('CXX','clang++'))
    args=parser.parse_args()
    if args.mode=='dual' and args.version!='both':
        parser.error('--mode dual requires --version both')
    if os.name!='posix' or not Path('/proc/self/cgroup').exists():
        print('BLOCKED Linux cgroup v2 and prepared native ROS gateways are required',flush=True);return 77
    runner=RaceRunner(args)
    runner.run=runner.base/'revoke-race'/time.strftime('%Y%m%d-%H%M%S');runner.run.mkdir(parents=True,mode=0o700)
    before={};after={};status='PASS'
    def interrupted(signum,frame):raise RuntimeError(f'FAIL interrupted by signal {signum}; bounded cleanup follows')
    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        before=network.cgroup_snapshot();versions=(1,2) if args.version=='both' else (int(args.version),)
        bridge=runner.run/'revoke-bridge'
        compiler=[args.cxx] if Path(args.cxx).is_file() else shlex.split(args.cxx)
        if not compiler:raise RuntimeError('BLOCKED C++ compiler command is empty')
        command=[*compiler,'-std=c++26','-stdlib=libc++','-O1','-g0','-Wall','-Wextra','-Werror','-pthread',
                 '-I'+str(runner.source/'src'),'-I'+str(runner.source/'third-parties/fast_io/include'),str(runner.source/'test/0073.ros-tests/support/source/ros_ingress/revoke_bridge.cpp'),'-o',str(bridge)]
        (runner.run/'compile-argv.json').write_text(json.dumps(command,indent=2)+'\n')
        compiled=subprocess.run(command,env=runner.env,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=60)
        (runner.run/'compile.log').write_bytes(compiled.stdout+compiled.stderr)
        if compiled.returncode:raise RuntimeError('FAIL native C++26 manager IPC race bridge compile')
        runner.prepare(versions)
        runner.snapshot(bridge,runner.run/'snapshots/revoke-bridge','native-revoke-bridge');bridge=runner.run/'snapshots/revoke-bridge'
        if args.mode in ('native','all'):
            for version in versions:runner.race(version,bridge)
        if args.mode in ('dual','all') and args.version=='both':
            for revoked in (1,2):runner.dual_revoke(revoked)
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        status='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL';runner.results.append({'status':status,'reason':str(error)})
        print(status,error,flush=True)
    finally:
        try:runner.cleanup()
        except (RuntimeError,OSError,subprocess.SubprocessError) as error:
            status='FAIL';runner.results.append({'status':'FAIL','reason':'cleanup failure '+str(error)})
    try:after=network.cgroup_snapshot()
    except RuntimeError as error:status='FAIL';runner.results.append({'status':'FAIL','reason':str(error)})
    def events(value):return dict(line.split() for line in value.get('memory.events','').splitlines())
    if before and after and any(events(before).get(key)!=events(after).get(key) for key in ('oom','oom_kill')):
        status='FAIL';runner.results.append({'status':'FAIL','reason':'OOM counters increased'})
    (runner.run/'report.json').write_text(json.dumps({'status':status,'artifacts':runner.artifacts,'results':runner.results,
        'cgroup_before':before,'cgroup_after':after},indent=2)+'\n')
    print(status,runner.run,flush=True);return 0 if status=='PASS' else 77 if status=='BLOCKED' else 1

if __name__=='__main__':raise SystemExit(main())
