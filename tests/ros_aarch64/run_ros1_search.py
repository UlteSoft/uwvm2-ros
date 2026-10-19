#!/usr/bin/env python3
"""Prepare and execute two fresh ROS1 C++17 ARM regression fixtures.

All native compilation/ROS execution is Linux-only in the verified 64 GiB
cgroup. Preparation is PREPARED and never a behavior PASS. Execute requires
exact frozen source, Noetic SDK, current Gateway compilation closure, parent
QEMU lease, owned PID evidence and cleanup. No Wasm E2E verdict is claimed.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import shutil
import signal
import socket
import subprocess
import sys
import time
import traceback
import shlex
import posixpath
import xmlrpc.client

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm, read_json_binding
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state
from run_resources import bind_frozen_work
from build_rpc_reader_diagnostic import closure
from run_unit import process_tree_rss
from run_action_qos import process_identity, owned_stopped_arm
from gateway_qualification import (qualify_gateway as qualify_current_gateway,
                                   architecture_binding,read_file_binding)
from native_runtime_environment import gateway_library_environment


def pin_file(path):
    """Use the shared nonblocking same-descriptor immutable input binder."""
    return read_file_binding(path)


def pins_preserved(rows):
    return all(pin_file(row['path'])==row for row in rows)


class OwnedProcesses:
    """Track observed births with pidfds, including later orphaned children.

    Only a direct Popen root or a live parent's actual procfs child is acquired.
    Registration precedes cgroup validation, so an observed departing child is
    still owned for cleanup. Signals never address a reused PID or broad group.
    This makes no claim about an unobserved daemon's ancestry.
    """
    def __init__(self,evidence):
        if not hasattr(os,'pidfd_open') or not hasattr(signal,'pidfd_send_signal'):
            raise RuntimeError('BLOCKED Linux pidfd cleanup support required')
        self.evidence=evidence;self.rows={};self.errors=[];self.unproven={}
    def acquire(self,pid,label,ancestry,parent=None):
        first=process_identity(pid)
        key=(pid,first['starttime_ticks'])
        # An already pinned birth remains ours after its parent exits. Do not
        # discard that existing cleanup responsibility because procfs now names
        # a subreaper/init as its parent. New births still require the original
        # parent observation (or the trusted caller's direct Popen relationship).
        if key in self.rows:return self.rows[key]
        expected_parent=os.getpid() if parent is None else parent['pid']
        if (first['parent_pid']!=expected_parent or
                (parent is not None and first['starttime_ticks']<parent['starttime_ticks'])):
            if first['state'] in ('Z','X'):return
            # A stale children-list PID cannot establish which birth belonged
            # to that parent. Do not abort an ordinary descendant scan or signal
            # this unproven birth. Retain it for the final audit instead: cleanup
            # is incomplete while it remains live and has not subsequently been
            # acquired through a proven parent. A trusted direct Popen mismatch
            # is different: this still-live caller cannot legitimately retire.
            if not hasattr(self,'unproven'):self.unproven={}
            self.unproven[key]={'identity':first,'label':label,'ancestry':list(ancestry),
                                'expected_parent':expected_parent}
            if parent is None:
                raise RuntimeError('direct Popen root lacks its observed caller parent: '+repr(first))
            return
        fd=os.pidfd_open(pid)
        try:
            second=process_identity(pid)
            # PPID is a relationship, not a birth identifier: an observed parent
            # can retire between these reads, while the same child and its pidfd
            # remain live. Keep the first proven ancestry for ownership, retain
            # both observations, and reject a replacement PID/birth before any
            # cgroup admission or signal. Transient scheduling state is likewise
            # not part of process identity.
            if any(second[field]!=first[field] for field in ('pid','starttime_ticks')):
                raise RuntimeError('owned process birth changed while opening pidfd: '
                                   +repr({'first':first,'second':second}))
        except BaseException:
            os.close(fd);raise
        self.rows[key]={'identity':first,'pidfd':fd,'label':label,'ancestry':list(ancestry),
                        'acquisition':{'first':first,'after_pidfd':second,
                                       'parent_changed':second['parent_pid']!=first['parent_pid']}}
        if hasattr(self,'unproven'):self.unproven.pop(key,None)
        self.check(self.rows[key])
        return self.rows[key]
    def alive(self,row):
        try:
            current=process_identity(row['identity']['pid'])
            return current['starttime_ticks']==row['identity']['starttime_ticks'] and current['state'] not in ('Z','X')
        except (FileNotFoundError,ProcessLookupError):return False
    def check(self,row):
        pid=row['identity']['pid']
        if not self.alive(row):return False
        try:membership=Path(f'/proc/{pid}/cgroup').read_text().splitlines()
        except (FileNotFoundError,ProcessLookupError):return False
        live=str(pid) in (self.evidence.leaf/'cgroup.procs').read_text().split()
        if membership!=self.evidence.before['membership']:
            if not self.alive(row):return False
            raise RuntimeError('observed owned process is outside the verified live cgroup')
        # A task can disappear from cgroup.procs before its final procfs records
        # disappear. This is unavailable live proof, never positive admission.
        return live and self.alive(row)
    def sample(self):
        pending=list(self.rows.values());seen=set()
        while pending:
            row=pending.pop();birth=(row['identity']['pid'],row['identity']['starttime_ticks'])
            if birth in seen:continue
            seen.add(birth)
            if not self.alive(row):continue
            if not self.check(row):continue
            pid=row['identity']['pid'];children=set()
            # Linux records children on the thread which called fork/clone.
            # Inspect every extant task, including workers other than the main
            # thread, and deduplicate before acquiring exact process births.
            try:tasks=list(Path(f'/proc/{pid}/task').iterdir())
            except (FileNotFoundError,ProcessLookupError):continue
            for task in tasks:
                if not task.name.isdecimal():continue
                try:children.update((task/'children').read_text().split())
                except (FileNotFoundError,ProcessLookupError):continue
            for child in children:
                try:
                    acquired=self.acquire(int(child),row['label'],row['ancestry']+[int(child)],row['identity'])
                    if acquired is not None:pending.append(acquired)
                except (FileNotFoundError,ProcessLookupError):continue
    def rss(self,label):
        total=0
        for row in self.rows.values():
            if row['label']!=label or not self.alive(row):continue
            try:
                for line in Path(f"/proc/{row['identity']['pid']}/status").read_text().splitlines():
                    if line.startswith('VmRSS:'):total+=int(line.split()[1])*1024
            except (FileNotFoundError,ProcessLookupError):pass
        return total
    def send(self,row,signum):
        if self.alive(row):signal.pidfd_send_signal(row['pidfd'],signum)
    def stop(self,children):
        for signum,budget in ((signal.SIGCONT,0),(signal.SIGTERM,3),(signal.SIGKILL,3)):
            sent=set();deadline=time.monotonic()+budget
            while True:
                # A still-live owned parent may fork while it is terminating.
                # Keep discovering all-thread children during this bounded
                # phase, and signal newly observed births through their pidfds.
                try:self.sample()
                except BaseException as error:self.errors.append(str(error))
                ordered=sorted(self.rows.values(),key=lambda row:len(row['ancestry']),reverse=True)
                for row in ordered:
                    key=(row['identity']['pid'],row['identity']['starttime_ticks'])
                    if key in sent:continue
                    try:
                        if signum==signal.SIGTERM:self.send(row,signal.SIGCONT)
                        self.send(row,signum);sent.add(key)
                    except (ProcessLookupError,FileNotFoundError):pass
                    except BaseException as error:self.errors.append(str(error))
                if not any(self.alive(row) for row in ordered) or time.monotonic()>=deadline:break
                time.sleep(.01)
        for process in children:
            try:process.wait(timeout=3)
            except BaseException as error:self.errors.append(str(error))
        ordered=sorted(self.rows.values(),key=lambda row:len(row['ancestry']),reverse=True)
        remaining=[row['identity'] for row in ordered if self.alive(row)]
        # An unavailable ancestry sample never authorizes a signal. A later
        # disappearance, zombie state or different birth discharges only that
        # old candidate; a still-live matching birth keeps cleanup incomplete.
        unproven=[]
        for candidate in getattr(self,'unproven',{}).values():
            try:current=process_identity(candidate['identity']['pid'])
            except (FileNotFoundError,ProcessLookupError):continue
            if (current['starttime_ticks']==candidate['identity']['starttime_ticks']
                    and current['state'] not in ('Z','X')):
                unproven.append({**candidate,'current_identity':current})
        for row in ordered:
            try:os.close(row['pidfd'])
            except OSError as error:self.errors.append(str(error))
        return {'status':('CLEANUP_INCOMPLETE' if unproven else
                          'PASS' if not remaining and not self.errors else 'FAIL'),
                'observed_owned_births':[{k:v for k,v in row.items() if k!='pidfd'} for row in ordered],
                'remaining_owned_processes':remaining,'unproven_live_candidates':unproven,'errors':self.errors,
                'scope':'Only observed Popen roots and proven descendants, pinned by birth/pidfd; no process-group signalling'}


def qualify_gateway(binary,report_path,source,sdk,frozen,sdk_before,aliases):
    """Delegate strict shared admission; the caller also retains full SDK guards."""
    binding=qualify_current_gateway(binary,report_path,source.parent/'source-manifest.json',source,sdk,1)
    return binding['binary'],binding['input_pins'],binding


def fixture_recipe(label,relative,database,log):
    """Recover the successful compiler argv and its final effective -MF."""
    source='/work/'+relative;compiler='/usr/bin/aarch64-linux-gnu-g++-9'
    if (len(database)!=len(PROBES) or {posixpath.normpath(row['file']) for row in database}!={'/work/'+value for value in PROBES.values()}):
        raise RuntimeError('exact two original fixture compile database sources required')
    entries=[row for row in database if posixpath.normpath(row['file'])==source]
    if len(entries)!=1 or entries[0]['directory']!='/tmp/build':raise RuntimeError('exact fixture compile database entry required')
    def decode(tokens):
        kept=[];outputs=[];dependency=[];optimization=debug=standard=None;ndebug=None;index=0
        while index<len(tokens):
            token=tokens[index]
            if token.startswith(('@','-Wp,')) or token in ('-M','-MM','-MMD','-E','-S','-fsyntax-only','-MG','-MP','-Xpreprocessor'):
                raise RuntimeError('opaque or non-compilation dependency recipe refused')
            if token=='-MD':dependency.append(token)
            elif token in ('-MF','-MT','-MQ'):
                if index+1==len(tokens):raise RuntimeError('missing dependency operand')
                dependency.extend((token,tokens[index+1]))
                if token=='-MF':outputs.append(tokens[index+1])
                index+=1
            elif token.startswith(('-MF','-MT','-MQ')):
                dependency.append(token)
                if token.startswith('-MF'):outputs.append(token[3:])
            elif token in ('-D','-U'):
                if index+1==len(tokens):raise RuntimeError('missing macro operand')
                kept.extend((token,tokens[index+1]))
                if tokens[index+1].split('=',1)[0]=='NDEBUG':ndebug=token=='-D'
                index+=1
            else:
                kept.append(token)
                if token.startswith('-O'):optimization=token
                elif token.startswith('-g'):debug=token
                elif token.startswith('-std='):standard=token
                elif token.startswith(('-D','-U')) and token[2:].split('=',1)[0]=='NDEBUG':ndebug=token.startswith('-D')
            index+=1
        if optimization!='-O1' or debug!='-g0' or standard!='-std=gnu++17' or ndebug is not False:
            raise RuntimeError('effective fixture O1/g0/C++17/enabled assertions differ')
        if (not kept or kept[0]!=compiler or kept.count('-c')!=1 or kept[kept.index('-c')+1:]!=[source]
                or kept.count('-o')!=1 or kept.index('-o')+1==len(kept)
                or kept.count('-include')!=1 or kept[kept.index('-include')+1]!='/tmp/evidence-stop.hpp'):
            raise RuntimeError('fixture compiler/source/object/barrier argv contract differs')
        object_path=posixpath.normpath(posixpath.join('/tmp/build',kept[kept.index('-o')+1]))
        if not object_path.startswith('/tmp/build/CMakeFiles/'+label+'.dir/') or not object_path.endswith('.o'):
            raise RuntimeError('fixture object output escapes its owned target')
        return {'codegen':kept,'dependencies':dependency,'mf_outputs':outputs,'object_guest_path':object_path}
    expected=decode(entries[0].get('arguments') or shlex.split(entries[0]['command']))
    found=[]
    for number,line in enumerate(log.splitlines(),1):
        if compiler not in line or source not in line:continue
        tokens=shlex.split(line);directory='/tmp/build'
        if len(tokens)>3 and tokens[0]=='cd' and tokens[2]=='&&':directory,tokens=tokens[1],tokens[3:]
        if not tokens or tokens[0]!=compiler or tokens.count('-c')!=1 or tokens[tokens.index('-c')+1:]!=[source]:continue
        actual=decode(tokens)
        if directory!='/tmp/build' or actual['codegen']!=expected['codegen'] or '-MD' not in actual['dependencies'] or not actual['mf_outputs']:
            raise RuntimeError('actual fixture compiler argv differs or lacks system-inclusive dependency output')
        guest=posixpath.normpath(posixpath.join(directory,actual['mf_outputs'][-1]))
        if not guest.startswith('/tmp/build/CMakeFiles/'+label+'.dir/') or not guest.endswith('.d'):
            raise RuntimeError('actual compiler dependency output escapes owned target metadata')
        found.append({'directory':directory,'argv':tokens,'log_line':number,'effective_dependency_output':guest,'controls':actual})
    if len(found)!=1:raise RuntimeError('exactly one successful verbose fixture compile recipe required')
    return found[0]


def dependency_rule(body,recipe):
    """Bind the actual -MD rule target to the successful compiler's object."""
    target,_=body.replace('\\\n',' ').split(':',1)
    objects=[posixpath.normpath(posixpath.join(recipe['directory'],item)) for item in shlex.split(target)]
    if objects!=[recipe['controls']['object_guest_path']]:
        raise RuntimeError('actual fixture -MD rule does not identify its compiled object')


