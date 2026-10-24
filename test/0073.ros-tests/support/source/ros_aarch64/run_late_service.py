#!/usr/bin/env python3
"""Exercise real ROS1 services that appear after the original Guest call starts.

The original service Wasm fixtures/native probes and all operation budgets stay
unchanged. Only the task-owned native launcher is held before exec. Release
requires the real Gateway's wait-for-service log and actual Master absence.
No service readiness hook prevents the original Guest from making its call.
"""
import argparse
import importlib
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import threading
import time
import traceback
import xmlrpc.client

sys.dont_write_bytecode = True
# Import the ARM adapter and its similarly named helpers before the frozen
# original runner directory can shadow generate_fixtures.py on sys.path.
from run import main as arm_main
from topic_gate import ShortTransport
from runtime import digest, require_arm, validate_mapping_receipts, validate_architecture_mappings


class LateNative:
    def __init__(self, popen, original_start):
        self.popen, self.original_start = popen, original_start
        self.rows, self.threads = [], []
        self.gateway_logging = []
        self.stop = threading.Event()

    @staticmethod
    def complete_events(path):
        # A concurrent writer can leave the last JSON line incomplete. Only
        # complete records participate in the release condition.
        data = path.read_text()
        lines = data.splitlines() if data.endswith('\n') else data.splitlines()[:-1]
        return [json.loads(line) for line in lines if line.startswith('{')]

    @staticmethod
    def wait_wall_ns(line):
        value = re.search(r'\[(\d+)\.(\d{1,9})\]: waitForService:', line)
        if value is None:
            raise RuntimeError('actual Gateway wait log has no exact ROS wall timestamp')
        return int(value[1])*1000000000+int(value[2].ljust(9, '0'))

    def bind_final_audit(self):
        valid = True
        for row in self.rows:
            if row['status'] != 'RELEASED':
                continue
            try:
                allowed = [event for event in self.complete_events(Path(row['worker_log']))
                           if event.get('event') == 'allow' and event.get('operation') == 'service.call'
                           and event.get('resource') == row['resource'] and event.get('error') == 0]
                if len(allowed) != 1:
                    raise RuntimeError('expected exactly one completed Wasm allow audit for the delayed service call')
                stamp = allowed[0].get('time_unix_ns')
                elapsed = row['released_time_unix_ns']-stamp if type(stamp) is int else -1
                if not 0 <= elapsed < 3000000000 or stamp > row['actual_Gateway_wait_time_unix_ns']:
                    raise RuntimeError('completed Wasm allow timestamp does not precede release within original 3000 ms')
                row.update(actual_Wasm_service_allow=allowed[0],
                           release_after_allow_ms=elapsed/1000000,
                           Wasm_allow_audit_binding='PASS',
                           Wasm_allow_audit_visibility='Audit buffered until audit_result destructor after the call returns; used only for final timestamp binding, not the prerelease gate')
            except BaseException:
                row.update(Wasm_allow_audit_binding='FAIL', Wasm_allow_audit_error=traceback.format_exc())
                valid = False
        return valid

    @staticmethod
    def birth(pid):
        value = Path(f'/proc/{pid}/stat').read_text()
        fields = value[value.rfind(')')+2:].split()
        return fields[0], int(fields[19])

    def __call__(self, command, *args, **kwargs):
        frame = sys._getframe(1)
        argv = list(command) if isinstance(command, (list, tuple)) else []
        original_launch = frame.f_code is self.original_start.__code__
        gateway = next((i for i, value in enumerate(argv)
                        if Path(value).name == 'uwvm-ros1-gateway'), None)
        if original_launch and gateway is not None:
            root = frame.f_locals['self'].roots[1]
            library_path = root/'opt/ros/noetic/lib/librosconsole.so'
            library = require_arm(library_path)
            setting = 'ROSCONSOLE_STDOUT_LINE_BUFFERED'
            if setting.encode()+b'\0' not in library_path.read_bytes():
                raise RuntimeError('installed ARM rosconsole does not advertise the explicit stdout flush setting')
            environment = dict(kwargs.get('env') or os.environ)
            previous = environment.get(setting)
            environment[setting] = '1'
            kwargs = dict(kwargs, env=environment)
            process = self.popen(command, *args, **kwargs)
            self.gateway_logging.append({'name': frame.f_locals['name'],
                'original_command': argv, 'actual_command': argv,
                'pid': process.pid, 'starttime_ticks': self.birth(process.pid)[1],
                'library_input': library,
                'setting': setting, 'previous_value': previous, 'requested_value': '1',
                'primary_source': 'https://raw.githubusercontent.com/ros/rosconsole/noetic-devel/src/rosconsole/rosconsole.cpp',
                'scope': 'Gateway-only rosconsole setting performs an explicit stdout fflush after each console line; only output visibility changes, original native argv and operation budgets remain unchanged'})
            return process
        selected = (original_launch and
                    any(Path(x).name == 'uwvm-ros1-native-probe' for x in argv) and
                    any(mode in argv for mode in ('--serve-trigger', '--serve-setbool')))
        if not selected:
            return self.popen(command, *args, **kwargs)
        runner, name = frame.f_locals['self'], frame.f_locals['name']
        mode = next(x for x in ('--serve-trigger', '--serve-setbool') if x in argv)
        resource = argv[argv.index(mode)+1]
        process = self.popen(['/bin/bash', '-c', 'kill -STOP $$; exec "$@"',
                              'uwvm-arm-late-service', *argv], *args, **kwargs)
        row = {'name': name, 'original_native_command': argv, 'native_launcher_pid': process.pid,
               'resource': resource, 'mode': mode, 'status': 'WAITING',
               'service_call_budget_ms': 3000, 'Worker_timeout_seconds': 20,
               'native_timeout_seconds': 12,
               'original_native_wait_timeout_seconds': 15,
               'held_process_scope': 'Host bash before SDK/native exec; not ARM execution proof'}
        self.rows.append(row)

        def release():
            try:
                end = time.monotonic()+8
                while time.monotonic() < end:
                    state, born = self.birth(process.pid)
                    if state == 'T': break
                    if process.poll() is not None: raise RuntimeError('late native launcher exited before hold')
                    time.sleep(.005)
                else: raise RuntimeError('late native launcher never reached actual stopped state')
                row['launcher_starttime_ticks'] = born
                case = name.removesuffix('-native1'+mode)
                worker_log = runner.run/(case+'-uwvm.log')
                gateway_log = runner.run/(case+'-gateway1.log')
                row.update(worker_log=str(worker_log), gateway_log=str(gateway_log))
                while time.monotonic() < end and not self.stop.is_set():
                    waiting = [line for line in gateway_log.read_text().splitlines()
                               if resource in line and 'waitForService' in line and 'waiting' in line]
                    if waiting:
                        master_query_ns = time.monotonic_ns()
                        with xmlrpc.client.ServerProxy(runner.env['ROS_MASTER_URI'], transport=ShortTransport()) as master:
                            master_result = master.getSystemState('/uwvm_arm_late_service')
                        code, _, state = master_result
                        if code != 1: raise RuntimeError('late service actual Master state unavailable')
                        servers = [nodes for topic, nodes in state[2] if topic == resource]
                        if servers: raise RuntimeError('native service already existed before deliberate release')
                        held, current_birth = self.birth(process.pid)
                        if held != 'T' or current_birth != born or process.poll() is not None:
                            raise RuntimeError('late native launcher identity/stopped state changed')
                        gateway_logging = next(item for item in self.gateway_logging
                                               if item['name'] == case+'-gateway1')
                        gateway_state, gateway_birth = self.birth(gateway_logging['pid'])
                        if gateway_state in ('Z', 'X') or gateway_birth != gateway_logging['starttime_ticks']:
                            raise RuntimeError('Gateway logging environment PID identity changed')
                        actual_environment = Path('/proc/'+str(gateway_logging['pid'])+'/environ').read_bytes().split(b'\0')
                        setting = gateway_logging['setting'].encode()+b'=1'
                        if actual_environment.count(setting) != 1:
                            raise RuntimeError('live Gateway did not inherit the rosconsole stdout flush setting')
                        gateway_logging['actual_live_setting'] = setting.decode()
                        gateway_logging['environment_read_monotonic_ns'] = time.monotonic_ns()
                        configuration = runner.run/case/'ros1.txt'
                        configuration_lines = configuration.read_text().splitlines()
                        if configuration_lines.count('set client timeout-ms 3000') != 1:
                            raise RuntimeError('original 3000 ms Gateway service budget changed')
                        released_wall_ns = time.time_ns()
                        wait_wall_ns = self.wait_wall_ns(waiting[-1])
                        if not 0 <= released_wall_ns-wait_wall_ns < 3000000000:
                            raise RuntimeError('deliberate release did not follow actual Gateway wait within the unchanged service budget')
                        row.update(actual_Gateway_wait_log=waiting[-1],
                                   actual_Gateway_wait_time_unix_ns=wait_wall_ns,
                                   actual_Master_service_nodes_before_release=[],
                                   actual_Gateway_config={'path': str(configuration),
                                       'sha256': digest(configuration), 'service_timeout_line': 'set client timeout-ms 3000'},
                                   actual_Master_query={'uri': runner.env['ROS_MASTER_URI'],
                                       'method': 'getSystemState', 'caller': '/uwvm_arm_late_service',
                                       'result': master_result, 'started_monotonic_ns': master_query_ns,
                                       'completed_monotonic_ns': time.monotonic_ns()},
                                   released_monotonic_ns=time.monotonic_ns(),
                                   released_time_unix_ns=released_wall_ns,
                                   release_after_wait_ms=(released_wall_ns-wait_wall_ns)/1000000,
                                   scope='Actual Gateway service invocation waiting while Master resource absent, followed by original native exec; completed Wasm allow audit is bound afterward')
                        os.kill(process.pid, signal.SIGCONT)
                        row['status'] = 'RELEASED'
                        return
                    if process.poll() is not None: raise RuntimeError('held native launcher exited')
                    time.sleep(.005)
                raise RuntimeError('no real call/wait/absent-service evidence before release deadline')
            except BaseException:
                row.update(status='FAIL', error=traceback.format_exc())
                # Cleanup may require an owned stopped process to resume. This
                # is never recorded as a successful delayed-service release.
                if process.poll() is None:
                    try: os.kill(process.pid, signal.SIGCONT)
                    except ProcessLookupError: pass

        thread = threading.Thread(target=release, daemon=True)
        self.threads.append(thread); thread.start()
        return process

    def finish(self):
        self.stop.set()
        for thread in self.threads:
            thread.join(timeout=1)
            if thread.is_alive(): raise RuntimeError('late service monitor survived bounded cleanup')
        if not self.rows or any(x['status'] != 'RELEASED' for x in self.rows):
            raise RuntimeError('actual absent-service call did not release every native witness')


