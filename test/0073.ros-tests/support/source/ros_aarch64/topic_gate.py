"""Two-phase actual ROS discovery gate for only the new multi-memory fixture."""
import json
import os
from pathlib import Path
import re
import signal
import threading
import time
import xmlrpc.client

from runtime import digest, require_arm


def arm_artifact(label, path):
    """ROS snapshot labels also name JSON profiles; only native names are ELF."""
    if label == 'uwvm':
        return True
    name = Path(path).name
    return any(label.startswith('ros'+str(version)+'/') and
               name.startswith('uwvm-ros'+str(version)+'-') for version in (1, 2))


def remote_history_valid(fields):
    scope = fields.get('history_depth_scope')
    return ((scope == 'remote-unavailable' and fields.get('history_policy') == 'unknown' and fields.get('depth') == '0') or
            (scope == 'remote-reported' and fields.get('history_policy') == 'keep-last' and fields.get('depth') == '4'))


def load_observers(path, source):
    path = Path(path).resolve()
    value = json.loads(path.read_text())
    if value.get('status') != 'PREPARED' or set(value.get('observers', {})) != {'1', '2'}:
        raise RuntimeError('BLOCKED two compiled ARM topic observers are required')
    preparation = value.get('preparation_report', {})
    preparation_path = Path(preparation['path'])
    if digest(preparation_path) != preparation.get('sha256') or json.loads(preparation_path.read_text()).get('status') != 'PASS':
        raise RuntimeError('FAIL native observer preparation is not qualified')
    for version, row in value['observers'].items():
        if require_arm(row['path'])['sha256'] != row['sha256']:
            raise RuntimeError('FAIL topic observer changed: ROS'+version)
        if digest(Path(source)/f'test/0073.ros-tests/support/source/ros{version}/native_probe.cc') != row['original_observer_source_sha256']:
            raise RuntimeError('FAIL observer predicate baseline differs from frozen source')
        for item in row['compiled_inputs']:
            if digest(item['path']) != item['sha256']:
                raise RuntimeError('FAIL topic observer compiler input changed: '+item['path'])
        if digest(row['raw_MMD_path']) != row['raw_MMD_sha256']:
            raise RuntimeError('FAIL topic observer compiler dependencies changed')
        if digest(Path(__file__).with_name(f'topic_observer_ros{version}.cc')) != row['addon_source_sha256']:
            raise RuntimeError('FAIL observer addon differs from the compiled source')
        for key in ('flags', 'compile_commands', 'generated_cmake', 'link'):
            if digest(row[key+'_path']) != row[key+'_sha256']:
                raise RuntimeError('FAIL observer compiler metadata changed: '+key)
        reused = row.get('reuse_prepared_binding')
        if reused:
            origin_manifest = Path(reused['manifest_path'])
            origin_report = Path(reused['preparation_report']['path'])
            if digest(origin_manifest) != reused['manifest_sha256'] or digest(origin_report) != reused['preparation_report']['sha256'] or json.loads(origin_report.read_text()).get('status') != 'PASS':
                raise RuntimeError('FAIL reused observer original prepared provenance changed')
        if not row['compiled_inputs']:
            raise RuntimeError('FAIL missing native observer compiler inputs')
    return value, {'path': str(path), 'sha256': digest(path), 'status': value['status']}


class ShortTransport(xmlrpc.client.Transport):
    def make_connection(self, host):
        result = super().make_connection(host)
        result.timeout = .5
        return result


