#!/usr/bin/env python3
"""Run two current custom-topic Wasm loopbacks through real IPC and native ROS.

Linux/verified-cgroup execution only. Native products must already be compiled
and qualified; this controller translates only the two frozen WAT fixtures.
The guest's stdin gate precedes its tested ROS calls. A live stopped ARM Worker
mapping is proven before releasing that gate. This is a single-Guest native
loopback, not an independent native peer or a complete G01 acceptance claim.
"""
import argparse
import fcntl
import importlib.util
import json
import os
from pathlib import Path
import shutil
import signal
import socket
import stat
import subprocess
import sys
import tempfile
import time
import traceback
import xmlrpc.client

sys.dont_write_bytecode = True
from runtime import Evidence, cgroup_incarnation, digest, elf, read_json_binding, require_arm
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state
from gateway_qualification import pins_preserved, qualify_gateway, read_file_binding
from run_ros1_search import OwnedProcesses
from run_unit import process_tree_rss
from run_action_qos import process_identity
from run_all import validate_fresh_worker_inputs
from run_resources import bind_frozen_work
from topic_gate import ShortTransport


FIXTURES = {
    1: ('tests/ros_custom_types/loopback_ros1.wat', 'uwvm_custom_ros1_fixture/Packet', 23),
    2: ('tests/ros_custom_types/loopback_ros2.wat', 'uwvm_ros_custom_test_msgs/msg/FixedSample', 45),
}
DEPLOYMENT = 'tests/ros_custom_types/deployment-r1/generated'


def topic_policy(version, descriptor, master_port, domain):
    """Construct explicit fixture permissions; native registration grants none."""
    connection = {'node-name': 'uwvm_custom_guest_ros'+str(version), 'namespace': '/uwvm_custom_guest'}
    if version == 1:
        connection.update({'master-uri': 'http://127.0.0.1:'+str(master_port),
                           'advertise-host': '127.0.0.1', 'transport': 'tcpros'})
    else:
        connection.update({'domain-id': str(domain), 'rmw-implementation': 'rmw_fastrtps_cpp',
                           'security-mode': 'disabled-test', 'keystore': 'not-applicable', 'enclave': 'not-applicable'})
    grants = []
    for alias, operation in (('pub', 'topic.publish'), ('sub', 'topic.subscribe')):
        limits = {'adapter-codec-identity': descriptor['adapter_codec_identity'],
                  'max-message-bytes': str(descriptor['wire_bytes']), 'max-rate-hz': '100', 'burst': '8',
                  'max-bytes-per-second': '65536', 'max-queue-bytes': '4096', 'overflow-policy': 'drop-oldest',
                  'validator': 'none', 'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '16',
                  'audit-max-records': '10000', 'audit-max-bytes': '1048576',
                  'max-publishers' if alias == 'pub' else 'max-subscribers': '1'}
        if version == 1:
            limits.update({'queue-depth': '8', 'transport': 'tcpros'})
            if alias == 'pub': limits['latch'] = 'false'
        else:
            limits.update({'max-inflight': '1', 'qos.reliability': 'reliable', 'qos.durability': 'volatile',
                           'qos.history': 'keep-last', 'qos.depth': '8', 'qos.deadline': 'infinite',
                           'qos.lifespan': 'infinite', 'qos.liveliness': 'automatic',
                           'qos.liveliness-lease-duration': 'infinite'})
        grants.append({'alias': alias, 'operation': operation, 'resource': '/uwvm_custom_guest/roundtrip_ros'+str(version),
                       'type': descriptor['type'], 'identity': descriptor['native_identity'], 'limits': limits})
    return connection, grants


def worker_arguments(worker, version, connection, grants, socket_path, wasm):
    """Select only the main module and the fixture's monotonic clock explicitly."""
    prefix = '--ros'+str(version)+'-single-'
    argv = [str(worker), prefix+'create', 'main', prefix+'enable', 'main']
    for key, value in {**connection, 'gateway-socket': str(socket_path), 'gateway-uid': str(os.getuid()),
                       'connect-timeout-ms': '10000', 'audit-deny-max-rate-hz': '100', 'audit-deny-burst': '16',
                       'audit-deny-max-records': '10000', 'audit-deny-max-bytes': '1048576'}.items():
        argv += [prefix+key, 'main', value]
    for grant in grants:
        argv += [prefix+'grant', 'main', *[grant[k] for k in ('alias','operation','resource','type','identity')]]
        for key, value in grant['limits'].items(): argv += [prefix+'set', 'main', grant['alias'], key, value]
    argv += ['--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
             '--wasip1-single-noinherit-system-environment', 'main',
             '--wasip1-single-grant', 'main', 'monotonic-clock',
             '--wasm-set-main-module-name', 'main', '--run', str(wasm)]
    return argv