def owned_native_samples(architecture, row, expected_sha256):
    """Prove native exec or one chroot-owned child without relabeling its PID."""
    validate_architecture_mappings(architecture)
    label = 'ros1/uwvm-ros1-native-probe'
    target = architecture['arm_targets'][label]
    if (target['elf_class'], target['elf_data'], target['e_machine']) != (2, 1, 183) or target['sha256'] != expected_sha256:
        return []
    held, born, released = row['native_launcher_pid'], row['launcher_starttime_ticks'], row['released_monotonic_ns']
    membership = architecture['cgroup_before']['membership']
    samples = architecture['actual_process_samples']
    parent_proof = any(sample['name'] == row['name'] and sample['pid'] == held
        and sample['starttime_ticks'] == born and sample['capture_monotonic_ns'] >= released
        and sample['live_cgroup_procs'] and sample['membership'] == membership for sample in samples)

    def mapped_target(sample):
        return validate_mapping_receipts(sample, label, target)

    result = []
    for sample in samples:
        same_pid = sample['pid'] == held and sample['starttime_ticks'] == born and sample['ancestry'] == [held]
        owned_child = parent_proof and sample['pid'] != held and sample['parent_pid'] == held \
            and sample['ancestry'] == [held, sample['pid']] and sample['starttime_ticks'] >= born
        if sample['name'] == row['name'] and (same_pid or owned_child) and label in sample['mapped_arm_targets'] \
                and sample['capture_monotonic_ns'] >= released and sample['live_cgroup_procs'] \
                and sample['membership'] == membership and sample['kernel_exe_elf']['e_machine'] == 62 \
                and sample['kernel_exe_elf']['sha256'] in architecture['expected_host_emulator_sha256'] \
                and mapped_target(sample):
            result.append(sample)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__, add_help=False)
    parser.add_argument('--kind', choices=('trigger', 'setbool', 'all'), default='all')
    parser.add_argument('--qualified-report', type=Path, required=True)
    parser.add_argument('--expected-gateway-sha256', required=True)
    own, remaining = parser.parse_known_args()
    if '--out' not in remaining or '--source' not in remaining:
        raise RuntimeError('--out and --source ARM adapter arguments are required')
    out = Path(remaining[remaining.index('--out')+1]).resolve()
    source = Path(remaining[remaining.index('--source')+1]).resolve()
    if '--suite' in remaining:
        raise RuntimeError('late-service wrapper supplies --suite network itself')
    sys.path.insert(0, str(source/'test/0073.ros-tests/support/source/ros'))
    network = importlib.import_module('run_network')
    qualified_path = own.qualified_report.resolve(strict=True)
    qualified = json.loads(qualified_path.read_text())
    qualified_sha256 = digest(qualified_path)
    if qualified.get('status') != 'PASS':
        raise RuntimeError('PASS native build qualification required')
    selected_targets = {name: row for name, row in qualified['targets'].items()
                        if name.startswith('uwvm-ros1-')}
    for name, row in selected_targets.items():
        if require_arm(row['path'])['sha256'] != row['sha256']:
            raise RuntimeError('qualified ARM target changed: '+name)
    if selected_targets['uwvm-ros1-gateway']['sha256'] != own.expected_gateway_sha256:
        raise RuntimeError('selected Gateway does not match explicit expected SHA256')
    original_main, original_popen = network.main, subprocess.Popen
    original_snapshot = network.Runner.snapshot
    snapshot_bindings = []

    def qualified_snapshot(runner, input_path, destination, label):
        target = selected_targets.get(Path(input_path).name)
        actual = target['path'] if target is not None else input_path
        original_snapshot(runner, actual, destination, label)
        if target is not None:
            if digest(destination) != target['sha256']:
                raise RuntimeError('qualified native snapshot differs: '+label)
            snapshot_bindings.append({'label': label, 'canonical_input_ignored': str(input_path),
                                      'qualified_input': target, 'snapshot': str(destination),
                                      'snapshot_sha256': digest(destination)})
    delayed = LateNative(original_popen, network.Runner.start)
    selected = ('trigger', 'setbool') if own.kind == 'all' else (own.kind,)
    raw_report, error = {}, None

    def late_main():
        p = argparse.ArgumentParser()
        for key in ('workspace', 'source', 'uwvm', 'llvm-libs'):
            p.add_argument('--'+key, required=True)
        args = p.parse_args()
        args.only = None
        runner = network.Runner(args)
        raw_report.update(status='FAIL', original_results=str(runner.run/'report.json'),
                          source=str(source), cases=[])
        try:
            runner.prepare({1})
            for kind in selected:
                name = 'ros1-delayed-native-service-'+kind
                runner.case(name, 'examples/ros1/service_client_'+kind+'.wasm',
                            {1: ['service-client-'+kind]},
                            [(1, '--serve-'+kind, '/uwvm_ros_test/service_'+kind)])
                row = next(x for x in delayed.rows if x['name'] == name+'-native1--serve-'+kind)
                if row['status'] != 'RELEASED':
                    raise RuntimeError('late witness release was not independently proven')
                native = runner.run/(row['name']+'.log')
                marker = 'PASS actual ROS1 native service witness --serve-'+kind
                if marker not in native.read_text():
                    raise RuntimeError('original exact one-request native witness did not pass')
                row.update(status='RELEASED', original_native_completion_marker=marker,
                           native_log=str(native), native_log_sha256=digest(native),
                           original_native_source_sha256=digest(source/'test/0073.ros-tests/support/source/ros1/native_probe.cc'),
                           callback_assertion_scope='Original native require(count == 1 && !wrong) before its unchanged 500 ms tail; Wasm fixture performs one typed service.call')
                raw_report['cases'].append(runner.results[-1])
            delayed.finish()
            raw_report['status'] = 'PASS'
            return 0
        except BaseException:
            raw_report.update(error=traceback.format_exc(), status='FAIL')
            return 1
        finally:
            delayed.stop.set()
            for thread in delayed.threads: thread.join(timeout=1)
            runner.cleanup()
            (runner.run/'report.json').write_text(json.dumps(raw_report, indent=2)+'\n')

    code = 1
    try:
        network.main = late_main
        network.Runner.snapshot = qualified_snapshot
        subprocess.Popen = delayed
        sys.argv = [__file__, '--suite', 'network', *remaining]
        code = arm_main()
    except BaseException:
        error = traceback.format_exc()
        print(error, file=sys.stderr, flush=True)
    finally:
        subprocess.Popen = original_popen
        network.main = original_main
        network.Runner.snapshot = original_snapshot
        delayed.stop.set()
        for thread in delayed.threads: thread.join(timeout=1)
        if out.is_dir():
            audit_bound = delayed.bind_final_audit()
            facts = {'status': 'PASS' if code == 0 and raw_report.get('status') == 'PASS' else 'FAIL',
                     'scope': 'Original ROS1 typed Wasm/native one-request assertions with a service deliberately absent at actual Gateway wait; no budget extension or prerelease service readiness',
                     'executed_driver_sha256': digest(__file__), 'delayed_native_witnesses': delayed.rows,
                     'qualified_report': {'path': str(qualified_path), 'sha256': qualified_sha256},
                     'expected_gateway_sha256': own.expected_gateway_sha256,
                     'selected_native_targets': selected_targets, 'qualified_snapshots': snapshot_bindings,
                     'gateway_output_buffering': delayed.gateway_logging,
                     'original_case_results': raw_report, 'error': error}
            if not audit_bound:
                facts['status'], code = 'FAIL', 1
            architecture = out/'architecture-evidence.json'
            if architecture.is_file():
                arch = json.loads(architecture.read_text())
                facts['architecture_report'] = {'path': str(architecture), 'sha256': digest(architecture)}
                for row in delayed.rows:
                    samples = owned_native_samples(arch, row,
                        selected_targets['uwvm-ros1-native-probe']['sha256']) if row['status'] == 'RELEASED' else []
                    row['owned_ARM_native_exec_samples'] = samples
                    row['native_execution_pid_model'] = 'held launcher spawned ARM child' if any(
                        sample['pid'] != row['native_launcher_pid'] for sample in samples) else 'held PID exec' if samples else 'unproven'
                    if row['status'] == 'RELEASED' and not samples:
                        facts['status'], code = 'FAIL', 1
                        row['exec_proof_error'] = 'no frozen ARM native mapping for the held PID or its strictly proved owned child after release'
            if digest(qualified_path) != qualified_sha256 or any(
                    require_arm(row['path'])['sha256'] != row['sha256'] for row in selected_targets.values()):
                facts['status'], code = 'FAIL', 1
                facts['input_preservation_error'] = 'selected qualification or native target changed during execution'
            (out/'late-service-evidence.json').write_text(json.dumps(facts, indent=2)+'\n')
    return code


if __name__ == '__main__':
    sys.exit(main())
