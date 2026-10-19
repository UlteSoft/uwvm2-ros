#!/usr/bin/env python3
"""Execute Wasm ROS resource imports with independent native network witnesses."""
import argparse
import json
import hashlib
import os
import re
from pathlib import Path
import signal
import subprocess
import time
import run_network as network
import scalar_parameters as scalar
import typed_parameter_events as typed_events
import array_parameters as arrays

AUDIT={'audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'20','audit-max-records':'10000','audit-max-bytes':'1048576'}
BASE={**AUDIT,'timeout-ms':'2000','max-inflight':'2','max-rate-hz':'100','burst':'20','max-bytes-per-second':'1048576'}
TF_ID={1:'b5764a33bfeb3588febc2682852579b0',2:'RIHS01_0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206'}
LIFECYCLE_ID={'lifecycle.get-state':'RIHS01_800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795','lifecycle.change-state':'RIHS01_356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5'}

def grant(version,operation,alias='cap'):
    cap={'alias':alias,'operation':operation,'resource':'/uwvm_resources/value','type':'builtin/string-v1','identity':'builtin:param-string-v1','limits':dict(BASE)}
    limits=cap['limits']
    if operation.startswith('param.'):
        limits.update({'max-parameter-bytes':'65536','validator':'none'})
        if version==2:
            cap['resource']='/uwvm_resources/native/value'
            limits.update({'parameter-node':'/uwvm_resources/native','parameter-name':'value'})
            if operation in ('param.get','param.set','param.describe','param.list'):
                limits.update(network.service_qos())
        if operation in ('param.list','param.search'):
            limits['allowed-name-prefix']='/uwvm_resources'
        if operation=='param.list':
            cap['resource']='/uwvm_resources' if version==1 else '/uwvm_resources/native'
            if version==2: limits.update({'parameter-name':'not-applicable','list-depth':'2'})
        if operation in ('param.declare','param.undeclare'):
            cap['resource']='/uwvm_ros_test/uwvm_ros2_wasm_network/value'
            limits['parameter-node']='/uwvm_ros_test/uwvm_ros2_wasm_network'
        if operation=='param.subscribe-events':
            cap.update({'type':'rcl_interfaces/msg/ParameterEvent','identity':'RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204'})
            limits.update({'max-subscribers':'1','max-event-fields':'8','max-event-queue-depth':'4','max-event-queue-bytes':'131072','overflow-policy':'drop-oldest',**qos()})
    elif operation.startswith('graph.'):
        cap.update({'resource':'/uwvm_resources','type':'builtin/graph-list-v1','identity':'builtin:graph-list-v1'})
        limits.update({'max-graph-result-bytes':'65536','allowed-name-prefix':'/uwvm_resources'})
        if operation=='graph.inspect-topic':cap['resource']='/uwvm_resources/topic'
        elif operation=='graph.inspect-node':cap['resource']='/uwvm_resources/native'
        elif operation=='graph.inspect-service':cap['resource']='/uwvm_resources/trigger' if version==1 else '/uwvm_resources/native/get_parameters'
        elif operation=='graph.inspect-action':cap['resource']='/uwvm_ros_test/action_native';limits['allowed-name-prefix']='/uwvm_ros_test'
    elif operation.startswith('time.'):
        cap.update({'resource':'/clock','type':'builtin/time-v1','identity':'builtin:time-nanoseconds-v1'})
        limits.pop('timeout-ms');limits['max-response-bytes']='8'
    elif operation=='log.write':
        cap.update({'resource':'/uwvm_ros_test/log','type':'builtin/log-v1','identity':'builtin:log-utf8-v1'})
        limits.pop('timeout-ms');limits.update({'max-message-bytes':'4096','max-log-bytes-per-second':'65536','severity':'info'})
    elif operation.startswith('tf.'):
        cap.update({'resource':'/tf','type':'geometry_msgs/TransformStamped' if version==1 else 'geometry_msgs/msg/TransformStamped','identity':TF_ID[version]})
        limits.update({'source-frame':'base','target-frame':'map','tf.max-translation-abs':'10','tf.quaternion-norm-tolerance':'0.000001','timeout-ms':'1500','max-inflight':'1','burst':'4','max-message-bytes':'4096','max-queue-bytes':'16384','overflow-policy':'drop-oldest','buffer-cache-ms':'1000','validator':'transform-bounds'})
        if version==1:
            limits.update({'queue-depth':'4','transport':'tcpros'})
            if operation=='tf.broadcast':limits['latch']='false'
        else:limits.update(qos())
    elif operation.startswith('lifecycle.'):
        cap.update({'resource':'/uwvm_lifecycle_witness','type':'lifecycle_msgs/srv/GetState' if operation=='lifecycle.get-state' else 'lifecycle_msgs/srv/ChangeState','identity':LIFECYCLE_ID[operation]})
        limits.update({'max-request-bytes':'64','max-response-bytes':'1024'})
        limits.update(network.service_qos())
        if operation=='lifecycle.change-state':limits['allowed-transition']='configure'
    else:raise ValueError(operation)
    return cap

def qos():
    return {'qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last','qos.depth':'32','qos.deadline':'infinite','qos.lifespan':'infinite','qos.liveliness':'automatic','qos.liveliness-lease-duration':'infinite'}

def scalar_grant(version,operation,kind,alias='cap',local=False):
    cap=grant(version,operation,alias)
    cap.update(type='builtin/'+kind+'-v1',identity='builtin:param-'+kind+'-v1')
    cap['limits']['max-parameter-bytes']='64'
    cap['limits']['validator']='finite' if kind=='float64' else 'none'
    bounds=scalar.VALUES[kind]['bounds']
    if bounds is not None:
        cap['limits'].update({'parameter-min-value':bounds[0],'parameter-max-value':bounds[1]})
    if local:
        cap['resource']='/uwvm_ros_test/uwvm_ros2_wasm_network/value'
        cap['limits']['parameter-node']='/uwvm_ros_test/uwvm_ros2_wasm_network'
    return cap

def scalar_grants(version,kind,scenario):
    local=scenario.startswith(('local_','undeclare_'))
    if scenario=='get_bounds':
        cap=scalar_grant(version,'param.get',kind,'get')
        cap['limits']['parameter-min-value']='0'
        return [cap]
    if not local:
        grants=[scalar_grant(version,'param.get',kind,'get'),scalar_grant(version,'param.set',kind,'set')]
        if scenario=='absent_denied':
            for cap in grants:
                cap['resource']='/uwvm_resources/native/scalar_absent_value'
                cap['limits']['parameter-name']='scalar_absent_value'
        return grants
    grants=[scalar_grant(version,'param.declare',kind,'declare',True),
            scalar_grant(version,'param.get',kind,'get',True)]
    if scenario=='local_roundtrip':
        grants += [scalar_grant(version,'param.set',kind,'set',True),
                   scalar_grant(version,'param.undeclare',kind,'undeclare',True)]
    else:
        mismatch={'bool':'float64','int64':'bool','float64':'int64'}[kind] if scenario=='undeclare_wrong_kind' else kind
        cap=scalar_grant(version,'param.undeclare',mismatch,'undeclare',True)
        if scenario=='undeclare_bounds':cap['limits']['parameter-min-value']='0'
        grants += [cap,scalar_grant(version,'param.set' if scenario=='undeclare_bounds' else 'param.undeclare',kind,'set' if scenario=='undeclare_bounds' else 'cleanup',True)]
    return grants

class RpcRunner(network.Runner):
    def compile_arrays(self,versions):
        self.array_compilations=[]
        for version in versions:
            for description in arrays.cases(version):
                name=description['name']
                source=self.source/f'examples/ros{version}'/(name.removeprefix(f'ros{version}-')+'.wat')
                output=source.with_suffix('.wasm');temporary=self.run/(name+'.wasm')
                before=hashlib.sha256(source.read_bytes()).hexdigest()
                command=[self.args.wat2wasm,'--enable-all',str(source),'-o',str(temporary)]
                row={'name':name,'command':command,'source':str(source),'source_sha256':before,'status':'FAIL'}
                self.array_compilations.append(row)
                try:result=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=30)
                except subprocess.TimeoutExpired as error:
                    (self.run/(name+'-fixture-build.log')).write_bytes(error.stdout or b'');raise
                (self.run/(name+'-fixture-build.log')).write_bytes(result.stdout)
                row['returncode']=result.returncode
                if result.returncode or hashlib.sha256(source.read_bytes()).hexdigest()!=before:
                    raise RuntimeError('FAIL array fixture compile/input stability '+name)
                data=temporary.read_bytes();output.write_bytes(data)
                row.update(status='PASS',binary=str(output),binary_sha256=hashlib.sha256(data).hexdigest(),bytes=len(data))
            self.snapshot(self.source/f'examples/ros{version}/array_parameter_profiles.json',
                          self.run/f'array-profiles-ros{version}.json',f'ros{version}/array-profiles')
        (self.run/'array-fixture-compilations.json').write_text(json.dumps(self.array_compilations,indent=2)+'\n')

    def array(self,version,description):
        kind,scenario,name=(description[key] for key in ('kind','scenario','name'))
        grants=arrays.grants(version,kind,scenario,grant)
        native=arrays.native(version,kind,scenario)
        local=not native;node=None;record=None;original=network.policy
        try:
            if native:
                if not (self.roots[version]/native[0].lstrip('/')).exists():
                    raise RuntimeError('BLOCKED missing array native witness '+native[0])
                node=self.start(self.native_command(version,native),name+'-array-witness')
                log=self.run/(name+'-array-witness.log');deadline=time.monotonic()+5
                while b'READY array ' not in log.read_bytes():
                    if node.poll() is not None or time.monotonic()>deadline:
                        raise RuntimeError('FAIL array native witness readiness '+name)
                    time.sleep(.02)
            def exact_policy(v,role,message_type):
                connection,_=original(v,[])
                return connection,grants
            network.policy=exact_policy
            short_directory='array-'+hashlib.sha256(name.encode()).hexdigest()[:12]
            self.case(name,f'examples/ros{version}/'+name.removeprefix(f'ros{version}-')+'.wasm',
                      {version:[]},[],case_directory=short_directory)
            record=self.results.pop()
            if node:
                if node.wait(timeout=35):raise RuntimeError('FAIL native array value/type witness '+name)
                contents=log.read_bytes()
                if b'PASS native array parameter witness' not in contents:
                    raise RuntimeError('FAIL missing native array witness completion '+name)
                observed=scenario in ('roundtrip','empty_roundtrip','create')
                if (b'OBSERVED_ARRAY_SET' if observed else b'NO_UNAUTHORIZED_ARRAY_WRITE') not in contents:
                    raise RuntimeError('FAIL missing native array observation marker '+name)
                record.update(native_witness_exit=0,native_witness_log=str(log),
                              native_witness_log_sha256=hashlib.sha256(contents).hexdigest(),
                              native_witness_artifact=self.artifacts[f'ros{version}/uwvm-ros{version}-resources-probe'])
                if version==2:
                    markers=re.findall(rb'native_set_requests=(\d+) native_get_requests=(\d+)',contents)
                    if not markers:raise RuntimeError('FAIL missing generated array request counts '+name)
                    writes,reads=map(int,markers[-1])
                    if writes!=(1 if observed else 0) or (observed and reads<3):
                        raise RuntimeError('FAIL unexpected generated array setter/getter counts '+name)
                    record.update(native_set_requests=writes,native_get_requests=reads)
            guest=self.artifacts[name+'/guest']
            compiled=next(row for row in self.array_compilations if row['name']==name)
            if guest['sha256']!=compiled['binary_sha256'] or hashlib.sha256(Path(compiled['source']).read_bytes()).hexdigest()!=compiled['source_sha256']:
                raise RuntimeError('FAIL executed array fixture differs from compiler inputs/output '+name)
            record.update(parameter_array_kind=kind,scenario=scenario,native_witness=native,
                          output_sentinel_bytes=4096,fixture_compilation=compiled,
                          validation='Gateway local typed array state re-read through a separate get capability' if local else
                                     'native XMLRPC value/element type sampled; empty array has no intrinsic element kind' if version==1 else
                                     'independent native typed array bits and generated setter/getter request counts',
                          guest_artifact={'path':guest['snapshot'],'source':guest['source'],
                                          'sha256':guest['sha256'],'bytes':Path(guest['snapshot']).stat().st_size})
            if scenario=='absent_denied':
                record['validation']='Guest absent-get2; native generated setter count0 and existing unrelated typed array unchanged'
            self.results.append(record);record=None
            print('PASS array parameter '+name,flush=True)
        except (RuntimeError,OSError,subprocess.SubprocessError) as error:
            if record is not None:
                record.update(status='FAIL',reason=str(error));self.results.append(record)
            raise
        finally:
            network.policy=original
            if node:self.stop(node)

    def compile_typed_events(self):
        self.typed_event_compilations=[]
        for description in typed_events.cases():
            name=description['name']
            source=self.source/'examples/ros2'/(name.removeprefix('ros2-')+'.wat')
            output=source.with_suffix('.wasm'); temporary=self.run/(name+'.wasm')
            before=hashlib.sha256(source.read_bytes()).hexdigest()
            command=[self.args.wat2wasm,'--enable-all',str(source),'-o',str(temporary)]
            row={'name':name,'command':command,'source':str(source),'source_sha256':before,'status':'FAIL'}
            self.typed_event_compilations.append(row)
            try:result=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=30)
            except subprocess.TimeoutExpired as error:
                (self.run/(name+'-fixture-build.log')).write_bytes(error.stdout or b'');raise
            (self.run/(name+'-fixture-build.log')).write_bytes(result.stdout)
            row['returncode']=result.returncode
            if result.returncode or hashlib.sha256(source.read_bytes()).hexdigest()!=before:
                raise RuntimeError('FAIL typed event fixture compile/input stability '+name)
            data=temporary.read_bytes();output.write_bytes(data)
            row.update(status='PASS',binary=str(output),binary_sha256=hashlib.sha256(data).hexdigest(),bytes=len(data))
        (self.run/'typed-event-fixture-compilations.json').write_text(json.dumps(self.typed_event_compilations,indent=2)+'\n')
        self.snapshot(self.source/'examples/ros2/typed_parameter_event_profiles.json',
                      self.run/'typed-event-profiles.json','typed-event-profiles')

    def typed_event(self,description):
        kind,scenario,name=(description[key] for key in ('kind','scenario','name'))
        grants=typed_events.grants(description,scalar_grant)
        denial=scenario in ('permission-denied','no-capability')
        native=['/tmp/ros2-build/uwvm-ros2-resources-probe','--typed-events',kind,
                'deny-subscription' if denial else scenario]
        original=network.policy;node=None
        try:
            node=self.start(self.native_command(2,native),name+'-typed-event-witness')
            log=self.run/(name+'-typed-event-witness.log');deadline=time.monotonic()+5
            while b'READY' not in log.read_bytes():
                if node.poll() is not None or time.monotonic()>deadline:
                    raise RuntimeError('FAIL typed ParameterEvent native witness readiness '+name)
                time.sleep(.02)
            def exact_policy(version,role,message_type):
                connection,_=original(version,[])
                return connection,grants
            network.policy=exact_policy
            def native_baseline():
                deadline=time.monotonic()+8
                while time.monotonic()<deadline:
                    markers=[line for line in log.read_bytes().splitlines() if line.startswith(b'BASELINE_READY ')]
                    if markers:
                        return {'trigger':'actual native/Gateway framework DDS reader GIDs captured before Guest resume',
                                'marker':markers[0].decode(),'deadline_seconds':8}
                    if node.poll() is not None:raise RuntimeError('FAIL typed-event native baseline witness exited '+name)
                    time.sleep(.02)
                raise RuntimeError('FAIL typed-event actual DDS baseline deadline '+name)
            short_directory='typed-event-'+hashlib.sha256(name.encode()).hexdigest()[:12]
            self.case(name,'examples/ros2/'+name.removeprefix('ros2-')+'.wasm',{2:[]},[],pre_resume=native_baseline,case_directory=short_directory)
            record=self.results.pop()
            if node.wait(timeout=15):raise RuntimeError('FAIL typed ParameterEvent native witness '+name)
            marker=b'PASS denied typed ParameterEvent subscription' if denial else b'PASS typed ParameterEvent'
            if marker not in log.read_bytes():raise RuntimeError('FAIL missing actual typed event witness completion '+name)
            guest=self.artifacts[name+'/guest']
            compiled=next(row for row in self.typed_event_compilations if row['name']==name)
            if guest['sha256']!=compiled['binary_sha256'] or hashlib.sha256(Path(compiled['source']).read_bytes()).hexdigest()!=compiled['source_sha256']:
                raise RuntimeError('FAIL executed typed event differs from compiler inputs/output '+name)
            record.update(operation='param.subscribe-events',parameter_event_kind=kind,scenario=scenario,
                          native_witness=native,native_witness_exit=0,native_witness_log=str(log),
                          native_witness_log_sha256=hashlib.sha256(log.read_bytes()).hexdigest(),
                          fixture_compilation=compiled,
                          expected_event_hex=[] if denial else [value.hex() for value in typed_events.expected_events(kind,scenario)],
                          validation='actual native writer observes zero selected subscriptions' if denial else
                                     'official generated native ParameterEvent; exact whole-envelope Wasm comparison; poll11 transactional sentinels',
                          guest_artifact={'path':guest['snapshot'],'source':guest['source'],
                                          'sha256':guest['sha256'],'bytes':Path(guest['snapshot']).stat().st_size})
            self.results.append(record)
            print('PASS typed ParameterEvent '+name,flush=True)
        finally:
            network.policy=original
            if node:self.stop(node)

    def compile_scalars(self,versions):
        self.scalar_compilations=[]
        for version in versions:
            for description in scalar.cases(version):
                name=description['name']
                source=self.source/f'examples/ros{version}'/(name.removeprefix(f'ros{version}-')+'.wat')
                output=source.with_suffix('.wasm')
                temporary=self.run/(name+'.wasm')
                before=hashlib.sha256(source.read_bytes()).hexdigest()
                command=[self.args.wat2wasm,'--enable-all',str(source),'-o',str(temporary)]
                row={'name':name,'command':command,'source':str(source),'source_sha256':before,'status':'FAIL'}
                self.scalar_compilations.append(row)
                try:result=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=30)
                except subprocess.TimeoutExpired as error:
                    (self.run/(name+'-fixture-build.log')).write_bytes(error.stdout or b'')
                    raise
                (self.run/(name+'-fixture-build.log')).write_bytes(result.stdout)
                row['returncode']=result.returncode
                if result.returncode or hashlib.sha256(source.read_bytes()).hexdigest()!=before:
                    raise RuntimeError('FAIL scalar fixture compile/input stability '+name)
                data=temporary.read_bytes()
                output.write_bytes(data)
                row.update(status='PASS',binary=str(output),binary_sha256=hashlib.sha256(data).hexdigest(),bytes=len(data))
        (self.run/'scalar-fixture-compilations.json').write_text(json.dumps(self.scalar_compilations,indent=2)+'\n')

    def scalar(self,version,description):
        kind,scenario,name=(description[key] for key in ('kind','scenario','name'))
        grants=scalar_grants(version,kind,scenario)
        mode={'roundtrip':'--scalar-observe-set','create':'--scalar-observe-create',
              'wrong_kind':'--scalar-wrong-kind','dynamic_denied':'--scalar-dynamic-denied'}.get(scenario,'--scalar-observe-denied')
        local=scenario.startswith(('local_','undeclare_'))
        native=[] if local else [f'/tmp/ros{version}-build/uwvm-ros{version}-resources-probe',mode,kind]
        node=None
        original=network.policy
        try:
            if native:
                if not (self.roots[version]/native[0].lstrip('/')).exists():raise RuntimeError('BLOCKED missing scalar native witness '+native[0])
                node=self.start(self.native_command(version,native),name+'-resource-witness')
                log=self.run/(name+'-resource-witness.log')
                deadline=time.monotonic()+5
                while b'READY' not in log.read_bytes():
                    if node.poll() is not None or time.monotonic()>deadline:raise RuntimeError('FAIL scalar native witness readiness')
                    time.sleep(.02)
            def exact_policy(v,role,message_type):
                connection,_=original(v,[])
                return connection,grants
            network.policy=exact_policy
            self.case(name,f'examples/ros{version}/'+name.removeprefix(f'ros{version}-')+'.wasm',{version:[]},[])
            record=self.results.pop()
            if node and node.wait(timeout=12):raise RuntimeError('FAIL scalar native value/type witness')
            guest=self.artifacts[name+'/guest']
            compiled=next(row for row in self.scalar_compilations if row['name']==name)
            if guest['sha256']!=compiled['binary_sha256'] or hashlib.sha256(Path(compiled['source']).read_bytes()).hexdigest()!=compiled['source_sha256']:
                raise RuntimeError('FAIL executed scalar fixture differs from compiler inputs/output '+name)
            record.update(parameter_kind=kind,scenario=scenario,native_witness=native,
                          validation='Gateway local typed state re-read through a separate get capability' if local else
                                     'independent native parameter value/type witness' if version==1 else
                                     'independent native parameter value/type and generated setter-request count witness',
                          guest_artifact={'path':guest['snapshot'],'source':guest['source'],
                                          'sha256':guest['sha256'],'bytes':Path(guest['snapshot']).stat().st_size},
                          fixture_compilation=compiled)
            if scenario=='absent_denied':
                record['validation']='Guest absent-get error2; independent native setter request count0 and existing unrelated typed value unchanged'
            self.results.append(record)
            print('PASS scalar parameter '+name,flush=True)
        finally:
            network.policy=original
            if node:self.stop(node)

    def rpc(self,version,name,operation,witness_mode=None,denied=False):
        cap=grant(version,operation)
        mode=witness_mode
        native=[]
        if operation=='graph.inspect-action':
            native=['/tmp/ros2-build/uwvm-ros2-action-probe','--serve-native','/uwvm_ros_test/action_native']
        elif operation.startswith(('param.','graph.')):
            mode=mode or '--witness'
            native=[f'/tmp/ros{version}-build/uwvm-ros{version}-resources-probe',mode]
        elif operation.startswith('tf.'):
            native=[f'/tmp/ros{version}-build/uwvm-ros{version}-tf-probe',mode or ('--publish' if operation=='tf.lookup' else '--observe'),'/tf']
        elif operation.startswith('lifecycle.'):
            native=['/tmp/ros2-build/uwvm-ros2-lifecycle-probe',mode or '--serve']
        if denied:
            cap=grant(version,{'param.set':'param.get','tf.broadcast':'tf.lookup','lifecycle.change-state':'lifecycle.get-state'}[operation])
        grants=[cap]
        if operation=='param.undeclare':grants.append(grant(version,'param.declare','setup'))
        if operation in ('param.declare','param.undeclare'):
            verify=grant(version,'param.get','verify')
            verify['resource']='/uwvm_ros_test/uwvm_ros2_wasm_network/value'
            verify['limits']['parameter-node']='/uwvm_ros_test/uwvm_ros2_wasm_network'
            grants.append(verify)
        node=None
        original=network.policy
        try:
            if native:
                if not (self.roots[version]/native[0].lstrip('/')).exists():raise RuntimeError('BLOCKED missing native resource witness '+native[0])
                node=self.start(self.native_command(version,native),name+'-resource-witness')
                log=self.run/(name+'-resource-witness.log')
                for _ in range(200):
                    if node.poll() is not None:raise RuntimeError('FAIL native resource witness exited before Guest')
                    if b'READY' in log.read_bytes():break
                    time.sleep(.02)
                else:raise RuntimeError('FAIL native resource witness readiness timeout')
            def exact_policy(v,role,message_type):
                connection,_=original(v,[])
                return connection,grants
            network.policy=exact_policy
            self.case(name,f'examples/ros{version}/'+name.removeprefix(f'ros{version}-')+'.wasm',{version:[]},[])
            record=self.results.pop()
            if operation=='log.write':
                for _ in range(100):
                    if b'hello' in (self.run/(name+f'-gateway{version}.log')).read_bytes():break
                    time.sleep(.01)
                else:raise RuntimeError('FAIL native logger did not emit Guest canonical text')
            if node and mode in ('--observe-set','--observe-denied','--observe-delete','--observe','--absent','--observe-configure'):
                if node.wait(timeout=12):raise RuntimeError('FAIL native side-effect witness')
            if node:
                # Expose the current independently executed native process.
                # Persistent publishers/servers are observed alive and then
                # retired in finally; their natural terminal exit is absent,
                # rather than fabricated as zero. A counter/absence observer
                # above must have its real successful terminal return code.
                contents=log.read_bytes()
                if len(contents)>16*1024*1024:raise RuntimeError('FAIL bounded native resource witness log')
                ended=node.poll()
                if ended is not None and ended!=0:raise RuntimeError('FAIL native resource witness ended unexpectedly')
                record['native_witnesses']=[{'version':version,'mode':native[1],
                    'pid':node.pid,'returncode':ended,'observed_alive_after_guest':ended is None,
                    'arguments':native,'log':{'path':str(log),'bytes':len(contents),'sha256':hashlib.sha256(contents).hexdigest()},
                    'scope':'Actual native live source or completed counter witness; a live source natural exit is NOT_RECORDED'}]
            record.update(operation=operation,native_witness=native,denied=denied)
            guest=self.artifacts[name+'/guest']
            record['guest_artifact']={'path':guest['snapshot'],'source':guest['source'],
                                      'sha256':guest['sha256'],'bytes':Path(guest['snapshot']).stat().st_size}
            self.results.append(record)
            print('PASS native resource validation '+name,flush=True)
        finally:
            network.policy=original
            if node:self.stop(node)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace',default='/tmp/uwvm-ros-extension-20261012')
    p.add_argument('--source',default='/tmp/uwvm-ros-extension-20261012/source')
    p.add_argument('--uwvm',default='/tmp/uwvm-ros-extension-20261012/product-final-v7-array/dual/uwvm')
    p.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''))
    p.add_argument('--wat2wasm',default='wat2wasm')
    p.add_argument('--version',choices=['1','2','both'],default='both')
    p.add_argument('--mode',choices=['resources','scalars','typed-events','arrays','tf','lifecycle','all'],default='all')
    args=p.parse_args()
    if args.mode in ('lifecycle','typed-events') and args.version=='1':
        p.error('--mode '+args.mode+' requires ROS 2 (--version 2 or both)')
    runner=RpcRunner(args);status='PASS'
    before={}
    runner.run=runner.base/'rpc'/time.strftime('%Y%m%d-%H%M%S');runner.run.mkdir(parents=True,mode=0o700)
    def interrupted(signum,frame):raise RuntimeError(f'FAIL interrupted by signal {signum}; bounded cleanup follows')
    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        before=network.cgroup_snapshot()
        versions=(1,2) if args.version=='both' else (int(args.version),)
        if args.mode in ('resources','scalars','all'):runner.compile_scalars(versions)
        if 2 in versions and args.mode in ('resources','typed-events','all'):runner.compile_typed_events()
        if args.mode in ('arrays','all'):runner.compile_arrays(versions)
        runner.prepare(set(versions))
        if 2 in versions and args.mode in ('resources','typed-events','all'):
            for description in typed_events.cases():runner.typed_event(description)
        if args.mode in ('resources','scalars','all'):
            for version in versions:
                for description in scalar.cases(version):runner.scalar(version,description)
        if args.mode in ('resources','all'):
            for version in versions:
                params=['get','set','list','delete','exists','search'] if version==1 else ['get','set','describe','list','declare','undeclare']
                for op in params:
                    mode='--observe-set' if op=='set' else '--observe-delete' if op=='delete' else None
                    runner.rpc(version,f'ros{version}-parameter_{op}','param.'+op,mode)
                for op in ('list_topics','list_nodes','list_services','inspect_topic','inspect_node','inspect_service'):
                    runner.rpc(version,f'ros{version}-graph_{op}','graph.'+op.replace('_','-'))
                times=['ros','wall'] if version==1 else ['ros','system','steady']
                for op in times:runner.rpc(version,f'ros{version}-time_read_{op}','time.read-'+op)
                runner.rpc(version,f'ros{version}-log_write','log.write')
                runner.rpc(version,f'ros{version}-parameter_set_denied','param.set','--observe-denied',True)
            if 2 in versions:runner.rpc(2,'ros2-graph_inspect_action','graph.inspect-action')
            if 2 in versions:runner.rpc(2,'ros2-parameter_events','param.subscribe-events','--events')
        if args.mode in ('tf','all'):
            for version in versions:
                runner.rpc(version,f'ros{version}-tf_lookup','tf.lookup')
                runner.rpc(version,f'ros{version}-tf_broadcast','tf.broadcast','--observe')
                runner.rpc(version,f'ros{version}-tf_broadcast_denied','tf.broadcast','--absent',True)
        if args.mode in ('lifecycle','all') and 2 in versions:
            runner.rpc(2,'ros2-lifecycle_get_state','lifecycle.get-state')
            runner.rpc(2,'ros2-lifecycle_change_state','lifecycle.change-state','--observe-configure')
            runner.rpc(2,'ros2-lifecycle_change_state_denied','lifecycle.change-state','--observe-denied',True)
        if args.mode in ('arrays','all'):
            for version in versions:
                for description in arrays.cases(version):runner.array(version,description)
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        status='BLOCKED' if 'BLOCKED' in str(error) else 'FAIL'
        runner.results.append({'status':status,'reason':str(error)});print(status,str(error),flush=True)
    finally:
        try:runner.cleanup()
        except (RuntimeError,OSError,subprocess.SubprocessError) as error:
            status='FAIL';runner.results.append({'status':'FAIL','reason':'cleanup failure: '+str(error)})
    try:after=network.cgroup_snapshot()
    except RuntimeError as error:
        after={};status='BLOCKED' if 'BLOCKED' in str(error) else 'FAIL';runner.results.append({'status':status,'reason':str(error)})
    def events(snapshot):return dict(line.split() for line in snapshot.get('memory.events','').splitlines())
    if before and after and any(events(before).get(key)!=events(after).get(key) for key in ('oom','oom_kill')):
        status='FAIL';runner.results.append({'status':'FAIL','reason':'OOM counters increased'})
    report={'status':status,'artifacts':runner.artifacts,'results':runner.results,
            'scalar_fixture_compilations':getattr(runner,'scalar_compilations',[]),
            'typed_event_fixture_compilations':getattr(runner,'typed_event_compilations',[]),
            'array_fixture_compilations':getattr(runner,'array_compilations',[]),
            'cgroup_before':before,'cgroup_after':after}
    (runner.run/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(status,runner.run,flush=True)
    return 0 if status=='PASS' else 77 if status=='BLOCKED' else 1

if __name__=='__main__':raise SystemExit(main())