class DiscoveryGate:
    def __init__(self, runner, evidence, case, version, observer_label):
        self.runner, self.evidence, self.case, self.version = runner, evidence, case, version
        self.observer_label = observer_label
        self.processes = {}
        self.error = None
        self.thread = None
        self.stop = threading.Event()
        self.row = {'case': case, 'version': version, 'status': 'WAITING',
                    'worker_timeout_seconds': 20, 'native_timeout_seconds': 12,
                    'expected_accepted_publishes': 2,
                    'native_predicate': 'received && !wrong, exact String hello; native count1 or2'}

    def register(self, name, process):
        self.processes[name] = process

    def marker(self, suffix, prefix, deadline):
        name = self.case+suffix
        process = self.processes[name]
        log = self.runner.run/(name+'.log')
        while time.monotonic() < deadline and not self.stop.is_set():
            lines = log.read_bytes().splitlines()
            found = [line.decode() for line in lines if line.startswith(prefix)]
            if found:
                return found[-1]
            if process.poll() is not None:
                raise RuntimeError('FAIL discovery gate process exited: '+name)
            time.sleep(.01)
        raise RuntimeError('FAIL actual discovery gate readiness deadline: '+name)

    def positive_pid(self, name, label):
        process = self.processes[name]
        self.evidence.observe(process.pid, name)
        with self.evidence._lock:
            rows = [sample for sample in self.evidence.samples.values()
                    if sample['name'] == name and sample['ancestry'][0] == process.pid
                    and label in sample['mapped_arm_targets']]
        if not rows:
            raise RuntimeError('FAIL missing exact-launch ARM mapping before gate: '+name)
        # The frozen target may be behind chroot/bash; use its actual QEMU PID.
        return rows[-1]['pid']

    def ros1_identity(self, observer_pid, observer_node):
        caller = '/uwvm_arm_discovery_gate'
        uri = self.runner.env['ROS_MASTER_URI']
        resource = '/uwvm_ros_test/output'
        publisher_node = '/uwvm_ros_test/uwvm_ros1_wasm_network'
        gateway_pid = self.positive_pid(self.case+'-gateway1', 'ros1/uwvm-ros1-gateway')
        with xmlrpc.client.ServerProxy(uri, transport=ShortTransport()) as master:
            code, _, state = master.getSystemState(caller)
            if code != 1:
                raise RuntimeError('FAIL actual ROS1 Master state unavailable')
            publishers, subscribers, _ = state
            actual_writers = next((nodes for topic, nodes in publishers if topic == resource), [])
            actual_readers = next((nodes for topic, nodes in subscribers if topic == resource), [])
            if actual_writers != [publisher_node] or observer_node not in actual_readers:
                raise RuntimeError('FAIL ROS1 exact resource/endpoint registration mismatch')
            facts = []
            for node, expected in ((publisher_node, gateway_pid), (observer_node, observer_pid)):
                code, _, address = master.lookupNode(caller, node)
                if code != 1 or not address.startswith(('http://127.0.0.1:', 'http://localhost:')):
                    raise RuntimeError('FAIL ROS1 exact local endpoint lookup')
                with xmlrpc.client.ServerProxy(address, transport=ShortTransport()) as peer:
                    code, _, actual_pid = peer.getPid(caller)
                if code != 1 or actual_pid != expected:
                    raise RuntimeError('FAIL ROS1 endpoint PID differs from live ARM launch')
                facts.append({'node': node, 'pid': actual_pid, 'URI': address})
        return {'resource': resource, 'writers': actual_writers, 'readers': actual_readers,
                'exact_ARM_endpoint_PIDs': facts}

    def before_resume(self):
        suffix = '-native'+str(self.version)+'--observe'
        ready = self.marker(suffix, b'READY_SUBSCRIBER ', time.monotonic()+8)
        observer_pid = self.positive_pid(self.case+suffix, self.observer_label)
        fields = dict(token.split('=', 1) for token in ready.split()[1:])
        if fields.get('pid') != str(observer_pid) or fields.get('resource') != '/uwvm_ros_test/output':
            raise RuntimeError('FAIL observer readiness PID/resource mismatch')
        self.row.update(subscriber_marker=ready, actual_observer_pid=observer_pid)

        def release():
            try:
                # This runs concurrently with original_case's SIGCONT and its
                # unchanged Worker.wait(timeout=20). Gate time consumes that budget.
                deadline = time.monotonic()+12
                published = self.marker('-uwvm', b'WASM_PUBLISHER_OPEN', deadline)
                matched = self.marker(suffix, b'READY_MATCHED ', deadline)
                fields = dict(token.split('=', 1) for token in matched.split()[1:])
                if fields.get('pid') != str(observer_pid) or fields.get('actual_publishers') != '1':
                    raise RuntimeError('FAIL actual matched writer PID/count')
                self.positive_pid(self.case+'-uwvm', 'uwvm')
                self.positive_pid(self.case+'-gateway'+str(self.version),
                                  'ros'+str(self.version)+'/uwvm-ros'+str(self.version)+'-gateway')
                if self.version == 1:
                    self.row['master_identity'] = self.ros1_identity(observer_pid,
                        dict(token.split('=', 1) for token in ready.split()[1:])['node'])
                elif not re.search(r'writer_gid=[0-9a-f]{32,}', matched) or set(fields.get('writer_gid', '0')) == {'0'} or fields.get('node') != '/uwvm_ros_test/uwvm_ros2_wasm_network':
                    raise RuntimeError('FAIL exact DDS writer endpoint identity')
                worker = self.processes[self.case+'-uwvm']
                worker.stdin.write(b'G'); worker.stdin.flush(); worker.stdin.close()
                self.row.update(publisher_marker=published, actual_match_marker=matched,
                                released_monotonic_ns=time.monotonic_ns(), status='RELEASED')
            except BaseException as error:
                self.error = error
                self.row.update(status='FAIL', error=str(error))
                worker = self.processes.get(self.case+'-uwvm')
                if worker and worker.stdin and not worker.stdin.closed:
                    worker.stdin.close()  # EOF fails the fixture, never grants readiness.

        self.thread = threading.Thread(target=release, daemon=True)
        self.thread.start()
        return self.row

    def finish(self):
        self.stop.set()
        if self.thread:
            self.thread.join(timeout=1)
            if self.thread.is_alive():
                raise RuntimeError('FAIL discovery gate thread survived case cleanup')
        if self.error:
            raise self.error
        if self.row['status'] != 'RELEASED':
            raise RuntimeError('FAIL native discovery never released the second publish')
        worker_log = (self.runner.run/(self.case+'-uwvm.log')).read_text()
        gateway_log = (self.runner.run/(self.case+'-gateway'+str(self.version)+'.log')).read_text()
        accepted = [line for line in gateway_log.splitlines() if
                    {'operation=topic.publish', 'alias=pub', 'error=0'}.issubset(line.split())]
        denials = [json.loads(line) for line in worker_log.splitlines() if line.startswith('{')]
        if len(accepted) != 2 or sum(row.get('error') == 22 for row in denials) != 7:
            raise RuntimeError('FAIL exact two accepted sends / seven invalid-memory22 assertions')
        self.row.update(status='PASS', gateway_accepted_publish_audits=accepted,
                        worker_invalid22_count=7, sentinel_preserved_by_Wasm_assertion=True)


