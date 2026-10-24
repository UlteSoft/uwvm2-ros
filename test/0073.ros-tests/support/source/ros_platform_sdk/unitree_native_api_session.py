#!/usr/bin/env python3
"""Own two independent native ROS2 carrier peers in one isolated network.

Only the trusted finite outer owner launches this inside its read-only native
SDK chroot. There is no robot, Gateway, Wasm import provider or motion endpoint.
All processes are retired even after mismatches, timeouts or setup exceptions.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import select
import shutil
import subprocess
import time


def fact(path):
    path=Path(path)
    sha=hashlib.sha256()
    with path.open('rb')as stream:
        for chunk in iter(lambda:stream.read(1<<20),b''):sha.update(chunk)
    return dict(bytes=path.stat().st_size,sha256=sha.hexdigest())


def mapped(pid):
    text=(Path('/proc')/str(pid)/'maps').read_text();files={}
    for line in text.splitlines():
        fields=line.split(None,5)
        if len(fields)!=6 or'x'not in fields[1]or not fields[5].startswith('/'):continue
        p=Path(fields[5]);assert any(p.is_relative_to(prefix)for prefix in map(Path,('/usr','/lib','/lib64','/opt/ros/humble','/tmp/out'))),str(p)
        info=p.stat();assert info.st_ino==int(fields[4])
        assert f'{os.major(info.st_dev):02x}:{os.minor(info.st_dev):02x}'==fields[3]
        files[str(p)]=dict(device=info.st_dev,inode=info.st_ino,**fact(p))
    assert files and any('unitree_api__rosidl_typesupport' in p for p in files)
    return dict(text=text,files=files)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',type=Path,required=True);parser.add_argument('--peer',type=Path,required=True)
    parser.add_argument('--install',type=Path,required=True);parser.add_argument('--vectors',type=Path,required=True)
    parser.add_argument('--parent-netns',required=True)
    a=parser.parse_args();a.out.mkdir();report=dict(status='RUNNING',pairs=[],Wasm_Gateway_transport_qualified=False,native_robot_qualified=False)
    def save():(a.out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    actors=[];logs=[]
    try:
        namespace=os.readlink('/proc/self/ns/net');assert namespace!=a.parent_netns
        interfaces=[line.split(':',1)[0].strip()for line in Path('/proc/net/dev').read_text().splitlines()[2:]]
        assert interfaces==['lo'],interfaces
        report['network_proof']=dict(namespace=namespace,parent_namespace=a.parent_netns,interfaces=interfaces,
            ROS_LOCALHOST_ONLY='1',physical_interfaces=False)
        for rmw in('rmw_fastrtps_cpp','rmw_cyclonedds_cpp'):
            for language in('rs','as','tinygo','js'):
                directory=a.out/(rmw+'-'+language);directory.mkdir()
                env=dict(os.environ,RMW_IMPLEMENTATION=rmw,ROS_DOMAIN_ID='209',ROS_LOCALHOST_ONLY='1',
                    ROS_SECURITY_ENABLE='false',ROS_LOG_DIR=str(directory/'ros-logs'),RCUTILS_LOGGING_USE_STDOUT='0',
                    LD_LIBRARY_PATH=str(a.install/'lib')+':/opt/ros/humble/lib:/opt/ros/humble/lib/x86_64-linux-gnu')
                env.pop('LD_PRELOAD',None);env.pop('ROS_NAMESPACE',None)
                for key in('FASTRTPS_DEFAULT_PROFILES_FILE','FASTDDS_DEFAULT_PROFILES_FILE','CYCLONEDDS_URI',
                    'RMW_FASTRTPS_USE_QOS_FROM_XML','RMW_FASTRTPS_PUBLICATION_MODE'):env.pop(key,None)
                profile=directory/'transport.xml'
                shutil.copyfile(Path(__file__).with_name('unitree_fastdds_loopback.xml'if rmw=='rmw_fastrtps_cpp'else'unitree_cyclone_loopback.xml'),profile)
                env['FASTRTPS_DEFAULT_PROFILES_FILE'if rmw=='rmw_fastrtps_cpp'else'CYCLONEDDS_URI']=str(profile)if rmw=='rmw_fastrtps_cpp'else profile.as_uri()
                pair=dict(rmw=rmw,language=language,source_execution='host-js'if language=='js'else'Wasm',
                    vector_file=fact(a.vectors/(language+'.txt')),received=0,default_Guest_authority=[],
                    transport_profile=fact(profile),transport='explicit UDPv4 loopback',
                    native_library_path=env['LD_LIBRARY_PATH'],
                    native_authority='explicit isolated namespace carrier fixture only')
                report['pairs'].append(pair);save();current=[]
                for role in('reader','writer'):
                    stderr=(directory/(role+'.stderr')).open('xb')
                    child=subprocess.Popen([str(a.peer),role,str(a.vectors/(language+'.txt'))],env=env,stdin=subprocess.PIPE,
                        stdout=subprocess.PIPE,stderr=stderr,close_fds=True)
                    actors.append(child);os.set_blocking(child.stdout.fileno(),False)
                    actor=dict(role=role,process=child,buffer=bytearray(),lines=[],stderr=stderr,
                        stdout_path=directory/(role+'.stdout'),captured_bytes=0)
                    actor['stdout_path'].touch(exist_ok=False);current.append(actor);logs.append(actor)
                reader,writer=current;deadline=time.monotonic()+60
                pair['native_pids']={actor['role']:actor['process'].pid for actor in current}
                pair['delivery_offsets_ns']=[];save()
                def line(actor):
                    while b'\n'not in actor['buffer']:
                        assert time.monotonic()<deadline,('native deadline',rmw,language)
                        ready,_,_=select.select([actor['process'].stdout],[],[],.05)
                        if not ready:
                            assert actor['process'].poll()is None,(actor['role'],actor['process'].returncode)
                            continue
                        value=os.read(actor['process'].stdout.fileno(),4096)
                        assert value,('native EOF',actor['role'],actor['process'].poll())
                        actor['captured_bytes']+=len(value);assert actor['captured_bytes']<=65536
                        with actor['stdout_path'].open('ab')as stream:stream.write(value)
                        actor['buffer'].extend(value);assert len(actor['buffer'])<=65536
                    raw,_,tail=actor['buffer'].partition(b'\n');actor['buffer']=bytearray(tail)
                    result=raw.decode('ascii');actor['lines'].append(result)
                    return result
                def credit(actor,value):actor['process'].stdin.write(value);actor['process'].stdin.flush()
                for actor in current:
                    assert line(actor)=='READY '+actor['role']
                    assert line(actor)=='MATCHED '+actor['role']
                    assert os.readlink('/proc/'+str(actor['process'].pid)+'/ns/net')==namespace
                mapping_started=time.monotonic_ns()
                pair['actual_loaded_files']={actor['role']:mapped(actor['process'].pid)for actor in current}
                pair['mapped_file_capture_elapsed_ns']=time.monotonic_ns()-mapping_started;save()
                deadline=time.monotonic()+30;data_started=time.monotonic_ns()
                credit(reader,b'S');credit(writer,b'S')
                for number in range(1,43):
                    credit(writer,b'P');assert line(writer)=='PUBLISHED '+str(number)
                    assert line(reader)=='DELIVERED '+str(number);pair['received']=number
                    pair['delivery_offsets_ns'].append(time.monotonic_ns()-data_started);save()
                credit(writer,b'F');credit(reader,b'F')
                for actor in current:
                    assert line(actor)=='PASS '+actor['role'];actor['process'].stdin.close()
                    assert actor['process'].wait(timeout=5)==0
                    tail=actor['process'].stdout.read();assert not actor['buffer']and not tail
                    assert len(actor['lines'])==45
                    actor['stderr'].close()
                for role in pair['actual_loaded_files'].values():
                    for path,row in role['files'].items():
                        assert fact(path)=={k:row[k]for k in('bytes','sha256')}
                        info=Path(path).stat();assert info.st_dev==row['device']and info.st_ino==row['inode']
                pair.update(status='FULL_GENERATED_REQUEST_DDS_DELIVERY_PASS',
                    data_phase_elapsed_ns=time.monotonic_ns()-data_started,
                    outputs={role:{stream:fact(directory/(role+'.'+stream))for stream in('stdout','stderr')}for role in('writer','reader')});save()
        report.update(status='TWO_RMW_FOUR_SOURCE_NATIVE_CARRIERS_PASS',complete_requests_received=sum(p['received']for p in report['pairs']))
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:
        for child in reversed(actors):
            if child.poll()is None:
                child.terminate()
                try:child.wait(timeout=3)
                except subprocess.TimeoutExpired:child.kill();child.wait()
            if child.stdin and not child.stdin.closed:child.stdin.close()
        for actor in logs:
            stream=actor['process'].stdout
            if stream and not stream.closed:
                while True:
                    try:value=os.read(stream.fileno(),4096)
                    except BlockingIOError:break
                    if not value:break
                    actor['captured_bytes']+=len(value);assert actor['captured_bytes']<=65536
                    with actor['stdout_path'].open('ab')as target:target.write(value)
                stream.close()
            if not actor['stderr'].closed:actor['stderr'].close()
        save()
    print(report['status'],report['complete_requests_received'],flush=True)


if __name__=='__main__':main()