def validate_fixture_link(label,recipe,link,verbose):
    """Check the recorded object is linked by one actual verbose GCC9 command."""
    tokens=shlex.split(link)
    if (not tokens or tokens[0]!='/usr/bin/aarch64-linux-gnu-g++-9' or tokens.count('-o')!=1
            or posixpath.normpath(posixpath.join('/tmp/build',tokens[tokens.index('-o')+1]))!='/tmp/build/'+label
            or recipe['controls']['object_guest_path'] not in {
                posixpath.normpath(posixpath.join('/tmp/build',value)) for value in tokens if value.endswith('.o')}):
        raise RuntimeError('fixture GCC9 link object/output contract differs')
    matches=[]
    for line in verbose.splitlines():
        if tokens[0] not in line:continue
        actual=shlex.split(line)
        if len(actual)>3 and actual[:3]==['cd','/tmp/build','&&']:actual=actual[3:]
        if actual==tokens:matches.append(line)
    if len(matches)!=1:raise RuntimeError('one successful actual verbose fixture link recipe required')

PROBES = {'ros1-parent-search':'tests/ros1_search/native_ros1.cc',
          'ros1-feedback-budget':'tests/ros_action/feedback_budget_ros1.cc'}
PROBE_CONTRACTS = {
    'ros1-parent-search': {'source':PROBES['ros1-parent-search'], 'requires_gateway':True,
        'marker':'PASS actual ROS1 exact/explicit-parent search and namespace isolation checks=16'},
    'ros1-feedback-budget': {'source':PROBES['ros1-feedback-budget'], 'requires_gateway':False,
        'marker':'PASS actual ROS1 feedback/result framing byte budgets scenarios=17 checks='},
}
DRIVER_INPUTS = ('run_ros1_search.py','runtime.py','rebuild_affected.py','run_resources.py',
                 'build_rpc_reader_diagnostic.py','run_unit.py','run_action_qos.py','gateway_qualification.py',
                 'native_runtime_environment.py')

