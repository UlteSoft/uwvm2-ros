#!/usr/bin/env python3
"""Prepare/run exactly two actual fixed custom topic binding fixtures on ARM.

Linux root/private mount namespace and the existing verified 64-GiB zero-swap
cgroup are mandatory. Preparation never claims ROS behavior. This controller
does not execute a Guest or authenticate Gateway IPC and cannot complete G01.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import shutil
import signal
import socket
import subprocess
import sys
import time
import traceback
import xmlrpc.client

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm, read_json_binding, cgroup_incarnation
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state, custom_selection
from build_gateways import custom_version_values
from run_resources import bind_frozen_work
from run_ros1_search import OwnedProcesses
from run_action_qos import owned_stopped_arm, process_identity
from run_unit import process_tree_rss
from run_local_parameter_events import fixture_closure
from gateway_qualification import read_file_binding, pins_preserved, architecture_binding

PROBES = {1: {'label':'custom-ros1-native', 'source':'test/0073.ros-tests/support/source/ros_custom_types/ros1/native_topics.cc',
              'standard':17, 'gcc':9, 'distro':'noetic',
              'marker':'PASS ROS1 fixed custom topic native binding exact_body=23'},
          2: {'label':'custom-ros2-native', 'source':'test/0073.ros-tests/support/source/ros_custom_types/ros2/native_topics.cc',
              'standard':20, 'gcc':13, 'distro':'jazzy',
              'marker':'PASS prepared real ROS2 fixed custom topic fixture checks='}}
DRIVERS = ('run_custom_topics.py','runtime.py','rebuild_affected.py','build_gateways.py',
           'run_resources.py','run_ros1_search.py','run_local_parameter_events.py',
           'run_action_qos.py','run_unit.py','build_rpc_reader_diagnostic.py','gateway_qualification.py')
BARRIER = '#include <csignal>\n__attribute__((constructor)) static void custom_arm_evidence_stop() { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }\n'
ROS2_SHARDS = 3


def compilation_sources(version):
    """The complete fixture target, without projecting its provider type table."""
    sources = ['/work/'+PROBES[version]['source']]
    if version == 2:
        sources += ['/work/src/ros/gateway/ros2/custom_topics_dispatch.cc']
        sources += [f'/tmp/build/custom-topic-shards/custom_topics_shard_{index}.cc' for index in range(ROS2_SHARDS)]
    return sources


def generated_sources(version, source, temporary):
    """Reopen every configured shard and require exact frozen template expansion."""
    if version == 1: return {}
    template, _ = read_file_binding(source/'src/ros/gateway/ros2/custom_topics_shard.cc.in',retain=True)
    template = template.decode('utf-8')
    result = {}
    for index in range(ROS2_SHARDS):
        logical = f'/tmp/build/custom-topic-shards/custom_topics_shard_{index}.cc'
        path = temporary/logical[5:]
        expected = template.replace('@UWVM_ROS2_CUSTOM_TOPIC_SHARD_INDEX@',str(index)).replace('@UWVM_ROS2_CUSTOM_TOPIC_SHARDS@',str(ROS2_SHARDS))
        raw, binding = read_file_binding(path,retain=True)
        if '@UWVM_ROS2_CUSTOM_TOPIC_SHARD_' in expected or raw != expected.encode('utf-8'):
            raise RuntimeError('configured shard differs from the full frozen provider template')
        result[logical] = binding
    return result


def cmake(version, selection):
    """Generate the closed fixture target using its original source unchanged."""
    probe = PROBES[version]; label = probe['label']; packages = selection['packages']
    lines = ['cmake_minimum_required(VERSION 3.16)', 'project(arm_custom_topics LANGUAGES CXX)',
             f'set(CMAKE_CXX_STANDARD {probe["standard"]})', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)']
    if version == 1:
        # This harness builds precisely the original native fixture. catkin's
        # unrelated gtest/gmock targets would add unbuilt database entries.
        lines += ['set(CATKIN_ENABLE_TESTING OFF CACHE BOOL "Native custom fixture only" FORCE)',
                  'find_package(catkin REQUIRED COMPONENTS roscpp '+ ' '.join(packages)+')']
    else:
        lines += ['find_package(ament_cmake REQUIRED)', 'find_package(rclcpp REQUIRED)',
                  'find_package(rosidl_typesupport_cpp REQUIRED)']
        lines += [f'find_package({package} REQUIRED)' for package in packages]
    lines += [f'add_executable({label} /work/{probe["source"]})',
              f'target_include_directories({label} PRIVATE /work/src /work/third-parties/fast_io/include'+
              (' ${catkin_INCLUDE_DIRS}' if version == 1 else '')+')',
              # Noetic's one-source CMake 3.16 needs an explicit -MF. Jazzy's
              # multi-source rules supply a distinct target-owned -MF per TU;
              # a shared explicit dependency filename would overwrite proof.
              f'target_compile_options({label} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++{probe["standard"]} -MD'+
              (f' -MF /tmp/build/CMakeFiles/{label}.dir/native-topics-explicit.o.d' if version == 1 else '')+
              (' -include /tmp/evidence-stop.hpp)' if version == 1 else ')'),
              f'target_compile_definitions({label} PRIVATE UWVM_ROS_CUSTOM_TOPIC_MANIFEST="'+selection['registry_header']['guest_path']+'" '+
              f'UWVM_ROS_CUSTOM_ROS{version}_HEADER="'+selection['native_header']['guest_path']+'")']
    if version == 1:
        lines += [f'target_link_libraries({label} ${{catkin_LIBRARIES}} pthread)',
                  f'if(catkin_EXPORTED_TARGETS)\nadd_dependencies({label} ${{catkin_EXPORTED_TARGETS}})\nendif()']
    else:
        lines += [f'ament_target_dependencies({label} rclcpp rosidl_typesupport_cpp '+' '.join(packages)+')',
                  f'target_link_libraries({label} pthread)',
                  f'set(UWVM_ROS2_CUSTOM_TOPIC_SHARDS {ROS2_SHARDS} CACHE STRING "Complete fixture template partitions" FORCE)',
                  'include(/work/src/ros/gateway/ros2/custom_topics.cmake)',
                  f'uwvm_ros2_custom_topic_sources({label})',
                  # The one original fixture owns the pre-main SIGSTOP. Adding
                  # that constructor to every shard would stop it repeatedly.
                  f'set_source_files_properties(/work/{probe["source"]} PROPERTIES COMPILE_OPTIONS "-include;/tmp/evidence-stop.hpp")']
    return '\n'.join(lines)+'\n'


def marker_valid(version, text):
    """Require the exact original completion line, never an unrelated PASS."""
    marker = PROBES[version]['marker']; lines = text.splitlines()
    selected = [line for line in lines if line.startswith(marker)]
    expected = selected == [marker] if version == 1 else len(selected) == 1 and selected[0][len(marker):].isdecimal() and int(selected[0][len(marker):]) > 0
    return expected and not any(line.startswith('FAIL ') for line in lines)


def compiler_recipes(version, database, log):
    """Bind every configured TU to one actual verbose compilation and final -MF."""
    probe = PROBES[version]; label = probe['label']; sources = compilation_sources(version)
    compiler = '/usr/bin/aarch64-linux-gnu-g++-'+str(probe['gcc'])
    if (len(database) != len(sources) or sorted(row.get('file','') for row in database) != sorted(sources) or
            any(row.get('directory') != '/tmp/build' for row in database)):
        raise RuntimeError('complete exact fixture/shard compile database source set required')
    def options(argv, source, actual):
        if not argv or argv[0] != compiler or argv.count('-c') != 1 or argv[argv.index('-c')+1:] != [source]:
            raise RuntimeError('actual fixture/shard source/compiler differs')
        cleaned, outputs = [], []; standard = optimization = debug = ndebug = None; index = 0; shard_count = None
        while index < len(argv):
            token = argv[index]
            if token.startswith(('@','-Wp,')) or token in ('-M','-MM','-MMD','-E','-S','-fsyntax-only','-MG','-MP','-Xpreprocessor'):
                raise RuntimeError('opaque or non-system-inclusive compile recipe')
            if token in ('-MD','-MF','-MT','-MQ'):
                if token != '-MD':
                    index += 1
                    if index == len(argv): raise RuntimeError('missing dependency option operand')
                    if token == '-MF': outputs.append(argv[index])
            elif token.startswith(('-MF','-MT','-MQ')):
                if token.startswith('-MF'): outputs.append(token[3:])
            elif token in ('-D','-U'):
                cleaned.append(token);index += 1
                if index == len(argv): raise RuntimeError('missing macro operand')
                cleaned.append(argv[index]); macro, _, value = argv[index].partition('=')
                if macro == 'NDEBUG': ndebug = token == '-D'
                if macro == 'UWVM_ROS2_CUSTOM_TOPIC_SHARDS': shard_count = value if token == '-D' else None
            else:
                cleaned.append(token)
                if token.startswith('-std='): standard = token
                if token.startswith('-O'): optimization = token
                if token.startswith('-g'): debug = token
                if token.startswith(('-D','-U')):
                    macro, _, value = token[2:].partition('=')
                    if macro == 'NDEBUG': ndebug = token.startswith('-D')
                    if macro == 'UWVM_ROS2_CUSTOM_TOPIC_SHARDS': shard_count = value if token.startswith('-D') else None
            index += 1
        if '-MD' not in argv or (actual and not outputs) or (standard,optimization,debug,ndebug) != (f'-std=gnu++{probe["standard"]}','-O1','-g0',False):
            raise RuntimeError('effective fixture standard/O1/g0/UNDEBUG/-MD differs')
        if version == 2 and shard_count != str(ROS2_SHARDS):
            raise RuntimeError('every actual ROS2 TU must use the complete configured shard partition')
        if source == '/work/'+probe['source']:
            if cleaned.count('-include') != 1 or cleaned[cleaned.index('-include')+1] != '/tmp/evidence-stop.hpp':
                raise RuntimeError('exact original fixture constructor barrier operand required')
        elif '-include' in cleaned:
            raise RuntimeError('dispatcher/shard must not duplicate the fixture SIGSTOP constructor')
        return cleaned, posixpath.normpath(posixpath.join('/tmp/build',outputs[-1])) if outputs else None
    expected = {row['file']:options(row.get('arguments') or shlex.split(row['command']),row['file'],False)[0] for row in database}
    matches = {source:[] for source in sources}
    for number, line in enumerate(log.splitlines(),1):
        if compiler not in line: continue
        argv = shlex.split(line)
        if argv[:3] == ['cd','/tmp/build','&&']: argv = argv[3:]
        if not argv or argv[0] != compiler or '-c' not in argv: continue
        if argv.count('-c') != 1 or argv.index('-c')+1 >= len(argv): raise RuntimeError('actual compiler source operand missing')
        source = argv[argv.index('-c')+1]
        if source not in matches: raise RuntimeError('undeclared actual target compilation')
        actual, depfile = options(argv,source,True)
        if actual != expected[source] or not depfile.startswith('/tmp/build/CMakeFiles/'+label+'.dir/') or not depfile.endswith('.d'):
            raise RuntimeError('verbose compiler argv or final -MF differs')
        if argv.count('-o') != 1 or argv.index('-o')+1 >= len(argv): raise RuntimeError('one actual object output required')
        obj = posixpath.normpath(posixpath.join('/tmp/build',argv[argv.index('-o')+1]))
        if not obj.startswith('/tmp/build/CMakeFiles/'+label+'.dir/') or not obj.endswith('.o'):
            raise RuntimeError('compiled object escapes its owned target')
        matches[source].append({'source_guest_path':source,'argv':argv,'working_directory':'/tmp/build','verbose_line':number,
                                'effective_mf':depfile,'compiled_object_guest_path':obj})
    if any(len(rows) != 1 for rows in matches.values()):
        raise RuntimeError('one successful actual verbose compilation per complete target TU required')
    recipes = [matches[source][0] for source in sources]
    for key in ('compiled_object_guest_path','effective_mf'):
        if len({row[key] for row in recipes}) != len(recipes): raise RuntimeError('TUs share an object or dependency output')
    return recipes


def validate_link(version, recipes, link, verbose):
    """Require one actual final link of exactly all distinct qualified objects."""
    argv = shlex.split(link); label = PROBES[version]['label']
    objects = [posixpath.normpath(posixpath.join('/tmp/build',item)) for item in argv if item.endswith('.o')]
    expected = [recipe['compiled_object_guest_path'] for recipe in recipes]
    if (not argv or argv[0] != recipes[0]['argv'][0] or argv.count('-o') != 1 or argv.index('-o')+1 >= len(argv) or
        posixpath.normpath(posixpath.join('/tmp/build',argv[argv.index('-o')+1])) != '/tmp/build/'+label or
        sorted(objects) != sorted(expected) or len(set(objects)) != len(objects) or
        any(item.startswith(('@','-Wp,')) for item in argv)):
        raise RuntimeError('complete fixture object/link output bijection differs')
    lines = [shlex.split(line) for line in verbose.splitlines() if argv[0] in line]
    normalized = [line[3:] if line[:3] == ['cd','/tmp/build','&&'] else line for line in lines]
    if normalized.count(argv) != 1: raise RuntimeError('one actual successful verbose link required')
    return {'argv':argv,'working_directory':'/tmp/build','compiled_objects':expected,
            'scope':'Exact actual link argv, including SDK libraries, runtime search paths and dynamic link flags'}


def file_api_sources(version, temporary):
    """Cross-check configured CXX sources; actual compile/link evidence is separate."""
    reply = temporary/'build/.cmake/api/v1/reply'; indexes = list(reply.glob('index-*.json')); bindings = []
    if len(indexes) != 1: raise RuntimeError('one fresh File API index required')
    index, binding = read_json_binding(indexes[0]);bindings.append(binding)
    selected = index.get('reply',{}).get('codemodel-v2',{})
    def document(name):
        if not isinstance(name,str) or not name or Path(name).name != name: raise RuntimeError('File API metadata escapes reply root')
        value, binding = read_json_binding(reply/name);bindings.append(binding);return value
    if selected.get('kind') != 'codemodel' or selected.get('version',{}).get('major') != 2:
        raise RuntimeError('actual codemodel-v2 reply required')
    model = document(selected.get('jsonFile'))
    if len(model['configurations']) != 1 or model['configurations'][0]['name'] != 'Release':
        raise RuntimeError('one actual Release File API configuration required')
    targets = [row for row in model['configurations'][0]['targets'] if row['name'] == PROBES[version]['label']]
    if len(targets) != 1: raise RuntimeError('one exact executable target required')
    target = document(targets[0].get('jsonFile'))
    if target['name'] != PROBES[version]['label'] or target['type'] != 'EXECUTABLE': raise RuntimeError('File API target differs')
    sources = []
    for row in target['sources']:
        if 'compileGroupIndex' not in row: continue
        if target['compileGroups'][row['compileGroupIndex']]['language'] != 'CXX': raise RuntimeError('unexpected compiled target language')
        sources.append(posixpath.normpath(posixpath.join(model['paths']['source'],row['path'])))
    if sorted(sources) != sorted(compilation_sources(version)) or len(set(sources)) != len(sources):
        raise RuntimeError('File API is not the complete exact configured source set')
    return {'sources':sources,'metadata':bindings,'scope':'Configured sources only; each still requires actual verbose compiler, ELF, full-MD and link proof'}


def prepared_temporary(item):
    """Recover the original build root independently of per-TU -MF nesting."""
    cmake_path=Path(item['metadata']['cmake']['path'])
    if cmake_path.name!='CMakeLists.txt' or cmake_path.parent.name!='cmake':
        raise RuntimeError('prepared generated CMake location differs')
    return cmake_path.parent.parent


def compilation_mapping(version, recipe, samples, command, membership, emulator_shas):
    """Bind each TU's actual GCC and frontend argv to observed ARM/QEMU births."""
    required = {f'ros{version}-native-gcc',f'ros{version}-native-frontend'}
    selected = [sample for sample in samples if sample.get('name') == f'compile-ros{version}/' and
        sample.get('ancestry',[None])[0] == command['pid'] and sample.get('live_cgroup_procs') is True and
        sample.get('membership') == membership and recipe['source_guest_path'] in sample.get('command',[]) and
        sample.get('starttime_ticks',0) >= command['launcher_starttime_ticks'] and
        sample.get('kernel_exe_elf',{}).get('e_machine') == 62 and
        sample['kernel_exe_elf'].get('sha256') in emulator_shas and
        required.intersection(sample.get('mapped_arm_targets',[]))]
    if not required <= {label for sample in selected for label in sample.get('mapped_arm_targets',[])}:
        raise RuntimeError('each TU requires its own actual compiler/frontend source argv and ARM mapping')
    keys = ('pid','parent_pid','ancestry','starttime_ticks','capture_monotonic_ns','command','mapped_arm_targets')
    return [{key:sample[key] for key in keys} for sample in selected]


