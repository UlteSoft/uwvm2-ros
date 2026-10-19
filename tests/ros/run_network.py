#!/usr/bin/env python3
"""Real ROS network witness runner. Run in the existing bounded Linux worker.
No mocks are used; missing executables, ROS rootfs or cgroup are BLOCKED.
"""
import argparse
import hashlib
import shutil
import json
import os
from pathlib import Path
import signal
import subprocess
import time

ROS2_STRING='RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18'

def service_qos():
    """Explicit test fixture profile; missing runtime grant fields never default."""
    return {'qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last',
            'qos.depth':'10','qos.deadline':'infinite','qos.lifespan':'infinite',
            'qos.liveliness':'automatic','qos.liveliness-lease-duration':'infinite'}

def checked(command, **kw):
    return subprocess.run(command, check=True, timeout=kw.pop('timeout',60), **kw)

def cgroup_snapshot():
    lines=Path('/proc/self/cgroup').read_text().splitlines()
    path=next((line[3:] for line in lines if line.startswith('0::')),None)
    if not path:
        raise RuntimeError('BLOCKED cgroup v2 is required for this bounded test runner')
    current=Path('/sys/fs/cgroup') / path.lstrip('/')
    limits=[]
    probe=current
    while True:
        file=probe/'memory.max'
        if file.exists():
            value=file.read_text().strip()
            if value!='max': limits.append(int(value))
        if probe==Path('/sys/fs/cgroup'): break
        probe=probe.parent
    if not limits or min(limits)!=68719476736:
        raise RuntimeError(f'BLOCKED effective cgroup limit must be existing 64 GiB, got {limits}')
    return {'path':path,'limits':limits,**{key:(current/key).read_text().strip() for key in ('memory.current','memory.peak','memory.events') if (current/key).exists()}}

