#!/usr/bin/env python3
"""Prepare and execute exactly two frozen ROS2 local-event probes on AArch64.

Run only in the inherited 64-GiB, swap-disabled cgroup and a private mount
namespace under the parent's fixed QEMU/binfmt lease. Preparation is PREPARED,
never a ROS behavior verdict. Execution requires the current qualified Gateway,
both fixture identities, stopped live ARM mappings, immutable compiler -MD
closure, zero OOM deltas and complete owned-process/mount cleanup.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import posixpath
import shlex
import shutil
import signal
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm, read_json_binding
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state
from run_resources import bind_frozen_work
from run_action_qos import owned_stopped_arm, process_identity
from gateway_qualification import read_file_binding, pins_preserved, qualify_gateway, architecture_binding
from native_runtime_environment import gateway_library_environment

PROBES = {
    'local-parameter-events-contract': 'test/0073.ros-tests/support/source/ros_local_parameter_events/contract_ros2.cc',
    'local-parameter-events-ipc': 'test/0073.ros-tests/support/source/ros_local_parameter_events/native_ros2.cc',
}
PROBE_CONTRACTS = {
    'local-parameter-events-contract': {'source': PROBES['local-parameter-events-contract'],
        'gateway_required': False, 'marker': 'PASS local ROS2 parameter event trusted contracts checks=33 actual declaration overrides; committed timeout110; exact queue grant; weak ownership and ceiling; no native-publication duplicate; UTF8/NUL/finite/stamp/rate'},
    'local-parameter-events-ipc': {'source': PROBES['local-parameter-events-ipc'],
        'gateway_required': True, 'marker': 'PASS local ROS2 parameter events complete_mutations=27 filtered_checks=135 retirements=3 overflow_policies=3 exact SDK envelopes; two authenticated sessions; nine kinds; tag0 and legacy; no native ParameterEvent writer; observed_gateway_dds_events=0'},
}
DRIVER_INPUTS = ('run_local_parameter_events.py', 'runtime.py', 'rebuild_affected.py',
                 'run_resources.py', 'build_rpc_reader_diagnostic.py', 'run_action_qos.py', 'run_unit.py',
                 'gateway_qualification.py', 'native_runtime_environment.py')
GATEWAY_LABEL = 'qualified-ros2-gateway'
GATEWAY_TARGET = 'uwvm-ros2-gateway'
DOMAIN = 203
BARRIER = '''#include <csignal>
__attribute__((constructor)) static void local_events_arm_evidence_stop() { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }
'''


def complete_identity(actual, expected):
    return all(actual.get(key) == expected.get(key) for key in
               ('path', 'sha256', 'bytes', 'device', 'inode', 'elf_class', 'elf_data', 'e_machine'))


def process_children(pid):
    """A native DDS/compiler thread can fork; inspect every task's children."""
    children = set()
    try:
        tasks = list(Path(f'/proc/{pid}/task').iterdir())
    except (FileNotFoundError, ProcessLookupError): return []
    for task in tasks:
        try: children.update(int(value) for value in (task/'children').read_text().split())
        except (FileNotFoundError, ProcessLookupError): pass
    return sorted(children)


def require_current_gateway(report, path, manifest_sha256):
    """Bind the actual selected product to current source/dependency qualification."""
    if report.get('status') != 'PASS' or report.get('source_manifest_sha256') != manifest_sha256:
        raise RuntimeError('current-source successful Gateway qualification required')
    item = report.get('targets', {}).get(GATEWAY_TARGET)
    if not isinstance(item, dict) or not complete_identity(require_arm(path), item):
        raise RuntimeError('selected Gateway differs from the complete qualified ARM identity')
    dependencies = report.get('source_dependencies', {}).get(GATEWAY_TARGET)
    if dependencies is None:
        candidates = {key: value for key, value in report.get('compiled_source_dependencies', {}).items()
                      if key.startswith('ros2:') and 'uwvm-ros2-gateway.dir/' in key}
        dependencies = {}
        for values in candidates.values():
            if not isinstance(values, dict):
                raise RuntimeError('invalid qualified Gateway compiler closure')
            for name, value in values.items():
                if name in dependencies and dependencies[name] != value:
                    raise RuntimeError('conflicting qualified Gateway compiler inputs')
                dependencies[name] = value
    if not isinstance(dependencies, dict) or not {'src/ros/gateway/ros2/main.cc', 'src/ros/gateway/ros2/resources.h'}.issubset(dependencies):
        raise RuntimeError('qualified Gateway closure omits current local event implementation')
    source = Path(report['source']).resolve(strict=True)
    frozen, manifest_binding = read_json_binding(source.parent/'source-manifest.json')
    if manifest_binding['sha256'] != manifest_sha256:
        raise RuntimeError('qualified Gateway source manifest changed')
    for name, value in dependencies.items():
        sha = value.get('sha256') if isinstance(value, dict) else value
        selected = (source/name).resolve(strict=True)
        if not selected.is_relative_to(source) or frozen.get(name, {}).get('sha256') != sha or digest(selected) != sha:
            raise RuntimeError('qualified Gateway source dependency differs: '+name)
    return {'target': GATEWAY_TARGET, 'binary': item, 'source_dependencies': dependencies}


def validate_prepared_contract(original, manifest_sha256):
    """A partial, old-recipe or behavior-only document cannot qualify preparation."""
    if (original.get('status') != 'PREPARED' or original.get('kind') != 'ros2-local-parameter-events-arm'
            or type(original.get('format_version')) is not int or original.get('format_version') != 1 or original.get('mode') != 'prepare'
            or type(original.get('domain')) is not int or original.get('domain') != DOMAIN
            or original.get('compilation_status') != 'PASS' or original.get('native_behavior_status') != 'NOT_PERFORMED'
            or type(original.get('actual_native_behavior_probes')) is not int or original.get('actual_native_behavior_probes') != 0 or set(original.get('probes', {})) != set(PROBES)
            or original.get('probe_contracts') != PROBE_CONTRACTS
            or original.get('source_manifest_sha256') != manifest_sha256):
        raise RuntimeError('exact current two-probe PREPARED compilation required')
    if (original.get('same_cgroup_incarnation') is not True or original.get('memory_swap_max_before') != '0'
            or original.get('memory_swap_max_after') != '0' or original.get('cgroup_incarnation_before') != original.get('cgroup_incarnation_after')
            or not original.get('cgroup_incarnation_before') or original.get('remaining_owned_processes') != []
            or original.get('remaining_owned_mounts') != [] or not original.get('mount_cleanup')
            or any(type(row.get('returncode')) is not int or row['returncode'] != 0 for row in original['mount_cleanup'])
            or len(original.get('process_cleanup', [])) != 2
            or any(row.get('errors') != [] or row.get('remaining') != [] for row in original['process_cleanup'])):
        raise RuntimeError('preparation lacks exact cgroup/swap/cleanup qualification')
    identity = original['cgroup_incarnation_before']
    if (not {'path','device','inode','boot_id','membership','memory.swap.max',
             'parent_recorded_container_started_at'}.issubset(identity)
            or identity['memory.swap.max'] != '0' or not identity['membership'] or not identity['boot_id']
            or not identity['parent_recorded_container_started_at']):
        raise RuntimeError('prepared inherited cgroup incarnation binding incomplete')
    required = ('source_preserved', 'sdk_preserved', 'driver_inputs_preserved', 'tools_preserved',
                'prepared_inputs_preserved', 'gateway_inputs_preserved', 'immutable_inputs_preserved')
    if not all(original.get(key) is True for key in required) or set(original.get('driver_inputs', {})) != set(DRIVER_INPUTS):
        raise RuntimeError('preparation immutable source/SDK/tools/driver proof incomplete')
    for name, recorded in original['driver_inputs'].items():
        if digest(Path(__file__).with_name(name)) != recorded.get('sha256'):
            raise RuntimeError('prepared executed recipe differs: '+name)
    commands = original.get('commands', [])
    if len(commands) != 2 or {row.get('name') for row in commands} != {'configure', 'compile'} or any(
            row.get('status') != 'PASS' or type(row.get('returncode')) is not int or row['returncode'] != 0
            or type(row.get('peak_observed_subtree_RSS_bytes')) is not int
            or not 0 <= row['peak_observed_subtree_RSS_bytes'] <= 2*1024**3 for row in commands):
        raise RuntimeError('exact successful serial configure/compile records required')


