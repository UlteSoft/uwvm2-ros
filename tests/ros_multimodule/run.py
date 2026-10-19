#!/usr/bin/env python3
"""One real UWVM process, five instances, five fixed policies, six native topics.

Run in the existing 64 GiB Linux worker. This suite owns only port 11379,
DDS domain 79, and /uwvm-five-test mounts in the two existing ROS rootfs.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import socket
import subprocess
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'ros'))
from run_network import Runner, cgroup_snapshot, checked, policy


class FiveRunner(Runner):
    def __init__(self, args):
        self.args = args
        self.base = Path(args.workspace).resolve()
        self.source = Path(args.source).resolve()
        self.run = self.base / 'five-module' / time.strftime('%Y%m%d-%H%M%S')
        self.run.mkdir(parents=True, mode=0o700)
        os.chmod(self.run, 0o700)
        self.roots = {1: self.base / 'gateway/noetic-rootfs', 2: self.base / 'gateway/jazzy-rootfs'}
        self.processes, self.mounts, self.results = [], [], []
        self.artifacts, self.frozen = {}, {}
        self.product = str(Path(args.uwvm).resolve())
        self.env = os.environ.copy()
        self.env.update({'ROS_MASTER_URI': 'http://127.0.0.1:11379', 'ROS_IP': '127.0.0.1',
                         'ROS_DOMAIN_ID': '79', 'RMW_IMPLEMENTATION': 'rmw_fastrtps_cpp',
                         'ROS_SECURITY_ENABLE': 'false', 'LD_LIBRARY_PATH': args.llvm_libs})

    def prepare_five(self):
        snapshots = self.run / 'snapshots'
        snapshots.mkdir(mode=0o700)
        self.snapshot(self.product, snapshots / 'uwvm', 'uwvm')
        source_snapshot = snapshots / 'source'
        source_snapshot.mkdir(mode=0o700)
        for relative in ('tests/ros_multimodule/run.py', 'tests/ros_multimodule/generate.py',
                         'tests/ros_multimodule/native_ros1.cc', 'tests/ros_multimodule/native_ros2.cc',
                         'tests/ros/run_network.py'):
            destination = source_snapshot / relative.replace('/', '_')
            self.snapshot(self.source / relative, destination, 'source/' + relative)
        actual = self.artifacts['uwvm']['sha256']
        if actual != self.args.expected_product_sha256:
            raise RuntimeError('FAIL product SHA differs from explicitly selected final product')
        self.product = str(snapshots / 'uwvm')
        for version in (1, 2):
            snapshot = snapshots / f'ros{version}'
            snapshot.mkdir(mode=0o700)
            for name in (f'uwvm-ros{version}-gateway', f'uwvm-ros{version}-five-module-probe'):
                source = self.roots[version] / f'tmp/ros{version}-build' / name
                if not source.is_file() or not os.access(source, os.X_OK):
                    raise RuntimeError('BLOCKED missing actual native executable ' + str(source))
                self.snapshot(source, snapshot / name, f'ros{version}/{name}')
                self.frozen[f'/tmp/ros{version}-build/{name}'] = f'/uwvm-five-test/snapshots/ros{version}/{name}'
            destination = self.roots[version] / 'uwvm-five-test'
            destination.mkdir(exist_ok=True)
            checked(['mount', '--bind', str(self.run), str(destination)])
            self.mounts.append(destination)
        master = self.start(self.native_command(1, ['roscore', '-p', '11379']), 'roscore')
        for _ in range(200):
            if master.poll() is not None:
                raise RuntimeError('FAIL isolated ROS1 Master exited')
            try:
                with socket.create_connection(('127.0.0.1', 11379), .1):
                    return
            except OSError:
                time.sleep(.025)
        raise RuntimeError('FAIL isolated ROS1 Master readiness timeout')

    def make_target(self, version, scope, name, members, aliases):
        connection, _ = policy(version, [])
        connection.update({'node-name': 'uwvm_five_' + name, 'namespace': '/five'})
        connection['master-uri' if version == 1 else 'domain-id'] = 'http://127.0.0.1:11379' if version == 1 else '79'
        grants = []
        for alias, resource in aliases:
            _, selected = policy(version, ['publish'])
            grant = selected[0]
            grant.update({'alias': alias, 'resource': resource})
            grant['limits'].update({'max-message-bytes': '128', 'max-rate-hz': '20', 'burst': '4', 'max-queue-bytes': '512'})
            grant['limits'].update({'latch': 'true'} if version == 1 else {'qos.durability': 'transient-local'})
            grants.append(grant)
        return {'version': version, 'scope': scope, 'name': name, 'members': members,
                'connection': connection, 'grants': grants}

    def execute(self):
        targets = [
            self.make_target(1, 'group', 'legacy_sensors', ['camera_a', 'camera_b'], [('camera_a', '/five/camera_a'), ('camera_b', '/five/camera_b')]),
            self.make_target(1, 'single', 'controller', ['controller'], [('control', '/five/controller')]),
            self.make_target(2, 'group', 'modern_planners', ['planner'], [('plan', '/five/planner')]),
            self.make_target(1, 'single', 'hybrid', ['hybrid'], [('hybrid1', '/five/hybrid1')]),
            self.make_target(2, 'single', 'hybrid', ['hybrid'], [('hybrid2', '/five/hybrid2')]),
        ]
        _, quota_grants = policy(1, ['publish'])
        quota = quota_grants[0]
        quota.update({'alias': 'quota', 'resource': '/five/shared_cameras'})
        quota['limits'].update({'max-message-bytes': '128', 'max-rate-hz': '0.001', 'burst': '2', 'max-queue-bytes': '512', 'latch': 'true'})
        targets[0]['grants'].append(quota)
        cli = []
        for index, target in enumerate(targets):
            version, name, scope = target['version'], target['name'], target['scope']
            prefix = f'--ros{version}-{scope}-'
            cli += [prefix + 'create', name]
            if scope == 'group':
                for member in target['members']:
                    cli += [prefix + 'add-module', name, member]
            cli += [prefix + 'enable', name]
            connection = {**target['connection'], 'gateway-socket': str(self.run / f's{index}.sock'),
                          'gateway-uid': str(os.getuid()), 'connect-timeout-ms': '1000',
                          'audit-deny-max-rate-hz': '100', 'audit-deny-burst': '100',
                          'audit-deny-max-records': '10000', 'audit-deny-max-bytes': '1048576'}
            for key, value in connection.items():
                cli += [prefix + key, name, value]
            for grant in target['grants']:
                cli += [prefix + 'grant', name, *[grant[key] for key in ('alias', 'operation', 'resource', 'type', 'identity')]]
                for key, value in grant['limits'].items():
                    cli += [prefix + 'set', name, grant['alias'], key, value]
        modules = {}
        for name in ('camera_a', 'camera_b', 'controller', 'planner', 'hybrid'):
            wasm = self.source / f'examples/ros_multimodule/{name}.wasm'
            wat = self.source / f'examples/ros_multimodule/{name}.wat'
            destination = self.run / (name + '.wasm')
            self.snapshot(wasm, destination, 'guest/' + name)
            self.snapshot(wat, self.run / (name + '.wat'), 'wat/' + name)
            modules[name] = str(destination)
        # The five test modules use only the monotonic clock for bounded waits.
        wasi_group=['--wasip1-group-create','basic_io']
        for member in modules:wasi_group += ['--wasip1-group-add-module','basic_io',member]
        wasi_group += ['--wasip1-group-enable','basic_io','--wasip1-group-noinherit-system-environment','basic_io',
                       '--wasip1-group-grant','basic_io','monotonic-clock']
        # WASI, ROS1 and ROS2 Groups are configured in this same actual run.
        # Their equal process/Instance membership never shares operation
        # authority. Only the explicitly named WASI Group receives the clock.
        command = [self.product, *cli, '--wasip1-global-noinherit-system-environment',
                   *wasi_group, '--wasm-set-main-module-name', 'hybrid']
        for name in ('camera_a', 'camera_b', 'controller', 'planner'):
            command += ['--wasm-preload-library', modules[name], name]
        command += ['--run', modules['hybrid']]
        (self.run / 'uwvm-argv.json').write_text(json.dumps(command, indent=2) + '\n')
        worker = self.start(['/bin/bash', '-c', 'kill -STOP $$; exec "$@"', 'uwvm-five-worker', *command], 'uwvm')
        self.worker_pid = worker.pid
        deadline = time.monotonic() + 3
        while time.monotonic() < deadline:
            if worker.poll() is not None:
                raise RuntimeError('FAIL worker exited before fixed peer binding')
            status = Path(f'/proc/{worker.pid}/status').read_text()
            if re.search(r'^State:\s+T', status, re.M):
                break
            time.sleep(.01)
        else:
            raise RuntimeError('FAIL worker stop handshake timeout')
        cgroup_membership = Path(f'/proc/{worker.pid}/cgroup').read_text()
        if cgroup_membership != Path('/proc/self/cgroup').read_text():
            raise RuntimeError('FAIL actual worker escaped bounded runner cgroup')
        self.identity = {'worker_pid': worker.pid, 'worker_uid': os.getuid(), 'worker_cgroup': cgroup_membership,
                         'same_fixed_peer_for_all_five_gateways': True}
        gateways = []
        for index, target in enumerate(targets):
            version = target['version']
            lines = [f'backend ros{version}', f'worker-uid {os.getuid()}', f'worker-pid {worker.pid}',
                     f'socket /uwvm-five-test/s{index}.sock', 'timeout-ms 10000', 'max-sessions 16',
                     'audit-max-records-per-second 100', 'audit-max-records 10000', 'audit-max-bytes 1048576']
            lines += [f'connection {key} {value}' for key, value in target['connection'].items()]
            for grant in target['grants']:
                lines.append('grant ' + ' '.join(grant[key] for key in ('alias', 'operation', 'resource', 'type', 'identity')))
                lines += [f"set {grant['alias']} {key} {value}" for key, value in grant['limits'].items()]
            config = self.run / f's{index}.txt'
            config.write_text('\n'.join(lines) + '\n')
            os.chmod(config, 0o600)
            gateway = self.start(self.native_command(version, [f'/tmp/ros{version}-build/uwvm-ros{version}-gateway', f'/uwvm-five-test/s{index}.txt']), f'gateway-{index}')
            gateways.append(gateway)
            deadline = time.monotonic() + 5
            while time.monotonic() < deadline:
                if gateway.poll() is not None:
                    raise RuntimeError(f'FAIL authoritative Gateway {index} exited before readiness')
                if (self.run / f's{index}.sock').exists():
                    break
                time.sleep(.025)
            else:
                raise RuntimeError(f'FAIL authoritative Gateway {index} socket deadline')
        observations = {
            1: [('/five/camera_a', 'camera-a'), ('/five/camera_b', 'camera-b'), ('/five/controller', 'controller'), ('/five/hybrid1', 'hybrid-ros1'), ('/five/shared_cameras', 'camera-a'), ('/five/shared_cameras', 'camera-b')],
            2: [('/five/planner', 'planner'), ('/five/hybrid2', 'hybrid-ros2')],
        }
        native = {}
        for version, pairs in observations.items():
            arguments = [f'/tmp/ros{version}-build/uwvm-ros{version}-five-module-probe'] + [value for pair in pairs for value in pair]
            node = self.start(self.native_command(version, arguments), f'native-{version}')
            native[version] = node
            deadline = time.monotonic() + 5
            while time.monotonic() < deadline:
                if node.poll() is not None:
                    raise RuntimeError(f'FAIL ROS{version} native observer exited before readiness')
                if b'READY' in (self.run / f'native-{version}.log').read_bytes():
                    break
                time.sleep(.025)
            else:
                raise RuntimeError(f'FAIL ROS{version} native observer readiness deadline')
        time.sleep(.5)
        os.kill(worker.pid, signal.SIGCONT)
        code = worker.wait(timeout=30)
        if code:
            raise RuntimeError(f'FAIL five-module UWVM exit {code}; inspect uwvm.log')
        for version, node in native.items():
            if node.wait(timeout=30):
                raise RuntimeError(f'FAIL ROS{version} actual native observer')
        sessions = {alias: set() for alias in ('camera_a', 'camera_b')}
        for line in (self.run / 'gateway-0.log').read_text().splitlines():
            fields = dict(re.findall(r'([a-z_]+)=([^ ]+)', line))
            if fields.get('operation') == 'topic.publish' and fields.get('error') == '0' and fields.get('alias') in sessions:
                if fields.get('worker_pid') != str(worker.pid):
                    raise RuntimeError('FAIL Group publication audit fixed Worker PID mismatch')
                sessions[fields['alias']].add(fields['session'])
        if any(len(values) != 1 for values in sessions.values()) or sessions['camera_a'] & sessions['camera_b']:
            raise RuntimeError('FAIL camera instances must have distinct authenticated sessions in the shared Group Gateway')
        quota_success = {next(iter(sessions[alias])): 0 for alias in sessions}
        for line in (self.run / 'gateway-0.log').read_text().splitlines():
            fields = dict(re.findall(r'([a-z_]+)=([^ ]+)', line))
            if fields.get('operation') == 'topic.publish' and fields.get('alias') == 'quota':
                if fields.get('error') != '0' or fields.get('session') not in quota_success:
                    raise RuntimeError('FAIL shared quota Gateway observed unauthorized extra request')
                quota_success[fields['session']] += 1
        if any(count != 2 for count in quota_success.values()):
            raise RuntimeError('FAIL shared policy quota must allow exactly two actual requests per camera session')
        module_instances = {}
        for line in (self.run / 'uwvm.log').read_text().splitlines():
            try:
                event = json.loads(line)
            except ValueError:
                continue
            if event.get('event') == 'gateway-result' and event.get('operation') == 'topic.publish' and event.get('error') == 0:
                module_instances.setdefault(event['module'], set()).add(event['instance'])
        if set(module_instances) != {'camera_a', 'camera_b', 'controller', 'planner', 'hybrid'} or any(len(ids) != 1 for ids in module_instances.values()) or len(set.union(*module_instances.values())) != 5:
            raise RuntimeError('FAIL actual host audit must identify five distinct instances, with hybrid shared across both backends')
        observed_counts = {}
        for version in (1, 2):
            output = (self.run / f'native-{version}.log').read_text()
            for resource, count, text in re.findall(r'^OBSERVED ([^ ]+) count=([0-9]+) exact=(.+)$', output, re.M):
                observed_counts.setdefault(resource, {})[text] = {'count': int(count), 'ros_version': version}
        expected_pairs = {(resource, text) for pairs in observations.values() for resource, text in pairs}
        actual_pairs = {(resource, text) for resource, contents in observed_counts.items() for text in contents}
        if actual_pairs != expected_pairs or any(entry['count'] < 2 for contents in observed_counts.values() for entry in contents.values()):
            raise RuntimeError('FAIL both actual messages from each module must be independently native-observed')
        if any(entry['count'] != 2 for entry in observed_counts['/five/shared_cameras'].values()):
            raise RuntimeError('FAIL denied third quota message must have no native effect')
        self.results.append({'name': 'required-five-module-group-single-hybrid', 'status': 'PASS', 'wasm_exit': code,
                             'simultaneous_wasi_ros_groups':{'wasi':'basic_io','ros1':'legacy_sensors','ros2':'modern_planners',
                                 'wasi_members':list(modules),'explicit_wasi_grants':['monotonic-clock'],'wasi_environment_inherited':False},
                             'product_sha256': self.artifacts['uwvm']['sha256'], 'targets': targets,
                             'native_observations': {str(key): value for key, value in observations.items()},
                             'camera_authenticated_sessions': {key: sorted(value) for key, value in sessions.items()},
                             'actual_host_instance_ids': {key: sorted(value) for key, value in module_instances.items()},
                             'actual_native_message_counts': observed_counts,
                             'shared_policy_quota_gateway_requests_per_session': quota_success,
                             'guest_assertions': ['all four preload start sections publish their exact content',
                                                  'hybrid main publishes independently to ROS1 and ROS2',
                                                  'camera_a cannot open controller alias (13)',
                                                  'camera_b cannot publish with camera_a live foreign handle (9)',
                                                  'both cameras independently publish twice through same quota alias; third returns11 and never reaches native ROS',
                                                  'each single-backend instance rejects opposite-backend alias (13)',
                                                  'hybrid cannot open Group aliases (13)',
                                                  'denied capability_open preserves output sentinel']})
        print('PASS required-five-module-group-single-hybrid', flush=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--workspace', default='/tmp/uwvm-ros-extension-20261012')
    parser.add_argument('--source', default='/tmp/uwvm-ros-extension-20261012/source')
    parser.add_argument('--uwvm', required=True)
    parser.add_argument('--expected-product-sha256', required=True)
    parser.add_argument('--llvm-libs', default=os.environ.get('LD_LIBRARY_PATH', ''))
    args = parser.parse_args()
    if os.name != 'posix' or not Path('/proc/self/cgroup').exists():
        print('BLOCKED Linux and actual cgroup required')
        return 2
    runner = FiveRunner(args)
    report = {'status': 'BLOCKED', 'results': []}
    def interrupted(signum, frame):
        raise RuntimeError(f'FAIL interrupted by signal {signum}; bounded owned-process cleanup follows')
    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    try:
        report['cgroup_before'] = cgroup_snapshot()
        runner.prepare_five()
        runner.execute()
        report['status'] = 'PASS'
    except (RuntimeError, OSError, subprocess.SubprocessError) as error:
        report['error'] = str(error)
        report['status'] = 'BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL'
        print(report['status'] + ' ' + str(error), flush=True)
    finally:
        try:
            runner.cleanup()
            report['owned_process_cleanup'] = 'PASS'
            report['cgroup_after'] = cgroup_snapshot()
            if 'cgroup_before' in report:
                before = dict(line.split() for line in report['cgroup_before']['memory.events'].splitlines())
                after = dict(line.split() for line in report['cgroup_after']['memory.events'].splitlines())
                if before.get('oom_kill') != after.get('oom_kill'):
                    raise RuntimeError('FAIL actual bounded cgroup OOM kill increased')
        except (RuntimeError, OSError, subprocess.SubprocessError) as error:
            report['status'], report['cleanup_error'] = 'FAIL', str(error)
        report.update({'results': runner.results, 'artifacts': runner.artifacts,
                       'identity': getattr(runner, 'identity', {}), 'ports': {'ros1_master': 11379, 'ros2_domain': 79}})
        (runner.run / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
        print('Report: ' + str(runner.run / 'report.json'), flush=True)
    return 0 if report['status'] == 'PASS' else 2 if report['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