def policy(version,role,message_type="String"):
    connection={'node-name':f'uwvm_ros{version}_wasm_network','namespace':'/uwvm_ros_test'}
    if version==1:
        connection.update({'master-uri':'http://127.0.0.1:11339','advertise-host':'127.0.0.1','transport':'tcpros'})
    else:
        connection.update({'domain-id':'73','rmw-implementation':'rmw_fastrtps_cpp','security-mode':'disabled-test','keystore':'not-applicable','enclave':'not-applicable'})
    grants=[]
    for direction in role:
        if direction.startswith('action-'):
            resource='/uwvm_ros_test/action_'+('native' if direction in ('action-client','action-result','action-flood') else 'guest')
            pairs=[('goal','action.send-goal')] if direction=='action-flood' else [('result','action.receive-result')] if direction=='action-result' else [('goal','action.send-goal'),('feedback','action.receive-feedback'),('result','action.receive-result')] if direction=='action-client' else [('server','action.advertise'),('responder','action.respond')]
            for alias,operation in pairs:
                limits={'fibonacci.max-order':'10','max-active-goals':'2','max-actions':'1','timeout-ms':'5000','max-goal-bytes':'64','max-feedback-bytes':'1024','max-result-bytes':'1024','cancel-scope':'not-applicable','max-rate-hz':'100','burst':'10','max-bytes-per-second':'65536','max-event-queue-depth':'10','max-event-queue-bytes':'10240','overflow-policy':'reject','validator':'none','audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'10','audit-max-records':'10000','audit-max-bytes':'1048576'}
                if version==2:
                    for channel in ('goal-service','cancel-service','result-service','feedback','status'):
                        profile={'reliability':'reliable','durability':'transient-local' if channel=='status' else 'volatile','history':'keep-last','depth':'8','deadline':'infinite','lifespan':'infinite','liveliness':'automatic','liveliness-lease-duration':'infinite'}
                        limits.update({f'qos.{channel}.{key}':value for key,value in profile.items()})
                if direction=='action-flood': limits.update({'max-rate-hz':'0.001','burst':'2'})
                grants.append({'alias':alias,'operation':operation,'resource':resource,'type':'actionlib_tutorials/Fibonacci' if version==1 else 'action_tutorials_interfaces/action/Fibonacci','identity':'f59df5767bf7634684781c92598b2406' if version==1 else 'RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0','limits':limits})
            continue
        if direction.startswith('service-'):
            name='Trigger' if direction in ('service-trigger','service-denied','service-client-trigger') else 'SetBool'
            identity={1:{'Trigger':'937c9679a518e3a18d831e57125ea522','SetBool':'09fb03525b03e7ea1fd3992bafd87e16'},2:{'Trigger':'RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f','SetBool':'RIHS01_abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a'}}[version][name]
            pairs=[('client','service.call')] if direction.startswith('service-client-') else [('server','service.advertise')] if direction=='service-denied' else [('server','service.advertise'),('responder','service.respond')]
            for alias,operation in pairs:
                limits={'timeout-ms':'3000','max-inflight':'1','max-request-bytes':'1024','max-response-bytes':'1024','max-rate-hz':'10','burst':'4','max-bytes-per-second':'65536','validator':'none','audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'10','audit-max-records':'10000','audit-max-bytes':'1048576'}
                if version==2: limits.update(service_qos())
                if alias=='server': limits.update({'max-services':'1','max-event-queue-depth':'4','max-event-queue-bytes':'4096','overflow-policy':'reject'})
                grants.append({'alias':alias,'operation':operation,'resource':'/uwvm_ros_test/service_'+name.lower(),'type':'std_srvs'+('/' if version==1 else '/srv/')+name,'identity':identity,'limits':limits})
            continue
        if direction=='graph-list':
            limits={'allowed-name-prefix':'/uwvm_ros_test','max-graph-result-bytes':'4096','timeout-ms':'2000','max-inflight':'1','max-rate-hz':'100','burst':'10','max-bytes-per-second':'65536','audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'10','audit-max-records':'10000','audit-max-bytes':'1048576'}
            grants.append({'alias':'cap','operation':'graph.list-topics','resource':'/uwvm_ros_test','type':'builtin/graph-list-v1','identity':'builtin:graph-list-v1','limits':limits})
            continue
        bounded_publish=direction=='publish-flood'
        if bounded_publish: direction='publish'
        alias='pub' if direction=='publish' else 'sub'
        limits={'max-message-bytes':'4096','max-rate-hz':'10','burst':'4','max-bytes-per-second':'65536','max-queue-bytes':'16384','overflow-policy':'drop-oldest','validator':'none','audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'10','audit-max-records':'10000','audit-max-bytes':'1048576','max-publishers' if alias=='pub' else 'max-subscribers':'1'}
        if version==1:
            limits.update({'queue-depth':'4','transport':'tcpros'})
            if alias=='pub': limits['latch']='false'
        else:
            limits.update({'max-inflight':'1','qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last','qos.depth':'4','qos.deadline':'infinite','qos.lifespan':'infinite','qos.liveliness':'automatic','qos.liveliness-lease-duration':'infinite'})
        if bounded_publish:
            limits.update({'max-rate-hz':'0.001','burst':'2','max-queue-bytes':'8192'})
            limits.update({'queue-depth':'2','latch':'true'} if version==1 else {'qos.depth':'2','qos.durability':'transient-local'})
        grants.append({'alias':alias,'operation':'topic.'+direction,'resource':'/uwvm_ros_test/'+('output' if alias=='pub' else 'input'),'type':({'String':'std_msgs','Bool':'std_msgs','Twist':'geometry_msgs','Image':'sensor_msgs'}[message_type]) + ('/' if version==1 else '/msg/') + message_type,'identity':{1:{'String':'992ce8a1687cec8c8bd883ec73ca41d1','Bool':'8b94c1b53db61fb6aed406028ad6332a','Twist':'9f195f881246fdfa2798d1d3eebca84a','Image':'060021388200f6f0f447d0fcd9c64743'},2:{'String':ROS2_STRING,'Bool':'RIHS01_feb91e995ff9ebd09c0cb3d2aed18b11077585839fb5db80193b62d74528f6c9','Twist':'RIHS01_9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a','Image':'RIHS01_d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47'}}[version][message_type],'limits':limits})
    return connection,grants

class Runner:
    def __init__(self,args):
        self.args=args
        self.base=Path(args.workspace).resolve()
        self.source=Path(args.source).resolve()
        self.run=self.base/'network'/time.strftime('%Y%m%d-%H%M%S')
        self.run.mkdir(parents=True,mode=0o700)
        os.chmod(self.run,0o700)
        self.roots={1:self.base/'gateway/noetic-rootfs',2:self.base/'gateway/jazzy-rootfs'}
        self.processes=[]
        self.mounts=[]
        self.results=[]
        self.artifacts={}
        self.frozen={}
        self.product=str(Path(args.uwvm).resolve())
        self.env=os.environ.copy()
        self.env.update({'ROS_MASTER_URI':'http://127.0.0.1:11339','ROS_IP':'127.0.0.1','ROS_DOMAIN_ID':'73','RMW_IMPLEMENTATION':'rmw_fastrtps_cpp','ROS_SECURITY_ENABLE':'false'})
        self.env['LD_LIBRARY_PATH']=args.llvm_libs
    def start(self,command,name,env=None):
        log=open(self.run/(name+'.log'),'wb')
        p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=env or self.env,start_new_session=True)
        log.close()
        self.processes.append(p)
        return p
    def native_command(self,version,arguments):
        arguments=list(arguments)
        if arguments and arguments[0] in self.frozen:
            arguments[0]=self.frozen[arguments[0]]
        distro='noetic' if version==1 else 'jazzy'
        return ['chroot',str(self.roots[version]),'/bin/bash','-lc',f'source /opt/ros/{distro}/setup.bash && exec "$@"','uwvm-ros-test',*arguments]
    def snapshot(self,source,destination,label):
        source=Path(source)
        before=source.stat()
        shutil.copy2(source,destination)
        digest=lambda path:hashlib.sha256(Path(path).read_bytes()).hexdigest()
        value=digest(destination)
        after=source.stat()
        if (before.st_size,before.st_mtime_ns)!=(after.st_size,after.st_mtime_ns) or digest(source)!=value:
            raise RuntimeError('FAIL executable changed during immutable run snapshot '+str(source))
        self.artifacts[label]={'source':str(source),'snapshot':str(destination),'sha256':value}
    def prepare(self,versions):
        snapshots=self.run/'snapshots'
        snapshots.mkdir(mode=0o700)
        self.snapshot(self.product,snapshots/'uwvm','uwvm')
        self.product=str(snapshots/'uwvm')
        for version in versions:
            snapshot=snapshots/f'ros{version}'
            snapshot.mkdir(mode=0o700)
            build=self.roots[version]/f'tmp/ros{version}-build'
            for binary in build.glob(f'uwvm-ros{version}-*'):
                if binary.is_file() and os.access(binary,os.X_OK):
                    self.snapshot(binary,snapshot/binary.name,f'ros{version}/'+binary.name)
                    self.frozen[f'/tmp/ros{version}-build/'+binary.name]=f'/uwvm-ros-test/snapshots/ros{version}/'+binary.name
            binary=self.roots[version]/f'tmp/ros{version}-build/uwvm-ros{version}-gateway'
            if not binary.exists(): raise RuntimeError(f'BLOCKED missing real gateway {binary}')
            destination=self.roots[version]/'uwvm-ros-test'
            destination.mkdir(exist_ok=True)
            checked(['mount','--bind',str(self.run),str(destination)])
            self.mounts.append(destination)
        if 1 in versions:
            master=self.start(self.native_command(1,['roscore','-p','11339']),'roscore')
            import socket
            for _ in range(200):
                if master.poll() is not None: raise RuntimeError('FAIL roscore exited')
                try:
                    with socket.create_connection(('127.0.0.1',11339),.1): break
                except OSError: time.sleep(.025)
            else: raise RuntimeError('FAIL ROS1 Master startup timeout')
    def case(self,name,module,roles,witnesses,message_type="String",group=None,preload=None,extra=(),fault=None,feedback_ack=False,pre_resume=None,case_directory=None):
        selected=getattr(self.args,'only',None)
        if selected and name not in selected: return
        directory_name=case_directory or name
        if '/' in directory_name or directory_name in ('.','..'):
            raise RuntimeError('FAIL invalid physical case directory')
        directory=self.run/directory_name
        directory.mkdir(mode=0o700)
        cli=[]
        policies={}
        for version,role in roles.items():
            connection,grants=policy(version,role,message_type)
            policies[version]=(connection,grants)
            prefix=f'--ros{version}-'+('group-' if group else 'global-')
            target=[group] if group else []
            if group: cli += [prefix+'create',group,prefix+'add-module',group,'provider']
            cli += [prefix+'enable',*target]
            for key,value in {**connection,'gateway-socket':str(directory/f'ros{version}.sock'),'gateway-uid':str(os.getuid()),'connect-timeout-ms':'1000','audit-deny-max-rate-hz':'100','audit-deny-burst':'100','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}.items(): cli += [prefix+key,*target,value]
            for grant in grants:
                cli += [prefix+'grant',*target,grant['alias'],grant['operation'],grant['resource'],grant['type'],grant['identity']]
                for key,value in grant['limits'].items(): cli += [prefix+'set',*target,grant['alias'],key,value]
        wasm=self.source/module
        if not wasm.exists(): wasm=self.source/str(module).removeprefix('examples/')
        self.snapshot(wasm,directory/'guest.wasm',name+'/guest')
        wasm=directory/'guest.wasm'
        isolation=() if '--wasip1-global-noinherit-system-environment' in extra else ('--wasip1-global-noinherit-system-environment',)
        # These integration fixtures deliberately time discovery and deadlines.
        # Grant only their monotonic clock; default-deny witnesses use their own
        # runner and receive no implicit WASI grant from this test profile.
        timing=('--wasip1-global-grant','monotonic-clock')
        command=[self.product,*cli,*extra,*isolation,*timing,'--wasm-set-main-module-name','main']
        if preload:
            provider=self.source/preload
            if not provider.exists(): provider=self.source/str(preload).removeprefix('examples/')
            self.snapshot(provider,directory/'provider.wasm',name+'/provider')
            provider=directory/'provider.wasm'
            command += ['--wasm-preload-library',str(provider),'provider']
        command += ['--run',str(wasm)]
        (directory/'uwvm-argv.json').write_text(json.dumps(command,indent=2))
        # The PID survives exec. No Guest identity is claimed in the IPC handshake.
        worker=self.start(['/bin/bash','-c','kill -STOP $$; exec "$@"','uwvm-ros-worker',*command],name+'-uwvm')
        gateways=[]
        nodes=[]
        try:
            for version,(connection,grants) in policies.items():
                lines=[f'backend ros{version}',f'worker-uid {os.getuid()}',f'worker-pid {worker.pid}',f'socket /uwvm-ros-test/{directory_name}/ros{version}.sock','timeout-ms 10000','max-sessions 16','audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
                lines += [f'connection {key} {value}' for key,value in connection.items()]
                for grant in grants:
                    lines.append('grant '+' '.join(grant[key] for key in ('alias','operation','resource','type','identity')))
                    lines += [f"set {grant['alias']} {key} {value}" for key,value in grant['limits'].items()]
                config=directory/f'ros{version}.txt'
                config.write_text('\n'.join(lines)+'\n'); os.chmod(config,0o600)
                gateway=self.start(self.native_command(version,[f'/tmp/ros{version}-build/uwvm-ros{version}-gateway',f'/uwvm-ros-test/{directory_name}/ros{version}.txt']),name+f'-gateway{version}')
                gateways.append(gateway)
                for _ in range(200):
                    if gateway.poll() is not None: raise RuntimeError(f'FAIL ROS{version} gateway startup')
                    if (directory/f'ros{version}.sock').exists(): break
                    time.sleep(.01)
                else: raise RuntimeError(f'FAIL ROS{version} gateway socket timeout')
            for version,mode,resource in witnesses:
                probe='action' if mode in ('--serve-native','--serve-native-ack','--call-guest','--active-native','--flood-native','--deny-native') else 'native'
                arguments=[f'/tmp/ros{version}-build/uwvm-ros{version}-{probe}-probe',mode,resource]
                if mode=='--serve-native-ack':
                    if version!=2 or not feedback_ack:raise RuntimeError('FAIL native feedback ACK requires its ROS2 readiness phase')
                    arguments += [f'/uwvm-ros-test/{directory_name}/action-goal-ready.ack']
                if probe=='native': arguments+=['hello',message_type]
                node=self.start(self.native_command(version,arguments),name+f'-native{version}'+mode)
                nodes.append((version,mode,resource,node,arguments))
                if mode in ('--serve-native-ack','--active-native','--flood-native','--observe-count2','--observe-flood','--deny-native','--deny-service'):
                    deadline=time.monotonic()+5
                    log=self.run/(name+f'-native{version}'+mode+'.log')
                    while time.monotonic()<deadline:
                        if node.poll() is not None: raise RuntimeError('FAIL active Action witness exited')
                        if b'READY' in log.read_bytes(): break
                        time.sleep(.025)
                    else: raise RuntimeError('FAIL active Action witness readiness deadline')
            time.sleep(.3)
            baseline_readiness=pre_resume() if pre_resume is not None else None
            os.kill(worker.pid,signal.SIGCONT)
            readiness=None
            if feedback_ack:
                log=self.run/(name+'-gateway2.log')
                deadline=time.monotonic()+5
                while True:
                    required={b'ros_audit',b'backend=2',('worker_pid='+str(worker.pid)).encode(),
                              b'operation=action.send-goal',b'alias=goal',b'error=0'}
                    matched=[line for line in log.read_bytes().splitlines() if required.issubset(line.split())]
                    if matched:break
                    if worker.poll() is not None or time.monotonic()>=deadline:
                        raise RuntimeError('FAIL actual accepted native goal readiness deadline')
                    time.sleep(.01)
                pending=directory/'.action-goal-ready.pending'
                pending.write_bytes(b'goal-ready\n');os.chmod(pending,0o600)
                ack=directory/'action-goal-ready.ack';pending.replace(ack)
                readiness={'trigger':'actual Gateway action.send-goal success after Jazzy UUID map is installed',
                           'audit':matched[0].decode(),'ack':str(ack),'deadline_seconds':5}
            if fault:
                output=self.run/(name+'-uwvm.log')
                def marker(text):
                    deadline=time.monotonic()+10
                    while time.monotonic()<deadline:
                        if text.encode() in output.read_bytes(): return
                        if worker.poll() is not None: raise RuntimeError('FAIL Guest exited before fault marker '+text)
                        time.sleep(.025)
                    raise RuntimeError('FAIL fault marker deadline '+text)
                marker('READY_DISCONNECT')
                failed_version=next(iter(roles))
                os.killpg(gateways[0].pid,signal.SIGKILL)
                gateways[0].wait(timeout=3)
                (directory/f'ros{failed_version}.sock').unlink(missing_ok=True)
                if fault=='recover':
                    marker('READY_RECOVER')
                    gateway=self.start(self.native_command(failed_version,[f'/tmp/ros{failed_version}-build/uwvm-ros{failed_version}-gateway',f'/uwvm-ros-test/{directory_name}/ros{failed_version}.txt']),name+f'-gateway{failed_version}-restarted')
                    gateways.append(gateway)
                    deadline=time.monotonic()+1
                    while time.monotonic()<deadline and not (directory/f'ros{failed_version}.sock').exists():
                        if gateway.poll() is not None: raise RuntimeError('FAIL recovery gateway startup')
                        time.sleep(.01)
                    if not (directory/f'ros{failed_version}.sock').exists(): raise RuntimeError('FAIL recovery socket deadline')
            code=worker.wait(timeout=20)
            if code: raise RuntimeError(f'FAIL Wasm exit {code}')
            native_witnesses=[]
            for version,mode,resource,node,arguments in nodes:
                # Retain each real independent native outcome. The publisher's
                # existing finite twelve-second loop must also finish; an old
                # case PASS label alone is not a per-process return-code proof.
                if mode not in ('--observe','--observe-count2','--observe-flood','--absent','--absent-service','--absent-subscription','--serve-trigger','--serve-setbool','--call-trigger','--call-setbool','--serve-native','--serve-native-ack','--call-guest','--active-native','--flood-native','--deny-native','--deny-service','--publish'):
                    raise RuntimeError('FAIL unclassified native witness mode')
                native_code=node.wait(timeout=15)
                if native_code: raise RuntimeError('FAIL native network witness')
                log=self.run/(name+f'-native{version}'+mode+'.log')
                contents=log.read_bytes()
                if len(contents)>16*1024*1024 or b'PASS ' not in contents:
                    raise RuntimeError('FAIL bounded native witness terminal log')
                native_witnesses.append({'version':version,'mode':mode,'resource':resource,
                    'pid':node.pid,'returncode':native_code,'arguments':arguments,
                    'log':{'path':str(log),'bytes':len(contents),'sha256':hashlib.sha256(contents).hexdigest()},
                    'scope':'Actual independent process exit and native assertions; unrecorded counters are not invented'})
            result={'name':name,'status':'PASS','wasm_exit':code,'guest_sha256':hashlib.sha256(wasm.read_bytes()).hexdigest(),'product_sha256':self.artifacts['uwvm']['sha256'],'gateway_sha256':{str(v):self.artifacts[f'ros{v}/uwvm-ros{v}-gateway']['sha256'] for v in roles},'versions':list(roles),'witnesses':[list(w) for w in witnesses]}
            result['native_witnesses']=native_witnesses
            if readiness:result['feedback_readiness']=readiness
            if baseline_readiness:result['native_baseline_readiness']=baseline_readiness
            self.results.append(result)
            print('PASS '+name,flush=True)
        finally:
            for p in [worker,*gateways,*[item[3] for item in nodes]]: self.stop(p)
    def stop(self,p):
        # ROS Master can leave descendants after its Python leader exits.
        try: os.killpg(p.pid,signal.SIGTERM)
        except ProcessLookupError:
            p.wait(timeout=3);return
        try: p.wait(timeout=3)
        except subprocess.TimeoutExpired:
            try: os.killpg(p.pid,signal.SIGKILL)
            except ProcessLookupError: pass
            p.wait(timeout=3)
        deadline=time.monotonic()+1
        while time.monotonic()<deadline:
            try: os.killpg(p.pid,0)
            except ProcessLookupError: return
            time.sleep(.025)
        try: os.killpg(p.pid,signal.SIGKILL)
        except ProcessLookupError: pass
    def cleanup(self):
        for p in reversed(self.processes): self.stop(p)
        for mount in reversed(self.mounts):
            for attempt in range(20):
                result=subprocess.run(['umount',str(mount)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=5)
                if not result.returncode: break
                time.sleep(.05)
            else:
                # Detach only this task-owned bind; outstanding file references
                # are released by the bounded process-group cleanup above.
                checked(['umount','--lazy',str(mount)],timeout=5)


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--mode',choices=['ros1','ros2','dual','all','multimodule','services','actions','sdk','security','extra-security','action-flood','publish-flood'],default='all')
    p.add_argument('--only',nargs='+',help='Run only these exact case names; an unknown name is BLOCKED')
    p.add_argument('--workspace',default='/tmp/uwvm-ros-extension-20261012')
    p.add_argument('--source',default='/tmp/uwvm-ros-extension-20261012/source')
    p.add_argument('--uwvm',default='/tmp/uwvm-ros-extension-20261012/product-final-v6-event/dual/uwvm')
    p.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''))
    args=p.parse_args()
    if os.name!='posix' or not Path('/proc/self/cgroup').exists(): print('BLOCKED Linux required'); return 2
    def interrupted(signum, frame):
        raise RuntimeError(f'FAIL interrupted by signal {signum}; bounded cleanup follows')
    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    runner=Runner(args)
    report={'status':'BLOCKED','results':[]}
    try:
        before=cgroup_snapshot();report['cgroup_before']=before
        versions=[1] if args.mode=='ros1' else [2] if args.mode=='ros2' else [1,2]
        runner.prepare(versions)
        if args.mode in ('multimodule','all'):
            for name,module,provider,flags in [('cross-module-proxy','proxy_caller.wasm','proxy_provider.wasm',()),('cross-module-tail-proxy','proxy_tail.wasm','proxy_provider.wasm',('--wasm-feature-enable-tail-call',)),('cross-module-handle','handle_attack.wasm','handle_provider.wasm',())]:
                runner.case(name,'examples/ros_dual/'+module,{1:['publish']},[(1,'--absent','/uwvm_ros_test/output')],group='sensors',preload='examples/ros_dual/'+provider,extra=flags)
        if args.mode in ('ros1','all'):
            runner.case('ros1-wasm-publish','examples/ros1/talker.wasm',{1:['publish']},[(1,'--observe','/uwvm_ros_test/output')])
            runner.case('ros1-wasm-subscribe','examples/ros1/listener.wasm',{1:['subscribe']},[(1,'--publish','/uwvm_ros_test/input')])
        if args.mode in ('ros2','all'):
            runner.case('ros2-wasm-publish','examples/ros2/talker.wasm',{2:['publish']},[(2,'--observe','/uwvm_ros_test/output')])
            runner.case('ros2-wasm-subscribe','examples/ros2/listener.wasm',{2:['subscribe']},[(2,'--publish','/uwvm_ros_test/input')])
        if args.mode in ('dual','all'):
            runner.case('hybrid-ros1-to-ros2','examples/ros_dual/hybrid_1_to_2.wasm',{1:['subscribe'],2:['publish']},[(1,'--publish','/uwvm_ros_test/input'),(2,'--observe','/uwvm_ros_test/output')])
            runner.case('hybrid-ros2-to-ros1','examples/ros_dual/hybrid_2_to_1.wasm',{2:['subscribe'],1:['publish']},[(2,'--publish','/uwvm_ros_test/input'),(1,'--observe','/uwvm_ros_test/output')])
        for version in (1,2):
            if args.mode in ('all','publish-flood'):
                runner.case(f'ros{version}-publish-flood',f'examples/ros{version}/publish_flood.wasm',{version:['publish-flood']},[(version,'--observe-flood','/uwvm_ros_test/output')])
            if args.mode in ('all','action-flood'):
                runner.case(f'ros{version}-action-flood',f'examples/ros{version}/action_flood.wasm',{version:['action-flood']},[(version,'--flood-native','/uwvm_ros_test/action_native')])
            if args.mode in (f'ros{version}','all','sdk'):
                for language in ('c','cpp'):
                    runner.case(f'ros{version}-sdk-'+language,f'examples/ros{version}/talker_{language}.wasm',{version:['publish']},[(version,'--observe','/uwvm_ros_test/output')])
            if args.mode in (f'ros{version}','all','actions'):
                runner.case(f'ros{version}-wasm-action-client',f'examples/ros{version}/action_client.wasm',{version:['action-client']},[(version,'--serve-native-ack' if version==2 else '--serve-native','/uwvm_ros_test/action_native')],feedback_ack=version==2)
                runner.case(f'ros{version}-wasm-action-server',f'examples/ros{version}/action_server.wasm',{version:['action-server']},[(version,'--call-guest','/uwvm_ros_test/action_guest')])
            if args.mode in (f'ros{version}','all','services'):
                for name in ('trigger','setbool'):
                    runner.case(f'ros{version}-wasm-service-server-{name}',f'examples/ros{version}/service_server_{name}.wasm',{version:['service-'+name]},[(version,'--call-'+name,'/uwvm_ros_test/service_'+name)])
                    runner.case(f'ros{version}-wasm-service-client-{name}',f'examples/ros{version}/service_client_{name}.wasm',{version:['service-client-'+name]},[(version,'--serve-'+name,'/uwvm_ros_test/service_'+name)])
        if args.mode in ('security','all'):
            for version in (1,2):
                for fixture,role,mode,resource in [('topic_denied','subscribe','--absent','/uwvm_ros_test/input'),('service_denied','service-denied','--deny-service','/uwvm_ros_test/service_trigger'),('graph_denied','graph-list','--absent','/uwvm_ros_test/forbidden'),('action_denied','action-result','--deny-native','/uwvm_ros_test/action_native'),('handle_security','publish','--absent','/uwvm_ros_test/output'),('memory64_bounds','publish','--absent','/uwvm_ros_test/output'),('memory64_growth','publish','--observe','/uwvm_ros_test/output')]:
                    runner.case(f'ros{version}-'+fixture,f'examples/ros{version}/{fixture}.wasm',{version:[role]},[(version,mode,resource)],extra=('--wasm-feature-enable-memory64',) if fixture.startswith('memory64') else ())
            runner.case('cross-backend-handle','examples/ros_dual/cross_backend_handle.wasm',{1:['publish'],2:['publish']},[(1,'--absent','/uwvm_ros_test/output'),(2,'--absent','/uwvm_ros_test/output')])
            runner.case('gateway-disconnect-independent','examples/ros_dual/disconnect_survive.wasm',{1:['subscribe'],2:['publish']},[(1,'--absent','/uwvm_ros_test/input'),(2,'--observe','/uwvm_ros_test/output')],fault='disconnect')
            runner.case('gateway-disconnect-recovery','examples/ros_dual/disconnect_recover.wasm',{1:['subscribe'],2:['publish']},[(1,'--publish','/uwvm_ros_test/input'),(2,'--observe','/uwvm_ros_test/output')],fault='recover')
            runner.case('gateway2-disconnect-independent','examples/ros_dual/disconnect_2_survive.wasm',{2:['subscribe'],1:['publish']},[(2,'--absent','/uwvm_ros_test/input'),(1,'--observe','/uwvm_ros_test/output')],fault='disconnect')
            runner.case('gateway2-disconnect-recovery','examples/ros_dual/disconnect_2_recover.wasm',{2:['subscribe'],1:['publish']},[(2,'--publish','/uwvm_ros_test/input'),(1,'--observe','/uwvm_ros_test/output')],fault='recover')
        if args.mode in ('extra-security','all'):
            for version in (1,2):
                runner.case(f'ros{version}-subscribe-denied',f'examples/ros{version}/topic_subscribe_denied.wasm',{version:['publish']},[(version,'--absent-subscription','/uwvm_ros_test/output')])
                runner.case(f'ros{version}-cancel-denied',f'examples/ros{version}/action_cancel_denied.wasm',{version:['action-result']},[(version,'--active-native','/uwvm_ros_test/action_native')])
        for kind in ('Bool','Twist','Image'):
            suffix=kind.lower()
            if args.mode in ('ros1','all'):
                runner.case('ros1-wasm-publish-'+suffix,'examples/ros1/talker_'+suffix+'.wasm',{1:['publish']},[(1,'--observe','/uwvm_ros_test/output')],message_type=kind)
                runner.case('ros1-wasm-subscribe-'+suffix,'examples/ros1/listener_'+suffix+'.wasm',{1:['subscribe']},[(1,'--publish','/uwvm_ros_test/input')],message_type=kind)
            if args.mode in ('ros2','all'):
                runner.case('ros2-wasm-publish-'+suffix,'examples/ros2/talker_'+suffix+'.wasm',{2:['publish']},[(2,'--observe','/uwvm_ros_test/output')],message_type=kind)
                runner.case('ros2-wasm-subscribe-'+suffix,'examples/ros2/listener_'+suffix+'.wasm',{2:['subscribe']},[(2,'--publish','/uwvm_ros_test/input')],message_type=kind)
            if args.mode in ('dual','all'):
                runner.case('hybrid-ros1-to-ros2-'+suffix,'examples/ros_dual/hybrid_1_to_2_'+suffix+'.wasm',{1:['subscribe'],2:['publish']},[(1,'--publish','/uwvm_ros_test/input'),(2,'--observe','/uwvm_ros_test/output')],message_type=kind)
                runner.case('hybrid-ros2-to-ros1-'+suffix,'examples/ros_dual/hybrid_2_to_1_'+suffix+'.wasm',{2:['subscribe'],1:['publish']},[(2,'--publish','/uwvm_ros_test/input'),(1,'--observe','/uwvm_ros_test/output')],message_type=kind)
        if args.only:
            missing=set(args.only)-{record['name'] for record in runner.results}
            if missing: raise RuntimeError('BLOCKED unknown or unavailable selected case: '+','.join(sorted(missing)))
        after=cgroup_snapshot();report['cgroup_after']=after
        def events(s): return dict(line.split() for line in s['memory.events'].splitlines())
        if events(after).get('oom_kill')!=events(before).get('oom_kill'): raise RuntimeError('FAIL cgroup OOM kill increased')
        report['status']='PASS'
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        report['error']=str(error);report['status']='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL';print(report['status']+' '+str(error),flush=True)
    finally:
        runner.cleanup();report['results']=runner.results;report['artifacts']=runner.artifacts
        (runner.run/'report.json').write_text(json.dumps(report,indent=2)+'\n')
        print('Report: '+str(runner.run/'report.json'))
    return 0 if report['status']=='PASS' else 2 if report['status']=='BLOCKED' else 1

if __name__=='__main__': raise SystemExit(main())