def ros1_peer_proof(text, row, evidence, owned):
    """Bind the fixture's live TCPROS peer to its observed ARM descendant birth."""
    lines=[line for line in text.splitlines() if line.startswith('TRANSPORT ROS1 ')]
    if len(lines)!=1:
        raise RuntimeError('one exact native ROS1 TCPROS transport record required')
    match=re.fullmatch(r'TRANSPORT ROS1 inbound=TCPROS outbound=TCPROS child_pid=([1-9][0-9]*) parent_pid=([1-9][0-9]*)',lines[0])
    if not match:
        raise RuntimeError('closed native ROS1 transport record differs')
    child,parent=map(int,match.groups())
    actual=row['actual_ARM_mapping_before_resume']['actual_stopped_ARM']
    if parent!=actual['pid'] or child==parent:
        raise RuntimeError('native ROS1 peer record differs from the actual ARM parent')
    candidates=[item for item in owned.rows.values() if item['label']=='behavior-ros1' and
        item['identity']['pid']==child and item['identity']['parent_pid']==parent and
        item['identity']['starttime_ticks']>=actual['starttime_ticks'] and
        item['ancestry'][0]==row['pid']]
    if len(candidates)!=1:
        raise RuntimeError('native ROS1 peer lacks one exact observed owned descendant birth')
    candidate=candidates[0]
    with evidence._lock:
        samples=[sample for sample in evidence.samples.values() if sample['name']=='behavior-ros1/' and
            sample['pid']==child and sample['starttime_ticks']==candidate['identity']['starttime_ticks'] and
            sample['ancestry']==candidate['ancestry'] and sample['live_cgroup_procs'] is True and
            sample['membership']==evidence.before['membership'] and 'custom-ros1-native' in sample['mapped_arm_targets'] and
            sample['kernel_exe_elf']['e_machine']==62 and sample['kernel_exe_elf']['sha256'] in evidence.emulator_shas]
    if not samples:
        raise RuntimeError('native ROS1 peer lacks actual ARM/QEMU mapping in the verified cgroup')
    return {'identity':candidate['identity'],'ancestry':candidate['ancestry'],'actual_ARM_mapping':samples[-1],
            'transport_record':lines[0],'scope':'Fork before ROS initialization, opposite caller IDs, actual connected TCPROS bus rows and exact child ACK/reap'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=('prepare','run','all'), required=True)
    for name in ('source','sdk-workspace','out','qemu','binfmt-evidence','custom-registry-header','custom-prefix'):
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--custom-native-header',action='append',required=True,help='Repeat exactly 1=header and 2=header')
    parser.add_argument('--custom-packages',action='append',required=True,help='Repeat exactly 1=pkg,... and 2=pkg,...')
    parser.add_argument('--prepared',type=Path)
    parser.add_argument('--container-started-at',required=True)
    parser.add_argument('--master-port',type=int,default=11389)
    args = parser.parse_args()
    if (args.mode == 'run') != (args.prepared is not None): parser.error('run requires --prepared; prepare/all must omit it')
    if not 1024 <= args.master_port <= 65535: parser.error('master port must be 1024..65535')
    headers, packages = custom_version_values(args.custom_native_header), custom_version_values(args.custom_packages)
    if set(headers) != {'1','2'} or set(packages) != {'1','2'}: parser.error('both exact native header/package versions are required')
    if sys.platform != 'linux' or os.geteuid() != 0: raise RuntimeError('Linux root/private namespace required; no local native run')
    if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'): raise RuntimeError('launch with unshare -m')
    source = args.source.resolve(strict=True); sdk_work = args.sdk_workspace.resolve(strict=True)
    sdks = {v:sdk_work/'gateway'/(p['distro']+'-rootfs') for v,p in PROBES.items()}
    out = args.out.resolve()
    if out.is_relative_to(source) or any(out.is_relative_to(sdk) for sdk in sdks.values()): raise RuntimeError('fresh output must be separate from source and SDK')
    if any(any(c.isspace() or c == '\\' for c in str(path)) for path in (source,out,*sdks.values())): raise RuntimeError('mount/dependency paths must not require mountinfo escaping')
    out.mkdir(parents=True,mode=0o700,exist_ok=False)
    report = {'status':'RUNNING','kind':'fixed-custom-topics-arm','format_version':2,'mode':args.mode,
              'scope':'Two native typed topic binding fixtures only; no authenticated Gateway IPC, Guest or complete G01 verdict',
              'actual_native_behavior_probes':0,'wasm_execution':'NOT_RUN','gateway_ipc_execution':'NOT_RUN','original_G01_complete':False,
              'commands':[],'probes':{},'driver_inputs':{},'selections':{},'tools':{}}
    mounts, children, locks, roots, temporaries, pins = [], [], [], {}, {}, []
    pin_index = {}
    evidence = owned = master = before = initial = None; snapshots = {}; aliases = {}; status = 'FAIL';stopping=False
    def save(): (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    def pin(path):
        fact = read_file_binding(path); prior = pin_index.get(fact['path'])
        if prior is not None and prior != fact: raise RuntimeError('previously pinned immutable input changed')
        if prior is None: pin_index[fact['path']] = fact;pins.append(fact)
        return fact
    def checked(argv): subprocess.run([str(x) for x in argv],check=True,capture_output=True,timeout=30)
    def incarnation():
        return {**cgroup_incarnation(evidence.leaf),'membership':Path('/proc/self/cgroup').read_text().splitlines(),
                'memory.swap.max':(evidence.leaf/'memory.swap.max').read_text().strip(),'container_started_at':args.container_started_at}
    def monitor():
        owned.sample()
        if incarnation() != before or int((evidence.leaf/'memory.current').read_text()) > 60*1024**3: raise RuntimeError('current cgroup identity/swap/headroom differs')
        if (Path('/proc/sys/fs/binfmt_misc')/name).read_text() != registered or os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != name:
            raise RuntimeError('active parent binfmt lease changed')
        if master is not None and master.poll() is not None: raise RuntimeError('owned ROS1 Master exited')
    def mapped(pid,target):
        return evidence.mapped(pid, target)
    def lease_lock(path):
        descriptor = os.open(path,os.O_CREAT|os.O_EXCL|os.O_WRONLY,0o600)
        try: os.write(descriptor,str(os.getpid()).encode()); fact=os.fstat(descriptor)
        finally: os.close(descriptor)
        locks.append((path,fact.st_dev,fact.st_ino))
    def interrupted(signum,frame):
        nonlocal stopping
        stopping=True
        raise RuntimeError('custom fixture interrupted: '+str(signum))
    signal.signal(signal.SIGTERM,interrupted); signal.signal(signal.SIGINT,interrupted)
    try:
        frozen, manifest_pin = read_json_binding(source.parent/'source-manifest.json'); pins.append(manifest_pin)
        report['source_manifest_sha256'] = manifest_pin['sha256']; report['source'] = str(source); report['sdk_workspace'] = str(sdk_work)
        initial = source_state(source,frozen)
        archive = out/'driver-inputs'; archive.mkdir()
        for name in DRIVERS:
            original=Path(__file__).with_name(name)
            if original.resolve() != (source/'test/0073.ros-tests/support/source/ros_aarch64'/name).resolve() or digest(original) != initial.get('test/0073.ros-tests/support/source/ros_aarch64/'+name): raise RuntimeError('driver/helper is outside frozen source')
            copy=archive/name; shutil.copy2(original,copy)
            report['driver_inputs'][name]={'original_binding':pin(original),'archive_binding':pin(copy)}
        spec=importlib.util.spec_from_file_location('custom_fixture_cgroup',source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py'); module=importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
        evidence=Evidence(out/'execution',module.probe); report['qemu']=evidence.emulator(args.qemu); pin(args.qemu)
        before=incarnation(); report['cgroup_incarnation_before']=before
        if before['memory.swap.max'] != '0': raise RuntimeError('zero swap required')
        owned=OwnedProcesses(evidence)
        lease, lease_pin=read_json_binding(args.binfmt_evidence); pins.append(lease_pin)
        name=lease.get('name')
        if not isinstance(name,str) or '/' in name or name in ('','.','..'): raise RuntimeError('invalid parent lease name')
        registered=(Path('/proc/sys/fs/binfmt_misc')/name).read_text(); fields=dict(line.split(None,1) for line in registered.splitlines()[1:] if ' ' in line)
        if (os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY')!=name or registered!=lease['entry'] or digest(args.qemu)!=lease['qemu_sha256'] or
            not registered.startswith('enabled\n') or 'F' not in fields['flags:'] or fields['interpreter']!=str(args.qemu.resolve()) or
            bytes.fromhex(fields['magic'])[18:20]!=b'\xb7\x00' or bytes.fromhex(fields['mask'])[18:20]!=b'\xff\xff'):
            raise RuntimeError('exact active fixed ELF183/QEMU lease required')
        report['binfmt']={**lease_pin,'entry':registered}
        report['memory_swap_max_before']=before['memory.swap.max']
        checked(['mount','--make-rprivate','/'])
        environment=dict(os.environ)
        for key in ('CC','CXX','LD_LIBRARY_PATH','LD_PRELOAD','QEMU_LD_PREFIX','CFLAGS','CXXFLAGS','LDFLAGS','PYTHONHOME','PYTHONPATH','ROS_HOSTNAME','ROS_NAMESPACE','FASTRTPS_DEFAULT_PROFILES_FILE','FASTDDS_DEFAULT_PROFILES_FILE'):
            environment.pop(key,None)
        environment.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',PYTHONDONTWRITEBYTECODE='1',
            ROS_MASTER_URI='http://127.0.0.1:'+str(args.master_port),ROS_IP='127.0.0.1',ROS_DOMAIN_ID='203',RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_SECURITY_ENABLE='false',ROS_LOG_DIR='/tmp/ros-logs')
        for version,probe in PROBES.items():
            sdk=sdks[version]
            if not (sdk/'.uwvm-task-owned').is_file(): raise RuntimeError('task-owned canonical SDK required')
            selection=custom_selection(version,args.custom_registry_header,headers[str(version)],[packages[str(version)]],args.custom_prefix,source,sdk,frozen)
            report['selections'][str(version)]=selection; pins.extend(selection['input_pins'])
            snapshots[version]=sdk_inventory(sdk); aliases[version]=sdk_root_aliases(sdk)
            snapshot=out/f'ros{version}-sdk-before.json';snapshot.write_text(json.dumps(snapshots[version],indent=2)+'\n')
            report.setdefault('sdk_before',{})[str(version)]=pin(snapshot)
            version_out=out/f'ros{version}';version_out.mkdir()
            root=version_out/'rootfs'; temporary=version_out/'tmp';root.mkdir();temporary.mkdir(); roots[version]=root;temporaries[version]=temporary
            checked(['mount','--rbind',sdk,root]);mounts.append(root);checked(['mount','--make-rprivate',root]);bind_frozen_work(root,source,sdk,version_out,checked)
            checked(['mount','--bind',temporary,root/'tmp']);home=version_out/'home';home.mkdir();checked(['mount','--bind',home,root/'root'])
            for directory in ('usr','opt','etc','var/lib/dpkg'):
                checked(['mount','--bind',sdk/directory,root/directory]);checked(['mount','-o','remount,bind,ro',root/directory])
            for directory in ('proc','sys','dev'):
                checked(['mount','--rbind',Path('/')/directory,root/directory]);checked(['mount','--make-rprivate',root/directory])
            (temporary/'ros-logs').mkdir()
            compiler=rooted(sdk,'/usr/bin/aarch64-linux-gnu-g++-'+str(probe['gcc']))
            frontend=next(rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/'+str(probe['gcc'])+'/cc1plus') for base in ('usr/lib','usr/libexec') if rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/'+str(probe['gcc'])+'/cc1plus').is_file())
            for suffix,path in [('gcc',compiler),('frontend',frontend),('cmake',rooted(sdk,'/usr/bin/cmake'))]:
                label=f'ros{version}-native-'+suffix;report['tools'][label]=evidence.target(label,path);pin(path)

        def run(version,name,arguments,required,timeout,stopped=False):
            probe=PROBES[version];command=['/usr/bin/chroot',str(roots[version]),'/bin/bash','-lc',
                'source /opt/ros/'+probe['distro']+'/setup.bash && export LD_LIBRARY_PATH=/opt/uwvm-custom-types/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} && exec "$@"','arm-custom-topics',*arguments]
            log_path=out/(name+'.log');row={'name':name,'version':version,'command':command,'status':'FAIL','timeout_seconds':timeout};report['commands'].append(row);save()
            with log_path.open('xb') as stream: child=subprocess.Popen(command,stdout=stream,stderr=subprocess.STDOUT,env=environment,start_new_session=True)
            children.append(child);owned.acquire(child.pid,name,[child.pid]);birth=process_identity(child.pid)['starttime_ticks'];row.update(pid=child.pid,launcher_starttime_ticks=birth);evidence.start(child,name+'/',command)
            deadline=time.monotonic()+timeout;peak=0
            if stopped:
                barrier_deadline=min(deadline,time.monotonic()+15)
                while child.poll() is None and time.monotonic()<barrier_deadline:
                    monitor();proof=owned_stopped_arm(evidence,child,name+'/',birth,[required[0]])
                    if proof is not None:
                        actual=proof['actual_stopped_ARM'];fd=os.pidfd_open(actual['pid'])
                        try:
                            current=owned_stopped_arm(evidence,child,name+'/',birth,[required[0]])
                            if current is None or current['actual_stopped_ARM']!=actual or process_identity(actual['pid'])!=actual: raise RuntimeError('stopped fixture identity/map changed')
                            evidence.before_resume(actual['pid'], actual)
                            signal.pidfd_send_signal(fd,signal.SIGCONT);row['actual_ARM_mapping_before_resume']=current
                        finally: os.close(fd)
                        break
                    time.sleep(.002)
                if 'actual_ARM_mapping_before_resume' not in row: raise RuntimeError('actual pre-main ARM barrier proof unavailable')
            while child.poll() is None:
                monitor();peak=max(peak,process_tree_rss(child.pid),owned.rss(name))
                if peak>2*1024**3 or time.monotonic()>deadline: raise RuntimeError('owned command exceeds RSS/deadline')
                if log_path.stat().st_size>16*1024**2: raise RuntimeError('bounded command log exceeded')
                time.sleep(.01)
            row.update(returncode=child.returncode,peak_observed_subtree_RSS_bytes=peak,log=pin(log_path))
            if child.returncode: raise RuntimeError('fixture command failed: '+name)
            evidence.check(required,name+'/')
            if stopped and not marker_valid(version,log_path.read_text(errors='replace')): raise RuntimeError('exact unique semantic fixture marker missing')
            if stopped and version==1: row['native_peer_proof']=ros1_peer_proof(log_path.read_text(errors='replace'),row,evidence,owned)
            if not pins_preserved(pins): raise RuntimeError('pinned compiler/input identity changed during command')
            row['status']='PASS';save()

        def start_master():
            nonlocal master
            try:
                with socket.create_connection(('127.0.0.1',args.master_port),.2):
                    raise RuntimeError('selected isolated Master port is already listening')
            except ConnectionRefusedError:pass
            command=['/usr/bin/chroot',str(root),'/bin/bash','-lc',
                'source /opt/ros/noetic/setup.bash && exec "$@"','arm-ros1-master','/opt/ros/noetic/bin/roscore','-p',str(args.master_port)]
            log=out/'ros1-master.log'
            with log.open('xb') as stream:master=subprocess.Popen(command,stdout=stream,stderr=subprocess.STDOUT,env=environment,start_new_session=True)
            children.append(master);owned.acquire(master.pid,'ros1-master',[master.pid]);evidence.start(master,'ros1-master/',command)
            evidence.target('ros1-native-python',rooted(sdk,'/usr/bin/python3'))
            class ShortTransport(xmlrpc.client.Transport):
                def make_connection(self,host):
                    connection=super().make_connection(host);connection.timeout=.5;return connection
            deadline=time.monotonic()+30;ready=False
            while time.monotonic()<deadline:
                monitor()
                try:
                    with xmlrpc.client.ServerProxy(environment['ROS_MASTER_URI'],transport=ShortTransport()) as client:
                        response=client.getUri('/uwvm_arm_ros1_search_driver')
                        pid_response=client.getPid('/uwvm_arm_ros1_search_driver')
                    ready=isinstance(response,list) and len(response)==3 and response[0]==1 and response[2].rstrip('/')==environment['ROS_MASTER_URI']
                    ready=ready and isinstance(pid_response,list) and len(pid_response)==3 and pid_response[0]==1 and type(pid_response[2]) is int
                    if ready:
                        owned.sample();pid=pid_response[2]
                        candidates=[row for row in owned.rows.values() if row['label']=='ros1-master' and row['identity']['pid']==pid
                            and owned.alive(row) and owned.check(row)]
                        if not candidates:raise RuntimeError('readiness Master PID is not an observed live owned birth')
                        candidate=candidates[0];actual=process_identity(pid)
                        if actual['starttime_ticks']!=candidate['identity']['starttime_ticks']:
                            raise RuntimeError('readiness Master birth changed')
                        evidence.observe(master.pid,'ros1-master/')
                        host=Path(f'/proc/{pid}/exe').stat()
                        with evidence._lock:
                            samples=[sample for sample in evidence.samples.values() if sample['name']=='ros1-master/'
                                and sample['pid']==pid and sample['starttime_ticks']==actual['starttime_ticks']
                                and sample['ancestry']==candidate['ancestry'] and sample['live_cgroup_procs']
                                and sample['membership']==evidence.before['membership']
                                and 'ros1-native-python' in sample['mapped_arm_targets']
                                and sample['kernel_exe_elf']['e_machine']==62 and sample['kernel_exe_elf']['sha256'] in evidence.emulator_shas
                                and (sample['kernel_exe_elf']['device'],sample['kernel_exe_elf']['inode'])==(host.st_dev,host.st_ino)]
                        if (not samples or not mapped(pid,evidence.targets['ros1-native-python']) or not owned.check(candidate)
                                or process_identity(pid)['starttime_ticks']!=actual['starttime_ticks']):
                            raise RuntimeError('readiness Master PID lacks current owned ARM Python/QEMU mapping proof')
                        master_proof={'identity':actual,'ancestry':candidate['ancestry'],'target_proof':samples[-1],
                            'current_mapping_check':True,'scope':'Actual readiness XMLRPC getPid bound to an observed live owned Master birth'}
                        break
                except (OSError,xmlrpc.client.Error):pass
                time.sleep(.02)
            if not ready:raise RuntimeError('bounded actual isolated ROS1 Master readiness failed')
            evidence.check(['ros1-native-python'],'ros1-master/')
            report['master']={'pid':master.pid,'command':command,'log':str(log),'uri':environment['ROS_MASTER_URI'],
                'status':'READY','readiness_reply':response,'readiness_pid_reply':pid_response,
                'actual_readiness_master_mapping':master_proof,'timeout_seconds':30}

        def closure(version, temporary, recipe, generated):
            dep = temporary/recipe['effective_mf'][5:]
            inputs, dep_pin = fixture_closure(dep,source,sdks[version],temporary,recipe);pins.append(dep_pin)
            selection = report['selections'][str(version)];probe = PROBES[version]
            required = {recipe['source_guest_path'],selection['registry_header']['guest_path'],selection['native_header']['guest_path']}
            if recipe['source_guest_path'] == '/work/'+probe['source']: required.add('/tmp/evidence-stop.hpp')
            if not required <= set(inputs):
                raise RuntimeError('each actual full-MD closure requires its exact source/providers and the original fixture barrier')
            if not any(name.startswith('/usr/include/c++/') for name in inputs) or not any(name.startswith('/opt/ros/'+probe['distro']+'/') for name in inputs):
                raise RuntimeError('per-TU actual system/ROS SDK dependency closure unavailable')
            for logical,input_ in inputs.items():
                actual = Path(input_['path']);input_pin = pin(actual)
                if logical.startswith('/work/'):
                    if initial.get(logical[6:]) != input_pin['sha256']: raise RuntimeError('TU project dependency differs from frozen source')
                elif logical.startswith('/tmp/'):
                    if logical != '/tmp/evidence-stop.hpp' and generated.get(logical) != input_pin:
                        raise RuntimeError('undeclared or changed generated compiler input')
                elif snapshots[version].get(str(actual.relative_to(sdks[version])),{}).get('sha256') != input_pin['sha256']:
                    raise RuntimeError('TU native dependency differs from full before SDK snapshot')
            return inputs, dep_pin

        def archive_product(path, destination):
            original = pin(path);shutil.copy2(path,destination);archived = pin(destination)
            if (original['sha256'],original['bytes']) != (archived['sha256'],archived['bytes']):
                raise RuntimeError('TU source/object/full-MD archive changed bytes')
            return {'original':original,'archive':archived}

        if args.mode in ('prepare','all'):
            for version,probe in PROBES.items():
                temporary=temporaries[version]; selection=report['selections'][str(version)];generated=temporary/'cmake';generated.mkdir()
                (temporary/'evidence-stop.hpp').write_text(BARRIER);pin(temporary/'evidence-stop.hpp')
                (generated/'CMakeLists.txt').write_text(cmake(version,selection));pin(generated/'CMakeLists.txt')
                query=temporary/'build/.cmake/api/v1/query/codemodel-v2';query.parent.mkdir(parents=True);query.write_text('');pin(query)
                run(version,f'configure-ros{version}',['/usr/bin/cmake','-S','/tmp/cmake','-B','/tmp/build',
                    '-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-'+str(probe['gcc']),'-DCMAKE_BUILD_TYPE=Release',
                    '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',*selection['cmake_arguments']],[f'ros{version}-native-cmake'],120)
                generated_inputs=generated_sources(version,source,temporary);pins.extend(generated_inputs.values())
                configured=file_api_sources(version,temporary);pins.extend(configured['metadata'])
                database,db_pin=read_json_binding(temporary/'build/compile_commands.json');pins.append(db_pin)
                run(version,f'compile-ros{version}',['/usr/bin/cmake','--build','/tmp/build','--parallel','1','--verbose'],
                    [f'ros{version}-native-cmake',f'ros{version}-native-gcc',f'ros{version}-native-frontend'],600)
                label=probe['label'];binary=temporary/'build'/label;meta=temporary/'build/CMakeFiles'/(label+'.dir');verbose=out/f'compile-ros{version}.log'
                recipes=compiler_recipes(version,database,verbose.read_text());units=[]
                unit_archive=out/f'ros{version}'/'translation-units';unit_archive.mkdir()
                command=next(row for row in report['commands'] if row['name']==f'compile-ros{version}')
                with evidence._lock: samples=list(evidence.samples.values())
                for ordinal,recipe in enumerate(recipes):
                    inputs,dep_pin=closure(version,temporary,recipe,generated_inputs)
                    logical=recipe['source_guest_path'];actual=source/logical[6:] if logical.startswith('/work/') else temporary/logical[5:]
                    obj=temporary/recipe['compiled_object_guest_path'][5:]
                    archives={kind:archive_product(path,unit_archive/(str(ordinal)+suffix)) for kind,path,suffix in
                        [('source',actual,'.cc'),('object',obj,'.o'),('depfile',Path(dep_pin['path']),'.o.d')]}
                    object_elf=require_arm(obj);archive_elf=require_arm(archives['object']['archive']['path'])
                    if (object_elf['sha256'],object_elf['bytes']) != (archive_elf['sha256'],archive_elf['bytes']): raise RuntimeError('archived TU ELF differs')
                    unit={'recipe':recipe,'object':object_elf,'archived_object':archive_elf,'depfile':dep_pin,'dependencies':inputs,
                          'archives':archives,'actual_compiler_mapping':compilation_mapping(version,recipe,samples,command,before['membership'],evidence.emulator_shas)}
                    receipt=unit_archive/(str(ordinal)+'.json');receipt.write_text(json.dumps(unit,indent=2)+'\n')
                    units.append({**unit,'receipt':pin(receipt)})
                link=validate_link(version,recipes,(meta/'link.txt').read_text(),verbose.read_text())
                metadata={name:pin(path) for name,path in [('cmake',generated/'CMakeLists.txt'),('flags',meta/'flags.make'),('link',meta/'link.txt'),('verbose',verbose),('database',temporary/'build/compile_commands.json'),('build_rules',meta/'build.make')]}
                report['probes'][label]={'binary':evidence.target(label,binary),'translation_units':units,'translation_unit_count':len(units),
                    'configured_sources':configured,'generated_sources':generated_inputs,'link':link,'metadata':metadata};pin(binary)
            report['compilation_status']='PASS'
        else:
            prepared,prepared_pin=read_json_binding(args.prepared,limit=64*1024*1024);pins.append(prepared_pin)
            if (prepared.get('kind')!='fixed-custom-topics-arm' or prepared.get('format_version')!=2 or prepared.get('status')!='PREPARED' or prepared.get('compilation_status')!='PASS' or
                prepared.get('source_manifest_sha256')!=manifest_pin['sha256'] or prepared.get('cgroup_incarnation_before')!=before or
                prepared.get('source')!=str(source) or prepared.get('sdk_workspace')!=str(sdk_work) or prepared.get('selections')!=report['selections'] or
                prepared.get('actual_native_behavior_probes')!=0 or set(prepared.get('probes',{}))!={p['label'] for p in PROBES.values()}):
                raise RuntimeError('exact current preparation/source/SDK/cgroup/selection required')
            if not all(prepared.get(key) is True for key in ('source_preserved','sdk_preserved','inputs_preserved','same_cgroup_incarnation')) or prepared.get('remaining_owned_processes')!=[] or prepared.get('remaining_owned_mounts')!=[]:
                raise RuntimeError('prepared immutability and cleanup incomplete')
            expected_commands={'configure-ros1','compile-ros1','configure-ros2','compile-ros2'}
            commands=prepared.get('commands',[])
            if len(commands)!=4 or {row.get('name') for row in commands}!=expected_commands or any(
                row.get('status')!='PASS' or row.get('returncode')!=0 or type(row.get('peak_observed_subtree_RSS_bytes')) is not int or
                not 0<=row['peak_observed_subtree_RSS_bytes']<=2*1024**3 for row in commands):
                raise RuntimeError('prepared exact serial bounded compilation rows differ')
            for fact in prepared['input_pins']:
                if pin(fact['path'])!=fact: raise RuntimeError('prepared immutable input replaced')
            architecture,arch_pin=architecture_binding(prepared['architecture_evidence']['path'],pins)
            if arch_pin!=prepared['architecture_evidence'] or prepared.get('tools')!=report['tools']:
                raise RuntimeError('prepared actual compilation architecture proof unavailable')
            for version,probe in PROBES.items():
                required={f'ros{version}-native-gcc',f'ros{version}-native-frontend',f'ros{version}-native-cmake'}
                compile_row=next(row for row in commands if row['name']==f'compile-ros{version}')
                samples=[sample for sample in architecture['actual_process_samples'] if sample.get('name')==f'compile-ros{version}/' and
                    sample.get('ancestry',[None])[0]==compile_row['pid'] and sample.get('live_cgroup_procs') is True and
                    sample.get('membership')==architecture['cgroup_before']['membership']]
                if not any(sample.get('pid')==compile_row['pid'] and sample.get('ancestry')==[compile_row['pid']] and
                           sample.get('starttime_ticks')==compile_row['launcher_starttime_ticks'] for sample in samples):
                    raise RuntimeError('prepared actual compiler launcher birth proof unavailable')
                observed={label for sample in samples if sample.get('kernel_exe_elf',{}).get('e_machine')==62 and
                    sample['kernel_exe_elf'].get('sha256') in architecture['expected_host_emulator_sha256'] and
                    sample.get('starttime_ticks',0)>=compile_row['launcher_starttime_ticks'] for label in sample.get('mapped_arm_targets',[])}
                if not required<=observed or any(architecture['arm_targets'].get(label)!=report['tools'][label] for label in required):
                    raise RuntimeError('prepared actual compiler/frontend mapping facts differ')
            for version,probe in PROBES.items():
                label=probe['label'];item=prepared['probes'][label]
                if require_arm(item['binary']['path'])!=item['binary']: raise RuntimeError('prepared native executable identity changed')
                database,_=read_json_binding(item['metadata']['database']['path'])
                verbose=Path(item['metadata']['verbose']['path']).read_text()
                recipes=compiler_recipes(version,database,verbose);units=item['translation_units']
                if item['translation_unit_count']!=len(recipes) or len(units)!=len(recipes) or [unit['recipe'] for unit in units]!=recipes:
                    raise RuntimeError('prepared complete actual compiler/TU set differs')
                original_temporary=prepared_temporary(item)
                generated_inputs=generated_sources(version,source,original_temporary)
                if generated_inputs!=item['generated_sources'] or file_api_sources(version,original_temporary)!=item['configured_sources']:
                    raise RuntimeError('prepared generated bytes/File API complete source set differs')
                pins.extend(generated_inputs.values())
                command=next(row for row in commands if row['name']==f'compile-ros{version}')
                for ordinal,(unit,recipe) in enumerate(zip(units,recipes)):
                    if Path(unit['depfile']['path'])!=original_temporary/recipe['effective_mf'][5:]: raise RuntimeError('prepared dependency output escaped original root')
                    if Path(unit['object']['path'])!=original_temporary/recipe['compiled_object_guest_path'][5:]: raise RuntimeError('prepared actual object escaped original root')
                    if require_arm(unit['object']['path'])!=unit['object'] or require_arm(unit['archived_object']['path'])!=unit['archived_object']:
                        raise RuntimeError('prepared original/archive AArch64 TU object identity changed')
                    inputs,dep_pin=closure(version,original_temporary,recipe,generated_inputs)
                    if inputs!=unit['dependencies'] or dep_pin!=unit['depfile']: raise RuntimeError('prepared per-TU actual full-MD closure differs')
                    logical=recipe['source_guest_path'];actual=source/logical[6:] if logical.startswith('/work/') else original_temporary/logical[5:]
                    originals={'source':actual,'object':Path(unit['object']['path']),'depfile':Path(unit['depfile']['path'])}
                    suffixes={'source':'.cc','object':'.o','depfile':'.o.d'}
                    unit_archive=original_temporary.parent/'translation-units'
                    for kind,facts in unit['archives'].items():
                        if (kind not in originals or Path(facts['original']['path'])!=originals[kind] or
                            Path(facts['archive']['path'])!=unit_archive/(str(ordinal)+suffixes[kind]) or
                            pin(facts['original']['path'])!=facts['original'] or pin(facts['archive']['path'])!=facts['archive']):
                            raise RuntimeError('prepared TU evidence archive identity differs')
                        if (facts['original']['sha256'],facts['original']['bytes'])!=(facts['archive']['sha256'],facts['archive']['bytes']): raise RuntimeError('prepared TU evidence archive bytes differ')
                    if set(unit['archives'])!={'source','object','depfile'}: raise RuntimeError('prepared TU archive incomplete')
                    if Path(unit['archived_object']['path'])!=Path(unit['archives']['object']['archive']['path']) or Path(unit['receipt']['path'])!=unit_archive/(str(ordinal)+'.json'):
                        raise RuntimeError('prepared TU ELF/receipt archive association differs')
                    receipt,receipt_pin=read_json_binding(unit['receipt']['path'])
                    if receipt_pin!=unit['receipt'] or receipt!={key:value for key,value in unit.items() if key!='receipt'}:
                        raise RuntimeError('prepared independently pinned TU receipt differs')
                    mapping=compilation_mapping(version,recipe,architecture['actual_process_samples'],command,architecture['cgroup_before']['membership'],architecture['expected_host_emulator_sha256'])
                    retained=unit['actual_compiler_mapping'];required={f'ros{version}-native-gcc',f'ros{version}-native-frontend'}
                    if (not retained or any(sample not in mapping for sample in retained) or
                        not required<={name for sample in retained for name in sample['mapped_arm_targets']}):
                        raise RuntimeError('prepared per-TU actual source/compiler ARM mapping differs')
                if validate_link(version,recipes,Path(item['metadata']['link']['path']).read_text(),verbose)!=item['link']:
                    raise RuntimeError('prepared complete all-object actual link differs')
                if Path(item['metadata']['cmake']['path']).read_text()!=cmake(version,report['selections'][str(version)]): raise RuntimeError('prepared exact generated CMake differs')
                copied=temporaries[version]/label;shutil.copy2(item['binary']['path'],copied);fact=evidence.target(label,copied);pin(copied)
                if fact['sha256']!=item['binary']['sha256'] or (fact['device'],fact['inode'])==(item['binary']['device'],item['binary']['inode']): raise RuntimeError('prepared executable private copy identity differs')
                report['probes'][label]={**item,'binary':fact}
            report['compilation_status']='REUSED_QUALIFIED_PREPARATION'

        if args.mode in ('run','all'):
            lease_lock(Path('/tmp')/f'uwvm-arm-ros1-master-{args.master_port}.lock')
            lease_lock(Path('/tmp')/'uwvm-arm-ros-domain-203.lock')
            root,sdk=roots[1],sdks[1]
            # Master readiness binds its XMLRPC PID to an observed owned ARM
            # Python/QEMU birth; the implementation is defined below.
            for version,probe in PROBES.items():
                try:
                    if version==1: start_master()
                    run(version,'behavior-ros'+str(version),[('/tmp/build/' if args.mode=='all' else '/tmp/')+probe['label']],
                        [probe['label']],120,stopped=True)
                    report['actual_native_behavior_probes']+=1
                except Exception:
                    if stopping: raise
                    failure={'version':version,'error':traceback.format_exc()}
                    # The shared cleanup owns every observed pidfd, including
                    # the Master and a failed fixture's fork peer. Retire all
                    # those births before another version can be launched.
                    failure['cleanup']=owned.stop(children)
                    # stop closes every pidfd even when cleanup reports FAIL.
                    # Retire this tracker before any save/raise can enter the
                    # outer finally; closed descriptor integers are never reused.
                    owned=None;children=[];master=None
                    report.setdefault('native_behavior_failures',[]).append(failure);save()
                    if failure['cleanup']['status']!='PASS': raise RuntimeError('failed native fixture cleanup incomplete')
                    owned=OwnedProcesses(evidence)
                    monitor()
                    if not pins_preserved(pins): raise RuntimeError('failed native fixture changed a qualified input')
            if report.get('native_behavior_failures') or report['actual_native_behavior_probes']!=2:
                raise RuntimeError('both independent native behavior fixtures must succeed for PASS')
        report['input_pins']=pins;status='PASS'
    except BaseException:
        report['error']=traceback.format_exc()
    finally:
        try:
            report['process_cleanup']=owned.stop(children) if owned is not None else {'status':'PASS','remaining_owned_processes':[]}
            prior=[failure['cleanup'] for failure in report.get('native_behavior_failures',[]) if 'cleanup' in failure]
            report['remaining_owned_processes']=[identity for cleanup in prior for identity in cleanup['remaining_owned_processes']]+report['process_cleanup']['remaining_owned_processes']
            if any(cleanup['status']!='PASS' for cleanup in prior):status='FAIL'
            if report['process_cleanup']['status']!='PASS': status='FAIL'
            report['source_preserved']=initial is not None and source_state(source,frozen)==initial
            report['sdk_preserved']=bool(snapshots) and all(sdk_inventory(sdks[v])==snapshots[v] and sdk_root_aliases(sdks[v])==aliases[v] for v in snapshots)
            report['inputs_preserved']=pins_preserved(pins);report['input_pins']=pins
            if not all(report[key] for key in ('source_preserved','sdk_preserved','inputs_preserved')):status='FAIL'
        except BaseException: report['cleanup_guard_error']=traceback.format_exc();status='FAIL'
        if evidence is not None:
            try:
                report['cgroup_incarnation_after']=incarnation();report['same_cgroup_incarnation']=before==report['cgroup_incarnation_after'];report['memory_swap_max_after']=report['cgroup_incarnation_after']['memory.swap.max']
                if not report['same_cgroup_incarnation']:status='FAIL'
            except BaseException:report['cgroup_final_error']=traceback.format_exc();status='FAIL'
            try:
                verification=evidence.finish(status);status=verification['status'];report['architecture_evidence']=read_file_binding(out/'execution/architecture-evidence.json')
            except BaseException:report['architecture_final_error']=traceback.format_exc();status='FAIL'
        report['mount_cleanup']=[]
        for mount in reversed(mounts):
            try:
                result=subprocess.run(['umount','-R',str(mount)],capture_output=True,text=True,timeout=30)
                report['mount_cleanup'].append({'path':str(mount),'returncode':result.returncode,'stderr':result.stderr})
                if result.returncode:status='FAIL'
            except BaseException:report['mount_cleanup'].append({'error':traceback.format_exc()});status='FAIL'
        report['remaining_owned_mounts']=[line for line in Path('/proc/self/mountinfo').read_text().splitlines() if any(line.split()[4]==str(root) or line.split()[4].startswith(str(root)+'/') for root in roots.values())]
        if report['remaining_owned_mounts']:status='FAIL'
        for path,device,inode in locks:
            try:
                fact=path.stat()
                if (fact.st_dev,fact.st_ino)!=(device,inode):raise RuntimeError('owned lock replaced')
                path.unlink()
            except BaseException:report.setdefault('lock_cleanup_errors',[]).append(traceback.format_exc());status='FAIL'
        report['status']='PREPARED' if status=='PASS' and args.mode=='prepare' else status;save()
    print(report['status'],out/'report.json',flush=True)
    return 0 if report['status'] in ('PASS','PREPARED') else 1


if __name__ == '__main__':
    raise SystemExit(main())