def validate_compilation_architecture(original, architecture, sdk, pins):
    """Bind tools and products to actual samples of the successful compile tree."""
    if any(architecture.get('cgroup_incarnation_before', {}).get(key) != original['cgroup_incarnation_before'].get(key)
           for key in ('path','device','inode','boot_id')):
        raise RuntimeError('prepared raw architecture cgroup incarnation differs from driver binding')
    compiler = rooted(sdk, '/usr/bin/aarch64-linux-gnu-g++-13')
    frontend = next((rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus')
                     for base in ('usr/lib', 'usr/libexec')
                     if rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus').is_file()), None)
    if frontend is None: raise RuntimeError('current canonical Jazzy GCC13 frontend unavailable')
    paths = {'native-gcc':compiler, 'native-frontend':frontend, 'native-cmake':rooted(sdk, '/usr/bin/cmake')}
    tools = {label:require_arm(path) for label,path in paths.items()}
    pins.extend(read_file_binding(path) for path in paths.values())
    if (original.get('tools') != tools or original.get('compiler') != tools['native-gcc']
            or original.get('frontend') != tools['native-frontend']
            or any(architecture.get('arm_targets', {}).get(label) != fact for label,fact in tools.items())):
        raise RuntimeError('prepared actual ARM tool identities differ')
    build = next(row for row in original['commands'] if row['name'] == 'compile')
    if (type(build.get('pid')) is not int or build['pid'] <= 0
            or type(build.get('launcher_starttime_ticks')) is not int or build['launcher_starttime_ticks'] <= 0):
        raise RuntimeError('prepared successful compiler launcher birth is unavailable')
    def owned(sample):
        return (sample.get('name') == 'compile/' and sample.get('ancestry', [None])[0] == build['pid']
                and sample.get('live_cgroup_procs') is True
                and sample.get('membership') == architecture['cgroup_before']['membership'])
    samples = [sample for sample in architecture.get('actual_process_samples', []) if owned(sample)]
    if not any(sample.get('pid') == build['pid'] and sample.get('ancestry') == [build['pid']]
               and sample.get('starttime_ticks') == build['launcher_starttime_ticks'] for sample in samples):
        raise RuntimeError('prepared compile launcher birth/ancestry proof unavailable')
    mapped = {label for sample in samples
              if type(sample.get('starttime_ticks')) is int and sample['starttime_ticks'] >= build['launcher_starttime_ticks']
              and sample.get('kernel_exe_elf', {}).get('e_machine') == 62
              and sample['kernel_exe_elf'].get('sha256') in architecture.get('expected_host_emulator_sha256', [])
              for label in sample.get('mapped_arm_targets', [])}
    if not set(tools).issubset(mapped): raise RuntimeError('prepared actual compiler subtree mappings incomplete')
    for label,item in original['probes'].items():
        if architecture.get('arm_targets', {}).get(label) != item['binary']:
            raise RuntimeError('prepared product differs from actual architecture target: '+label)


def validate_semantic_marker(label, text):
    """Accept exactly one complete original fixture line, including DDS zero."""
    lines = [line for line in text.splitlines() if line.startswith('PASS local ROS2 parameter')]
    if lines != [PROBE_CONTRACTS[label]['marker']]:
        raise RuntimeError('one exact semantic completion line required: '+label)


def cmake():
    lines = ['cmake_minimum_required(VERSION 3.16)', 'project(arm_local_parameter_events LANGUAGES CXX)',
             'set(CMAKE_CXX_STANDARD 20)', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)',
             'find_package(ament_cmake REQUIRED)', 'find_package(rclcpp REQUIRED)',
             'find_package(rcl_interfaces REQUIRED)', 'find_package(rcl_action REQUIRED)',
             'find_package(rosidl_typesupport_cpp REQUIRED)']
    for label, source in PROBES.items():
        lines += [f'add_executable({label} /work/{source})',
                  f'target_include_directories({label} PRIVATE /work/src /work/third-parties/fast_io/include)',
                  f'target_compile_options({label} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++20 -MD -include /tmp/evidence-stop.hpp)',
                  f'ament_target_dependencies({label} rclcpp rcl_interfaces rcl_action rosidl_typesupport_cpp)',
                  f'target_link_libraries({label} pthread)']
    return '\n'.join(lines)+'\n'


def compile_recipe(label, database, log):
    """Choose the actual verbose GCC command and its last effective -MF."""
    source = '/work/'+PROBES[label]
    if len(database) != len(PROBES) or {posixpath.normpath(row['file']) for row in database} != {'/work/'+value for value in PROBES.values()}:
        raise RuntimeError('exact two original fixture compile database sources required')
    entries = [row for row in database if posixpath.normpath(row['file']) == source]
    if len(entries) != 1 or entries[0]['directory'] != '/tmp/build':
        raise RuntimeError('one original fixture compile database entry required')
    compiler = '/usr/bin/aarch64-linux-gnu-g++-13'

    def options(argv, require_output=True):
        if not argv or argv[0] != compiler or argv.count('-c') != 1 or argv[argv.index('-c')+1:] != [source]:
            raise RuntimeError('actual fixture source/compiler argv differs')
        cleaned, outputs = [], [];standard = optimization = debug = ndebug = None;index = 0
        forbidden = ('-M', '-MM', '-MMD', '-E', '-S', '-fsyntax-only', '-MG', '-MP')
        while index < len(argv):
            token = argv[index]
            if token in forbidden or token.startswith(('@', '-Wp,')) or token == '-Xpreprocessor':
                raise RuntimeError('opaque or non-system-inclusive compiler recipe')
            if token in ('-MD', '-MF', '-MT', '-MQ'):
                if token != '-MD':
                    index += 1
                    if index >= len(argv): raise RuntimeError('missing dependency output operand')
                    if token == '-MF': outputs.append(argv[index])
            elif token.startswith(('-MF', '-MT', '-MQ')):
                if token.startswith('-MF'): outputs.append(token[3:])
            elif token in ('-D', '-U'):
                cleaned.append(token);index += 1
                if index >= len(argv): raise RuntimeError('missing macro operand')
                cleaned.append(argv[index])
                if argv[index].split('=',1)[0] == 'NDEBUG': ndebug = token == '-D'
            else:
                cleaned.append(token)
                if token.startswith('-std='): standard = token
                if token.startswith('-O'): optimization = token
                if token.startswith('-g'): debug = token
                if token.startswith(('-D', '-U')) and token[2:].split('=', 1)[0] == 'NDEBUG': ndebug = token.startswith('-D')
            index += 1
        if '-MD' not in argv or require_output and not outputs or (standard, optimization, debug, ndebug) != ('-std=gnu++20', '-O1', '-g0', False):
            raise RuntimeError('effective CPP20/O1/g0/UNDEBUG/-MD controls differ')
        if cleaned.count('-include') != 1 or cleaned.index('-include')+1 >= len(cleaned) or cleaned[cleaned.index('-include')+1] != '/tmp/evidence-stop.hpp':
            raise RuntimeError('exact constructor barrier compiler operand required')
        return cleaned, posixpath.normpath(posixpath.join('/tmp/build', outputs[-1])) if outputs else None

    candidates = []
    for number, line in enumerate(log.splitlines(), 1):
        if compiler not in line or source not in line: continue
        argv = shlex.split(line)
        if len(argv) > 3 and argv[0] == 'cd' and argv[2] == '&&':
            if argv[1] != '/tmp/build': raise RuntimeError('verbose compiler working directory differs')
            argv = argv[3:]
        if not argv or argv[0] != compiler or '-c' not in argv: continue
        actual, depfile = options(argv)
        expected, _ = options(entries[0].get('arguments') or shlex.split(entries[0]['command']),False)
        if actual != expected or not depfile.startswith('/tmp/build/CMakeFiles/'+label+'.dir/') or not depfile.endswith('.d'):
            raise RuntimeError('actual compiler command or final dependency output differs')
        if argv.count('-o') != 1 or argv.index('-o')+1 >= len(argv): raise RuntimeError('one actual object output required')
        object_path = posixpath.normpath(posixpath.join('/tmp/build',argv[argv.index('-o')+1]))
        if not object_path.startswith('/tmp/build/CMakeFiles/'+label+'.dir/') or not object_path.endswith('.o'):
            raise RuntimeError('actual compiled object escapes target metadata')
        candidates.append({'argv': argv, 'working_directory': '/tmp/build', 'verbose_line': number,
                           'effective_mf': depfile,'compiled_object_guest_path':object_path})
    if len(candidates) != 1: raise RuntimeError('one actual verbose CPP20 fixture compilation required')
    return candidates[0]


def validate_fixture_link(label, recipe, link, verbose):
    """Require one original verbose GCC13 link of the actual compiled object."""
    argv = shlex.split(link)
    if (not argv or argv[0] != '/usr/bin/aarch64-linux-gnu-g++-13' or argv.count('-o') != 1
            or argv.index('-o')+1 >= len(argv)
            or posixpath.normpath(posixpath.join('/tmp/build', argv[argv.index('-o')+1])) != '/tmp/build/'+label
            or [posixpath.normpath(posixpath.join('/tmp/build', value)) for value in argv if value.endswith('.o')]
                != [recipe['compiled_object_guest_path']]
            or any(value.startswith(('@','-Wp,')) for value in argv)):
        raise RuntimeError('fixture actual link object/output differs')
    found = []
    for line in verbose.splitlines():
        if argv[0] not in line: continue
        actual = shlex.split(line)
        if len(actual)>3 and actual[:3] == ['cd', '/tmp/build', '&&']: actual = actual[3:]
        if actual == argv: found.append(line)
    if len(found) != 1: raise RuntimeError('one successful actual verbose fixture link required')


def fixture_closure(depfile,source,sdk,temporary,recipe):
    """Bind the actual object rule and dependency text from the same descriptor."""
    raw,binding = read_file_binding(depfile,limit=16*1024**2,retain=True)
    target,body = raw.decode('utf-8').replace('\\\n',' ').split(':',1)
    outputs = [posixpath.normpath(posixpath.join(recipe['working_directory'],item)) for item in shlex.split(target)]
    if outputs != [recipe['compiled_object_guest_path']]: raise RuntimeError('actual depfile rule differs from compiled object')
    inputs = {}
    for token in shlex.split(body):
        logical = posixpath.normpath(token)
        if logical.startswith('/work/'): path = source/logical[6:]
        elif logical.startswith('/tmp/'): path = temporary/logical[5:]
        elif logical.startswith(('/usr/','/opt/','/etc/','/lib/','/lib64/','/bin/','/sbin/')): path = rooted(sdk,logical)
        else: raise RuntimeError('unclassified actual compiler dependency: '+token)
        base = source if logical.startswith('/work/') else temporary if logical.startswith('/tmp/') else sdk
        if not path.resolve(strict=True).is_relative_to(base): raise RuntimeError('compiler dependency escapes owned/frozen root')
        pin = read_file_binding(path)
        inputs[logical] = {'path':str(path),'sha256':pin['sha256'],'bytes':pin['bytes']}
    return inputs,binding


class OwnedProcesses:
    """Pin only observed descendants; never signal a potentially reused group ID."""
    def __init__(self, process, evidence):
        self.process, self.evidence, self.records = process, evidence, {}
        self.unproven = {}

    def adopt(self, pid, parent):
        actual = process_identity(pid)
        # An already pinned birth remains ours after reparenting. A children
        # list alone never proves a newly observed, already-reparented birth.
        if pid in self.records:
            if self.records[pid]['birth'] != actual['starttime_ticks']: raise RuntimeError('owned descendant PID reused')
            return self.records[pid]
        expected_parent = os.getpid() if parent is None else parent['pid']
        if (actual['parent_pid'] != expected_parent or
                (parent is not None and actual['starttime_ticks'] < parent['birth'])):
            if actual['state'] in ('Z', 'X'): return None
            self.unproven[(pid, actual['starttime_ticks'])] = {
                'identity': actual, 'expected_parent': expected_parent}
            if parent is None:
                raise RuntimeError('direct Popen root lacks its observed caller parent')
            return None
        fd = os.pidfd_open(pid)
        try:
            after = process_identity(pid)
            # The first sample established ancestry. Parent retirement may
            # legitimately change PPID around pidfd_open; only PID and birth
            # identify this retained task. Current cgroup is checked below.
            if any(after[key] != actual[key] for key in ('pid', 'starttime_ticks')):
                raise RuntimeError('descendant birth changed while pinning')
        except BaseException:
            os.close(fd);raise
        # Register first, so a descendant escaping the accepted cgroup remains
        # ours to clean up rather than becoming an omitted process on failure.
        self.records[pid] = {'pid': pid, 'birth': actual['starttime_ticks'], 'pidfd': fd, 'parent': actual['parent_pid'],
            'acquisition': {'first': actual, 'after_pidfd': after,
                            'parent_changed': after['parent_pid'] != actual['parent_pid']}}
        self.unproven.pop((pid, actual['starttime_ticks']), None)
        self.check_scope(pid)
        return self.records[pid]

    def check_scope(self, pid):
        row = self.records[pid]
        def alive():
            try:
                current = process_identity(pid)
                return current['starttime_ticks'] == row['birth'] and current['state'] not in ('Z', 'X')
            except (FileNotFoundError, ProcessLookupError): return False
        if not alive(): return False
        try: membership = Path(f'/proc/{pid}/cgroup').read_text().splitlines()
        except (FileNotFoundError, ProcessLookupError): return False
        if membership != self.evidence.before['membership']:
            if not alive(): return False
            raise RuntimeError('owned descendant escaped inherited cgroup')
        # Procfs can outlive cgroup.procs removal during exit. Missing live
        # membership is unavailable proof, never permission to resume a task.
        return str(pid) in (self.evidence.leaf/'cgroup.procs').read_text().split() and alive()

    def live(self):
        result = []
        for row in self.records.values():
            try:
                current = process_identity(row['pid'])
                if current['starttime_ticks'] != row['birth']:
                    row['identity_error'] = 'owned PID path reused; pinned original is never replaced';continue
                if current['state'] not in ('Z', 'X'): result.append(row)
            except (FileNotFoundError, ProcessLookupError): pass
        return result

    def refresh(self):
        pending = list(self.live());seen = set()
        while pending:
            row = pending.pop()
            if row['pid'] in seen: continue
            seen.add(row['pid'])
            if not self.check_scope(row['pid']): continue
            try:
                for pid in process_children(row['pid']):
                    acquired = self.adopt(pid, row)
                    if acquired is not None: pending.append(acquired)
            except (FileNotFoundError, ProcessLookupError): pass
        if any(row.get('identity_error') for row in self.records.values()):
            raise RuntimeError('owned process path identity changed')

    def rss(self):
        """Sample every pinned live birth, including children forked by workers."""
        total = 0
        for row in self.live():
            try:
                text = Path(f"/proc/{row['pid']}/status").read_text()
                if process_identity(row['pid'])['starttime_ticks'] != row['birth']: continue
                for line in text.splitlines():
                    if line.startswith('VmRSS:'): total += int(line.split()[1])*1024
            except (FileNotFoundError,ProcessLookupError): pass
        return total

    def cleanup(self):
        errors = []
        # Popen may already exist when its initial metadata read/pidfd acquire
        # fails. Retry only that trusted unreaped direct root through the same
        # ancestry/birth/cgroup contract; never substitute a numeric PID kill.
        if self.process.pid not in self.records and self.process.poll() is None:
            try: self.adopt(self.process.pid, None)
            except (OSError, RuntimeError) as error:
                errors.append('direct Popen cleanup admission: ' + str(error))
        for signum, seconds in ((signal.SIGTERM, 3), (signal.SIGKILL, 3)):
            deadline = time.monotonic()+seconds
            while time.monotonic() < deadline:
                try: self.refresh()
                except (OSError, RuntimeError) as error: errors.append(str(error))
                alive = self.live()
                if not alive: break
                for row in reversed(alive):
                    try:
                        signal.pidfd_send_signal(row['pidfd'], signal.SIGCONT)
                        signal.pidfd_send_signal(row['pidfd'], signum)
                    except ProcessLookupError: pass
                time.sleep(.02)
            if not self.live(): break
        try: self.process.wait(timeout=1)
        except subprocess.TimeoutExpired: errors.append('owned launcher did not reap')
        remaining = [{key:value for key,value in row.items() if key != 'pidfd'} for row in self.live()]
        # Never signal a birth whose first parent sample was unavailable. A
        # later exit/zombie/replacement discharges only that old candidate; a
        # still-live same birth keeps the existing two-field cleanup FAIL.
        for (pid, birth), candidate in self.unproven.items():
            try:
                current = process_identity(pid)
                if current['starttime_ticks'] == birth and current['state'] not in ('Z', 'X'):
                    remaining.append({'identity': current, 'expected_parent': candidate['expected_parent'],
                                      'ownership': 'UNPROVEN_NO_SIGNAL', 'status': 'CLEANUP_INCOMPLETE'})
                    errors.append('CLEANUP_INCOMPLETE: live descendant ancestry was never proven')
            except (FileNotFoundError, ProcessLookupError): pass
        if self.process.pid not in self.records and self.process.poll() is None:
            remaining.append({'pid': self.process.pid, 'expected_parent': os.getpid(),
                              'ownership': 'UNPROVEN_DIRECT_POPEN_NO_SIGNAL', 'status': 'CLEANUP_INCOMPLETE'})
            errors.append('CLEANUP_INCOMPLETE: live direct Popen root could not be pinned')
        for row in self.records.values(): os.close(row['pidfd'])
        errors.extend(row['identity_error'] for row in self.records.values() if row.get('identity_error'))
        return {'errors':sorted(set(errors)), 'remaining':remaining}


def gateway_stop_proof(evidence, scope, fixture_proof, name):
    """Pause only the exact Gateway child of the already proven ARM fixture."""
    fixture = fixture_proof['actual_stopped_ARM'];pid = fixture['pid']
    if process_identity(pid)['starttime_ticks'] != fixture['starttime_ticks']: raise RuntimeError('IPC fixture birth changed')
    for child in process_children(pid):
        row = scope.records.get(child)
        if row is None: continue
        current = process_identity(child)
        if current['parent_pid'] != pid or current['starttime_ticks'] != row['birth']: continue
        evidence.observe(scope.process.pid, name)
        with evidence._lock:
            samples = [sample for sample in evidence.samples.values() if sample['name'] == name and sample['pid'] == child
                       and sample['starttime_ticks'] == row['birth'] and GATEWAY_LABEL in sample['mapped_arm_targets']]
        if not samples: continue
        signal.pidfd_send_signal(row['pidfd'], signal.SIGSTOP)
        deadline = time.monotonic()+3
        while time.monotonic() < deadline:
            current = process_identity(child)
            if (current['starttime_ticks'] != row['birth'] or current['parent_pid'] != pid
                    or current['state'] in ('Z', 'X')):
                raise RuntimeError('Gateway exited or changed birth/parent before stopped mapping proof: '
                                   + json.dumps(current, sort_keys=True))
            if current['state'] in ('T', 't'): break
            time.sleep(.002)
        else: raise RuntimeError('actual Gateway did not stop before evidence release')
        if not scope.check_scope(child): raise RuntimeError('stopped Gateway live membership unavailable')
        evidence.observe(scope.process.pid, name)
        host = Path(f'/proc/{child}/exe').stat()
        target = evidence.targets[GATEWAY_LABEL]
        match = evidence.mapped(child, target)
        with evidence._lock:
            candidates = [sample for sample in evidence.samples.values() if sample['opened_target_mappings']['identity_after']['state'] in ('T','t') and sample['name'] == name and sample['pid'] == child
                          and sample['starttime_ticks'] == row['birth'] and sample['parent_pid'] == pid
                          and sample['ancestry'] == fixture_proof['ancestry']+[child]
                          and sample['membership'] == evidence.before['membership'] and sample['live_cgroup_procs']
                          and GATEWAY_LABEL in sample['mapped_arm_targets']
                          and sample['kernel_exe_elf']['e_machine'] == 62
                          and sample['kernel_exe_elf']['sha256'] in evidence.emulator_shas
                          and (sample['kernel_exe_elf']['device'],sample['kernel_exe_elf']['inode']) == (host.st_dev,host.st_ino)]
        final_host = Path(f'/proc/{child}/exe').stat()
        if (not match or not candidates or process_identity(child) != current or not scope.check_scope(child)
                or (final_host.st_dev, final_host.st_ino) != (host.st_dev, host.st_ino)
                or process_identity(pid)['starttime_ticks'] != fixture['starttime_ticks']):
            raise RuntimeError('actual stopped Gateway identity/map/parent proof differs')
        proof = {'actual_stopped_ARM':current, 'ancestry':fixture_proof['ancestry']+[child],
                 'target_proof':candidates[-1], 'current_mapping_check':True, 'fixture_parent_birth':fixture['starttime_ticks']}
        evidence.before_resume(child, current)
        signal.pidfd_send_signal(row['pidfd'], signal.SIGCONT)
        return proof
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=('prepare','execute','all'), required=True)
    for name in ('source','rootfs','out','qemu','binfmt-evidence'):
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--prepared',type=Path)
    parser.add_argument('--gateway',type=Path)
    parser.add_argument('--gateway-report',type=Path)
    parser.add_argument('--container-started-at',required=True)
    args = parser.parse_args()
    if args.mode == 'execute' and args.prepared is None: parser.error('execute requires --prepared')
    if args.mode != 'execute' and args.prepared is not None: parser.error('--prepared applies only to execute')
    if args.mode in ('execute','all') and (args.gateway is None or args.gateway_report is None):
        parser.error('behavior requires the current qualified --gateway and --gateway-report')
    source,sdk = args.source.resolve(strict=True),args.rootfs.resolve(strict=True)
    out = args.out.resolve()
    if out.is_relative_to(source) or out.is_relative_to(sdk) or source.is_relative_to(out) or sdk.is_relative_to(out):
        parser.error('fresh output must be separate from source and original SDK')
    if any(any(char.isspace() or char == '\\' for char in str(path)) for path in (source, sdk, out)):
        parser.error('mount/dependency proof requires paths without mountinfo escaping')
    out.mkdir(parents=True,mode=0o700,exist_ok=False)
    root,temporary = out/'rootfs',out/'native-temp';root.mkdir();temporary.mkdir()
    report = {'status':'RUNNING','format_version':1,'kind':'ros2-local-parameter-events-arm',
        'mode':args.mode,'domain':DOMAIN,'scope':'Exactly two ROS2 native local parameter event fixtures; no Guest or complete original acceptance claim',
        'source':str(source),'rootfs':str(sdk),'probe_contracts':PROBE_CONTRACTS,
        'commands':[],'probes':{},'driver_inputs':{},'actual_native_behavior_probes':0,
        'native_behavior_status':'NOT_PERFORMED','compilation_status':'NOT_PERFORMED'}
    evidence = initial = sdk_before = aliases_before = incarnation_before = None
    scopes,pins,mounts,tools = [],[],[],{};domain_lock = None;lease_state = None;status = 'FAIL'

    def save():
        (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')

    def json_pin(path,limit=16*1024**2):
        value,binding = read_json_binding(path,limit=limit);pins.append(binding);return value,binding

    def file_pin(path):
        path = Path(path).resolve(strict=True)
        value = read_file_binding(path)
        pins.append(value);return value

    def check_pins():
        if not pins_preserved(pins): raise RuntimeError('immutable input binding changed')

    def checked(command):
        subprocess.run([str(value) for value in command],check=True,capture_output=True,timeout=30)

    def incarnation():
        info = evidence.leaf.stat()
        return {'path':str(evidence.leaf),'device':info.st_dev,'inode':info.st_ino,
                'membership':Path('/proc/self/cgroup').read_text().splitlines(),
                'boot_id':Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                'memory.swap.max':(evidence.leaf/'memory.swap.max').read_text().strip(),
                'parent_recorded_container_started_at':args.container_started_at}

    def guard():
        if incarnation() != incarnation_before or (evidence.leaf/'memory.swap.max').read_text().strip() != '0':
            raise RuntimeError('inherited cgroup incarnation/swap binding changed')
        if (evidence.leaf/'cpuset.cpus.effective').read_text().strip() != '0-31':
            raise RuntimeError('inherited explicit cpuset differs from 0-31')
        if int((evidence.leaf/'memory.current').read_text()) > 60*1024**3:
            raise RuntimeError('accepted cgroup current memory exceeds 60 GiB run budget')
        if lease_state is not None and (lease_state['path'].read_text() != lease_state['entry'] or
                os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != lease_state['name']):
            raise RuntimeError('active parent fixed QEMU lease changed')

    def run(name,arguments,required,timeout,marker=None,stopped=False,gateway=False):
        guard();check_pins()
        for label,facts in tools.items():
            if require_arm(facts['path']) != facts: raise RuntimeError('native tool identity changed: '+label)
        command = ['/usr/bin/chroot',str(root),'/bin/bash','-lc',
                   'source /opt/ros/jazzy/setup.bash && exec "$@"','arm-local-parameter-events',*arguments]
        row = {'name':name,'command':command,'status':'FAIL','required_arm_targets':list(required),
               'timeout_seconds':timeout};report['commands'].append(row);save()
        log = out/(name+'.log')
        with log.open('xb') as stream:
            process = subprocess.Popen(command,stdout=stream,stderr=subprocess.STDOUT,env=environment,start_new_session=True)
        scope = OwnedProcesses(process,evidence);scopes.append(scope);row['pid'] = process.pid
        scope.adopt(process.pid,None)
        birth = scope.records[process.pid]['birth'];row['launcher_starttime_ticks'] = birth
        evidence.start(process,name+'/',command)
        if stopped:
            deadline = time.monotonic()+15
            while process.poll() is None and time.monotonic()<deadline:
                scope.refresh();guard();proof = owned_stopped_arm(evidence,process,name+'/',birth,required[:1])
                if proof is not None:
                    actual = proof['actual_stopped_ARM'];owned = scope.records.get(actual['pid'])
                    if owned is None or owned['birth'] != actual['starttime_ticks']: raise RuntimeError('unowned stopped fixture')
                    evidence.check(required[:1],name+'/')
                    fresh = owned_stopped_arm(evidence,process,name+'/',birth,required[:1])
                    if (fresh is None or fresh['actual_stopped_ARM'] != actual
                            or fresh['ancestry'] != proof['ancestry'] or not scope.check_scope(actual['pid'])):
                        raise RuntimeError('fixture current birth/map/cgroup/host changed before resume')
                    row['actual_ARM_mapping_before_resume'] = fresh
                    evidence.before_resume(actual['pid'], actual)
                    signal.pidfd_send_signal(owned['pidfd'],signal.SIGCONT);break
                time.sleep(.002)
            if 'actual_ARM_mapping_before_resume' not in row: raise RuntimeError('stopped actual ARM fixture proof missing')
        deadline = time.monotonic()+timeout;peak = 0
        while process.poll() is None:
            scope.refresh();guard();peak = max(peak,scope.rss())
            if peak > (2 if name in ('configure','compile') else 8)*1024**3: raise RuntimeError('bounded native subtree RSS exceeded')
            if log.stat().st_size > 16*1024**2: raise RuntimeError('bounded native log exceeded')
            if gateway and 'actual_gateway_ARM_mapping_before_resume' not in row:
                proof = gateway_stop_proof(evidence,scope,row['actual_ARM_mapping_before_resume'],name+'/')
                if proof is not None: row['actual_gateway_ARM_mapping_before_resume'] = proof
            if time.monotonic() > deadline: raise TimeoutError('bounded native command deadline: '+name)
            time.sleep(.01)
        row.update(returncode=process.returncode,peak_observed_subtree_RSS_bytes=peak,log_sha256=digest(log))
        if process.returncode: raise RuntimeError('native command returned failure: '+name)
        scope.refresh()
        if scope.live(): raise RuntimeError('native command left an owned descendant alive')
        if gateway and 'actual_gateway_ARM_mapping_before_resume' not in row: raise RuntimeError('actual stopped Gateway proof missing')
        evidence.check(required,name+'/')
        if marker is not None: validate_semantic_marker(name.removeprefix('behavior-'),log.read_text(errors='replace'))
        guard();check_pins();row['status'] = 'PASS';save();print('PASS',name,flush=True)

    def interrupted(signum,frame):
        raise RuntimeError('native local event driver interrupted '+str(signum))

    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        if sys.platform != 'linux' or os.geteuid() != 0: raise RuntimeError('BLOCKED root Linux private namespace required')
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('BLOCKED launch unshare -m within the parent cgroup/binfmt lease')
        if not hasattr(os,'pidfd_open') or not hasattr(signal,'pidfd_send_signal'):
            raise RuntimeError('BLOCKED owned-process pidfd APIs required')
        if not (sdk/'.uwvm-task-owned').is_file(): raise RuntimeError('BLOCKED task-owned canonical Jazzy SDK required')
        probe_fd = os.pidfd_open(os.getpid())
        try: signal.pidfd_send_signal(probe_fd,0)
        finally: os.close(probe_fd)
        frozen,manifest_binding = json_pin(source.parent/'source-manifest.json')
        report['source_manifest'] = manifest_binding;report['source_manifest_sha256'] = manifest_binding['sha256']
        initial = source_state(source,frozen)
        archive = out/'driver-inputs';archive.mkdir()
        for name in DRIVER_INPUTS:
            path = Path(__file__).with_name(name)
            if path.resolve() != (source/'test/0073.ros-tests/support/source/ros_aarch64'/name).resolve() or digest(path) != initial.get('test/0073.ros-tests/support/source/ros_aarch64/'+name):
                raise RuntimeError('executed helper not from the exact frozen source: '+name)
            original = file_pin(path);destination = archive/name;shutil.copy2(path,destination)
            if digest(destination) != original['sha256']: raise RuntimeError('executed helper changed while archiving')
            archived = file_pin(destination)
            report['driver_inputs'][name] = {**original,'archive':str(destination),
                                           'original_binding':original,'archive_binding':archived}
        probe_path = source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py';report['cgroup_probe'] = file_pin(probe_path)
        spec = importlib.util.spec_from_file_location('local_event_cgroup',probe_path)
        probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution',probe.probe);report['qemu'] = evidence.emulator(args.qemu);file_pin(args.qemu)
        incarnation_before = incarnation();report['cgroup_incarnation_before'] = incarnation_before
        report['memory_swap_max_before'] = (evidence.leaf/'memory.swap.max').read_text().strip();guard()
        lease,lease_binding = json_pin(args.binfmt_evidence)
        if not isinstance(lease.get('name'),str) or '/' in lease['name'] or lease['name'] in ('','.','..'):
            raise RuntimeError('invalid parent binfmt lease name')
        registered_path = Path('/proc/sys/fs/binfmt_misc')/lease['name'];registered = registered_path.read_text()
        fields = dict(line.split(None,1) for line in registered.splitlines()[1:] if ' ' in line)
        if (os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != lease['name'] or registered != lease['entry']
                or digest(args.qemu) != lease['qemu_sha256'] or not registered.startswith('enabled\n')
                or 'F' not in fields['flags:'] or fields['interpreter'] != str(args.qemu.resolve())
                or bytes.fromhex(fields['magic'])[18:20] != b'\xb7\x00' or bytes.fromhex(fields['mask'])[18:20] != b'\xff\xff'):
            raise RuntimeError('exact parent fixed-interpreter ELF183 binfmt lease required')
        report['binfmt'] = {**lease_binding,'entry':registered}
        lease_state = {'path':registered_path,'entry':registered,'name':lease['name']};guard()
        sdk_before,aliases_before = sdk_inventory(sdk),sdk_root_aliases(sdk)
        sdk_record = out/'sdk-before.json';sdk_record.write_text(json.dumps(sdk_before,indent=2)+'\n')
        report['sdk_before'] = file_pin(sdk_record);report['sdk_root_aliases'] = aliases_before
        checked(['mount','--make-rprivate','/']);checked(['mount','--rbind',sdk,root]);mounts.append(root)
        checked(['mount','--make-rprivate',root]);report['source_mount'] = bind_frozen_work(root,source,sdk,out,checked)
        checked(['mount','--bind',temporary,root/'tmp']);home = out/'owned-home';home.mkdir();checked(['mount','--bind',home,root/'root'])
        for name in ('usr','opt','etc','var/lib/dpkg'):
            checked(['mount','--bind',sdk/name,root/name]);checked(['mount','-o','remount,bind,ro',root/name])
        for name in ('proc','sys','dev'):
            checked(['mount','--rbind',Path('/')/name,root/name]);checked(['mount','--make-rprivate',root/name])
        checked(['mount','-o','remount,bind,ro',root]);checked(['mount','--make-rprivate',root])
        environment = dict(os.environ)
        for name in ('CC','CXX','LD_LIBRARY_PATH','LD_PRELOAD','QEMU_LD_PREFIX','CFLAGS','CXXFLAGS','LDFLAGS','FASTRTPS_DEFAULT_PROFILES_FILE','FASTDDS_DEFAULT_PROFILES_FILE'):
            environment.pop(name,None)
        environment.update(PYTHONDONTWRITEBYTECODE='1',ROS_DOMAIN_ID=str(DOMAIN),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_SECURITY_ENABLE='false',ROS_LOG_DIR='/tmp/ros-logs')
        (temporary/'ros-logs').mkdir()
        compiler = rooted(sdk,'/usr/bin/aarch64-linux-gnu-g++-13')
        frontends = [rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus') for base in ('usr/lib','usr/libexec')]
        frontend = next(path for path in frontends if path.is_file())
        for label,path in [('native-cmake',rooted(sdk,'/usr/bin/cmake')),('native-gcc',compiler),('native-frontend',frontend)]:
            tools[label] = evidence.target(label,path);file_pin(path)
        report['tools'] = tools;report['compiler'] = tools['native-gcc'];report['frontend'] = tools['native-frontend']
        if args.mode in ('prepare','all'):
            (temporary/'evidence-stop.hpp').write_text(BARRIER);generated = temporary/'cmake';generated.mkdir();(generated/'CMakeLists.txt').write_text(cmake())
            file_pin(temporary/'evidence-stop.hpp');file_pin(generated/'CMakeLists.txt')
            run('configure',['/usr/bin/cmake','-S','/tmp/cmake','-B','/tmp/build','-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-13',
                '-DCMAKE_BUILD_TYPE=Release','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'],['native-cmake'],120)
            run('compile',['/usr/bin/cmake','--build','/tmp/build','--parallel','1','--verbose'],['native-cmake','native-gcc','native-frontend'],600)
            compile_log,compile_log_binding = read_file_binding(out/'compile.log',limit=16*1024**2,retain=True)
            pins.append(compile_log_binding);report['compile_log'] = compile_log_binding
            database,_ = json_pin(temporary/'build/compile_commands.json')
            for label,relative in PROBES.items():
                recipe = compile_recipe(label,database,compile_log.decode('utf-8'))
                raw = temporary/recipe['effective_mf'][5:];dep = out/(label+'.d');shutil.copy2(raw,dep)
                inputs,dep_binding = fixture_closure(dep,source,sdk,temporary,recipe);pins.append(dep_binding)
                raw_binding = file_pin(raw)
                if raw_binding['sha256'] != dep_binding['sha256']: raise RuntimeError('actual compiler depfile differs from archived bytes')
                required = {'/work/'+relative,'/work/test/0073.ros-tests/support/source/ros_local_parameter_events/common.h','/work/sdk/origin/common/codec.hpp','/tmp/evidence-stop.hpp'}
                if not required.issubset(inputs) or not any(name.startswith('/usr/') for name in inputs) or not any(name.startswith('/opt/') for name in inputs):
                    raise RuntimeError('actual -MD closure lacks fixture/shared SDK/barrier/native SDK')
                item = {'binary':evidence.target(label,temporary/'build'/label),
                        'raw_dependencies':{**file_pin(dep),'inputs':inputs,'actual_output_binding':raw_binding},
                        'compile_recipe':recipe,'metadata':{}}
                meta = temporary/'build/CMakeFiles'/(label+'.dir')
                for name,path in [('flags',meta/'flags.make'),('link',meta/'link.txt'),('compile_commands',temporary/'build/compile_commands.json'),('cmake',generated/'CMakeLists.txt')]:
                    item['metadata'][name] = file_pin(path)
                link,link_binding = read_file_binding(meta/'link.txt',limit=16*1024**2,retain=True)
                if link_binding != item['metadata']['link']: raise RuntimeError('link recipe changed during validation')
                validate_fixture_link(label,recipe,link.decode('utf-8'),compile_log.decode('utf-8'))
                for value in inputs.values(): file_pin(value['path'])
                report['probes'][label] = item;file_pin(item['binary']['path'])
            report['compilation_status'] = 'PASS'
        else:
            original,prepared_binding = json_pin(args.prepared);validate_prepared_contract(original,report['source_manifest_sha256'])
            if original.get('source') != str(source) or original.get('rootfs') != str(sdk):
                raise RuntimeError('prepared exact frozen source/canonical SDK paths differ')
            if original['cgroup_incarnation_before'] != incarnation_before:
                raise RuntimeError('prepared compilation and behavior require the same inherited cgroup incarnation')
            for name,item in original['driver_inputs'].items():
                for key in ('original_binding','archive_binding'):
                    bound = file_pin(item[key]['path'])
                    if bound != item[key] or bound['sha256'] != item['sha256']:
                        raise RuntimeError('prepared original helper/archive identity changed: '+name)
            architecture,architecture_pin = architecture_binding(original['architecture_evidence']['path'],pins)
            if architecture_pin['sha256'] != original['architecture_evidence']['sha256']:
                raise RuntimeError('prepared actual compilation ARM/cgroup proof changed or failed')
            validate_compilation_architecture(original,architecture,sdk,pins)
            if original.get('qemu') != report['qemu']:
                raise RuntimeError('prepared exact QEMU host emulator identity differs')
            original_sdk,original_sdk_binding = json_pin(original['sdk_before']['path'],limit=64*1024**2)
            if original_sdk_binding['sha256'] != original['sdk_before']['sha256'] or original_sdk != sdk_before or original['sdk_root_aliases'] != aliases_before:
                raise RuntimeError('prepared full SDK keyset/bytes/root aliases differ')
            report['prepared'] = prepared_binding
            original_log,original_log_binding = read_file_binding(original['compile_log']['path'],limit=16*1024**2,retain=True)
            if original_log_binding != original['compile_log']: raise RuntimeError('prepared compiler log file binding changed')
            pins.append(original_log_binding);report['compile_log'] = original_log_binding
            for label,item in original['probes'].items():
                if not complete_identity(require_arm(item['binary']['path']),item['binary']): raise RuntimeError('prepared binary identity differs')
                file_pin(item['binary']['path']);dep = item['raw_dependencies'];file_pin(dep['path'])
                original_temp = Path(item['binary']['path']).parent.parent
                original_closure,original_dep_binding = fixture_closure(Path(dep['path']),source,sdk,original_temp,item['compile_recipe'])
                pins.append(original_dep_binding)
                actual_pin = file_pin(dep['actual_output_binding']['path'])
                if (original_dep_binding != {key:dep[key] for key in original_dep_binding}
                        or actual_pin != dep['actual_output_binding'] or actual_pin['sha256'] != original_dep_binding['sha256']
                        or original_closure != dep['inputs']):
                    raise RuntimeError('prepared actual GCC -MD exactset differs')
                for value in dep['inputs'].values(): file_pin(value['path'])
                for name,value in item['metadata'].items():
                    if file_pin(value['path']) != value: raise RuntimeError('prepared raw codegen/link metadata binding changed')
                original_database,_ = json_pin(item['metadata']['compile_commands']['path'])
                if compile_recipe(label,original_database,original_log.decode('utf-8')) != item['compile_recipe']:
                    raise RuntimeError('prepared actual verbose compile binding differs')
                barrier,barrier_pin = read_file_binding(dep['inputs']['/tmp/evidence-stop.hpp']['path'],limit=16*1024**2,retain=True)
                generated_cmake,generated_pin = read_file_binding(item['metadata']['cmake']['path'],limit=16*1024**2,retain=True)
                pins.extend((barrier_pin,generated_pin))
                if barrier.decode('utf-8') != BARRIER or generated_cmake.decode('utf-8') != cmake():
                    raise RuntimeError('prepared exact generated constructor/CMake recipe differs')
                link,link_pin = read_file_binding(item['metadata']['link']['path'],limit=16*1024**2,retain=True);pins.append(link_pin)
                validate_fixture_link(label,item['compile_recipe'],link.decode('utf-8'),original_log.decode('utf-8'))
                shutil.copy2(item['binary']['path'],temporary/label);facts = evidence.target(label,temporary/label)
                if facts['sha256'] != item['binary']['sha256'] or not complete_identity(require_arm(item['binary']['path']),item['binary']):
                    raise RuntimeError('prepared artifact changed during private copy')
                if (facts['device'],facts['inode']) == (item['binary']['device'],item['binary']['inode']):
                    raise RuntimeError('prepared artifact copy must have distinct file identity')
                report['probes'][label] = {**item,'binary':facts};file_pin(temporary/label)
            report['compilation_status'] = 'REUSED_QUALIFIED_PREPARATION'
        if args.mode in ('execute','all'):
            gateway_report,gateway_report_binding = json_pin(args.gateway_report)
            origin = args.gateway.resolve(strict=True);binding = require_current_gateway(gateway_report,origin,report['source_manifest_sha256'])
            strict = qualify_gateway(origin,args.gateway_report,source.parent/'source-manifest.json',source,sdk,2)
            report['gateway_binding'] = {**binding,'report':gateway_report_binding,'strict_qualification':strict};file_pin(origin)
            for value in strict['input_pins']:
                if not {'path','device','inode','bytes','mtime_ns','ctime_ns','sha256'}.issubset(value):
                    raise RuntimeError('strict Gateway qualification pin lacks complete file identity')
                pins.append(value)
            loader = gateway_library_environment(strict, sdk, sdk_before)
            pins.extend(loader['input_pins'])
            environment.update(loader['environment'])
            report['native_gateway_library_environment'] = loader
            shutil.copy2(origin,temporary/'gateway');facts = evidence.target(GATEWAY_LABEL,temporary/'gateway')
            if facts['sha256'] != binding['binary']['sha256'] or not complete_identity(require_arm(origin),binding['binary']): raise RuntimeError('Gateway changed during independent snapshot')
            if (facts['device'],facts['inode']) == (binding['binary']['device'],binding['binary']['inode']):
                raise RuntimeError('Gateway snapshot must have distinct file identity')
            report['gateway_snapshot'] = facts;file_pin(temporary/'gateway')
            lock = Path('/tmp')/f'uwvm-arm-ros-domain-{DOMAIN}.lock';fd = os.open(lock,os.O_CREAT|os.O_EXCL|os.O_WRONLY,0o600)
            os.write(fd,str(os.getpid()).encode());identity = os.fstat(fd);os.close(fd);domain_lock = (lock,identity.st_dev,identity.st_ino)
            for label,contract in PROBE_CONTRACTS.items():
                inside = '/tmp/'+label if args.mode == 'execute' else '/tmp/build/'+label
                required = [label]+([GATEWAY_LABEL] if contract['gateway_required'] else [])
                run('behavior-'+label,[inside]+(['/tmp/gateway'] if contract['gateway_required'] else []),required,120,
                    contract['marker'],stopped=True,gateway=contract['gateway_required'])
            report['actual_native_behavior_probes'] = 2;report['native_behavior_status'] = 'PASS'
        status = 'PASS'
    except BaseException as error:
        report['error'] = traceback.format_exc()
        if 'BLOCKED' in str(error): status = 'BLOCKED'
    finally:
        report['process_cleanup'] = []
        for scope in reversed(scopes):
            try:
                result = scope.cleanup();report['process_cleanup'].append(result)
                if result['remaining'] or result['errors']: status = 'FAIL'
            except BaseException: report['process_cleanup'].append({'error':traceback.format_exc()});status = 'FAIL'
        report['remaining_owned_processes'] = [row for item in report['process_cleanup'] for row in item.get('remaining',[])]
        try:
            check_pins();report['immutable_inputs_preserved'] = True
            report['source_preserved'] = source_state(source,frozen) == initial if initial is not None else None
            report['sdk_preserved'] = sdk_inventory(sdk) == sdk_before and sdk_root_aliases(sdk) == aliases_before if sdk_before is not None else None
            report['driver_inputs_preserved'] = all(digest(value['archive']) == value['sha256'] for value in report['driver_inputs'].values()) if report['driver_inputs'] else None
            report['tools_preserved'] = all(require_arm(value['path']) == value for value in tools.values()) if tools else None
            report['prepared_inputs_preserved'] = True;report['gateway_inputs_preserved'] = True
            if any(report[key] is False for key in ('source_preserved','sdk_preserved','driver_inputs_preserved','tools_preserved')): status = 'FAIL'
            if status == 'PASS' and not all(report[key] is True for key in ('source_preserved','sdk_preserved','driver_inputs_preserved','tools_preserved')): status = 'FAIL'
        except BaseException: report['preservation_error'] = traceback.format_exc();status = 'FAIL'
        if evidence:
            try:
                guard();report['cgroup_incarnation_after'] = incarnation()
                report['same_cgroup_incarnation'] = report['cgroup_incarnation_after'] == incarnation_before
                report['memory_swap_max_after'] = (evidence.leaf/'memory.swap.max').read_text().strip()
            except BaseException: report['cgroup_guard_error'] = traceback.format_exc();status = 'FAIL'
            try:
                result = evidence.finish(status);status = result['status']
                report['architecture_evidence'] = {'path':str(evidence.out/'architecture-evidence.json'),'sha256':digest(evidence.out/'architecture-evidence.json')}
            except BaseException: report['architecture_error'] = traceback.format_exc();status = 'FAIL'
        report['mount_cleanup'] = []
        for mount in reversed(mounts):
            try:
                result = subprocess.run(['umount','-R',str(mount)],capture_output=True,text=True,timeout=30)
                report['mount_cleanup'].append({'path':str(mount),'returncode':result.returncode,'stderr':result.stderr})
                if result.returncode: status = 'FAIL'
            except BaseException: report['mount_cleanup'].append({'error':traceback.format_exc()});status = 'FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')] if sys.platform == 'linux' else []
        if report['remaining_owned_mounts']: status = 'FAIL'
        if domain_lock:
            try:
                path,device,inode = domain_lock;current = path.stat()
                if (current.st_dev,current.st_ino) != (device,inode): raise RuntimeError('owned domain lock identity replaced')
                path.unlink();report['domain_lock_cleanup'] = 'PASS'
            except BaseException: report['domain_lock_cleanup_error'] = traceback.format_exc();status = 'FAIL'
        report['status'] = 'PREPARED' if status == 'PASS' and args.mode == 'prepare' else status
        if args.mode == 'prepare': report['scope'] = 'CPP20 ARM compilation/closure preparation only; no native ROS behavior performed'
        save()
    print(report['status'],out/'report.json',flush=True)
    return 0 if report['status'] in ('PASS','PREPARED') else 77 if report['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
