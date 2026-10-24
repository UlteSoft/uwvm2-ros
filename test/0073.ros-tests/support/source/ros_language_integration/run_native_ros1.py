#!/usr/bin/env python3
"""Run pinned portable-language Wasms against an independent native ROS1 peer.

This is an explicit Linux test owner, not a production launcher. Prepare an
owned bounded tmpfs and an inputs.json containing the exact cgroup/boot identity,
selected SDK/Gateway/Runtime, previously qualified Wasms and file hashes. The
owner never changes memory limits, downloads tools, grants a Guest network or
filesystem access, or accepts an in-process/mock native witness. All raw output
and failed attempts remain under the supplied root. Reusing a Wasm qualifies
that exact product, not later source changes or another architecture.
"""
import argparse
import hashlib
import http.client
import importlib.util
import json
import os
from pathlib import Path
import re
import resource
import shlex
import signal
import socket
import stat
import subprocess
import sys
import time
import traceback
import xmlrpc.client


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--cohort', choices=('five-language', 'java', 'csharp', 'javascript', 'python', 'moonbit'), default='five-language')
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / ('inputs-ros1.json' if args.cohort != 'five-language' else 'inputs.json')).read_bytes())
    budget = None
    if args.cohort != 'five-language':
        assert len(inputs['wasms']) == 1 and inputs['wasms'][0]['language'] == args.cohort
        spec = importlib.util.spec_from_file_location('selected_java_budget', Path(__file__).with_name('cgroup_budget.py'))
        budget = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(budget)
    group = Path(inputs['cgroup']['path'])
    baseline = (group / 'memory.events').read_text()
    actors = []
    report = {'status': 'RUNNING', 'scope': 'Pinned five-language ROS1 Noetic '
              'x86_64 TCPROS publication, enabled-zero-grant and retirement; '
              'no all-release, vendor, hardware, QEMU or performance credit',
              'inputs': inputs, 'commands': [], 'cases': [], 'zero_grant': [],
              'events_before': baseline, 'protocol_events': []}
    report['cohort'] = args.cohort
    if budget is not None:
        report['scope'] = 'One fresh ' + args.cohort + ' ROS1 Noetic x86_64 TCPROS publication, enabled-zero-grant and quiet retirement; cache-aware bounded owner, no all-release/vendor/QEMU/performance credit'
    output = root / 'out'
    output.mkdir(exist_ok=True)
    for name in ('commands', 'home', 'tmp'):
        (output / name).mkdir(exist_ok=True)

    def save():
        (output / 'native-ros1-results.json').write_text(json.dumps(report, indent=2) + '\n')

    def event(language, phase):
        report['protocol_events'].append({'language': language, 'phase': phase,
                                         'monotonic_ns': time.monotonic_ns()})
        save()

    def guard():
        assert sys.platform == 'linux'
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        info = group.stat()
        assert [info.st_dev, info.st_ino] == inputs['cgroup']['identity']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        if budget is not None:
            oom = lambda body: {key: value for key, value in map(str.split, body.splitlines()) if key.startswith('oom')}
            assert oom((group / 'memory.events').read_text()) == oom(baseline)
            current = budget.sample(group)
            accounting = report.setdefault('memory_accounting', {})
            accounting.setdefault('first', current)
            accounting['last'] = current
            if current['admission_bytes'] > accounting.get('highest_admission', {}).get('admission_bytes', -1):
                accounting['highest_admission'] = current
            return current['admission_bytes']
        assert (group / 'memory.events').read_text() == baseline
        return int((group / 'memory.current').read_text())

    def admit():
        end = time.monotonic() + 60
        while guard() >= 61 << 30:
            if time.monotonic() >= end:
                raise RuntimeError('shared cgroup lacks the selected 3GiB admission margin')
            time.sleep(0.5)

    def children(pid):
        found, pending = {pid}, [pid]
        while pending:
            parent = pending.pop()
            try:
                values = (Path('/proc') / str(parent) / 'task' / str(parent) / 'children').read_text().split()
            except (FileNotFoundError, ProcessLookupError):
                continue
            for child in map(int, values):
                if child not in found:
                    found.add(child)
                    pending.append(child)
        return found

    def rss(pid):
        try:
            return int((Path('/proc') / str(pid) / 'statm').read_text().split()[1]) * os.sysconf('SC_PAGE_SIZE')
        except (FileNotFoundError, ProcessLookupError):
            return 0

    def observe():
        assert guard() < 63 << 30, 'shared cgroup 1GiB running margin'
        total = 0
        for actor in actors:
            process = actor['process']
            if process.poll() is not None:
                continue
            used = 0
            for pid in children(process.pid):
                used += rss(pid)
                try:
                    assert (Path('/proc') / str(pid) / 'cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
                except (FileNotFoundError, ProcessLookupError):
                    pass
            total += used
            row = actor['row']
            row['max_sampled_owned_rss'] = max(row['max_sampled_owned_rss'], used)
            assert time.monotonic() - actor['started'] < row['deadline_seconds'], 'actor deadline'
            assert sum((actor['directory'] / name).stat().st_size for name in ('stdout', 'stderr')) <= 8 << 20, 'output bound'
        assert total <= 768 << 20, '768MiB sampled owned process-tree RSS bound'

    def limits():
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        resource.setrlimit(resource.RLIMIT_AS, (16 << 30, 16 << 30))
        resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
        resource.setrlimit(resource.RLIMIT_FSIZE, (64 << 20, 64 << 20))
        resource.setrlimit(resource.RLIMIT_NOFILE, (1024, 1024))

    def launch(label, argv, extra=None, held=False, deadline=45):
        guard()
        directory = output / 'commands' / label
        directory.mkdir()
        env = {'PATH': '/usr/bin:/bin', 'HOME': str(output / 'home'),
               'TMPDIR': str(output / 'tmp'), 'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8',
               'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1'}
        env.update(extra or {})
        argv = list(map(str, argv))
        read_fd = write_fd = None
        actual = argv
        if held:
            read_fd, write_fd = os.pipe()
            actual = ['/usr/bin/python3', '-I', '-S', '-B', str(root / 'held-exec.py'), str(read_fd), *argv]
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            process = subprocess.Popen(actual, env=env, stdin=subprocess.PIPE if held else subprocess.DEVNULL,
                                       stdout=stdout, stderr=stderr, start_new_session=True,
                                       pass_fds=(read_fd,) if held else (), preexec_fn=limits)
        if held:
            os.close(read_fd)
        row = {'label': label, 'argv': argv, 'launch_argv': actual, 'env': env,
               'pid': process.pid, 'deadline_seconds': deadline, 'exit': None,
               'forced_signal': None, 'max_sampled_owned_rss': 0,
               'started_monotonic_ns': time.monotonic_ns()}
        actor = {'process': process, 'row': row, 'directory': directory,
                 'started': time.monotonic(), 'release': write_fd,
                 'pidfd': os.pidfd_open(process.pid)}
        actors.append(actor)
        report['commands'].append(row)
        save()
        return actor

    def text(actor, stream='stdout'):
        return (actor['directory'] / stream).read_text()

    def wait(actor, expected=0):
        while actor['process'].poll() is None:
            observe()
            time.sleep(0.02)
        actor['row']['exit'] = actor['process'].wait()
        actor['row']['finished_monotonic_ns'] = time.monotonic_ns()
        for name in ('stdout', 'stderr'):
            actor['row'][name + '_sha256'] = digest(actor['directory'] / name)
        save()
        assert actor['row']['exit'] == expected, (actor['row']['label'], actor['row']['exit'], text(actor, 'stderr')[-2000:])

    def marker(actor, prefix):
        while True:
            observe()
            if any(line.startswith(prefix) for line in text(actor).splitlines()):
                return
            assert actor['process'].poll() is None, (actor['row']['label'], prefix, text(actor, 'stderr')[-2000:])
            time.sleep(0.02)

    def graph():
        # Fixed numeric loopback endpoint, finite socket/read and response bounds.
        # Never use environment proxies, arbitrary XMLRPC URLs or remote peers.
        body = xmlrpc.client.dumps(('/roswasm_language_owner',), methodname='getSystemState').encode()
        connection = http.client.HTTPConnection('127.0.0.1', 11386, timeout=2)
        try:
            connection.request('POST', '/', body, {'Content-Type': 'text/xml'})
            response = connection.getresponse()
            assert response.status == 200
            raw = response.read((1 << 20) + 1)
            assert len(raw) <= 1 << 20
        finally:
            connection.close()
        values, _ = xmlrpc.client.loads(raw)
        assert len(values) == 1 and len(values[0]) == 3 and values[0][0] == 1
        state = values[0][2]
        assert len(state) == 3 and all(type(category) is list and len(category) <= 128 for category in state)
        return state

    def endpoints(state, category, topic):
        rows = [row for row in state[category] if len(row) == 2 and row[0] == topic]
        assert len(rows) <= 1
        return sorted(rows[0][1]) if rows else []

    def maps(actor, admitted):
        records = {}
        raw = (Path('/proc') / str(actor['process'].pid) / 'maps').read_text()
        for line in raw.splitlines():
            fields = line.split(None, 5)
            if len(fields) != 6 or 'x' not in fields[1] or not fields[5].startswith('/'):
                continue
            path = Path(fields[5])
            assert any(path == prefix or path.is_relative_to(prefix) for prefix in admitted), str(path)
            item = path.stat()
            records[str(path)] = {'identity': [item.st_dev, item.st_ino, item.st_size, item.st_mtime_ns, item.st_ctime_ns]}
        return {'text': raw, 'files': records}

    master = None
    save()
    try:
        os.sched_setaffinity(0, {20})
        guard()
        assert root.is_mount() and root.stat().st_dev != root.parent.stat().st_dev
        for row in inputs['pins']:
            assert Path(row['path']).stat().st_size == row['bytes'] and digest(row['path']) == row['sha256'], row['path']
        for row in inputs.get('native_header_aliases', []):
            assert os.readlink(row['path']) == row['target']
        spec = importlib.util.spec_from_file_location('selected_policy', root / 'source/sdk/origin/tools/example_policy.py')
        policy = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(policy)
        sdk = Path(inputs['sdk'])
        peer_sdk = Path(inputs.get('peer_sdk', inputs['sdk']))
        loader = Path(inputs['loader'])
        peer_loader = Path(inputs.get('peer_loader', inputs['loader']))
        libraries = inputs['library_path']
        peer_libraries = inputs.get('peer_library_path', libraries)
        gateway = Path(inputs['gateway'])
        runtime = Path(inputs['runtime'])
        peer = output / 'native-ros1-peer'
        argv = ['/usr/bin/clang++-21', '--sysroot=' + str(peer_sdk), '--gcc-toolchain=' + str(peer_sdk / 'usr'),
                '-std=c++20', '-O1', '-g', '-pthread', '-I', root / 'source/third-parties/fast_io/include',
                '-I', sdk / 'opt/ros/noetic/include', '-I', root / 'source/native-include', '-MD', '-MF', output / 'native-ros1-peer.d',
                root / 'source/test/0073.ros-tests/support/source/ros_language_integration/native_ros1_peer.cc', '-o', peer,
                '-L', sdk / 'opt/ros/noetic/lib', '-Wl,--dynamic-linker=' + str(peer_loader),
                '-Wl,--disable-new-dtags', '-Wl,-rpath,' + peer_libraries, '-Wl,-rpath-link,' + peer_libraries,
                *inputs['native_libraries'], '-ldl', '-latomic']
        admit()
        compiler = launch('native-peer-build', argv, deadline=180)
        wait(compiler)
        report['peer'] = {'path': str(peer), 'bytes': peer.stat().st_size, 'sha256': digest(peer)}
        report['compiler'] = {'path': '/usr/bin/clang++-21', 'sha256': digest(Path('/usr/bin/clang++-21').resolve())}
        dependencies = shlex.split((output / 'native-ros1-peer.d').read_text().replace('\\\n', ' '))[1:]
        report['compile_dependencies'] = {path: {'bytes': Path(path).stat().st_size,
                                                'sha256': digest(path)}
                                          for path in dict.fromkeys(dependencies)}
        (root / 'held-exec.py').write_text('import os,sys\nfd=int(sys.argv[1])\nif os.read(fd,1)!=b"E":raise SystemExit(125)\nos.close(fd)\nos.execvpe(sys.argv[2],sys.argv[2:],os.environ)\n')
        with socket.socket() as reservation:
            reservation.bind(('127.0.0.1', 11386))
        env = {'ROS_MASTER_URI': 'http://127.0.0.1:11386', 'ROS_IP': '127.0.0.1',
               'ROS_LOG_DIR': str(output / 'ros-logs'), 'PYTHONDONTWRITEBYTECODE': '1',
               'PYTHONNOUSERSITE': '1', 'PYTHONHOME': str(sdk / 'usr'),
               'PYTHONPATH': str(sdk / 'opt/ros/noetic/lib/python3/dist-packages') + ':' + str(sdk / 'usr/lib/python3/dist-packages'),
               'LD_LIBRARY_PATH': libraries}
        admit()
        master = launch('native-master', [loader, '--inhibit-cache', '--library-path', libraries,
                          sdk / 'usr/bin/python3.8', '-B', sdk / 'opt/ros/noetic/bin/rosmaster', '--core', '-p', '11386'], env, deadline=300)
        until = time.monotonic() + 15
        while True:
            observe()
            try:
                initial = graph()
                assert initial == [[], [], []], initial
                break
            except (ConnectionError, OSError, http.client.HTTPException):
                assert time.monotonic() < until and master['process'].poll() is None
                time.sleep(0.05)
        for index, row in enumerate(inputs['wasms']):
            admit()
            lang = row['language']
            module = Path(row['path'])
            clocks = ['--wasip1-global-grant', 'monotonic-clock'] if lang == 'go' else []
            zero_directory = root / ('zero' + str(index))
            zero_directory.mkdir(mode=0o700)
            zero_namespace = '/uwvm_language_r47_zero/c' + str(index)
            zero_options = argparse.Namespace(backend=1, gateway_timeout_ms=15000,
                socket=zero_directory / 'g', gateway_uid=os.getuid(), worker_uid=os.getuid(),
                worker_pid=1, node_name='roswasm_language_gateway', namespace=zero_namespace,
                domain_id=None, disabled_security_tutorial=False, master_uri=env['ROS_MASTER_URI'],
                advertise_host='127.0.0.1', operation=[], resource=zero_namespace + '/telemetry')
            zero_plan = policy.policy(zero_options)
            assert zero_plan['grants'] == []
            zero = launch(lang + '-enabled-zero-grant', [runtime, '-Rint',
                *zero_plan['Worker_arguments'], *clocks, '--run', module,
                *([] if lang in ('javascript', 'moonbit') else ['deny'])], held=True)
            zero_options.worker_pid = zero['process'].pid
            zero_bound = policy.policy(zero_options)
            assert zero_bound['Worker_arguments'] == zero_plan['Worker_arguments']
            zero_config = zero_directory / 'gateway.conf'
            zero_config.write_text(zero_bound['Gateway_config_text'])
            zero_config.chmod(0o600)
            (zero_directory / 'policy.json').write_text(json.dumps(zero_bound, indent=2) + '\n')
            zero_gateway = launch(lang + '-zero-grant-gateway', [loader, '--inhibit-cache',
                '--library-path', libraries, gateway, zero_config], env)
            while not zero_options.socket.exists():
                observe()
                assert zero_gateway['process'].poll() is None, text(zero_gateway, 'stderr')[-2000:]
                time.sleep(0.02)
            zero_graph = graph()
            # Native roscpp may register its own logging administration services.
            # They are not Guest grants; zero topic endpoints must exist even
            # while this independently owned Gateway is running.
            assert zero_graph[0] == [] and zero_graph[1] == []
            os.write(zero['release'], b'E')
            os.close(zero['release'])
            zero['release'] = None
            wait(zero)
            wait(zero_gateway)
            assert text(zero) == 'PASS_ROS_DEFAULT_DENIED\n'
            assert graph() == [[], [], []]
            zero_audit = [json.loads(line) for line in text(zero, 'stderr').splitlines()]
            zero_denials = [item for item in zero_audit if item.get('event') == 'unresolved-denial']
            assert len(zero_denials) == 2 and all(item['error'] == 13 and item['ros'] == 'ros1' for item in zero_denials)
            report['zero_grant'].append({'language': lang, 'argv': zero['row']['argv'],
                'policy': zero_bound, 'no_native_topic_endpoints': True,
                'graph_before_guest': zero_graph, 'graph_after_retirement': graph(),
                'exact_denials': zero_denials})
            directory = root / ('case' + str(index))
            directory.mkdir(mode=0o700)
            namespace = '/uwvm_language_r47/c' + str(index)
            topic = namespace + '/telemetry'
            peer_env = dict(env, LD_LIBRARY_PATH=peer_libraries)
            observer = launch(lang + '-native-peer', [peer_loader, '--inhibit-cache', '--library-path', peer_libraries, peer, namespace], peer_env)
            marker(observer, 'READY_NATIVE_ROS1_TELEMETRY ')
            event(lang, 'peer-ready')
            registration_deadline = time.monotonic() + 5
            while True:
                before = graph()
                report['last_peer_registration_graph'] = before
                save()
                assert endpoints(before, 0, topic) == [], ('unexpected publisher before Guest', before)
                if endpoints(before, 1, topic) == [namespace + '/roswasm_language_native_peer']:
                    break
                observe()
                assert time.monotonic() < registration_deadline, ('subscriber registration deadline', before)
                time.sleep(0.02)
            options = argparse.Namespace(backend=1, gateway_timeout_ms=15000, socket=directory / 'g',
                gateway_uid=os.getuid(), worker_uid=os.getuid(), worker_pid=1,
                node_name='roswasm_language_gateway', namespace=namespace, domain_id=None,
                disabled_security_tutorial=False, master_uri=env['ROS_MASTER_URI'],
                advertise_host='127.0.0.1', operation=['publish'], resource=topic)
            plan = policy.policy(options)
            identity = 'ros1_language_' + lang
            worker = launch(lang + '-native-worker', [runtime, '-Rint', *plan['Worker_arguments'],
                '--wasm-set-main-module-name', identity, *clocks, '--run', module,
                *([] if lang in ('javascript', 'moonbit') else ['publish'])], held=True)
            options.worker_pid = worker['process'].pid
            bound = policy.policy(options)
            assert bound['Worker_arguments'] == plan['Worker_arguments']
            config = directory / 'gateway.conf'
            config.write_text(bound['Gateway_config_text'])
            config.chmod(0o600)
            (directory / 'policy.json').write_text(json.dumps(bound, indent=2) + '\n')
            native = launch(lang + '-native-gateway', [loader, '--inhibit-cache', '--library-path', libraries, gateway, config], env)
            while not options.socket.exists():
                observe()
                assert native['process'].poll() is None, text(native, 'stderr')[-2000:]
                time.sleep(0.02)
            assert stat.S_ISSOCK(options.socket.lstat().st_mode) and stat.S_IMODE(options.socket.lstat().st_mode) == 0o600
            os.write(worker['release'], b'E')
            os.close(worker['release'])
            worker['release'] = None
            event(lang, 'worker-exec-released')
            marker(worker, 'READY_ROS_PUBLISH')
            marker(observer, 'MATCHED_NATIVE_ROS1_TELEMETRY ')
            matched = graph()
            assert endpoints(matched, 0, topic) == [namespace + '/roswasm_language_gateway']
            assert endpoints(matched, 1, topic) == [namespace + '/roswasm_language_native_peer']
            event(lang, 'native-match-and-master-graph-checked')
            mapping = {'gateway': maps(native, [sdk, Path(inputs['gateway_library_root']), gateway]),
                       'peer': maps(observer, [sdk, peer_sdk, peer])}
            worker['process'].stdin.write(b'P')
            worker['process'].stdin.flush()
            event(lang, 'P-credit-sent')
            marker(worker, 'PUBLISHED_ROS_VALUE')
            marker(observer, 'DELIVERED_NATIVE_ROS1_TELEMETRY ')
            event(lang, 'complete-native-value-and-header-observed')
            worker['process'].stdin.write(b'A')
            worker['process'].stdin.flush()
            event(lang, 'A-credit-sent')
            worker['process'].stdin.close()
            wait(worker)
            wait(native)
            wait(observer)
            retired = graph()
            assert endpoints(retired, 0, topic) == [] and endpoints(retired, 1, topic) == []
            event(lang, 'natural-exits-quiet-and-master-retirement-checked')
            assert text(worker) == 'READY_ROS_PUBLISH\nPUBLISHED_ROS_VALUE\nPASS_ROS_PUBLISH\n'
            audit = [json.loads(line) for line in text(worker, 'stderr').splitlines()]
            denials = [item for item in audit if item.get('event') == 'unresolved-denial']
            assert len(denials) == 1 and denials[0]['error'] == 13 and denials[0]['ros'] == 'ros1' and denials[0]['module'] == identity
            assert all(item.get('error') in (0, 13) for item in audit)
            lines = text(observer).splitlines()
            assert len(lines) == 4 and text(observer, 'stderr') == ''
            protocol = []
            for phase, line in zip(('READY', 'MATCHED', 'DELIVERED', 'PASS'), lines):
                pattern = (phase + '_NATIVE_ROS1_TELEMETRY namespace=' + re.escape(namespace) +
                    ' gateway=' + re.escape(namespace) + r'/roswasm_language_gateway transport=tcpros type=std_msgs/String md5=992ce8a1687cec8c8bd883ec73ca41d1 latch=0 warmup=([01]) delivered=([01]) publishers=([01])')
                match = re.fullmatch(pattern, line)
                assert match, line
                protocol.append({'phase': phase, 'warmup': int(match[1]), 'delivered': int(match[2]), 'publishers': int(match[3])})
            assert [item['delivered'] for item in protocol] == [0, 0, 1, 1]
            assert protocol[0]['publishers'] == 0 and protocol[1]['publishers'] == 1 and protocol[-1]['publishers'] == 0
            for role in mapping.values():
                for path, item in role['files'].items():
                    info = Path(path).stat()
                    assert [info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns] == item['identity']
                    item['sha256'] = digest(path)
            report['cases'].append({'language': lang, 'wasm': row, 'policy': bound, 'protocol': protocol,
                'graph_before': before, 'graph_matched': matched, 'graph_after_retirement': retired,
                'complete_value': 'bounded WasmROS telemetry', 'quiet_seconds_after_zero_publishers': 2,
                'actual_executable_maps': mapping, 'ungranted_alias_denial': denials[0]})
            save()
            print('NATIVE ROS1 PASS', lang, flush=True)
        for row in inputs['pins']:
            assert Path(row['path']).stat().st_size == row['bytes'] and digest(row['path']) == row['sha256']
        for path, row in report['compile_dependencies'].items():
            assert Path(path).stat().st_size == row['bytes'] and digest(path) == row['sha256']
        for row in inputs.get('native_header_aliases', []):
            assert os.readlink(row['path']) == row['target']
        report['input_and_compile_dependencies_preserved'] = True
        report['status'] = ('ACTUAL_' + args.cohort.upper() + '_NATIVE_ROS1_DELIVERY_PASS' if args.cohort != 'five-language'
                            else 'ACTUAL_FIVE_LANGUAGE_NATIVE_ROS1_DELIVERY_PASS')
    except BaseException as error:
        report['status'] = 'FAILED_RETAINED'
        report['error'] = repr(error)
        report['traceback'] = traceback.format_exc()
        print('NATIVE ROS1 FAILURE', repr(error), flush=True)
    finally:
        if master is not None and master['process'].poll() is None:
            # The explicitly owned long-lived Master has a separate normal
            # administrative stop. Guest, peer and Gateway successes require
            # natural exit0 and never count this stop as their retirement.
            master['row']['administrative_stop'] = 'SIGINT after owned cases'
            signal.pidfd_send_signal(master['pidfd'], signal.SIGINT)
            try:
                master['process'].wait(timeout=5)
            except subprocess.TimeoutExpired:
                pass
        for actor in reversed(actors):
            if actor['release'] is not None:
                os.close(actor['release'])
            if actor['process'].poll() is None:
                os.killpg(actor['process'].pid, signal.SIGKILL)
                actor['row']['forced_signal'] = 'SIGKILL owned process group after failed or expired test'
            actor['row']['exit'] = actor['process'].wait()
            os.close(actor['pidfd'])
            for name in ('stdout', 'stderr'):
                actor['row'][name + '_sha256'] = digest(actor['directory'] / name)
        report['events_after'] = (group / 'memory.events').read_text()
        if report['status'].endswith('_PASS'):
            assert report['events_after'] == baseline
            assert all(actor['row']['exit'] == 0 and actor['row']['forced_signal'] is None for actor in actors)
        save()
    return 0 if report['status'].endswith('_PASS') else 1


if __name__ == '__main__':
    raise SystemExit(main())