BARRIER = '''#include <csignal>
__attribute__((constructor)) static void arm_ros1_search_evidence_stop() { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }
'''


def cmake():
    """Generate exactly two Noetic fixtures with explicit full GCC depfiles."""
    lines = ['cmake_minimum_required(VERSION 3.16)',
             'project(arm_ros1_search LANGUAGES CXX)',
             'set(CMAKE_CXX_STANDARD 17)', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)',
             # Catkin otherwise adds unused gtest/gmock compile-database rows.
             # Keep the strict closed two-source database contract unchanged.
             'set(CATKIN_ENABLE_TESTING OFF CACHE BOOL "Owned ROS1 regression fixtures only" FORCE)',
             'find_package(catkin REQUIRED COMPONENTS roscpp actionlib actionlib_msgs actionlib_tutorials)']
    for label, source in PROBES.items():
        lines += [f'add_executable({label} /work/{source})',
                  f'target_include_directories({label} PRIVATE /work/src ${{catkin_INCLUDE_DIRS}})',
                  # Noetic's CMake 3.16 does not append GCC -MF automatically.
                  # The parser still verifies the actual final effective -MF,
                  # its target-owned path, and its exact compiled-object rule.
                  f'target_compile_options({label} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD -MF /tmp/build/CMakeFiles/{label}.dir/fixture-explicit.o.d -include /tmp/evidence-stop.hpp)',
                  f'target_link_libraries({label} ${{catkin_LIBRARIES}} pthread)']
    return '\n'.join(lines)+'\n'


