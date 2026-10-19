#!/usr/bin/env python3
"""Build/execute new C++17 Action QoS probes with strict ARM/cgroup evidence.

This owns new outputs and a private mounted view of the immutable Jazzy SDK.
It reuses no historical native target and never registers its own binfmt entry.
prepare records GCC -MD project+SDK closure; execute accepts only that manifest.
All four probes are mandatory; an older two-probe preparation is rejected.
No Worker/Guest or complete 40-field RMW profile adoption claim is made.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import shutil
import signal
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state
from run_resources import bind_frozen_work
from build_rpc_reader_diagnostic import closure
from run_unit import process_tree_rss

PROBES = {'action-qos-load':'tests/ros_qos/gateway_load.cc',
          'action-qos-native':'tests/ros_qos/native_ros2.cc',
          'action-qos-cancel-contract':'tests/ros_qos/cancel_contract.cc',
          'action-qos-cancel-native':'tests/ros_qos/cancel_native_ros2.cc'}
PROBE_CONTRACTS = {
    'action-qos-load': {'source':PROBES['action-qos-load'],
        'marker':'PASS Gateway load Action contract cases=', 'links':['pthread'], 'ros_headers':False},
    'action-qos-native': {'source':PROBES['action-qos-native'],
        'marker':'PASS actual ROS2 incoming Action QoS legal_fields=35 denied_responses=145 forbidden_DDS_feedback=0 normal_DDS_feedback=1 goal_state_and_quota_preserved=1',
        'links':['pthread'], 'ros_headers':True},
    'action-qos-cancel-contract': {'source':PROBES['action-qos-cancel-contract'],
        'marker':'PASS cancellation comparator independent_fields=8 checks=', 'links':['pthread'], 'ros_headers':False},
    'action-qos-cancel-native': {'source':PROBES['action-qos-cancel-native'],
        'marker':'PASS actual cancellation preflight own_denials=8 later_any_mismatch_no_effect=1 matching_own_any_incoming=1 injected_actual_qos_refusals=4',
        'links':['pthread','${CMAKE_DL_LIBS}'], 'ros_headers':True},
}
DRIVER_INPUTS = ('run_action_qos.py','runtime.py','rebuild_affected.py','run_resources.py',
                 'build_rpc_reader_diagnostic.py','run_unit.py')
ORIGINAL_PROBES, ORIGINAL_CONTRACTS, ORIGINAL_DRIVER_INPUTS = PROBES, PROBE_CONTRACTS, DRIVER_INPUTS
ORIGINAL_SCOPE = ('Four CPP17 Action trusted-load, incoming response, cancellation contract/native probes; '
                  'no Guest or RMW 40-field adoption verdict; incoming actual-QoS faults are controlled test-only interposition')
ADOPTION_MARKER = ('PASS actual Action RMW adoption client_fields=40 server_fields=40 mutated_profiles=35 invalid_profiles=5 actual_profile_queries=576 '
                   'native_goals=1 native_feedback=1 native_result=1')
ADOPTION_SCOPE = ('One CPP17 actual RMW native Action client/server profile witness; unchanged constructor forwarding, '
                  'all five channels and forty fields per role queried from real native handles; '
                  '35 independently mutated valid profiles, five invalid history profiles refused before native construction, '
                  '576 actual-profile queries across mutations and baseline, independent RCL contexts; '
                  'no Guest admission or six-language delivery verdict')


def select_probe_set(value):
    """Select a closed source/marker inventory before preparation or execution.

    The original four probes and their raw reports remain a distinct suite.
    Selecting native-adoption never reuses or promotes their earlier results.
    """
    global PROBES, PROBE_CONTRACTS, DRIVER_INPUTS
    if value == 'original-four':
        PROBES, PROBE_CONTRACTS, DRIVER_INPUTS = ORIGINAL_PROBES, ORIGINAL_CONTRACTS, ORIGINAL_DRIVER_INPUTS
        return ORIGINAL_SCOPE
    PROBES = {'action-qos-adoption':'tests/ros_qos/native_adoption_ros2.cc'}
    PROBE_CONTRACTS = {'action-qos-adoption':{'source':PROBES['action-qos-adoption'],
        'marker':ADOPTION_MARKER,'links':['pthread','${CMAKE_DL_LIBS}'],'ros_headers':True}}
    DRIVER_INPUTS = ORIGINAL_DRIVER_INPUTS + ('run_native_adoption.py',)
    return ADOPTION_SCOPE
BARRIER = '''#include <csignal>
__attribute__((constructor)) static void arm_action_qos_evidence_stop() { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }
'''


def process_identity(pid):
    fields = Path(f'/proc/{pid}/stat').read_text().rsplit(')',1)[1].split()
    return {'pid':pid,'state':fields[0],'parent_pid':int(fields[1]),'starttime_ticks':int(fields[19])}


def owned_stopped_arm(evidence, process, name, root_birth, required):
    """Accept an actual ARM stop on this root or one strictly owned direct child.

    Rust/coreutils chroot can fork. Keep launcher and ARM identities separate;
    no grandchildren, command-line-only match, stale PID or other cgroup passes.
    """
    root = process_identity(process.pid)
    if root['starttime_ticks']!=root_birth or root['state'] in ('Z','X') or process.poll() is not None:
        raise RuntimeError('owned root identity changed before ARM evidence release')
    membership = evidence.before['membership'];live = set((evidence.leaf/'cgroup.procs').read_text().split())
    if Path(f'/proc/{process.pid}/cgroup').read_text().splitlines()!=membership or str(process.pid) not in live:
        raise RuntimeError('owned root is not live in the inherited cgroup')
    children = [int(value) for value in Path(f'/proc/{process.pid}/task/{process.pid}/children').read_text().split()]
    evidence.observe(process.pid,name)
    with evidence._lock:
        samples = list(evidence.samples.values());targets = {label:evidence.targets[label] for label in required}
        emulators = set(evidence.emulator_shas)
    parent_proof = any(row['name']==name and row['pid']==process.pid and row['starttime_ticks']==root_birth
        and row['ancestry']==[process.pid] and row['live_cgroup_procs'] and row['membership']==membership for row in samples)
    candidates = []
    for pid in [process.pid,*children]:
        try:
            actual = process_identity(pid)
            if actual['state'] not in ('T','t'):continue
            own_root = pid==process.pid and actual['starttime_ticks']==root_birth
            own_child = pid!=process.pid and parent_proof and actual['parent_pid']==process.pid and actual['starttime_ticks']>=root_birth
            if not (own_root or own_child):continue
            ancestry = [process.pid] if own_root else [process.pid,pid]
            if Path(f'/proc/{pid}/cgroup').read_text().splitlines()!=membership or str(pid) not in live:continue
            if not all(evidence.mapped(pid, target) for target in targets.values()):continue
            host = Path(f'/proc/{pid}/exe').stat()
            proofs = [row for row in samples if row['opened_target_mappings']['identity_after']['state'] in ('T','t') and row['name']==name and row['pid']==pid
                and row['starttime_ticks']==actual['starttime_ticks'] and row['parent_pid']==actual['parent_pid']
                and row['ancestry']==ancestry and row['live_cgroup_procs'] and row['membership']==membership
                and set(required).issubset(row['mapped_arm_targets']) and row['kernel_exe_elf']['e_machine']==62
                and row['kernel_exe_elf']['sha256'] in emulators
                and (row['kernel_exe_elf']['device'],row['kernel_exe_elf']['inode'])==(host.st_dev,host.st_ino)]
            if not proofs:continue
            if process_identity(pid)!=actual or process_identity(process.pid)['starttime_ticks']!=root_birth:continue
            candidates.append({'launcher':root,'actual_stopped_ARM':actual,'ancestry':ancestry,
                'target_proof':proofs[0],'current_mapping_check':True,'live_cgroup_membership':membership,
                'native_execution_pid_model':'root exec' if own_root else 'owned direct chroot child'})
        except (FileNotFoundError,ProcessLookupError):continue
    if len(candidates)>1:raise RuntimeError('multiple stopped ARM roots/children match one owned launch')
    return candidates[0] if candidates else None


def cmake():
    lines = ['cmake_minimum_required(VERSION 3.16)',
             'project(arm_action_qos LANGUAGES CXX)',
             'set(CMAKE_CXX_STANDARD 17)', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)',
             'find_package(ament_cmake REQUIRED)', 'find_package(rclcpp REQUIRED)',
             'find_package(rclcpp_action REQUIRED)', 'find_package(action_tutorials_interfaces REQUIRED)']
    for label, source in PROBES.items():
        lines += [f'add_executable({label} /work/{source})',
                  f'target_include_directories({label} PRIVATE /work/src)',
                  f'target_compile_options({label} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD -include /tmp/evidence-stop.hpp)',
                  f'ament_target_dependencies({label} rclcpp rclcpp_action action_tutorials_interfaces)',
                  f'target_link_libraries({label} '+ ' '.join(PROBE_CONTRACTS[label]['links'])+')']
    return '\n'.join(lines)+'\n'


def validate_prepared_contract(original, manifest_sha256):
    """Reject partial/old preparations before copying any executable."""
    if (original.get('status')!='PASS' or original.get('mode') not in ('prepare','all')
            or original.get('format_version')!=2 or set(original.get('probes',{}))!=set(PROBES)
            or original.get('probe_contracts')!=PROBE_CONTRACTS):
        raise RuntimeError('current exact selected Action probe preparation required')
    if original.get('source_manifest_sha256')!=manifest_sha256:
        raise RuntimeError('prepared source manifest differs from the current exact frozen QoS source')
    if (original.get('same_cgroup_incarnation') is not True
            or original.get('cgroup_incarnation_before')!=original.get('cgroup_incarnation_after')):
        raise RuntimeError('prepared compilation did not retain one exact cgroup incarnation')
    recorded = original.get('driver_inputs',{})
    if set(recorded)!=set(DRIVER_INPUTS):
        raise RuntimeError('prepared executed driver/helper input set differs')
    for name in DRIVER_INPUTS:
        if recorded[name].get('sha256')!=digest(Path(__file__).with_name(name)):
            raise RuntimeError('prepared driver/helper bytes differ from the current recipe: '+name)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=('prepare','execute','all'), required=True)
    parser.add_argument('--probe-set', choices=('original-four','native-adoption'), default='original-four',
        help='Independent closed native suite; prior four-probe receipts cannot qualify native adoption')
    for name in ('source','rootfs','out','qemu','binfmt-evidence'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--prepared', type=Path)
    parser.add_argument('--domain', type=int, default=74)
    parser.add_argument('--container-started-at', required=True,
        help='Exact Docker State.StartedAt captured by parent launch provenance')
    args = parser.parse_args(argv)
    scope = select_probe_set(args.probe_set)
    if args.mode == 'execute' and args.prepared is None:
        parser.error('execute requires --prepared new Action QoS preparation report')
    if args.mode != 'execute' and args.prepared is not None:
        parser.error('--prepared is supported only by execute')
    if not 0 <= args.domain <= 232:
        parser.error('domain must be 0..232')
    source, sdk = args.source.resolve(strict=True), args.rootfs.resolve(strict=True)
    manifest = source.parent/'source-manifest.json';frozen = json.loads(manifest.read_text())
    out = args.out.resolve();out.mkdir(parents=True, mode=0o700, exist_ok=False)
    root, temporary = out/'rootfs', out/'native-temp';root.mkdir();temporary.mkdir()
    mounts, children, evidence, initial, sdk_before, aliases_before, before = [], [], None, None, None, None, None
    domain_lock = None
    prepared_pins = []
    report = {'status':'RUNNING', 'format_version':2, 'mode':args.mode, 'domain':args.domain,
        'scope':scope, 'probe_set':args.probe_set,
        'probe_contracts':PROBE_CONTRACTS,
        'source':str(source), 'rootfs':str(sdk), 'source_manifest':str(manifest), 'source_manifest_sha256':digest(manifest),
        'commands':[], 'probes':{}, 'driver_inputs':{}}
    archive = out/'driver-inputs';archive.mkdir()
    for name in DRIVER_INPUTS:
        path = Path(__file__).with_name(name);copied = archive/name;shutil.copy2(path,copied)
        if digest(path)!=digest(copied):raise RuntimeError('executed helper changed while archiving: '+name)
        report['driver_inputs'][name] = {'path':str(path), 'archive':str(copied), 'sha256':digest(path)}
    status = 'FAIL'

    def save():
        (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')

    def checked(command):
        subprocess.run([str(value) for value in command],check=True,capture_output=True,timeout=30)

    def incarnation():
        stat = evidence.leaf.stat()
        return {'path':str(evidence.leaf), 'device':stat.st_dev, 'inode':stat.st_ino,
            'membership':Path('/proc/self/cgroup').read_text().splitlines(),
            'boot_id':Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
            'parent_recorded_container_started_at':args.container_started_at}

    def stop():
        for child in reversed(children):
            if child.poll() is None:
                try:
                    os.killpg(child.pid,signal.SIGCONT);os.killpg(child.pid,signal.SIGTERM);child.wait(timeout=3)
                except (ProcessLookupError,subprocess.TimeoutExpired):
                    try:os.killpg(child.pid,signal.SIGKILL)
                    except ProcessLookupError:pass
                    child.wait(timeout=3)

    def interrupted(signum, frame):
        raise RuntimeError('interrupted Action QoS driver signal '+str(signum))

    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        if sys.platform != 'linux' or os.geteuid() != 0:
            raise RuntimeError('BLOCKED root Linux private mount namespace required')
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('BLOCKED launch unshare -m inside the parent cgroup/binfmt lease')
        lease = json.loads(args.binfmt_evidence.read_text())
        if os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY')!=lease['name']:
            raise RuntimeError('exact parent binfmt lease environment is missing or differs')
        registered = (Path('/proc/sys/fs/binfmt_misc')/lease['name']).read_text()
        if registered != lease['entry'] or digest(args.qemu) != lease['qemu_sha256']:
            raise RuntimeError('active parent fixed QEMU lease changed')
        fields = dict(line.split(None,1) for line in registered.splitlines()[1:] if ' ' in line)
        magic, mask = bytes.fromhex(fields['magic']), bytes.fromhex(fields['mask'])
        if (not registered.startswith('enabled\n') or 'F' not in fields['flags:'] or
                fields['interpreter'] != str(args.qemu.resolve()) or magic[18:20] != b'\xb7\x00' or mask[18:20] != b'\xff\xff'):
            raise RuntimeError('exact fixed-interpreter ELF183 lease required')
        report['binfmt'] = {'path':str(args.binfmt_evidence),'sha256':digest(args.binfmt_evidence),'entry':registered}
        initial = source_state(source,frozen);sdk_before, aliases_before = sdk_inventory(sdk), sdk_root_aliases(sdk)
        (out/'sdk-before.json').write_text(json.dumps(sdk_before,indent=2)+'\n')
        report['sdk_before'] = {'path':str(out/'sdk-before.json'),'sha256':digest(out/'sdk-before.json')}
        report['sdk_root_aliases'] = aliases_before
        spec = importlib.util.spec_from_file_location('action_qos_cgroup',source/'tests/ros_cgroup/probe.py')
        probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution',probe.probe);evidence.emulator(args.qemu)
        before = incarnation();report['cgroup_incarnation_before'] = before
        checked(['mount','--make-rprivate','/']);checked(['mount','--rbind',sdk,root]);mounts.append(root)
        checked(['mount','--make-rprivate',root]);report['source_mount'] = bind_frozen_work(root,source,sdk,out,checked)
        checked(['mount','--bind',temporary,root/'tmp']);checked(['mount','--make-rprivate',root])
        home = out/'owned-home';home.mkdir();checked(['mount','--bind',home,root/'root'])
        for name in ('usr','opt','etc','var/lib/dpkg'):
            checked(['mount','--bind',sdk/name,root/name]);checked(['mount','-o','remount,bind,ro',root/name])
        for name in ('proc','sys','dev'):
            checked(['mount','--rbind',Path('/')/name,root/name]);checked(['mount','--make-rprivate',root/name])
        environment = dict(os.environ)
        for name in ('CC','CXX','LD_LIBRARY_PATH','LD_PRELOAD','QEMU_LD_PREFIX','CFLAGS','CXXFLAGS','LDFLAGS','FASTRTPS_DEFAULT_PROFILES_FILE','FASTDDS_DEFAULT_PROFILES_FILE'):
            environment.pop(name,None)
        environment.update(PYTHONDONTWRITEBYTECODE='1',ROS_DOMAIN_ID=str(args.domain),
            RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_SECURITY_ENABLE='false',ROS_LOG_DIR='/tmp/ros-logs')
        (temporary/'ros-logs').mkdir()

        def run(name, arguments, required, timeout, marker=None, stopped=False):
            command = ['/usr/bin/chroot',str(root),'/bin/bash','-lc',
                'source /opt/ros/jazzy/setup.bash && exec "$@"','arm-action-qos',*arguments]
            row = {'name':name,'command':command,'status':'FAIL','timeout_seconds':timeout};report['commands'].append(row);save()
            log_path = out/(name+'.log')
            with log_path.open('xb') as log:
                child = subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=environment,start_new_session=True)
            children.append(child);row['pid'] = child.pid
            root_birth = process_identity(child.pid)['starttime_ticks'];row['launcher_starttime_ticks'] = root_birth
            evidence.start(child,name+'/',command)
            if stopped:
                deadline = time.monotonic()+15
                while child.poll() is None and time.monotonic()<deadline:
                    proof = owned_stopped_arm(evidence,child,name+'/',root_birth,required)
                    if proof is not None:
                        evidence.check(required,name+'/');actual = proof['actual_stopped_ARM']
                        # pidfd pins the actual stopped process before signalling.
                        pinned = os.pidfd_open(actual['pid'])
                        try:
                            if process_identity(actual['pid'])!=actual:
                                raise RuntimeError('actual stopped ARM identity changed before resume')
                            evidence.before_resume(actual['pid'], actual)
                            signal.pidfd_send_signal(pinned,signal.SIGCONT)
                        finally:os.close(pinned)
                        row['actual_ARM_mapping_before_resume'] = proof;break
                    time.sleep(.002)
                if not row.get('actual_ARM_mapping_before_resume'):
                    raise RuntimeError('actual ARM evidence barrier was not reached: '+name)
            deadline = time.monotonic()+timeout;peak = 0
            while child.poll() is None:
                peak = max(peak,process_tree_rss(child.pid))
                if name=='compile' and peak>2*1024**3:raise RuntimeError('native compile subtree RSS exceeds 2 GiB')
                if time.monotonic()>deadline:raise TimeoutError('bounded Action QoS command deadline: '+name)
                time.sleep(.01)
            row['returncode'] = child.returncode;row['peak_observed_subtree_RSS_bytes'] = peak;row['log_sha256'] = digest(log_path)
            if row['returncode']:raise RuntimeError('Action QoS command returned failure: '+name)
            evidence.check(required,name+'/')
            if marker is not None and marker not in log_path.read_text(errors='replace'):
                raise RuntimeError('exact semantic completion marker missing: '+name)
            row['status'] = 'PASS';save();print('PASS',name,flush=True)

        if args.mode in ('prepare','all'):
            compiler = rooted(sdk,'/usr/bin/aarch64-linux-gnu-g++-13')
            frontend = next(rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus')
                for base in ('usr/lib','usr/libexec') if rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus').is_file())
            report['compiler'] = evidence.target('native-gcc',compiler)
            report['frontend'] = evidence.target('native-frontend',frontend)
            evidence.target('native-cmake',rooted(sdk,'/usr/bin/cmake'))
            (temporary/'evidence-stop.hpp').write_text(BARRIER)
            generated = temporary/'cmake';generated.mkdir();(generated/'CMakeLists.txt').write_text(cmake())
            run('configure',['/usr/bin/cmake','-S','/tmp/cmake','-B','/tmp/build',
                '-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-13','-DCMAKE_BUILD_TYPE=Release',
                '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'],['native-cmake'],120)
            run('compile',['/usr/bin/cmake','--build','/tmp/build','--parallel','1','--verbose'],
                ['native-cmake','native-gcc','native-frontend'],600)
            for label, relative in PROBES.items():
                binary = temporary/'build'/label;evidence.target(label,binary)
                meta = temporary/'build/CMakeFiles'/(label+'.dir');found = list(meta.rglob('*.o.d'))
                if len(found)!=1:raise RuntimeError('exact actual GCC dependency file required: '+label)
                copied = out/(label+'.d');shutil.copy2(found[0],copied)
                inputs = closure(copied,source,sdk,temporary)
                if '/work/'+relative not in inputs or '/tmp/evidence-stop.hpp' not in inputs or not any(k.startswith('/usr/') for k in inputs) or PROBE_CONTRACTS[label]['ros_headers'] and not any(k.startswith('/opt/') for k in inputs):
                    raise RuntimeError('actual GCC -MD closure missing original source, barrier or SDK: '+label)
                item = {'binary':require_arm(binary), 'raw_dependencies':{'path':str(copied),'sha256':digest(copied),'inputs':inputs},'metadata':{}}
                for name,path in [('flags',meta/'flags.make'),('link',meta/'link.txt'),
                        ('compile_commands',temporary/'build/compile_commands.json'),('cmake',generated/'CMakeLists.txt')]:
                    item['metadata'][name] = {'path':str(path),'sha256':digest(path)}
                if '${CMAKE_DL_LIBS}' in PROBE_CONTRACTS[label]['links'] and '-ldl' not in (meta/'link.txt').read_text().split():
                    raise RuntimeError('actual selected RMW witness link recipe lacks libdl')
                report['probes'][label] = item
        else:
            original = json.loads(args.prepared.read_text())
            validate_prepared_contract(original,report['source_manifest_sha256'])
            if original['cgroup_incarnation_before'] != before:
                raise RuntimeError('prepared compilation and behavior require the same inherited cgroup incarnation')
            architecture_ref = original['architecture_evidence'];architecture_path = Path(architecture_ref['path'])
            if digest(architecture_path)!=architecture_ref['sha256'] or json.loads(architecture_path.read_text()).get('status')!='PASS':
                raise RuntimeError('prepared actual compilation ARM/cgroup evidence changed or failed')
            if digest(original['sdk_before']['path']) != original['sdk_before']['sha256'] or json.loads(Path(original['sdk_before']['path']).read_text()) != sdk_before or original['sdk_root_aliases'] != aliases_before:
                raise RuntimeError('prepared complete SDK byte/keyset/root-alias closure differs')
            report['prepared'] = {'path':str(args.prepared),'sha256':digest(args.prepared),
                'actual_compilation_architecture_evidence':architecture_ref}
            prepared_pins.extend([{'path':str(args.prepared),'sha256':report['prepared']['sha256']},
                architecture_ref, original['sdk_before']])
            for label,item in original['probes'].items():
                original_binary = Path(item['binary']['path']);facts = require_arm(original_binary)
                if facts['sha256']!=item['binary']['sha256']:raise RuntimeError('prepared native binary changed: '+label)
                dep = item['raw_dependencies'];dep_path = Path(dep['path'])
                if digest(dep_path)!=dep['sha256']:raise RuntimeError('prepared GCC dependencies changed: '+label)
                original_temp = original_binary.parent.parent
                if closure(dep_path,Path(original['source']),Path(original['rootfs']),original_temp)!=dep['inputs']:
                    raise RuntimeError('prepared reported dependency exactset differs from actual original GCC -MD file: '+label)
                for logical,value in dep['inputs'].items():
                    path = source/logical[6:] if logical.startswith('/work/') else rooted(sdk,logical) if logical.startswith(('/usr/','/opt/')) else Path(value['path'])
                    if not path.is_file() or digest(path)!=value['sha256'] or path.stat().st_size!=value['bytes']:
                        raise RuntimeError('prepared source/SDK/generated input changed: '+logical)
                for value in item['metadata'].values():
                    if digest(value['path'])!=value['sha256']:raise RuntimeError('prepared raw codegen/link metadata changed')
                if '${CMAKE_DL_LIBS}' in PROBE_CONTRACTS[label]['links'] and '-ldl' not in Path(item['metadata']['link']['path']).read_text().split():
                    raise RuntimeError('prepared selected RMW witness link recipe lacks libdl')
                prepared_pins.extend([{'path':str(original_binary),'sha256':facts['sha256']},
                    {'path':str(dep_path),'sha256':dep['sha256']},*item['metadata'].values()])
                shutil.copy2(original_binary,temporary/label);evidence.target(label,temporary/label)
                report['probes'][label] = {**item,'binary':require_arm(temporary/label)}
        if args.mode in ('execute','all'):
            lock = Path('/tmp')/f'uwvm-arm-ros-domain-{args.domain}.lock'
            fd = os.open(lock,os.O_CREAT|os.O_EXCL|os.O_WRONLY,0o600);os.write(fd,str(os.getpid()).encode())
            stat = os.fstat(fd);os.close(fd);domain_lock = (lock,stat.st_dev,stat.st_ino)
            for label in PROBES:
                inside = '/tmp/'+label if args.mode=='execute' else '/tmp/build/'+label
                marker = PROBE_CONTRACTS[label]['marker']
                run('behavior-'+label,[inside],[label],90,marker,stopped=True)
            report['actual_native_behavior_probes'] = len(PROBES)
        status = 'PASS'
    except BaseException as error:
        report['error'] = traceback.format_exc()
        if 'BLOCKED' in str(error):status = 'BLOCKED'
    finally:
        stop()
        try:
            report['source_preserved'] = initial is not None and source_state(source,frozen)==initial and digest(manifest)==report['source_manifest_sha256']
            report['sdk_preserved'] = sdk_before is not None and sdk_inventory(sdk)==sdk_before and sdk_root_aliases(sdk)==aliases_before
            report['driver_inputs_preserved'] = all(digest(value['path'])==value['sha256'] and digest(value['archive'])==value['sha256'] for value in report['driver_inputs'].values())
            report['prepared_inputs_preserved'] = all(digest(value['path'])==value['sha256'] for value in prepared_pins)
            if not report['source_preserved'] or not report['sdk_preserved'] or not report['driver_inputs_preserved'] or not report['prepared_inputs_preserved']:status = 'FAIL'
        except BaseException:report['preservation_error'] = traceback.format_exc();status = 'FAIL'
        if evidence:
            result = evidence.finish(status);status = result['status']
            report['architecture_evidence'] = {'path':str(evidence.out/'architecture-evidence.json'),'sha256':digest(evidence.out/'architecture-evidence.json')}
            try:
                report['cgroup_incarnation_after'] = incarnation()
                report['same_cgroup_incarnation'] = before is not None and before==report['cgroup_incarnation_after']
            except BaseException:
                report['cgroup_incarnation_error'] = traceback.format_exc();report['same_cgroup_incarnation'] = False
            if not report['same_cgroup_incarnation']:status = 'FAIL'
        report['mount_cleanup'] = []
        for path in reversed(mounts):
            result = subprocess.run(['umount','-R',str(path)],capture_output=True,text=True,timeout=30)
            report['mount_cleanup'].append({'path':str(path),'returncode':result.returncode,'stderr':result.stderr})
            if result.returncode:status = 'FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4]==str(root) or line.split()[4].startswith(str(root)+'/')]
        if report['remaining_owned_mounts']:status = 'FAIL'
        if domain_lock:
            path, device, inode = domain_lock
            try:
                stat = path.stat()
                if (stat.st_dev,stat.st_ino)==(device,inode):path.unlink()
                else:status = 'FAIL'
            except FileNotFoundError:pass
        report['status'] = status;save()
    print(status,out/'report.json',flush=True)
    return 0 if status=='PASS' else 77 if status=='BLOCKED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