class OriginalObserverReady(DiscoveryGate):
    """Preserve original observer/Guest behavior; wait for this observer's graph.

    A registered reader proves SDK startup. Lazy publishers are still created
    by the original Guest's first publish; no pre-publication writer match is
    claimed here. All original native and Worker deadlines remain unchanged.
    """
    def __init__(self, runner, evidence, case, witnesses, message_type, processes, seen_gids):
        super().__init__(runner, evidence, case, 0, None)
        self.processes = processes
        self.witnesses = [(v, resource) for v, mode, resource in witnesses if mode == '--observe']
        self.message_type, self.seen_gids = message_type, seen_gids
        self.row = {'case': case, 'status': 'WAITING', 'observers': [],
                    'scope': 'Actual original observer reader startup; no lazy writer-match claim',
                    'worker_timeout_seconds': 20, 'native_timeout_seconds': 12}

    def before_resume(self):
        for version, resource in self.witnesses:
            started = time.monotonic()
            deadline = started+8
            name = self.case+f'-native{version}--observe'
            last = None
            while time.monotonic() < deadline:
                if self.processes[name].poll() is not None:
                    raise RuntimeError('FAIL original observer exited before graph readiness: '+name)
                try:
                    actual_pid = self.positive_pid(name, f'ros{version}/uwvm-ros{version}-native-probe')
                    break
                except RuntimeError as error:
                    last = str(error); time.sleep(.01)
            else:
                raise RuntimeError('FAIL original observer ARM startup deadline: '+str(last))
            expected_node = f'/uwvm_ros{version}_network_witness'
            family = {'String': 'std_msgs', 'Bool': 'std_msgs', 'Twist': 'geometry_msgs', 'Image': 'sensor_msgs'}[self.message_type]
            topic_type = family+('/' if version == 1 else '/msg/')+self.message_type
            row = {'version': version, 'pid': actual_pid, 'resource': resource,
                   'expected_node': expected_node, 'expected_type': topic_type}
            if version == 1:
                caller = '/uwvm_arm_original_observer_gate'
                while time.monotonic() < deadline:
                    try:
                        with xmlrpc.client.ServerProxy(self.runner.env['ROS_MASTER_URI'], transport=ShortTransport()) as master:
                            code, _, state = master.getSystemState(caller)
                            readers = next((nodes for topic, nodes in state[1] if topic == resource), []) if code == 1 else []
                            code, _, types = master.getTopicTypes(caller)
                            actual_type = next((kind for topic, kind in types if topic == resource), None) if code == 1 else None
                            code, _, address = master.lookupNode(caller, expected_node)
                        if expected_node not in readers or actual_type != topic_type or code != 1 or not address.startswith(('http://127.0.0.1:', 'http://localhost:')):
                            raise RuntimeError('actual ROS1 reader registration not ready')
                        with xmlrpc.client.ServerProxy(address, transport=ShortTransport()) as peer:
                            code, _, pid = peer.getPid(caller)
                        if code != 1 or pid != actual_pid:
                            raise RuntimeError('registered ROS1 observer PID differs from this ARM launch')
                        row.update(actual_node_PID=pid, actual_reader_nodes=readers, actual_type=actual_type, URI=address)
                        break
                    except (OSError, RuntimeError, xmlrpc.client.Error) as error:
                        last = str(error); time.sleep(.01)
                else:
                    raise RuntimeError('FAIL original ROS1 exact reader readiness deadline: '+str(last))
            else:
                helper_name = self.case+'-reader-graph-query2'
                argv = [self.runner._arm_topic_observers[2], '--discover-subscription', resource,
                        expected_node, topic_type, ','.join(sorted(self.seen_gids))]
                helper = self.runner.start(self.runner.native_command(2, argv), helper_name)
                self.processes[helper_name] = helper
                log = self.runner.run/(helper_name+'.log')
                while time.monotonic() < deadline:
                    found = [line.decode() for line in log.read_bytes().splitlines()
                             if line.startswith(b'READY_SUBSCRIPTION_GRAPH ')]
                    if found:
                        marker = found[-1]
                        break
                    if helper.poll() is not None:
                        raise RuntimeError('FAIL ARM original-reader graph helper exited')
                    time.sleep(.01)
                else:
                    raise RuntimeError('FAIL original ROS2 exact reader graph readiness deadline')
                helper_pid = self.positive_pid(helper_name, 'ros2/uwvm-ros2-topic-observer')
                fields = dict(token.split('=', 1) for token in marker.split()[1:])
                gid = fields.get('reader_gid', '')
                history_scope = fields.get('history_depth_scope')
                history_valid = remote_history_valid(fields)
                if fields.get('pid') != str(helper_pid) or fields.get('resource') != resource or fields.get('node') != expected_node or fields.get('type') != topic_type or fields.get('reliable') != '1' or fields.get('volatile') != '1' or not fields.get('history', '').isdigit() or not history_valid or not re.fullmatch('[0-9a-f]{32,}', gid) or set(gid) == {'0'} or gid in self.seen_gids:
                    raise RuntimeError('FAIL actual DDS reader identity/GID/QoS does not match this case')
                self.seen_gids.add(gid)
                os.kill(helper_pid, signal.SIGCONT)
                if helper.wait(timeout=3):
                    raise RuntimeError('FAIL actual ARM reader graph helper completion')
                # DDS does not expose a PID in its endpoint metadata. This is a
                # controlled-domain correlation with a separately proven live
                # original observer PID; it is not an exact DDS-to-PID binding.
                row.update(marker=marker, reader_gid=gid, actual_helper_pid=helper_pid,
                    identity_scope='new exact node/resource/type/reliability/durability DDS GID plus this launch ARM PID in task-owned domain73',
                    actual_remote_history=fields['history'], actual_remote_depth=fields['depth'],
                    history_depth_scope=history_scope,
                    local_profile_source={'path': str(self.runner.source/'test/0073.ros-tests/support/source/ros2/native_probe.cc'),
                        'sha256': digest(self.runner.source/'test/0073.ros-tests/support/source/ros2/native_probe.cc'),
                        'declaration': 'rclcpp::QoS{4}.reliable()',
                        'scope': 'Pinned original source local construction; no remote history/depth validation is claimed when the API reports unknown0'})
            row['actual_readiness_seconds'] = round(time.monotonic()-started, 6)
            self.row['observers'].append(row)
        self.row['status'] = 'PASS'
        return self.row
