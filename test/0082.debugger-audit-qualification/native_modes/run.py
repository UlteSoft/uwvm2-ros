"""Public Linux native mode qualification; live authority belongs to an executor.

A plan and its trusted admission adapter are explicit, separately hash-pinned
inputs. See execution.py for the boundary. This driver never acquires a lease,
changes limits, builds the VM, modifies guest bytes per mode or relaxes unwind.
"""
from pathlib import Path
import argparse
import importlib.util
import json
import os
import platform
import random
import re
import select
import socket
import statistics
import struct
import sys
import time
from execution import Child, need, sha


def birth(pid):
    raw = Path('/proc', str(pid), 'stat').read_text()
    return raw[raw.rfind(')') + 2:].split()[19]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('plan', 'executor', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for name in ('plan-sha256', 'executor-sha256'):
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args()
    need(sys.platform == 'linux', 'Native qualification is Linux only')
    need(sha(args.plan) == args.plan_sha256 and sha(args.executor) == args.executor_sha256,
         'Explicit plan or executor digest differs')
    plan = json.loads(args.plan.read_text())
    need(plan.get('schema') == 'debugger-native-mode-plan-v1', 'Unsupported plan schema')
    spec = importlib.util.spec_from_file_location('native_mode_admission_provider', args.executor)
    need(spec is not None and spec.loader is not None, 'Executor adapter cannot load')
    adapter = importlib.util.module_from_spec(spec); spec.loader.exec_module(adapter)
    need(adapter.API_VERSION == 1, 'Incompatible live-admission adapter')
    executor = adapter.bind(plan)
    initial = executor.admit('driver-entry')
    out = args.out.absolute(); out.mkdir(parents=True, exist_ok=False)
    source = Path(__file__).resolve().parent
    cpu = plan['cpu']; need(cpu in os.sched_getaffinity(0), 'CPU outside admitted affinity')
    os.sched_setaffinity(0, {cpu})
    products = {name: executor.verify_product(row) for name, row in plan['products'].items()}
    need('subject' in products and set(products) <= {'subject', 'feature_off'}, 'Supported explicit product roles required')
    if 'feature_off' in products:
        need(products['feature_off'].get('features_compiled') is True and
             products['feature_off'].get('runtime_features_enabled') is False,
             'Feature-off reference requires original compiled-feature provenance and disabled execution policy')
    need(1 <= plan['rounds'] <= 63 and len(plan['workloads']) in (1, 2), 'Unbounded plan')
    result = {'schema': 'debugger-public-native-modes-v1', 'passed': False, 'executed': 0,
              'initial_admission': initial, 'plan_sha256': args.plan_sha256,
              'adapter_sha256': args.executor_sha256, 'products': products,
              'platform': platform.uname()._asdict(), 'cpu': cpu, 'commands': [], 'rows': [],
              'workloads': [], 'native_feature_off_reference_configured': 'feature_off' in products,
              'native_feature_off_reference_executed': False,
              'pure_guest_CPU_time_measured': False, 'no_regression_accepted': False,
              'full_audit_accepted': False,
              'clock_scope': 'Timed guest window includes the bounded WASI monotonic clock boundary calls. Calibration is reported raw, never subtracted. No management pause occurs inside successful unhit-mode windows.',
              'attachment_scope': 'Launch-authorized inherited endpoint; debug cases stop before guest entry. Arbitrary PID or mid-execution attachment is not tested.',
              'retirement_wait': 'Kernel pidfd POLLIN then original waitpid; no exponential timed-wait polling'}
    def save():
        (out / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
    def command(label, argv):
        row = {'label': label, 'argv': argv, 'admission': executor.admit(label)}
        result['commands'].append(row); save()
        child = None
        try:
            with (out / (label + '.stdout')).open('xb') as stdout, (out / (label + '.stderr')).open('xb') as stderr:
                child = Child(executor.command(argv), stdout, stderr)
                row.update(pid=child.process.pid, birth=birth(child.process.pid))
                row['exit'] = child.wait(60)
            need(row['exit'] == 0, 'Actual tool command failed: ' + label)
        finally:
            if child is not None: child.close()
            row['after'] = executor.admit(label + '-retired'); save()
    def action(endpoint, row, text, terminal=False):
        started = time.monotonic_ns(); encoded = text.encode('ascii')
        need(endpoint.send(encoded) == len(encoded), 'Short management request')
        closed = None
        try:
            data, _, flags, _ = endpoint.recvmsg(65536)
        except (ConnectionResetError, BrokenPipeError) as error:
            if not terminal: raise
            data = b''; flags = 0; closed = type(error).__name__
        finished = time.monotonic_ns()
        row['actions'].append({'command': text, 'reply': data.decode(errors='replace'),
                               'started_ns': started, 'finished_ns': finished,
                               'elapsed_ns': finished - started, 'terminal_close': closed})
        need(not flags & (socket.MSG_TRUNC | socket.MSG_CTRUNC), 'Truncated control reply')
        need((data or terminal) and b'error:' not in data.lower(), 'Control command failed: ' + text)
        # No file I/O/checkpoint between continue and process retirement.
        return data
    def actual_stop(child, binary):
        proc = Path('/proc') / str(child.process.pid); before = birth(child.process.pid)
        actual = (proc / 'exe').stat(); expected = binary.stat()
        need((actual.st_dev, actual.st_ino) == (expected.st_dev, expected.st_ino), 'Stopped executable differs')
        need((proc / 'cgroup').read_text() == Path('/proc/self/cgroup').read_text(), 'Stopped child escaped admission')
        poller = select.poll(); poller.register(child.pidfd, select.POLLIN)
        need(not poller.poll(0), 'Stopped child already retired')
        detail = {'birth': before, 'pidfd_live': True, 'exe_device': actual.st_dev, 'exe_inode': actual.st_ino,
                  'status': (proc / 'status').read_text(), 'limits': (proc / 'limits').read_text()}
        need(before == birth(child.process.pid), 'Stopped process birth changed')
        return detail
    try:
        save()
        (out / 'cpuinfo.txt').write_text(Path('/proc/cpuinfo').read_text())
        command('wabt-version', [plan['tools']['wat2wasm'], '--version'])
        for item in plan['workloads']:
            name = item['name']; need(name in ('smoke', 'timed'), 'Unsupported workload name')
            permissions = item.get('wasi_permissions', [])
            need(permissions == (['monotonic-clock'] if name == 'timed' else []),
                 'Timed workload requires its explicit monotonic-clock grant; smoke requires none')
            directory = out / name; directory.mkdir()
            producer = None
            if name == 'timed':
                wat = directory / 'guest.wat'
                command('timed-oracle', [sys.executable, '-B', str(source / 'make_timed_fixture.py'),
                        '--template', str(source / 'fixtures/timed-calls-and-writes.wat.in'), '--out', str(wat)])
                producer = json.loads((out / 'timed-oracle.stdout').read_text())
            else:
                wat = source / 'fixtures/calls-and-writes.wat'
            wasm = directory / 'guest.wasm'
            command(name + '-compile', [plan['tools']['wat2wasm'], str(wat), '-o', str(wasm)])
            command(name + '-validate', [plan['tools']['wasm-validate'], str(wasm)])
            command(name + '-disassemble', [plan['tools']['wasm-objdump'], '-d', str(wasm)])
            frozen = sha(wasm); wasm.chmod(0o444)
            rounds = item.get('rounds', plan['rounds'])
            need(1 <= rounds <= 63, 'Bounded workload rounds required')
            workload = {'name': name, 'rounds': rounds, 'wat_sha256': sha(wat), 'wasm_sha256': frozen,
                        'wasm_bytes': wasm.stat().st_size, 'oracle': producer,
                        'explicit_wasi_permissions': permissions}
            result['workloads'].append(workload); save()
            leaf, never, related = (2, 3, 5) if name == 'timed' else (0, 1, 7)
            modes = {'normal': [], 'idle': [], 'one-unhit': [f'breakwasm 0 {never} 0'],
                     'multiple-unhit': [f'breakwasm 0 {never} 0', f'breakwasm 0 {never} 1'],
                     'related-unhit': [f'breakwasm 0 {leaf} {related}']}
            randomizer = random.Random(plan['seed'])
            for round_index in range(rounds):
                order = [(p, m) for p in products for m in (modes if p == 'subject' else ['normal'])]
                randomizer.shuffle(order)
                for product, mode in order:
                    binary = Path(plan['products'][product]['binary'])
                    need(sha(binary) == products[product]['binary_sha256'] and sha(wasm) == frozen, 'Frozen product/guest changed')
                    row = {'workload': name, 'round': round_index, 'product': product, 'mode': mode,
                           'passed': False, 'actions': [], 'wasm_before': frozen,
                           'admission': executor.admit(name + ':' + product + ':' + mode)}
                    result['rows'].append(row); save()
                    endpoint = inherited = child = None
                    label = f'{round_index}-{product}-{mode}'
                    stdout_path = directory / (label + '.stdout'); stderr_path = directory / (label + '.stderr')
                    argv = [str(binary), '-m', 'run' if mode == 'normal' else 'debug', '-Raot', '-Rct', '0',
                            '-Rllvm-exception-dispatch', 'native-unwind', '-Rllvm-cache-path', 'disable']
                    for permission in permissions:
                        argv += ['--wasip1-global-grant', permission]
                    if mode != 'normal':
                        endpoint, inherited = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                        inherited.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1); endpoint.settimeout(60)
                        argv += ['--debug-jit-control-fd', str(inherited.fileno())]
                    argv += ['--run', str(wasm)]; row['argv'] = argv
                    launch = executor.command(argv)
                    begin = time.monotonic_ns()
                    try:
                        with stdout_path.open('xb') as stdout, stderr_path.open('xb') as stderr:
                            child = Child(launch, stdout, stderr, () if inherited is None else (inherited.fileno(),))
                            row.update(pid=child.process.pid, birth=birth(child.process.pid), launch_started_ns=begin)
                            if inherited is not None: inherited.close(); inherited = None
                            if endpoint is not None:
                                deadline = time.monotonic() + 60
                                while True:
                                    reply = action(endpoint, row, 'wait')
                                    if b'stop-id ' in reply and (b'thread ' in reply or b'prepared; no Wasm instruction executed' in reply): break
                                    need(time.monotonic() < deadline, 'Initial stop deadline exceeded')
                                    time.sleep(.001)
                                row['initial_stop_observed_ns'] = time.monotonic_ns()
                                row['actual_stopped_child'] = actual_stop(child, binary)
                                for point in modes[mode]:
                                    reply = action(endpoint, row, point)
                                    found = re.search(rb'breakpoint ([0-9]+)', reply)
                                    need(found is not None, 'Breakpoint installation lacked actual ID')
                                    row.setdefault('breakpoints', []).append(int(found[1]))
                                if modes[mode]: action(endpoint, row, 'info breakpoints')
                                row['resume_started_ns'] = time.monotonic_ns()
                                action(endpoint, row, 'continue', terminal=True)
                            row['exit'] = child.wait(60)
                            row['retired_ns'] = time.monotonic_ns(); row['pidfd_retired'] = True
                        need(row['exit'] == 0, 'Guest assertion or natural exit failed')
                        row['wasm_after'] = sha(wasm); need(row['wasm_after'] == frozen, 'Guest bytes changed')
                        raw = stdout_path.read_bytes()
                        if name == 'timed':
                            need(len(raw) == 48, 'Expected exact guest timing record')
                            magic, start, end, calibration_start, calibration_end, checksum, updates = struct.unpack('<8sQQQQII', raw)
                            need(magic == b'NMODET01' and calibration_start <= calibration_end <= start < end and
                                 checksum == producer['checksum'] and updates == producer['updates'], 'Guest timing/checksum record invalid')
                            row['guest_window_ns'] = end - start
                            row['clock_calibration_ns'] = calibration_end - calibration_start
                            row['guest_clock_record'] = {'start': start, 'end': end, 'calibration_start': calibration_start,
                                                         'calibration_end': calibration_end, 'checksum': checksum, 'updates': updates}
                            row['management_pauses_inside_guest_window'] = 0
                        else:
                            need(not raw, 'Smoke guest unexpectedly wrote output')
                        row['passed'] = True
                    except BaseException as error:
                        row['error'] = repr(error)
                    finally:
                        if child is not None: child.close()
                        if inherited is not None: inherited.close()
                        if endpoint is not None: endpoint.close()
                        row['stdout_sha256'] = sha(stdout_path); row['stderr_sha256'] = sha(stderr_path)
                        row['after'] = executor.admit(name + ':' + product + ':' + mode + ':retired')
                        result['executed'] += 1; save()
                if any(not row['passed'] for row in result['rows']): break
            if any(not row['passed'] for row in result['rows']): break
        expected = sum(item.get('rounds', plan['rounds']) for item in plan['workloads']) * (5 + int('feature_off' in products))
        result['expected'] = expected
        result['passed'] = result['executed'] == expected and all(row['passed'] for row in result['rows'])
        result['native_feature_off_reference_executed'] = result['passed'] and 'feature_off' in products
        result['intervals'] = []
        for item in result['workloads']:
            for product in products:
                for mode in ('normal', 'idle', 'one-unhit', 'multiple-unhit', 'related-unhit'):
                    rows = [row for row in result['rows'] if row['passed'] and row['workload'] == item['name'] and row['product'] == product and row['mode'] == mode]
                    if not rows: continue
                    interval = {'workload': item['name'], 'product': product, 'mode': mode, 'samples': len(rows),
                                'launch_to_retirement_median_ns': statistics.median(row['retired_ns'] - row['launch_started_ns'] for row in rows)}
                    if mode != 'normal':
                        interval['resume_to_retirement_median_ns'] = statistics.median(row['retired_ns'] - row['resume_started_ns'] for row in rows)
                        interval['observed_initial_pause_management_median_ns'] = statistics.median(row['resume_started_ns'] - row['initial_stop_observed_ns'] for row in rows)
                    if item['name'] == 'timed':
                        interval['guest_window_median_ns'] = statistics.median(row['guest_window_ns'] for row in rows)
                        interval['clock_calibration_median_ns'] = statistics.median(row['clock_calibration_ns'] for row in rows)
                    result['intervals'].append(interval)
        result['final_admission'] = executor.finish()
    except BaseException as error:
        result.update(error=repr(error), passed=False)
    finally:
        result['raw_files'] = [{'path': str(p.relative_to(out)), 'bytes': p.stat().st_size, 'sha256': sha(p)}
                               for p in sorted(out.rglob('*')) if p.is_file() and p.name != 'result.json']
        save()
    need(result['passed'], 'Finite native modes failed; inspect preserved result and raw logs')


if __name__ == '__main__':
    main()