def gateway_configuration(version, worker_pid, connection, grants):
    """Bind a trusted Gateway to the exact owned Worker PID and mirrored policy."""
    lines = ['backend ros'+str(version), 'worker-uid '+str(os.getuid()), 'worker-pid '+str(worker_pid),
             'socket /guest/ros'+str(version)+'.sock', 'timeout-ms 10000', 'max-sessions 2',
             'audit-max-records-per-second 100', 'audit-max-records 10000', 'audit-max-bytes 1048576']
    lines += ['connection '+key+' '+value for key,value in connection.items()]
    for grant in grants:
        lines += ['grant '+' '.join(grant[k] for k in ('alias','operation','resource','type','identity'))]
        lines += ['set '+grant['alias']+' '+key+' '+value for key,value in grant['limits'].items()]
    return '\n'.join(lines)+'\n'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source','manifest','worker','worker-report','worker-config','ros1-gateway','ros1-gateway-report',
                 'ros2-gateway','ros2-gateway-report','sdk-workspace','out','qemu','binfmt-evidence','wat2wasm'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--container-started-at', required=True)
    parser.add_argument('--master-port', type=int, default=11473)
    parser.add_argument('--domain', type=int, default=213)
    args = parser.parse_args()
    if not 1024 <= args.master_port <= 65535 or not 0 <= args.domain <= 232: parser.error('invalid isolated port/domain')
    if sys.platform != 'linux' or os.geteuid() != 0: raise RuntimeError('Linux root verified-cgroup mount environment required')
    source = args.source.resolve(strict=True); out = args.out.resolve(); out.mkdir(mode=0o700, parents=True, exist_ok=False)
    report = {'status':'RUNNING', 'kind':'custom-guest-arm', 'format_version':1,
              'scope':'One raw-import Wasm Guest per backend, real IPC and native custom-topic loopback; no independent native peer/full G01 claim',
              'source':str(source), 'container_started_at':args.container_started_at, 'cases':[], 'commands':[],
              'mounts':[], 'actual_guest_cases':0, 'actual_guest_behavior':'NOT_PERFORMED'}
    pins, snapshots, mounts, children, locks = [], {}, [], [], []
    evidence = owned = None; short = None; status = 'FAIL'; initial = None; worker_deadline = None
    def save(): (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    def json_pin(path,limit=64*1024**2):
        value,binding=read_json_binding(path,limit=limit);pins.append(binding);return value,binding
    def checked(command): subprocess.run(list(map(str,command)),check=True,capture_output=True,timeout=30)
    def bind(origin,destination,readonly=False):
        destination=Path(destination);destination.mkdir(parents=True,exist_ok=True)
        checked(['mount','--rbind',origin,destination]);mounts.append(destination);checked(['mount','--make-rprivate',destination])
        if readonly:checked(['mount','-o','remount,bind,ro',destination])
        report['mounts'].append({'source':str(origin),'destination':str(destination),'readonly':readonly})
    def monitor():
        owned.sample()
        if worker_deadline is not None and time.monotonic() > worker_deadline:
            raise TimeoutError('Worker total 60-second launch/body deadline exhausted')
        if cgroup_incarnation(evidence.leaf)!=before or (evidence.leaf/'memory.swap.max').read_text().strip()!='0':
            raise RuntimeError('cgroup incarnation/swap limit changed')
        if int((evidence.leaf/'memory.current').read_text())>60*1024**3:raise RuntimeError('64GiB group reserve admission failed')
    def start(name,command,environment=None,stdin=None):
        log=out/(name+'.log')
        launched=time.monotonic()
        with log.open('xb') as stream:
            child=subprocess.Popen(command,stdin=stdin,stdout=stream,stderr=subprocess.STDOUT,
                                   env=environment or clean,start_new_session=True)
        children.append(child);owned.acquire(child.pid,name,[child.pid]);evidence.start(child,name+'/',command)
        row={'name':name,'command':command,'pid':child.pid,'starttime_ticks':process_identity(child.pid)['starttime_ticks'],
             'launched_monotonic':launched,
             'status':'RUNNING','log':str(log),'cgroup_membership':Path(f'/proc/{child.pid}/cgroup').read_text().splitlines()}
        report['commands'].append(row);save();return child,row
    def wait(child,row,budget,required=()):
        deadline=time.monotonic()+budget;peak=0
        while child.poll() is None:
            monitor();peak=max(peak,process_tree_rss(child.pid),owned.rss(row['name']))
            if peak>8*1024**3 or Path(row['log']).stat().st_size>16*1024**2:raise RuntimeError('bounded subprocess RSS/log exceeded')
            if time.monotonic()>deadline:raise TimeoutError('bounded command deadline '+row['name'])
            time.sleep(.002)
        row.update(returncode=child.returncode,peak_observed_subtree_RSS_bytes=peak,log_sha256=digest(row['log']))
        if child.returncode:raise RuntimeError('native/Guest command failed '+row['name'])
        evidence.check(required,row['name']+'/');row['status']='PASS';save()
    def current_map(pid,target):
        return evidence.mapped(pid, target)
    def stopped_mapping(child,row,label,budget=20,expected_pid=None):
        deadline=time.monotonic()+budget;target=evidence.targets[label]
        while child.poll() is None and time.monotonic()<deadline:
            monitor()
            for candidate in list(owned.rows.values()):
                if candidate['label']!=row['name'] or not owned.alive(candidate) or not owned.check(candidate):continue
                pid=candidate['identity']['pid']
                # roscore and its XMLRPC Master can both map ARM Python. Only
                # the exact PID supplied by the owned ready Master is admitted.
                if expected_pid is not None and pid!=expected_pid:continue
                try:
                    if not current_map(pid,target):continue
                    owned.send(candidate,signal.SIGSTOP);until=time.monotonic()+1
                    while owned.alive(candidate) and process_identity(pid)['state'] not in ('T','t') and time.monotonic()<until:time.sleep(.001)
                    actual=process_identity(pid)
                    if actual['state'] not in ('T','t'):raise RuntimeError('owned native image failed to stop')
                    evidence.observe(child.pid,row['name']+'/')
                    with evidence._lock:
                        matches=[sample for sample in evidence.samples.values() if sample['opened_target_mappings']['identity_after']['state'] in ('T','t') and sample['name']==row['name']+'/'
                            and sample['pid']==pid and sample['starttime_ticks']==actual['starttime_ticks']
                            and sample['ancestry']==candidate['ancestry'] and sample['live_cgroup_procs']
                            and sample['membership']==evidence.before['membership'] and label in sample['mapped_arm_targets']
                            and sample['kernel_exe_elf']['e_machine']==62 and sample['kernel_exe_elf']['sha256'] in evidence.emulator_shas]
                    host=Path(f'/proc/{pid}/exe').stat()
                    if (not matches or not current_map(pid,target) or not owned.check(candidate) or process_identity(pid)!=actual
                            or (matches[-1]['kernel_exe_elf']['device'],matches[-1]['kernel_exe_elf']['inode'])!=(host.st_dev,host.st_ino)):
                        raise RuntimeError('stopped actual image/proc birth/cgroup proof differs')
                    proof={'actual_stopped_ARM':actual,'target_proof':matches[-1],'current_mapping_check':True,
                           'ancestry':candidate['ancestry'],'live_cgroup_membership':evidence.before['membership']}
                    evidence.before_resume(pid, actual)
                    owned.send(candidate,signal.SIGCONT);row['actual_ARM_mapping_before_resume']=proof;save();return proof
                except (FileNotFoundError,ProcessLookupError):continue
            time.sleep(.002)
        raise RuntimeError('actual stopped ARM/QEMU image proof missing '+label)
    clean=dict(os.environ)
    for name in ('LD_PRELOAD','LD_LIBRARY_PATH','QEMU_LD_PREFIX','PYTHONPATH','PYTHONHOME','ROS_HOSTNAME','ROS_NAMESPACE',
                 'FASTRTPS_DEFAULT_PROFILES_FILE','FASTDDS_DEFAULT_PROFILES_FILE'):
        clean.pop(name,None)
    clean.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',PYTHONDONTWRITEBYTECODE='1')
    def native(root,version,argv):
        distro='noetic' if version==1 else 'jazzy'
        script='source /opt/ros/'+distro+'/setup.bash; export LD_LIBRARY_PATH=/opt/uwvm-custom-types/lib:$LD_LIBRARY_PATH; exec "$@"'
        return ['/usr/bin/chroot',str(root),'/bin/bash','-lc',script,'uwvm-custom-guest-native',*argv]
    def copy_product(origin,destination,label):
        facts=require_arm(origin);pin=read_file_binding(origin);pins.append(pin);shutil.copy2(origin,destination);destination.chmod(0o500)
        copied=require_arm(destination)
        if (copied['sha256'],copied['bytes'])!=(facts['sha256'],facts['bytes']):raise RuntimeError('owned native snapshot differs')
        evidence.target(label,destination);snapshots[label]=copied
        return {'origin':facts,'origin_pin':pin,'snapshot':copied}
    try:
        if Path(__file__).resolve().parent!=source/'tests/ros_aarch64':raise RuntimeError('execute controller from exact frozen source')
        if os.readlink('/proc/self/ns/mnt')==os.readlink(f'/proc/{os.getppid()}/ns/mnt'):raise RuntimeError('launch through unshare -m')
        frozen,manifest_pin=json_pin(args.manifest);initial=source_state(source,frozen);report['source_manifest_sha256']=manifest_pin['sha256']
        for path in Path(__file__).parent.glob('*.py'):
            relative=str(path.relative_to(source))
            if frozen.get(relative,{}).get('sha256')!=digest(path):raise RuntimeError('executed helper absent or changed in source manifest')
        config,config_pin=json_pin(args.worker_config);worker_report,worker_pin=json_pin(args.worker_report)
        if (worker_report.get('status')!='PASS' or worker_report.get('source')!=str(source)
                or worker_report.get('source_manifest_sha256')!=manifest_pin['sha256'] or worker_report.get('config_sha256')!=config_pin['sha256']):
            raise RuntimeError('fresh current Worker source/config report required')
        worker_bindings=validate_fresh_worker_inputs(worker_report,source,frozen)
        for binding in worker_bindings.values():
            for record in binding['raw_compile_dependencies'].values():
                raw_pin=read_file_binding(record['path'])
                if raw_pin['sha256']!=record['sha256']:raise RuntimeError('Worker raw dependency changed during qualification')
                pins.extend((raw_pin,read_file_binding(record['object']['path'])))
        for label in ('compiler','qemu'):pins.append(worker_report[label]['input'])
        dual=[row for row in worker_report['profiles'] if row['profile']=='dual']
        if len(dual)!=1 or dual[0].get('status')!='PASS' or dual[0]['binary']!=require_arm(args.worker):raise RuntimeError('exact current dual Worker required')
        header=(source/DEPLOYMENT/'manifest.hpp').resolve(strict=True)
        expected='-DUWVM_ROS_CUSTOM_TOPIC_MANIFEST="'+str(header)+'"'
        for command in dual[0]['commands'][:3]:
            if (expected not in command['command'] or '-DUWVM_IMPORT_ROS1' not in command['command']
                    or '-DUWVM_IMPORT_ROS2' not in command['command']):raise RuntimeError('custom manifest and both backends must be compiled into dual Worker')
        report['worker_qualification']={'report':worker_pin,'config':config_pin,'dual':worker_bindings['dual'],'manifest_definition':expected}
        deployment,deployment_pin=json_pin(source/DEPLOYMENT/'generation.json');report['deployment']=deployment_pin
        if deployment.get('status')!='GENERATED_SOURCE_ONLY' or deployment.get('native_registry_emitted') is not True:raise RuntimeError('official native deployment generation required')
        descriptors={}
        for version,(_,typename,size) in FIXTURES.items():
            rows=[row for row in deployment['types'] if row['ros_version']==version and row['type']==typename]
            if len(rows)!=1 or rows[0]['wire_bytes']!=size or not rows[0].get('native_identity'):raise RuntimeError('exact fixed custom descriptor absent')
            descriptors[version]=rows[0]
        spec=importlib.util.spec_from_file_location('custom_guest_cgroup',source/'tests/ros_cgroup/probe.py')
        probe=importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence=Evidence(out/'execution',probe.probe,interval=.0005);evidence.emulator(args.qemu);before=cgroup_incarnation(evidence.leaf)
        if (evidence.leaf/'memory.swap.max').read_text().strip()!='0':raise RuntimeError('zero swap required')
        owned=OwnedProcesses(evidence);report['cgroup_incarnation_before']=before
        lease,lease_pin=json_pin(args.binfmt_evidence);entry=(Path('/proc/sys/fs/binfmt_misc')/lease['name']).read_text()
        if (entry!=lease['entry'] or lease['name']!=os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY')
                or lease['qemu_sha256']!=digest(args.qemu) or config['qemu_argv'][0]!=str(args.qemu.resolve())):
            raise RuntimeError('exact provided QEMU/temporary binfmt/config binding required')
        report['binfmt']={**lease_pin,'entry':entry};pins.append(read_file_binding(args.qemu))
        assembler=read_file_binding(args.wat2wasm);pins.append(assembler);report['wat2wasm']={**assembler,'elf':elf(args.wat2wasm)}
        for version in (1,2):
            sdk=args.sdk_workspace.resolve()/'gateway'/('noetic-rootfs' if version==1 else 'jazzy-rootfs')
            binding=qualify_gateway(getattr(args,f'ros{version}_gateway'),getattr(args,f'ros{version}_gateway_report'),args.manifest,source,sdk,version)
            pins.extend(binding['input_pins']);report.setdefault('gateway_qualification',{})[str(version)]=binding
        checked(['mount','--make-rprivate','/'])
        short=Path(tempfile.mkdtemp(prefix='uwvm-custom-guest-',dir='/tmp'));short.chmod(0o700)
        report['owned_socket_directory']=str(short)
        for label in ('master-'+str(args.master_port),'domain-'+str(args.domain)):
            lock=open('/tmp/uwvm-custom-guest-'+label+'.lock','a');fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB);locks.append(lock)
        for version,(wat_relative,typename,size) in FIXTURES.items():
            case={'version':version,'status':'RUNNING','descriptor':descriptors[version],'expected_bytes':size};report['cases'].append(case);save()
            directory=out/('ros'+str(version));directory.mkdir();tmp=directory/'tmp';tmp.mkdir()
            sdk=args.sdk_workspace.resolve()/'gateway'/('noetic-rootfs' if version==1 else 'jazzy-rootfs')
            old_sdk=sdk_inventory(sdk);old_aliases=sdk_root_aliases(sdk);root=directory/'rootfs';root.mkdir()
            bind(sdk,root);report.setdefault('source_mounts',{})[str(version)]=bind_frozen_work(root,source,sdk,directory,checked)
            mounts.append(root/'work');bind(tmp,root/'tmp');home=directory/'home';home.mkdir();bind(home,root/'root')
            for name in ('usr','opt','etc','var/lib/dpkg'):bind(sdk/name,root/name,True)
            for name in ('proc','sys','dev'):bind(Path('/')/name,root/name)
            bind(short,root/'guest')
            case['worker_snapshot']=copy_product(args.worker,directory/'worker','worker-'+str(version))
            case['gateway_snapshot']=copy_product(getattr(args,f'ros{version}_gateway'),tmp/'gateway','gateway-'+str(version))
            wat=source/wat_relative;pins.append(read_file_binding(wat));wasm=directory/'loopback.wasm'
            compiler,compile_row=start('ros'+str(version)+'-wat2wasm',[str(args.wat2wasm),str(wat),'-o',str(wasm)])
            wait(compiler,compile_row,30)
            samples=[row for row in evidence.samples.values() if row['name']==compile_row['name']+'/' and row['ancestry'][0]==compiler.pid
                     and row['kernel_exe_elf']['sha256']==assembler['sha256'] and row['live_cgroup_procs']]
            if not samples:raise RuntimeError('actual wat2wasm executable/cgroup sample missing')
            if not wasm.is_file() or wasm.read_bytes()[:8]!=b'\x00asm\x01\x00\x00\x00':raise RuntimeError('actual WAT output is not Wasm version 1')
            case['wat_translation']={'command':compile_row,'actual_tool_sample':samples[-1],'wat':read_file_binding(wat),'wasm':read_file_binding(wasm)}
            environment={**clean,'ROS_MASTER_URI':'http://127.0.0.1:'+str(args.master_port),'ROS_IP':'127.0.0.1',
                         'ROS_DOMAIN_ID':str(args.domain),'RMW_IMPLEMENTATION':'rmw_fastrtps_cpp',
                         'ROS_SECURITY_ENABLE':'false','ROS_LOG_DIR':'/tmp/ros-logs'}
            (tmp/'ros-logs').mkdir();master=None
            if version==1:
                try:
                    with socket.create_connection(('127.0.0.1',args.master_port),.2):raise RuntimeError('isolated Master port already used')
                except ConnectionRefusedError:pass
                evidence.target('master-python',rooted(sdk,'/usr/bin/python3'))
                master,master_row=start('ros1-master',native(root,1,['/opt/ros/noetic/bin/roscore','-p',str(args.master_port)]),environment)
                deadline=time.monotonic()+30
                while time.monotonic()<deadline:
                    monitor()
                    if master.poll() is not None:raise RuntimeError('owned Master exited')
                    try:
                        with xmlrpc.client.ServerProxy(environment['ROS_MASTER_URI'],transport=ShortTransport()) as client:
                            uri=client.getUri('/uwvm_custom_guest_controller');pid=client.getPid('/uwvm_custom_guest_controller')
                        if uri[0]==1 and uri[2].rstrip('/')==environment['ROS_MASTER_URI'] and pid[0]==1:
                            proof=stopped_mapping(master,master_row,'master-python',expected_pid=pid[2])
                            if proof['actual_stopped_ARM']['pid']!=pid[2]:raise RuntimeError('Master readiness PID differs from actual owned image')
                            case['master_readiness']={'uri':uri,'pid':pid,'mapping':proof};break
                    except (OSError,xmlrpc.client.Error):pass
                    time.sleep(.02)
                if 'master_readiness' not in case:raise RuntimeError('bounded owned Master readiness absent')
            connection,grants=topic_policy(version,descriptors[version],args.master_port,args.domain)
            socket_path=short/('ros'+str(version)+'.sock')
            argv=worker_arguments(directory/'worker',version,connection,grants,socket_path,wasm)
            command=[*config['qemu_argv'],*argv]
            worker,worker_row=start('ros'+str(version)+'-guest',['/bin/bash','-c','kill -STOP $$; exec "$@"','custom-worker',*command],clean,subprocess.PIPE)
            worker_deadline=worker_row['launched_monotonic']+60
            until=time.monotonic()+5
            while process_identity(worker.pid)['state'] not in ('T','t') and time.monotonic()<until:monitor();time.sleep(.001)
            if process_identity(worker.pid)['state'] not in ('T','t'):raise RuntimeError('owned launch shell failed to pause')
            cfg=short/('ros'+str(version)+'.conf');cfg.write_text(gateway_configuration(version,worker.pid,connection,grants));cfg.chmod(0o600)
            case['worker_argv']=argv;case['worker_execution_argv']=command;case['gateway_config']=read_file_binding(cfg)
            case['gateway_config_text']=cfg.read_text();case['grants']=grants;case['connection']=connection
            gateway,gateway_row=start('ros'+str(version)+'-gateway',native(root,version,['/tmp/gateway','/guest/'+cfg.name]),environment)
            stopped_mapping(gateway,gateway_row,'gateway-'+str(version))
            deadline=time.monotonic()+30
            while not socket_path.exists() and time.monotonic()<deadline:
                monitor()
                if gateway.poll() is not None:raise RuntimeError('custom Gateway exited before socket readiness')
                time.sleep(.01)
            if not socket_path.exists() or not stat.S_ISSOCK(socket_path.stat().st_mode):raise RuntimeError('bounded actual Gateway socket missing')
            owner=next(row for row in owned.rows.values() if row['identity']['pid']==worker.pid and row['label']==worker_row['name'])
            owned.send(owner,signal.SIGCONT)
            case['worker_mapping_before_guest_ROS_body']=stopped_mapping(worker,worker_row,'worker-'+str(version))
            worker.stdin.write(b'G');worker.stdin.flush();worker.stdin.close()
            remaining=worker_deadline-time.monotonic()
            if remaining<=0:raise TimeoutError('Worker total 60-second launch/body deadline exhausted')
            wait(worker,worker_row,remaining,['worker-'+str(version)])
            worker_deadline=None
            evidence.check(['gateway-'+str(version)],gateway_row['name']+'/')
            if gateway.poll() is not None or master is not None and master.poll() is not None:raise RuntimeError('native backend ended before Guest completion')
            case.update(status='PASS',guest_proc_exit=0,invalid_boolean_expected_errno=22,stale_handle_expected_errno=9,
                        complete_record_byte_comparison=True,scope='Assertions are inside this exact frozen Guest; no independent native peer claim')
            cleanup=owned.stop(children);case['process_cleanup']=cleanup;owned=None;children=[]
            if cleanup['status']!='PASS':raise RuntimeError('owned backend process cleanup failed')
            owned=OwnedProcesses(evidence)
            case['sdk_preserved']=sdk_inventory(sdk)==old_sdk and sdk_root_aliases(sdk)==old_aliases
            if not case['sdk_preserved']:raise RuntimeError('static SDK inputs/aliases changed')
            report['actual_guest_cases']+=1;save();print('PASS actual ROS'+str(version)+' custom raw-import Guest loopback bytes='+str(size),flush=True)
        report['source_preserved']=source_state(source,frozen)==initial;report['inputs_preserved']=pins_preserved(pins)
        report['snapshots_preserved']=all(require_arm(facts['path'])==facts for facts in snapshots.values())
        if not all(report[key] for key in ('source_preserved','inputs_preserved','snapshots_preserved')):raise RuntimeError('final source/product/control inputs changed')
        report['input_pins']=pins;report['actual_guest_behavior']='PASS';status='PASS'
    except BaseException as error:
        report['error']=repr(error);report['traceback']=traceback.format_exc()
    finally:
        if owned is not None:
            try:
                report['process_cleanup']=owned.stop(children)
                if report['process_cleanup']['status']!='PASS':status='FAIL'
            except BaseException as error:report['cleanup_error']=repr(error);status='FAIL'
        if evidence is not None:
            try:
                result=evidence.finish(status,{'scope':report['scope'],'container_started_at':args.container_started_at})
                status=result['status'];report['architecture_evidence']={'path':str(out/'execution/architecture-evidence.json'),
                    'sha256':digest(out/'execution/architecture-evidence.json')}
            except BaseException as error:report['architecture_error']=repr(error);status='FAIL'
        report['mount_cleanup']=[]
        for mount in reversed(mounts):
            result=subprocess.run(['umount','-R',str(mount)],capture_output=True,timeout=30)
            report['mount_cleanup'].append({'path':str(mount),'returncode':result.returncode})
            if result.returncode:status='FAIL'
        report['remaining_owned_mounts']=[line for line in Path('/proc/self/mountinfo').read_text().splitlines() if str(out) in line]
        if report['remaining_owned_mounts']:status='FAIL'
        for lock in locks:lock.close()
        if short is not None:
            # Logs/configs are copied before deleting only this task-owned short directory.
            try:
                shutil.copytree(short,out/'socket-configs',symlinks=True,ignore=shutil.ignore_patterns('*.sock'))
                for case in report['cases']:
                    archive=out/'socket-configs'/('ros'+str(case['version'])+'.conf')
                    if archive.is_file():case['gateway_config_archive']=read_file_binding(archive)
                for file in short.iterdir():file.unlink()
                short.rmdir()
            except BaseException as error:report['socket_cleanup_error']=repr(error);status='FAIL'
        report['status']=status;save()
        if status!='PASS':print(report.get('traceback',report.get('error','FAIL custom Guest evidence')),flush=True)
    return 0 if status=='PASS' else 1


if __name__=='__main__':raise SystemExit(main())
