#!/usr/bin/env python3
"""Actual finite Sport Wasm/Runtime/Gateway/generated-rclcpp loopback owner.

Requires a trusted, pinned matching production build closure and a separately
created isolated loopback network namespace. This program neither builds shared
Runtime/Gateway code nor creates network/Guest administrative permissions.
Missing prerequisites fail before actor launch. No native or firmware mock.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import selectors
import signal
import socket
import stat
import struct
import subprocess
import time


SOURCE_PROOF='f5ee4d38f46566593d03bd8f12a75d44303c4b0589c2f072bf6d3da492601169'
TYPES={
    'Request':('RIHS01_60dda80d19538b06c8f79b77222b1b7642dd753b8439a6bd535ff0cba08c5378',
        'UWVMROS1_39bb2426d68c160c07e7cff951d234c8ec9d0c70fddb5d432d9b238d08565eb2'),
    'Response':('RIHS01_5300d019bb652073e7248995c878765c68ab519bcbdbad2e2a17c81abd8888b9',
        'UWVMROS1_c048bd88b9c7d86cde04e6a589c50aeba396be29acf0d58bc6972f2dd8d8b749')}
NONBASIC=('realtime-clock','monotonic-clock','process-clock','thread-clock','random','proc-raise','sched-yield','stdio-full-rights')


def require(value,reason):
    if not value:raise ValueError(reason)


def fact(path,*,kernel_link=False):
    path=Path(path);descriptor=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_CLOEXEC|(0 if kernel_link else os.O_NOFOLLOW))
    try:
        before=os.fstat(descriptor);require(stat.S_ISREG(before.st_mode),'ordinary pinned input required')
        with os.fdopen(descriptor,'rb',closefd=False) as stream:digest=hashlib.file_digest(stream,'sha256').hexdigest()
        after=os.fstat(descriptor)
        require((before.st_dev,before.st_ino,before.st_size,before.st_mtime_ns,before.st_ctime_ns)==
            (after.st_dev,after.st_ino,after.st_size,after.st_mtime_ns,after.st_ctime_ns),'pinned input changed')
        return dict(path=str(path),bytes=before.st_size,sha256=digest)
    finally:os.close(descriptor)


def checked(row):
    require(set(row)=={'path','bytes','sha256'} and Path(row['path']).is_absolute(),'exact physical file pin required')
    require(fact(row['path'])==row,'selected immutable input differs: '+row['path'])
    return Path(row['path'])


def network(expected):
    actual=os.readlink('/proc/self/ns/net')
    require(actual==expected and actual!=os.readlink('/proc/1/ns/net'),'actual separately isolated network namespace required')
    require(socket.if_nameindex()==[(1,'lo')],'the isolated network namespace must contain only lo')
    return actual


def policy():
    connection={'node-name':'unitree_task05_gateway','namespace':'/uwvm_task05','domain-id':'1',
        'rmw-implementation':'rmw_cyclonedds_cpp','security-mode':'disabled-test',
        'keystore':'not-applicable','enclave':'not-applicable'}
    grants=[]
    for alias,operation,kind,resource in [('task05.request','topic.publish','Request','request'),
        ('task05.response','topic.subscribe','Response','response')]:
        native,codec=TYPES[kind]
        limits={'max-message-bytes':'65536','max-rate-hz':'100','burst':'1','adapter-codec-identity':codec,
            'max-bytes-per-second':'6553600','max-queue-bytes':'65536','max-inflight':'1',
            'overflow-policy':'reject','validator':'none','audit':'allow-and-deny',
            'audit-max-rate-hz':'100','audit-burst':'16','audit-max-records':'10000','audit-max-bytes':'1048576',
            'max-publishers' if operation=='topic.publish' else 'max-subscribers':'1',
            'qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last','qos.depth':'1',
            'qos.deadline':'infinite','qos.lifespan':'infinite','qos.liveliness':'automatic',
            'qos.liveliness-lease-duration':'infinite'}
        grants.append(dict(alias=alias,operation=operation,type='unitree_api/msg/'+kind,
            resource='/uwvm_task05/'+resource,identity=native,limits=limits))
    return connection,grants


def runtime_argv(worker,guest,backends,socket_path,positive):
    argv=[str(worker),'-Rint','--wasm-set-main-module-name','main','--wasip1-global-disable',
        '--wasip1-global-noinherit-system-environment','--wasip1-single-create','main',
        '--wasip1-single-enable','main','--wasip1-single-hide-host-api','main',
        '--wasip1-single-noinherit-system-environment','main']
    for right in NONBASIC:argv+=['--wasip1-single-deny','main',right]
    require(backends in ([2],[1,2]),'exact compiled ROS backend list required')
    for version in backends:
        argv+=['--ros'+str(version)+'-global-disable','--ros'+str(version)+'-single-create','main',
            '--ros'+str(version)+'-single-'+('enable' if positive and version==2 else 'disable'),'main']
    if positive:
        connection,grants=policy()
        settings=dict(connection,**{'gateway-socket':str(socket_path),'gateway-uid':str(os.getuid()),
            'connect-timeout-ms':'10000','audit-deny-max-rate-hz':'100','audit-deny-burst':'16',
            'audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'})
        for key,value in settings.items():argv+=['--ros2-single-'+key,'main',value]
        for grant in grants:
            argv+=['--ros2-single-grant','main',*[grant[key] for key in ('alias','operation','resource','type','identity')]]
            for key,value in grant['limits'].items():argv+=['--ros2-single-set','main',grant['alias'],key,value]
    return [*argv,'--run',str(guest),'Damp','task05.request','task05.response','0','1000','64']


class Actor:
    def __init__(self,label,argv,out,environment,expected,namespace):
        self.label=label;self.expected=expected;self.namespace=namespace;self.cgroup=Path('/proc/self/cgroup').read_text()
        self.stdout=(out/(label+'.stdout')).open('xb');self.stderr=(out/(label+'.stderr')).open('xb')
        self.child=subprocess.Popen(list(map(str,argv)),stdin=subprocess.PIPE,stdout=subprocess.PIPE,
            stderr=self.stderr,env=environment,bufsize=0)
        self.fd=os.pidfd_open(self.child.pid);self.birth=self.identity();self.buffer=bytearray();self.total=0
        self.selector=selectors.DefaultSelector();self.selector.register(self.child.stdout,selectors.EVENT_READ)
        self.until=time.monotonic()+30
    def identity(self):
        raw=Path('/proc/'+str(self.child.pid)+'/stat').read_text()
        return raw.rsplit(')',1)[1].split()[19]
    def observe(self):
        pid=self.child.pid;root=Path('/proc/'+str(pid))
        require(self.identity()==self.birth and (root/'cgroup').read_text()==self.cgroup,'actor birth/cgroup changed')
        require(os.readlink(root/'ns/net')==self.namespace,'actor escaped the isolated loopback namespace')
        require(all(int(value)==os.getuid() for value in (root/'status').read_text().split('Uid:',1)[1].splitlines()[0].split()),'actor UID differs')
        require(fact(root/'exe',kernel_link=True)==dict(self.expected['artifact'],path=str(root/'exe')),'actual actor executable differs')
        expected={row['path']:row for row in self.expected['mapped_inputs']};observed=[]
        for line in (root/'maps').read_text().splitlines():
            fields=line.split(maxsplit=5)
            if len(fields)==6 and fields[5].startswith('/'):
                name=fields[5];require(name in expected,'actual mapped dependency was not selected: '+name)
                checked(expected[name]);observed.append(expected[name])
        require(self.identity()==self.birth,'actor birth changed during map capture')
        return dict(pid=pid,start_ticks=self.birth,network_namespace=self.namespace,mapped_inputs=observed)
    def write(self,body):
        require(self.child.poll() is None and time.monotonic()<self.until,'owned actor is no longer live')
        require(os.write(self.child.stdin.fileno(),body)==len(body),'partial actor input write')
    def exact(self,size):
        require(0<=size<=65536,'finite actor output extent required')
        while len(self.buffer)<size:
            remaining=self.until-time.monotonic();require(remaining>0,'finite actor output deadline exhausted')
            require(self.selector.select(min(remaining,.1)) or self.child.poll() is None,'actor terminated before expected output')
            if not self.selector.select(0):continue
            body=os.read(self.child.stdout.fileno(),min(65536,size-len(self.buffer)))
            require(body,'actor stdout ended before complete output');self.total+=len(body)
            require(self.total<=65536,'finite actor output quota exceeded');self.stdout.write(body);self.stdout.flush();self.buffer.extend(body)
        result=bytes(self.buffer[:size]);del self.buffer[:size];return result
    def line(self):
        result=bytearray()
        while not result.endswith(b'\n'):
            result+=self.exact(1);require(len(result)<=256,'bounded marker line required')
        return bytes(result)
    def expect(self,text):require(self.line()==text,'actual actor marker differs: '+self.label)
    def finish(self,expected=0):
        self.child.stdin.close();require(self.child.wait(timeout=5)==expected,'actual actor exit differs')
        require(not self.buffer and self.selector.select(1),'actor stdout was not closed after its actual exit')
        require(os.read(self.child.stdout.fileno(),1)==b'','unexpected trailing actor output')
    def close(self):
        if self.child.poll() is None:
            try:signal.pidfd_send_signal(self.fd,signal.SIGTERM)
            except ProcessLookupError:pass
            try:self.child.wait(timeout=2)
            except subprocess.TimeoutExpired:signal.pidfd_send_signal(self.fd,signal.SIGKILL);self.child.wait(timeout=2)
        self.child.stdin.close();self.selector.close();self.stdout.close();self.stderr.close();self.child.stdout.close();os.close(self.fd)


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--selection',type=Path,required=True);args=parser.parse_args()
    run=Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True);require(os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD']=='1','shared finite owner required')
    out=run/'work/p';out.mkdir();report=dict(status='ADMITTING',cases=[],inputs=[],default_authority=[],native_ROS_runs=0,
        official_suite_runs=0,hardware_runs=0,QEMU_runs=0,VTune_runs=0,complete_platform_support=False)
    actors=[];pins=[]
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        selection=json.loads(args.selection.read_bytes());require(selection['format']=='uwvm-task05-sport-loopback-selection-v1','exact trusted selection v1 required')
        closure_path=checked(selection['build_closure']);closure=json.loads(closure_path.read_bytes())
        require(closure['format']=='uwvm-task05-production-loopback-closure-v1' and closure['status']=='BUILT_NOT_TRANSPORT_QUALIFIED','actual matching production build closure required')
        require(closure['source_proof_sha256']==SOURCE_PROOF,'preserved genuine Humble installed-IDL typed proof required')
        for row in closure['source_inputs']:checked(row);pins.append(row)
        require(closure['source_inputs'] and closure['native_provider_header'].endswith('/native_source_provider.hpp'),'proof-before-native provider wrapper required')
        require(closure['native_provider_header'] in {row['path'] for row in closure['source_inputs']},'selected native provider was not pinned')
        registry=checked(closure['registry_header']);require(fact(registry)['sha256']==closure['registry_sha256'],'actual shared registry pin differs');pins.append(closure['registry_header'])
        require(all(native in registry.read_text() and codec in registry.read_text() for native,codec in TYPES.values()),'preserved complete native/codec identities differ')
        for name in ('worker','gateway','peer'):
            actor=closure[name];program=checked(actor['artifact']);require(program.read_bytes()[:4]==b'\x7fELF','real native actor ELF required')
            require(actor['registry_sha256']==closure['registry_sha256'],'native actors/Worker must select the same exact registry')
            for row in [actor['artifact'],*actor['mapped_inputs'],*actor['compiler_dependencies']]:checked(row);pins.append(row)
            require(actor['compiler_dependencies'],'actual compiler dependency closure required')
        guest=checked(selection['gated_guest']);require(guest.read_bytes()[:8]==b'\0asm\1\0\0\0','actual gated command Wasm required');pins.append(selection['gated_guest'])
        namespace=network(selection['network_namespace'])
        allowed={'LD_LIBRARY_PATH','AMENT_PREFIX_PATH','RMW_IMPLEMENTATION','ROS_LOCALHOST_ONLY','ROS_DOMAIN_ID','CYCLONEDDS_URI'}
        require(set(selection['native_environment'])<=allowed,'only explicit native loader/ROS environment fields are accepted')
        environment={key:value for key,value in os.environ.items() if key in ('PATH','HOME','TMPDIR','LANG','LC_ALL')}
        environment.update(selection['native_environment'])
        xml=checked(selection['cyclonedds_xml']);pins.append(selection['cyclonedds_xml'])
        require(environment.get('CYCLONEDDS_URI')=='file://'+str(xml),'actual pinned DDS XML required')
        require(environment.get('ROS_LOCALHOST_ONLY')=='1' and environment.get('ROS_DOMAIN_ID')=='1' and environment.get('RMW_IMPLEMENTATION')=='rmw_cyclonedds_cpp','exact loopback domain/RMW environment required')
        # Default-denial is a fresh process and never starts Gateway/native ROS.
        worker=Actor('denied',runtime_argv(closure['worker']['artifact']['path'],guest,closure['worker']['ros_backends'],None,False),out,environment,closure['worker'],namespace);actors.append(worker)
        worker.expect(b'READY_UNITREE_SPORT\n');report['denied_worker']=worker.observe();worker.write(b'G')
        worker.expect(b'DONE primary=13 subscription=0 publisher=0\n');worker.finish(13);worker.close();actors.remove(worker)
        report['cases'].append(dict(name='fresh-ungranted-Guest',status='PASS',native_ROS_runs=0));save()
        for index,native_status in enumerate((0,-7)):
            network(namespace);socket_path=out/('s'+str(index));require(len(os.fsencode(socket_path))<104 and not socket_path.exists(),'fresh short owned UNIX socket required')
            data='{"reply":"λ","status":'+str(native_status)+'}'
            parameter='{"keep":"λ"}'.encode();binary=bytes((0,255,1,128));identity=9007199254740993
            request=struct.pack('<qqqi?',identity,1001,-identity,-101,False)+struct.pack('<I',len(parameter))+parameter+struct.pack('<I',len(binary))+binary
            reply_data=data.encode();reply=struct.pack('<qqi',identity,1001,native_status)+struct.pack('<I',len(reply_data))+reply_data+struct.pack('<I',len(binary))+binary
            request_path=out/('request'+str(index));response_path=out/('response'+str(index));request_path.write_bytes(request);response_path.write_bytes(reply)
            worker=Actor('worker'+str(index),runtime_argv(closure['worker']['artifact']['path'],guest,closure['worker']['ros_backends'],socket_path,True),out,environment,closure['worker'],namespace);actors.append(worker)
            worker.expect(b'READY_UNITREE_SPORT\n');worker_proof=worker.observe();connection,grants=policy()
            lines=['backend ros2','worker-uid '+str(os.getuid()),'worker-pid '+str(worker.child.pid),'socket '+str(socket_path),
                'timeout-ms 10000','max-sessions 1','audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
            lines+=['connection '+key+' '+value for key,value in connection.items()]
            for grant in grants:
                lines+=['grant '+' '.join(grant[key] for key in ('alias','operation','resource','type','identity'))]
                lines+=['set '+grant['alias']+' '+key+' '+value for key,value in grant['limits'].items()]
            config=out/('gateway'+str(index)+'.conf');config.write_text('\n'.join(lines)+'\n');config.chmod(0o600)
            gateway=Actor('gateway'+str(index),[closure['gateway']['artifact']['path'],config],out,environment,closure['gateway'],namespace);actors.append(gateway)
            until=time.monotonic()+10
            while not socket_path.exists():require(gateway.child.poll() is None and time.monotonic()<until,'actual Gateway did not create its owned socket');time.sleep(.001)
            require(stat.S_ISSOCK(socket_path.lstat().st_mode) and socket_path.lstat().st_uid==os.getuid(),'actual owned UNIX socket required')
            gateway_proof=gateway.observe();worker.write(b'G');worker.expect(b'READY\n')
            peer=Actor('peer'+str(index),[closure['peer']['artifact']['path'],'/uwvm_task05/request','/uwvm_task05/response',request_path,response_path],out,environment,closure['peer'],namespace);actors.append(peer)
            peer.expect(b'READY\n');peer.expect(b'MATCHED\n');peer_proof=peer.observe()
            worker.write(struct.pack('<I',len(request))+request);worker.expect(b'SENT\n')
            peer.expect(b'REQUEST_ALL_FIELDS_MATCHED id=9007199254740993 api=1001\n');peer.expect(b'RESPONSE_PUBLISHED_ONCE\n')
            for poll in range(64):
                worker.write(b'P'+struct.pack('<Q',poll+1));marker=worker.line()
                if marker.startswith(b'REPLY '):
                    require(marker==('REPLY '+str(len(reply))+'\n').encode(),'complete reply length differs')
                    require(worker.exact(len(reply))==reply and worker.exact(1)==b'\n','complete canonical reply differs');break
                require(marker==b'IDLE\n','unexpected unmatched reply or Guest status');time.sleep(.001)
            else:raise ValueError('finite positive reply poll budget exhausted')
            worker.expect(b'DONE primary=0 subscription=0 publisher=0\n');worker.finish();peer.write(b'F');peer.expect(b'DONE received=1 sent=1\n');peer.finish()
            report['cases'].append(dict(name='native-status-'+str(native_status),status='PASS',request=fact(request_path),response=fact(response_path),
                worker=worker_proof,gateway=gateway_proof,peer=peer_proof,policy=grants,full_native_request_equality=True,full_canonical_reply_equality=True,
                native_status=native_status,request_attempts=1,native_replies=1,physical_robot_completion=False));report['native_ROS_runs']+=1;save()
            for actor in (peer,worker,gateway):actor.close();actors.remove(actor)
        for row in pins:checked(row)
        report.update(status='PASS',passed=3,denominator=3,input_pins_preserved=True,transport_only=True)
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:
        for actor in reversed(actors):actor.close()
        report['inputs']=pins;save()


if __name__=='__main__':main()