def validate_prepared_contract(original, manifest_sha256):
    """Reject partial/old preparations before copying any executable."""
    if (original.get('status')!='PREPARED' or original.get('mode')!='prepare'
            or original.get('kind')!='ros1-search-arm' or type(original.get('format_version')) is not int
            or original.get('format_version')!=1 or original.get('compilation_status')!='PASS'
            or type(original.get('actual_native_behavior_probes')) is not int or original.get('actual_native_behavior_probes')!=0
            or original.get('actual_behavior_status')!='NOT_PERFORMED' or set(original.get('probes',{}))!=set(PROBES)
            or original.get('probe_contracts')!=PROBE_CONTRACTS):
        raise RuntimeError('current exact two-probe ROS1 preparation required')
    if original.get('source_manifest_sha256')!=manifest_sha256:
        raise RuntimeError('prepared source manifest differs from the current exact frozen ROS1 source')
    if (original.get('same_cgroup_incarnation') is not True
            or not original.get('cgroup_incarnation_before')
            or original.get('cgroup_incarnation_before')!=original.get('cgroup_incarnation_after')
            or original['cgroup_incarnation_before'].get('memory.swap.max')!='0'):
        raise RuntimeError('prepared compilation did not retain one exact cgroup incarnation')
    if (not all(original.get(key) is True for key in ('source_preserved','sdk_preserved',
                'driver_inputs_preserved','prepared_inputs_preserved','tools_preserved','gateway_inputs_preserved'))
            or original.get('memory_swap_max_before')!='0' or original.get('memory_swap_max_after')!='0'
            or original.get('remaining_owned_processes')!=[]
            or original.get('process_cleanup',{}).get('status')!='PASS'
            or original.get('process_cleanup',{}).get('remaining_owned_processes')!=[]
            or original.get('remaining_owned_mounts')!=[] or not original.get('mount_cleanup')
            or any(row.get('returncode')!=0 for row in original['mount_cleanup'])):
        raise RuntimeError('prepared source/SDK/input/process/mount preservation incomplete')
    commands=original.get('commands',[])
    if (len(commands)!=2 or {row.get('name') for row in commands}!={'configure','compile'}
            or any(row.get('status')!='PASS' or row.get('returncode')!=0
                or type(row.get('peak_observed_subtree_RSS_bytes')) is not int
                or not 0<=row['peak_observed_subtree_RSS_bytes']<=2*1024**3 for row in commands)):
        raise RuntimeError('prepared bounded serial configure/compile commands incomplete')
    recorded = original.get('driver_inputs',{})
    if set(recorded)!=set(DRIVER_INPUTS):
        raise RuntimeError('prepared executed driver/helper input set differs')
    for name in DRIVER_INPUTS:
        if recorded[name].get('sha256')!=digest(Path(__file__).with_name(name)):
            raise RuntimeError('prepared driver/helper bytes differ from the current recipe: '+name)


def validate_compilation_architecture(original,architecture,sdk,pins):
    """Require real compiler tools mapped in the successful owned build subtree."""
    if any(architecture.get('cgroup_incarnation_before',{}).get(key)!=original['cgroup_incarnation_before'].get(key)
            for key in ('path','device','inode','boot_id')):
        raise RuntimeError('prepared raw architecture cgroup differs from the driver incarnation binding')
    compiler=rooted(sdk,'/usr/bin/aarch64-linux-gnu-g++-9')
    frontend=next((rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/9/cc1plus')
        for base in ('usr/lib','usr/libexec') if rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/9/cc1plus').is_file()),None)
    if frontend is None:raise RuntimeError('current canonical Noetic GCC9 frontend unavailable')
    tools={'native-gcc':compiler,'native-frontend':frontend,'native-cmake':rooted(sdk,'/usr/bin/cmake')}
    for label,path in tools.items():
        pins.append(pin_file(path))
        if architecture.get('arm_targets',{}).get(label)!=require_arm(path):
            raise RuntimeError('prepared actual ARM compiler/frontend/CMake identity differs: '+label)
    if (original.get('compiler')!=architecture['arm_targets']['native-gcc']
            or original.get('frontend')!=architecture['arm_targets']['native-frontend']):
        raise RuntimeError('prepared compiler/frontend declarations differ from observed targets')
    build=next(row for row in original['commands'] if row['name']=='compile')
    if (type(build.get('pid')) is not int or build['pid']<=0
            or type(build.get('launcher_starttime_ticks')) is not int or build['launcher_starttime_ticks']<=0):
        raise RuntimeError('prepared compiler launcher birth is unavailable')
    samples=[row for row in architecture.get('actual_process_samples',[])
        if row.get('name')=='compile/' and row.get('ancestry',[None])[0]==build['pid']
        and row.get('live_cgroup_procs') is True and row.get('membership')==architecture['cgroup_before']['membership']]
    if not any(row.get('pid')==build['pid'] and row.get('ancestry')==[build['pid']]
            and row.get('starttime_ticks')==build['launcher_starttime_ticks'] for row in samples):
        raise RuntimeError('prepared compiler launcher birth/ancestry proof unavailable')
    mapped={label for row in samples if type(row.get('starttime_ticks')) is int
        and row['starttime_ticks']>=build['launcher_starttime_ticks']
        and row.get('kernel_exe_elf',{}).get('e_machine')==62
        and row['kernel_exe_elf'].get('sha256') in architecture.get('expected_host_emulator_sha256',[])
        for label in row.get('mapped_arm_targets',[])}
    if not set(tools)<=mapped:raise RuntimeError('prepared actual compile subtree tool mapping evidence incomplete')
    for label,item in original['probes'].items():
        if architecture.get('arm_targets',{}).get(label)!=item['binary']:
            raise RuntimeError('prepared product differs from architecture target: '+label)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=('prepare','execute','all'), required=True)
    for name in ('source','rootfs','out','qemu','binfmt-evidence'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--prepared', type=Path)
    parser.add_argument('--master-port', type=int, default=11383)
    parser.add_argument('--gateway', type=Path)
    parser.add_argument('--gateway-report', type=Path)
    parser.add_argument('--container-started-at', required=True,
        help='Exact Docker State.StartedAt captured by parent launch provenance')
    args = parser.parse_args()
    if args.mode == 'execute' and args.prepared is None:
        parser.error('execute requires --prepared new ROS1 search preparation report')
    if args.mode != 'execute' and args.prepared is not None:
        parser.error('--prepared is supported only by execute')
    if not 1024 <= args.master_port <= 65535:
        parser.error('master port must be 1024..65535')
    if args.mode in ('execute','all') and (args.gateway is None or args.gateway_report is None):
        parser.error('execute/all requires --gateway and --gateway-report')
    source, sdk = args.source.resolve(strict=True), args.rootfs.resolve(strict=True)
    manifest = source.parent/'source-manifest.json';frozen,manifest_binding = read_json_binding(manifest)
    out = args.out.resolve()
    if out.is_relative_to(source) or out.is_relative_to(sdk):
        raise RuntimeError('fresh owned output must be outside frozen source and immutable SDK')
    if any(any(char.isspace() or char=='\\' for char in str(path)) for path in (source,sdk,out)):
        raise RuntimeError('mount/dependency proof requires paths without mountinfo escaping')
    out.mkdir(parents=True, mode=0o700, exist_ok=False)
    root, temporary = out/'rootfs', out/'native-temp';root.mkdir();temporary.mkdir()
    mounts, children, evidence, initial, sdk_before, aliases_before, before = [], [], None, None, None, None, None
    port_lock = None
    prepared_pins = [manifest_binding];tool_pins=[];gateway_pins=[]
    owned = None
    report = {'status':'RUNNING', 'kind':'ros1-search-arm', 'format_version':1, 'mode':args.mode,
        'compilation_status':'NOT_PERFORMED', 'master_port':args.master_port, 'actual_native_behavior_probes':0,
        'scope':'Two fresh C++17 ROS1 parent-search/feedback-budget native regressions; no Wasm E2E or full task verdict',
        'probe_contracts':PROBE_CONTRACTS,
        'source':str(source), 'rootfs':str(sdk), 'source_manifest':str(manifest), 'source_manifest_sha256':manifest_binding['sha256'],
        'source_manifest_binding':manifest_binding,
        'commands':[], 'probes':{}, 'driver_inputs':{},'tools':{}}
    archive = out/'driver-inputs';archive.mkdir()
    for name in DRIVER_INPUTS:
        path = Path(__file__).with_name(name);copied = archive/name;shutil.copy2(path,copied)
        if digest(path)!=digest(copied):raise RuntimeError('executed helper changed while archiving: '+name)
        original_binding,archive_binding=pin_file(path),pin_file(copied)
        expected=frozen.get('tests/ros_aarch64/'+name)
        if not expected or any(original_binding[key]!=expected[key] for key in ('sha256','bytes')):
            raise RuntimeError('executed helper differs from the complete frozen source: '+name)
        prepared_pins.extend((original_binding,archive_binding))
        report['driver_inputs'][name] = {'path':str(path), 'archive':str(copied), 'sha256':original_binding['sha256'],
            'original_binding':original_binding,'archive_binding':archive_binding}
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
            'memory.swap.max':(evidence.leaf/'memory.swap.max').read_text().strip(),
            'parent_recorded_container_started_at':args.container_started_at}

    def stop():
        if owned is not None:report['process_cleanup']=owned.stop(children)
        else:report['process_cleanup']={'status':'PASS','remaining_owned_processes':[],'observed_owned_births':[]}

    def interrupted(signum, frame):
        raise RuntimeError('interrupted ROS1 search driver signal '+str(signum))

    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        if sys.platform != 'linux' or os.geteuid() != 0:
            raise RuntimeError('BLOCKED root Linux private mount namespace required')
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('BLOCKED launch unshare -m inside the parent cgroup/binfmt lease')
        if not (sdk/'.uwvm-task-owned').is_file():raise RuntimeError('BLOCKED task-owned canonical Noetic SDK required')
        lease,lease_binding = read_json_binding(args.binfmt_evidence);prepared_pins.append(lease_binding)
        if not isinstance(lease.get('name'),str) or '/' in lease['name'] or lease['name'] in ('','.','..'):
            raise RuntimeError('invalid parent binfmt lease name')
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
        report['binfmt'] = {**lease_binding,'entry':registered}
        initial = source_state(source,frozen);sdk_before, aliases_before = sdk_inventory(sdk), sdk_root_aliases(sdk)
        (out/'sdk-before.json').write_text(json.dumps(sdk_before,indent=2)+'\n')
        sdk_before_pin=pin_file(out/'sdk-before.json');prepared_pins.append(sdk_before_pin)
        report['sdk_before'] = sdk_before_pin
        report['sdk_root_aliases'] = aliases_before
        spec = importlib.util.spec_from_file_location('action_qos_cgroup',source/'tests/ros_cgroup/probe.py')
        probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution',probe.probe);evidence.emulator(args.qemu)
        report['qemu_binding']=pin_file(args.qemu);prepared_pins.append(report['qemu_binding'])
        before = incarnation();report['cgroup_incarnation_before'] = before
        report['memory_swap_max_before']=before['memory.swap.max']
        if before['memory.swap.max']!='0':raise RuntimeError('BLOCKED exact zero-swap cgroup required')
        owned=OwnedProcesses(evidence)
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
        for name in ('PYTHONHOME','PYTHONPATH','ROS_HOSTNAME','ROS_NAMESPACE'):
            environment.pop(name,None)
        environment['PATH']='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin'
        environment.update(PYTHONDONTWRITEBYTECODE='1',ROS_MASTER_URI='http://127.0.0.1:'+str(args.master_port),
            ROS_IP='127.0.0.1',ROS_LOG_DIR='/tmp/ros-logs')
        (temporary/'ros-logs').mkdir()

        def monitor():
            owned.sample()
            if incarnation()!=before:raise RuntimeError('cgroup incarnation or swap limit changed during command')
            if int((evidence.leaf/'memory.current').read_text())>60*1024**3:
                raise RuntimeError('cgroup current memory exceeds the 60 GiB admission ceiling')
            if master is not None and master.poll() is not None:
                raise RuntimeError('owned ROS1 Master exited before behavior completion')

        def mapped(pid,target):
            return evidence.mapped(pid, target)

        def stop_gateway(child,name,row):
            if row.get('actual_gateway_ARM_mapping_before_resume'):return
            target=evidence.targets['ros1-current-gateway']
            for candidate in list(owned.rows.values()):
                if candidate['label']!=name or not owned.alive(candidate):continue
                pid=candidate['identity']['pid']
                try:
                    if not mapped(pid,target):continue
                    if not owned.check(candidate):continue
                    owned.send(candidate,signal.SIGSTOP)
                    deadline=time.monotonic()+1
                    while owned.alive(candidate) and process_identity(pid)['state'] not in ('T','t') and time.monotonic()<deadline:time.sleep(.001)
                    actual=process_identity(pid)
                    if actual['state'] not in ('T','t'):raise RuntimeError('owned Gateway did not stop for live map proof')
                    evidence.observe(child.pid,name+'/')
                    with evidence._lock:
                        proofs=[sample for sample in evidence.samples.values() if sample['opened_target_mappings']['identity_after']['state'] in ('T','t') and sample['name']==name+'/' and sample['pid']==pid
                            and sample['starttime_ticks']==actual['starttime_ticks'] and sample['ancestry']==candidate['ancestry']
                            and sample['live_cgroup_procs'] and sample['membership']==evidence.before['membership']
                            and 'ros1-current-gateway' in sample['mapped_arm_targets'] and sample['kernel_exe_elf']['e_machine']==62
                            and sample['kernel_exe_elf']['sha256'] in evidence.emulator_shas]
                    if not proofs or not mapped(pid,target):raise RuntimeError('stopped owned Gateway has no exact live ELF183/QEMU mapping proof')
                    host=Path(f'/proc/{pid}/exe').stat()
                    if (proofs[0]['kernel_exe_elf']['device'],proofs[0]['kernel_exe_elf']['inode'])!=(host.st_dev,host.st_ino):
                        raise RuntimeError('stopped Gateway host executable image changed before resume')
                    if not owned.check(candidate):raise RuntimeError('stopped Gateway has no current live cgroup membership before resume')
                    if process_identity(pid)!=actual:raise RuntimeError('stopped Gateway identity changed before resume')
                    row['actual_gateway_ARM_mapping_before_resume']={'identity':actual,'actual_stopped_ARM':actual,
                        'ancestry':candidate['ancestry'],'target_proof':proofs[0],
                        'current_mapping_check':True,'live_cgroup_membership':evidence.before['membership'],
                        'scope':'Stopped after detecting its original mapped image; no claim of Gateway pre-main instrumentation'}
                    evidence.before_resume(pid, actual)
                    owned.send(candidate,signal.SIGCONT);return
                except (FileNotFoundError,ProcessLookupError):continue

        master=None
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

        def run(name, arguments, required, timeout, marker=None, stopped=False):
            command = ['/usr/bin/chroot',str(root),'/bin/bash','-lc',
                'source /opt/ros/noetic/setup.bash && exec "$@"','arm-ros1-search',*arguments]
            row = {'name':name,'command':command,'status':'FAIL','timeout_seconds':timeout};report['commands'].append(row);save()
            log_path = out/(name+'.log')
            with log_path.open('xb') as log:
                child = subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,env=environment,start_new_session=True)
            children.append(child);row['pid'] = child.pid
            owned.acquire(child.pid,name,[child.pid])
            root_birth = process_identity(child.pid)['starttime_ticks'];row['launcher_starttime_ticks'] = root_birth
            evidence.start(child,name+'/',command)
            if stopped:
                deadline = time.monotonic()+15
                while child.poll() is None and time.monotonic()<deadline:
                    monitor()
                    proof = owned_stopped_arm(evidence,child,name+'/',root_birth,[required[0]])
                    if proof is not None:
                        evidence.check([required[0]],name+'/');actual = proof['actual_stopped_ARM']
                        # pidfd pins the actual stopped process before signalling.
                        pinned = os.pidfd_open(actual['pid'])
                        try:
                            if process_identity(actual['pid'])!=actual:
                                raise RuntimeError('actual stopped ARM identity changed before resume')
                            refreshed=owned_stopped_arm(evidence,child,name+'/',root_birth,[required[0]])
                            if refreshed is None or refreshed['actual_stopped_ARM']!=actual:
                                raise RuntimeError('stopped ARM live map/cgroup proof changed after pidfd acquisition')
                            proof=refreshed
                            evidence.before_resume(actual['pid'], actual)
                            signal.pidfd_send_signal(pinned,signal.SIGCONT)
                        finally:os.close(pinned)
                        row['actual_ARM_mapping_before_resume'] = proof;break
                    time.sleep(.002)
                if not row.get('actual_ARM_mapping_before_resume'):
                    raise RuntimeError('actual ARM evidence barrier was not reached: '+name)
            deadline = time.monotonic()+timeout;peak = 0
            while child.poll() is None:
                monitor()
                peak = max(peak,process_tree_rss(child.pid),owned.rss(name))
                if peak>2*1024**3:raise RuntimeError('owned command RSS exceeds 2 GiB')
                if 'ros1-current-gateway' in required:stop_gateway(child,name,row)
                if time.monotonic()>deadline:raise TimeoutError('bounded ROS1 search command deadline: '+name)
                time.sleep(.01)
            row['returncode'] = child.returncode;row['peak_observed_subtree_RSS_bytes'] = peak;row['log_sha256'] = digest(log_path)
            if row['returncode']:raise RuntimeError('ROS1 search command returned failure: '+name)
            evidence.check(required,name+'/')
            if 'ros1-current-gateway' in required and not row.get('actual_gateway_ARM_mapping_before_resume'):
                raise RuntimeError('missing actual stopped Gateway mapping before resume')
            if marker is not None:
                lines=log_path.read_text(errors='replace').splitlines()
                selected=[line for line in lines if line==marker or line.startswith(marker) and marker.endswith('checks=') and line[len(marker):].isdecimal() and int(line[len(marker):])>0]
                if len(selected)!=1 or any(line.startswith('FAIL ') for line in lines):
                    raise RuntimeError('exact unique semantic completion marker missing or failure logged: '+name)
            row['status'] = 'PASS';save();print('PASS',name,flush=True)

        if args.mode in ('prepare','all'):
            compiler = rooted(sdk,'/usr/bin/aarch64-linux-gnu-g++-9')
            frontend = next(rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/9/cc1plus')
                for base in ('usr/lib','usr/libexec') if rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/9/cc1plus').is_file())
            report['compiler'] = evidence.target('native-gcc',compiler)
            report['frontend'] = evidence.target('native-frontend',frontend)
            evidence.target('native-cmake',rooted(sdk,'/usr/bin/cmake'))
            report['tools']={label:evidence.targets[label] for label in ('native-gcc','native-frontend','native-cmake')}
            tool_pins=[pin_file(value['path']) for value in report['tools'].values()];prepared_pins.extend(tool_pins)
            (temporary/'evidence-stop.hpp').write_text(BARRIER)
            generated = temporary/'cmake';generated.mkdir();(generated/'CMakeLists.txt').write_text(cmake())
            run('configure',['/usr/bin/cmake','-S','/tmp/cmake','-B','/tmp/build',
                '-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-9','-DCMAKE_BUILD_TYPE=Release',
                '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'],['native-cmake'],120)
            run('compile',['/usr/bin/cmake','--build','/tmp/build','--parallel','1','--verbose'],
                ['native-cmake','native-gcc','native-frontend'],600)
            report['compilation_status']='PASS'
            for label, relative in PROBES.items():
                binary = temporary/'build'/label;evidence.target(label,binary)
                meta = temporary/'build/CMakeFiles'/(label+'.dir')
                database,database_binding=read_json_binding(temporary/'build/compile_commands.json')
                recipe=fixture_recipe(label,relative,database,(out/'compile.log').read_text())
                actual_dep=temporary/recipe['effective_dependency_output'][5:]
                if not actual_dep.is_file() or actual_dep.is_symlink() or not actual_dep.resolve().is_relative_to(temporary):
                    raise RuntimeError('actual effective -MF fixture depfile unavailable or escaped')
                copied = out/(label+'.d');shutil.copy2(actual_dep,copied)
                dependency_rule(copied.read_text(),recipe)
                inputs = closure(copied,source,sdk,temporary)
                if '/work/'+relative not in inputs or '/tmp/evidence-stop.hpp' not in inputs or not any(k.startswith('/usr/') for k in inputs) or not any(k.startswith('/opt/ros/noetic/') for k in inputs):
                    raise RuntimeError('actual GCC -MD closure missing original source, barrier or SDK: '+label)
                item = {'binary':require_arm(binary), 'raw_dependencies':{'path':str(copied),'sha256':digest(copied),'inputs':inputs},'metadata':{},'actual_compiler_recipe':recipe}
                for name,path in [('flags',meta/'flags.make'),('link',meta/'link.txt'),
                        ('compile_commands',temporary/'build/compile_commands.json'),('cmake',generated/'CMakeLists.txt'),('verbose',out/'compile.log')]:
                    item['metadata'][name] = {'path':str(path),'sha256':digest(path)}
                validate_fixture_link(label,recipe,(meta/'link.txt').read_text(),(out/'compile.log').read_text())
                prepared_pins.extend([pin_file(binary),pin_file(copied),
                    *[pin_file(value['path']) for value in inputs.values()],
                    *[pin_file(value['path']) for value in item['metadata'].values()]])
                report['probes'][label] = item
        else:
            original,prepared_binding = read_json_binding(args.prepared);prepared_pins.append(prepared_binding)
            validate_prepared_contract(original,report['source_manifest_sha256'])
            if original['cgroup_incarnation_before'] != before:
                raise RuntimeError('prepared compilation and behavior require the same inherited cgroup incarnation')
            if original.get('source')!=str(source) or original.get('rootfs')!=str(sdk):
                raise RuntimeError('prepared exact frozen source/canonical SDK paths differ')
            for name,item in original['driver_inputs'].items():
                for key in ('original_binding','archive_binding'):
                    pin=pin_file(item[key]['path']);prepared_pins.append(pin)
                    if pin!=item[key] or pin['sha256']!=item['sha256']:
                        raise RuntimeError('prepared executed helper identity/archive changed: '+name)
            architecture_ref = original['architecture_evidence'];architecture_path = Path(architecture_ref['path'])
            architecture_rows,architecture_pin=architecture_binding(architecture_path,prepared_pins)
            if architecture_pin['sha256']!=architecture_ref['sha256']:
                raise RuntimeError('prepared actual compilation ARM/cgroup evidence changed or failed')
            prior_pins=len(prepared_pins);validate_compilation_architecture(original,architecture_rows,sdk,prepared_pins)
            tool_pins=prepared_pins[prior_pins:];report['tools']=original['tools']
            if report['tools']!={label:architecture_rows['arm_targets'][label] for label in ('native-gcc','native-frontend','native-cmake')}:
                raise RuntimeError('prepared exact compiler tool inventory differs')
            prepared_sdk,prepared_sdk_binding=read_json_binding(original['sdk_before']['path'],limit=256*1024**2);prepared_pins.append(prepared_sdk_binding)
            if prepared_sdk_binding['sha256'] != original['sdk_before']['sha256'] or prepared_sdk != sdk_before or original['sdk_root_aliases'] != aliases_before:
                raise RuntimeError('prepared complete SDK byte/keyset/root-alias closure differs')
            report['prepared'] = {**prepared_binding,
                'actual_compilation_architecture_evidence':architecture_ref}
            for label,item in original['probes'].items():
                original_binary = Path(item['binary']['path']);facts = require_arm(original_binary)
                if facts!=item['binary']:raise RuntimeError('prepared native binary identity changed: '+label)
                dep = item['raw_dependencies'];dep_path = Path(dep['path'])
                if digest(dep_path)!=dep['sha256']:raise RuntimeError('prepared GCC dependencies changed: '+label)
                original_temp = original_binary.parent.parent
                if closure(dep_path,Path(original['source']),Path(original['rootfs']),original_temp)!=dep['inputs']:
                    raise RuntimeError('prepared reported dependency exactset differs from actual original GCC -MD file: '+label)
                for logical,value in dep['inputs'].items():
                    path = source/logical[6:] if logical.startswith('/work/') else rooted(sdk,logical) if logical.startswith(('/usr/','/opt/')) else Path(value['path'])
                    if not path.is_file() or digest(path)!=value['sha256'] or path.stat().st_size!=value['bytes']:
                        raise RuntimeError('prepared source/SDK/generated input changed: '+logical)
                    prepared_pins.append(pin_file(path))
                barrier=Path(dep['inputs']['/tmp/evidence-stop.hpp']['path'])
                if barrier.read_text()!=BARRIER or Path(item['metadata']['cmake']['path']).read_text()!=cmake():
                    raise RuntimeError('prepared generated barrier/CMake recipe differs from current exact recipe')
                for value in item['metadata'].values():
                    if digest(value['path'])!=value['sha256']:raise RuntimeError('prepared raw codegen/link metadata changed')
                prepared_database,prepared_database_binding=read_json_binding(item['metadata']['compile_commands']['path']);prepared_pins.append(prepared_database_binding)
                verbose=Path(item['metadata']['verbose']['path']).read_text()
                recipe=fixture_recipe(label,PROBES[label],prepared_database,verbose)
                if recipe!=item['actual_compiler_recipe']:
                    raise RuntimeError('prepared actual fixture compiler recipe changed')
                dependency_rule(dep_path.read_text(),recipe)
                validate_fixture_link(label,recipe,Path(item['metadata']['link']['path']).read_text(),verbose)
                prepared_pins.extend([pin_file(original_binary),pin_file(dep_path),*[pin_file(value['path']) for value in item['metadata'].values()]])
                shutil.copy2(original_binary,temporary/label);evidence.target(label,temporary/label)
                if require_arm(original_binary)!=facts or evidence.targets[label]['sha256']!=facts['sha256']:
                    raise RuntimeError('prepared product identity changed during private copy: '+label)
                if (evidence.targets[label]['device'],evidence.targets[label]['inode'])==(facts['device'],facts['inode']):
                    raise RuntimeError('prepared product copy must own a distinct file identity: '+label)
                report['probes'][label] = {**item,'binary':require_arm(temporary/label)}
            report['compilation_status']='REUSED_QUALIFIED_PREPARATION'
        if args.mode in ('execute','all'):
            gateway_facts, gateway_pins, gateway_binding = qualify_gateway(args.gateway,args.gateway_report,source,sdk,frozen,sdk_before,aliases_before)
            prepared_pins.extend(gateway_pins);report['gateway_qualification']=gateway_binding
            loader = gateway_library_environment(gateway_binding, sdk, sdk_before)
            prepared_pins.extend(loader['input_pins'])
            environment.update(loader['environment'])
            report['native_gateway_library_environment'] = loader
            shutil.copy2(args.gateway,temporary/'gateway');report['gateway'] = evidence.target('ros1-current-gateway',temporary/'gateway')
            if require_arm(args.gateway)!=gateway_facts or report['gateway']['sha256']!=gateway_facts['sha256']:
                raise RuntimeError('complete qualified Gateway identity changed during private copy')
            if (report['gateway']['device'],report['gateway']['inode'])==(gateway_facts['device'],gateway_facts['inode']):
                raise RuntimeError('qualified Gateway copy must own a distinct file identity')
            report['gateway_report'] = gateway_binding['report']
            lock = Path('/tmp')/f'uwvm-arm-ros1-master-{args.master_port}.lock'
            fd = os.open(lock,os.O_CREAT|os.O_EXCL|os.O_WRONLY,0o600);os.write(fd,str(os.getpid()).encode())
            stat = os.fstat(fd);os.close(fd);port_lock = (lock,stat.st_dev,stat.st_ino)
            start_master()
            for label in PROBES:
                inside = '/tmp/'+label if args.mode=='execute' else '/tmp/build/'+label
                arguments=[inside,'/tmp/gateway'] if PROBE_CONTRACTS[label]['requires_gateway'] else [inside]
                required=[label,'ros1-current-gateway'] if PROBE_CONTRACTS[label]['requires_gateway'] else [label]
                run('behavior-'+label,arguments,required,120,PROBE_CONTRACTS[label]['marker'],stopped=True)
                report['actual_native_behavior_probes']+=1
        status = 'PREPARED' if args.mode=='prepare' else 'PASS'
    except BaseException as error:
        report['error'] = traceback.format_exc()
        if 'BLOCKED' in str(error):status = 'BLOCKED'
    finally:
        try:stop()
        except BaseException:
            report['process_cleanup']={'status':'FAIL','remaining_owned_processes':['cleanup unavailable'],
                'error':traceback.format_exc()};status='FAIL'
        report['remaining_owned_processes']=report['process_cleanup'].get('remaining_owned_processes',['cleanup unavailable'])
        try:
            report['source_preserved'] = initial is not None and source_state(source,frozen)==initial and digest(manifest)==report['source_manifest_sha256']
            report['sdk_preserved'] = sdk_before is not None and sdk_inventory(sdk)==sdk_before and sdk_root_aliases(sdk)==aliases_before
            report['driver_inputs_preserved'] = all(digest(value['path'])==value['sha256'] and digest(value['archive'])==value['sha256'] for value in report['driver_inputs'].values())
            report['prepared_inputs_preserved'] = pins_preserved(prepared_pins)
            report['tools_preserved']=pins_preserved(tool_pins) and all(require_arm(value['path'])==value for value in report['tools'].values())
            report['gateway_inputs_preserved']=pins_preserved(gateway_pins)
            if not all(report[key] for key in ('source_preserved','sdk_preserved','driver_inputs_preserved',
                    'prepared_inputs_preserved','tools_preserved','gateway_inputs_preserved')):status = 'FAIL'
        except BaseException:report['preservation_error'] = traceback.format_exc();status = 'FAIL'
        if evidence:
            try:
                result = evidence.finish('PASS' if status=='PREPARED' else status)
                status = 'PREPARED' if status=='PREPARED' and result['status']=='PASS' else result['status']
                report['architecture_evidence'] = {'path':str(evidence.out/'architecture-evidence.json'),'sha256':digest(evidence.out/'architecture-evidence.json')}
            except BaseException:
                report['architecture_error']=traceback.format_exc();status='FAIL'
            try:
                report['cgroup_incarnation_after'] = incarnation()
                report['memory_swap_max_after']=report['cgroup_incarnation_after']['memory.swap.max']
                report['same_cgroup_incarnation'] = before is not None and before==report['cgroup_incarnation_after']
            except BaseException:
                report['cgroup_incarnation_error'] = traceback.format_exc();report['same_cgroup_incarnation'] = False
            if not report['same_cgroup_incarnation']:status = 'FAIL'
        if report.get('process_cleanup',{}).get('status')!='PASS':status='FAIL'
        report['retained_input_bindings']=prepared_pins
        if report.get('master'):
            report['master']['log_sha256']=digest(report['master']['log'])
            report['master']['returncode_after_owned_cleanup']=master.returncode
        report['mount_cleanup'] = []
        for path in reversed(mounts):
            try:
                result = subprocess.run(['umount','-R',str(path)],capture_output=True,text=True,timeout=30)
                report['mount_cleanup'].append({'path':str(path),'returncode':result.returncode,'stderr':result.stderr})
                if result.returncode:status = 'FAIL'
            except BaseException:
                report['mount_cleanup'].append({'path':str(path),'returncode':None,'error':traceback.format_exc()});status='FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4]==str(root) or line.split()[4].startswith(str(root)+'/')]
        if report['remaining_owned_mounts']:status = 'FAIL'
        if port_lock:
            path, device, inode = port_lock
            try:
                stat = path.stat()
                if (stat.st_dev,stat.st_ino)==(device,inode):path.unlink()
                else:status = 'FAIL'
            except FileNotFoundError:pass
        if status=='PASS' and (args.mode=='prepare' or report['actual_native_behavior_probes']!=len(PROBES)):
            status='FAIL'
        report['actual_behavior_status']='PASS' if status=='PASS' else 'NOT_PERFORMED' if args.mode=='prepare' else status
        report['status'] = status;save()
    print(status,out/'report.json',flush=True)
    return 0 if status in ('PREPARED','PASS') else 77 if status=='BLOCKED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
