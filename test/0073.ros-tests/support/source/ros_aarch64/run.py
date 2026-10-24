#!/usr/bin/env python3
"""Adapt frozen ROS network/RPC witnesses to ARM Workers and ARM Gateways.

Original runners and production sources are never edited. The rootfs must
already contain ARM native targets and working binfmt, or provide explicit
QEMU plus a captured ROS environment. Execution evidence is kept separately.
"""
import argparse
import builtins
import importlib
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest
from generate_fixtures import generate
from artifacts import Pool
from topic_gate import DiscoveryGate, OriginalObserverReady, arm_artifact, load_observers


class StartupEnvelope:
    """Extend only identified startup polling loops for QEMU execution."""

    def __init__(self, module, sites):
        self.module, self.sites = module, sites
        self.rows = []
        self.previous = module.__dict__.get('range')
        self.had_range = 'range' in module.__dict__

    def install(self):
        owner = self

        def startup_range(*arguments):
            frame = sys._getframe(1)
            site = owner.sites.get(frame.f_code)
            if arguments != (200,) or site is None:
                return builtins.range(*arguments)
            kind, seconds, interval = site
            process = frame.f_locals.get('master' if kind == 'ros1-master' else 'gateway')
            started = getattr(process, '_uwvm_arm_launch_time', time.monotonic())
            row = {'kind': kind, 'pid': process.pid if process is not None else None,
                   'old_iterations': 200, 'qemu_iterations': 1200,
                   'old_nominal_seconds': 200*interval, 'qemu_max_seconds': seconds,
                   'poll_interval_seconds': interval, 'iterations': 0,
                   'scope': 'Outer startup polling only; original protocol, operation, '
                            'native probe and CLI timeouts remain unchanged'}
            owner.rows.append(row)
            gateway_socket = (frame.f_locals['directory']/f"ros{frame.f_locals['version']}.sock"
                              if kind == 'gateway' else None)

            def iterations():
                try:
                    for count in builtins.range(1200):
                        if time.monotonic()-started >= seconds:
                            row['deadline_reached'] = True
                            break
                        row['iterations'] = count+1
                        yield count
                finally:
                    row['actual_seconds'] = round(time.monotonic()-started, 6)
                    row['process_returncode'] = process.poll() if process is not None else None
                    if gateway_socket is not None:
                        row['readiness_observed'] = gateway_socket.exists()
                        row['socket'] = str(gateway_socket)
                    else:
                        # This is an observation after the original TCP test;
                        # it does not substitute for that test or its failure.
                        import socket
                        port = 11359 if owner.module.__name__ == 'original_ros_native' else 11339
                        try:
                            with socket.create_connection(('127.0.0.1', port), .1):
                                row['readiness_observed'] = True
                        except OSError:
                            row['readiness_observed'] = False

            return iterations()

        self.module.range = startup_range

    def restore(self):
        if self.had_range:
            self.module.range = self.previous
        else:
            self.module.__dict__.pop('range', None)


class RpcGeneratedOutputs:
    """Keep original RPC compilation metadata while protecting its source tree."""

    def __init__(self, source, out):
        self.source = Path(source).resolve()
        self.root = Path(out).resolve()/'rpc-generated'
        if self.root.is_relative_to(self.source):
            raise RuntimeError('RPC generated output must be outside frozen source')
        self.source_before = {str(path.relative_to(self.source)): digest(path)
                              for path in self.source.rglob('*') if path.is_file()}
        self.outputs = {}
        self.old_bytes = Path.write_bytes
        self.old_text = Path.write_text
        self.old_snapshot = None
        self.bound = []

    def source_path(self, path):
        lexical = Path(path).absolute()
        resolved = lexical.resolve()
        if lexical.is_relative_to(self.source) and not resolved.is_relative_to(self.source):
            raise RuntimeError('frozen source write follows an escaping symlink: '+str(path))
        return resolved if resolved.is_relative_to(self.source) else None

    def install(self, network):
        self.network = network
        self.old_snapshot = network.Runner.snapshot
        owner = self

        def write_bytes(path, data):
            logical = owner.source_path(path)
            if logical is None:
                return owner.old_bytes(path, data)
            if logical.suffix != '.wasm':
                raise RuntimeError('write to frozen non-Wasm source refused: '+str(logical))
            actual = owner.root/logical.relative_to(owner.source)
            actual.parent.mkdir(parents=True, exist_ok=True)
            prior = owner.outputs.get(str(logical))
            original_sha = prior['original_source_sha256'] if prior else digest(logical) if logical.is_file() else None
            count = owner.old_bytes(actual, data)
            owner.outputs[str(logical)] = {'logical_source': str(logical), 'actual_output': str(actual),
                'original_source_sha256': original_sha, 'compiled_output_sha256': digest(actual),
                'bytes': actual.stat().st_size}
            return count

        def write_text(path, *positional, **keyword):
            if owner.source_path(path) is not None:
                raise RuntimeError('text write to frozen source refused: '+str(path))
            return owner.old_text(path, *positional, **keyword)

        def snapshot(receiver, input_path, destination, label):
            logical = str(Path(input_path).resolve())
            generated = owner.outputs.get(logical)
            owner.old_snapshot(receiver, generated['actual_output'] if generated else input_path,
                               destination, label)
            if generated:
                receiver.artifacts[label].update(logical_source=logical,
                    actual_compiled_source=generated['actual_output'],
                    compiled_output_sha256=generated['compiled_output_sha256'])

        Path.write_bytes, Path.write_text = write_bytes, write_text
        network.Runner.snapshot = snapshot

    def bind_rpc(self, module):
        for method, rows_name, filename in (
            ('compile_arrays', 'array_compilations', 'array-fixture-compilations.json'),
            ('compile_scalars', 'scalar_compilations', 'scalar-fixture-compilations.json'),
            ('compile_typed_events', 'typed_event_compilations', 'typed-event-fixture-compilations.json')):
            original = getattr(module.RpcRunner, method)

            def compile(receiver, *positional, _original=original, _rows=rows_name,
                        _filename=filename, **keyword):
                result = _original(receiver, *positional, **keyword)
                rows = getattr(receiver, _rows)
                for row in rows:
                    logical = row.get('binary')
                    generated = self.outputs.get(str(Path(logical).resolve())) if logical else None
                    if generated:
                        if row['binary_sha256'] != generated['compiled_output_sha256'] or row['bytes'] != generated['bytes']:
                            raise RuntimeError('RPC compiler metadata differs from actual generated output')
                        row.update(logical_binary=logical, binary=generated['actual_output'],
                                   original_source_sha256=generated['original_source_sha256'])
                (receiver.run/_filename).write_text(json.dumps(rows, indent=2)+'\n')
                return result

            self.bound.append((module.RpcRunner, method, original))
            setattr(module.RpcRunner, method, compile)

    def restore(self):
        Path.write_bytes, Path.write_text = self.old_bytes, self.old_text
        if self.old_snapshot is not None:
            self.network.Runner.snapshot = self.old_snapshot
        for cls, method, original in self.bound:
            setattr(cls, method, original)

    def evidence(self):
        current = {str(path.relative_to(self.source)): digest(path)
                   for path in self.source.rglob('*') if path.is_file()}
        if current != self.source_before:
            raise RuntimeError('RPC frozen source file inventory or bytes changed')
        for logical, row in self.outputs.items():
            original = Path(logical)
            if (digest(original) if original.is_file() else None) != row['original_source_sha256']:
                raise RuntimeError('frozen RPC source Wasm changed: '+logical)
            if digest(row['actual_output']) != row['compiled_output_sha256']:
                raise RuntimeError('owned RPC generated output changed: '+row['actual_output'])
        return {'scope': 'Original RPC WAT compilation outputs redirected outside frozen source; '
                         'snapshot and original compilation rows reference actual owned bytes',
                'root': str(self.root), 'outputs': list(self.outputs.values())}


def parse():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--suite', choices=('network', 'rpc', 'multi-memory'), required=True)
    p.add_argument('--source', type=Path, required=True, help='Frozen current source tree')
    p.add_argument('--workspace', type=Path, required=True, help='Prepared ARM gateway rootfs workspace')
    p.add_argument('--uwvm', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True, help='Fresh architecture evidence directory')
    p.add_argument('--worker-prefix-json', default='[]',
                   help='JSON argv before ARM Worker, e.g. ["/usr/bin/qemu-aarch64","-L","/arm/sysroot"]')
    p.add_argument('--native-mode', choices=('binfmt', 'explicit'), default='binfmt')
    p.add_argument('--native-qemu-inside', default='/usr/bin/qemu-aarch64-static')
    p.add_argument('--native-env-json', type=Path,
                   help='For explicit mode: JSON {"1":{ROS env...},"2":{ROS env...}} captured from each ARM SDK')
    p.add_argument('--binfmt-evidence', type=Path, help='Provided active temporary binfmt lease JSON; this runner never registers a handler')
    p.add_argument('--artifact-pool', type=Path, help='Verified separate frozen ARM pool; snapshots use hardlinks to conserve bounded disk space')
    p.add_argument('--ros1-rootfs', type=Path)
    p.add_argument('--ros2-rootfs', type=Path)
    p.add_argument('--llvm-libs', default='')
    p.add_argument('--wat2wasm', default='wat2wasm')
    p.add_argument('--topic-observers-manifest', type=Path,
                   help='Prepared test-owned ARM ROS1/2 discovery observers for multi-memory')
    p.add_argument('--cache-native-sdk-environment', action='store_true',
                   help='Network startup only: reuse an environment captured from the actual SDK setup shell')
    args, remaining = p.parse_known_args()
    if args.native_mode == 'explicit' and args.native_env_json is None:
        p.error('--native-mode explicit requires --native-env-json')
    if args.cache_native_sdk_environment and (args.suite != 'network' or args.native_mode != 'binfmt'):
        p.error('--cache-native-sdk-environment is restricted to network with verified binfmt execution')
    for flag in ('--source', '--workspace', '--uwvm', '--out', '--llvm-libs', '--wat2wasm'):
        if flag in remaining:
            p.error('runner overrides must be provided as ARM adapter arguments: ' + flag)
    return args, remaining


def main():
    args, remaining = parse()
    # Import test-owned helpers before the original suite changes sys.path.
    cache_type = None
    if args.cache_native_sdk_environment:
        from sdk_env_cache import SdkEnvironmentCache
        cache_type = SdkEnvironmentCache
    source, workspace, worker = args.source.resolve(), args.workspace.resolve(), args.uwvm.resolve()
    prefix = json.loads(args.worker_prefix_json)
    if not isinstance(prefix, list) or not all(isinstance(item, str) for item in prefix):
        raise ValueError('--worker-prefix-json must contain an argv string array')
    native_env = json.loads(args.native_env_json.read_text()) if args.native_env_json else {}
    if not isinstance(native_env, dict):
        raise ValueError('--native-env-json must contain a version-keyed object')
    binfmt = None
    if args.native_mode == 'binfmt':
        if args.binfmt_evidence is None:
            raise RuntimeError('BLOCKED binfmt mode requires --binfmt-evidence from the verified temporary lease')
        provided = json.loads(args.binfmt_evidence.read_text())
        nested_entry = provided.get('entry')
        name = provided.get('entry_name') or provided.get('name') or (nested_entry.get('name') if isinstance(nested_entry, dict) else None)
        if not isinstance(name, str) or '/' in name or name in ('', '.', '..'):
            raise RuntimeError('BLOCKED provided binfmt lease has no valid entry_name')
        entry = Path('/proc/sys/fs/binfmt_misc')/name
        current = entry.read_text()
        fields = dict(line.split(None, 1) for line in current.splitlines()[1:] if ' ' in line)
        magic, mask = bytes.fromhex(fields.get('magic', '')), bytes.fromhex(fields.get('mask', ''))
        if not current.startswith('enabled\n') or 'F' not in fields.get('flags:', '') or len(magic) < 20 or len(mask) < 20 or magic[18:20] != b'\xb7\x00' or mask[18:20] != b'\xff\xff':
            raise RuntimeError('BLOCKED provided binfmt lease is not an enabled fixed-interpreter ELF183 handler')
        binfmt = {'provided_path': str(args.binfmt_evidence.resolve()),
                  'provided_sha256': digest(args.binfmt_evidence), 'provided': provided,
                  'entry_name': name, 'active_entry_content': current}
    sys.path.insert(0, str(source/'test/0073.ros-tests/support/source/ros_cgroup'))
    from probe import probe
    sys.path.insert(0, str(source/'test/0073.ros-tests/support/source/ros'))
    network = importlib.import_module('run_network')
    evidence = Evidence(args.out.resolve(), probe)
    pool = Pool(args.artifact_pool) if args.artifact_pool else None
    if pool:
        pool.install()
    if prefix:
        evidence.emulator(prefix[0])
    if binfmt:
        emulator_path = fields.get('interpreter')
        if not emulator_path:
            raise RuntimeError('BLOCKED provided binfmt handler has no interpreter')
        actual_emulator = evidence.emulator(emulator_path)
        supplied_sha = provided.get('qemu_sha256')
        if supplied_sha is not None and actual_emulator['sha256'] != supplied_sha:
            raise RuntimeError('BLOCKED provided binfmt emulator SHA does not match active handler')
        binfmt['actual_emulator'] = actual_emulator
    evidence.target('input-worker', worker)
    original_start, original_prepare = network.Runner.start, network.Runner.prepare
    original_snapshot = network.Runner.snapshot
    original_native, original_case = network.Runner.native_command, network.Runner.case
    startup = StartupEnvelope(network, {
        original_prepare.__code__: ('ros1-master', 30, .025),
        original_case.__code__: ('gateway', 12, .01)})
    startup.install()
    launch_requirements = []
    prepared_runs = []
    discovery_rows = []
    observer_binding = None
    observers = None
    active_gate = None
    processes_by_name = {}
    seen_reader_gids = set()
    original_observer_readiness = []
    environment_caches = {}
    rpc_outputs = RpcGeneratedOutputs(source, evidence.out) if args.suite == 'rpc' else None
    if rpc_outputs:
        rpc_outputs.install(network)
        original_snapshot = network.Runner.snapshot

    def snapshot():
        facts = probe()
        if facts.get('status') != 'PASS':
            raise RuntimeError('BLOCKED inherited 64 GiB cgroup not proven')
        row = facts['cgroup_ancestors'][0]
        return {'path': facts['membership'][0].split(':', 2)[2],
                'limits': [int(x['memory.max']) for x in facts['cgroup_ancestors']
                           if x.get('memory.max') not in (None, 'max')],
                **{key: row.get(key) for key in ('memory.current', 'memory.peak', 'memory.events')}}

    def prepare(self, versions):
        if args.ros1_rootfs:
            self.roots[1] = args.ros1_rootfs.resolve()
        if args.ros2_rootfs:
            self.roots[2] = args.ros2_rootfs.resolve()
        # ARM inventory is collected before the first rootfs process starts.
        for version in versions:
            build = self.roots[version]/f'tmp/ros{version}-build'
            for binary in build.glob(f'uwvm-ros{version}-*'):
                if binary.is_file() and os.access(binary, os.X_OK):
                    evidence.target('input-ros'+str(version)+'/'+binary.name, binary)
            if version == 1:
                evidence.target('ros1:python', (self.roots[1]/'usr/bin/python3').resolve(strict=True))
        original_prepare(self, versions)
        for label, artifact in self.artifacts.items():
            if arm_artifact(label, artifact['snapshot']):
                evidence.target(label, Path(artifact['snapshot']))
        if observers is not None:
            self._arm_topic_observers = {}
            for version in versions:
                basename = f'uwvm-ros{version}-topic-observer'
                destination = self.run/'snapshots'/f'ros{version}'/basename
                self.snapshot(Path(observers['observers'][str(version)]['path']), destination,
                              f'ros{version}/'+basename)
                self._arm_topic_observers[version] = f'/uwvm-ros-test/snapshots/ros{version}/'+basename
        if 1 in versions:
            master = next(row for row in reversed(launch_requirements) if row['name'] == 'roscore')
            evidence.check(['ros1:python'], 'roscore')
            observed = {label for sample in evidence.samples.values()
                        if sample['name'] == 'roscore' and sample['ancestry'][0] == master['pid']
                        for label in sample['mapped_arm_targets']}
            if 'ros1:python' not in observed:
                raise RuntimeError('FAIL missing ARM ROS Master Python mapping before case execution')
        prepared_runs.append(str(self.run))
        if cache_type is not None:
            cache = cache_type(self, evidence, subprocess.Popen)
            environment_caches[id(self)] = cache
            for version in versions:
                cache.capture(version)

    def freeze(self, input_path, destination, label):
        original_snapshot(self, input_path, destination, label)
        if arm_artifact(label, self.artifacts[label]['snapshot']):
            evidence.target(label, Path(self.artifacts[label]['snapshot']))

    def native(self, version, arguments):
        arguments = list(arguments)
        if active_gate is not None and version == active_gate.version and arguments[1:2] == ['--observe']:
            arguments[0] = self._arm_topic_observers[version]
        if args.native_mode == 'binfmt':
            return original_native(self, version, arguments)
        arguments = list(arguments)
        if arguments and arguments[0] in self.frozen:
            arguments[0] = self.frozen[arguments[0]]
        # No emulated shell is used here. ROS setup variables come from a
        # separately captured SDK environment, so nested exec needs binfmt only
        # for native probes that themselves spawn another ARM executable.
        return ['chroot', str(self.roots[version]), args.native_qemu_inside,
                '-L', '/', *arguments]

    def start(self, command, name, env=None):
        command = list(command)
        original = list(command)
        child_env = env
        if self.product in command:
            index = command.index(self.product)
            command[index:index] = prefix
        version = next((v for v in (1, 2) if str(self.roots[v]) in command), None)
        if version is not None and args.native_mode == 'explicit':
            selected = native_env.get(str(version))
            if not isinstance(selected, dict) or not all(isinstance(k, str) and isinstance(v, str)
                                                       for k, v in selected.items()):
                raise RuntimeError('BLOCKED missing captured ROS'+str(version)+' environment')
            child_env = dict(self.env if env is None else env)
            for key in ('CC', 'CXX', 'LD_LIBRARY_PATH', 'LD_PRELOAD', 'QEMU_LD_PREFIX'):
                child_env.pop(key, None)
            child_env.update(selected)
            for key in ('ROS_MASTER_URI', 'ROS_IP', 'ROS_DOMAIN_ID', 'RMW_IMPLEMENTATION', 'ROS_SECURITY_ENABLE'):
                if key in self.env:
                    child_env[key] = self.env[key]
        started = time.monotonic()
        cache = environment_caches.get(id(self))
        cache_launch = None
        cache_keywords = None
        if cache is not None:
            command, cache_keywords, cache_launch = cache.transform(command,
                {'env': self.env if child_env is None else child_env, 'start_new_session': True})
        if cache_launch is not None and cache_launch['status'] == 'CACHED':
            # Preserve the original Runner log, session and ownership behavior;
            # only the repeated SDK setup shell has been removed.
            with (self.run/(name+'.log')).open('wb') as stream:
                process = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT,
                                           **cache_keywords)
            self.processes.append(process)
        elif active_gate is not None and name == active_gate.case+'-uwvm':
            # Only this new fixture reads the owned stdin discovery gate.
            with (self.run/(name+'.log')).open('wb') as stream:
                process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=stream,
                    stderr=subprocess.STDOUT, env=child_env or self.env, start_new_session=True)
            self.processes.append(process)
        else:
            process = original_start(self, command, name, child_env)
        process._uwvm_arm_launch_time = started
        required = []
        if name == 'roscore':
            required.append('ros1:python')
        # Match launch argv to frozen ARM artifacts, never to a Wasm filename.
        for label, target in evidence.targets.items():
            if label.startswith(('input-', 'sdk-cache')):
                continue
            target_name = Path(target['path']).name
            if label == 'uwvm' and self.product in original or label != 'uwvm' and any(
                    Path(token).name == target_name for token in original):
                required.append(label)
        launch = {'name': name, 'pid': process.pid, 'required': required}
        if cache_launch is not None:
            cache_launch.update(name=name, pid=process.pid)
            launch.update(original_command=original, command=command,
                          sdk_environment_cache=cache_launch)
        launch_requirements.append(launch)
        evidence.start(process, name, command)
        if cache_launch is not None:
            cache.observe_launch(process, cache_launch, name, required)
        processes_by_name[name] = process
        if active_gate is not None and name.startswith(active_gate.case+'-'):
            active_gate.register(name, process)
        return process

    def case(self, name, *positional, **keyword):
        selected = getattr(self.args, 'only', None)
        if observers is not None and active_gate is None and (not selected or name in selected):
            witnesses = keyword['witnesses'] if 'witnesses' in keyword else positional[2]
            if any(mode == '--observe' for _, mode, _ in witnesses):
                message_type = keyword.get('message_type', positional[3] if len(positional) > 3 else 'String')
                ready = OriginalObserverReady(self, evidence, name, witnesses, message_type,
                                              processes_by_name, seen_reader_gids)
                original_observer_readiness.append(ready.row)
                previous = keyword.get('pre_resume')

                def before_resume():
                    prior = previous() if previous else None
                    result = ready.before_resume()
                    if prior is not None:
                        result['original_pre_resume_result'] = prior
                    return result

                keyword['pre_resume'] = before_resume
        original_case(self, name, *positional, **keyword)
        selected = getattr(self.args, 'only', None)
        if selected and name not in selected:
            return
        required = {label for launch in launch_requirements
                    if launch['name'].startswith(name+'-') for label in launch['required']}
        if 'uwvm' not in required:
            raise RuntimeError('FAIL no actual ARM Worker launch for '+name)
        evidence.check(required, name)

    network.cgroup_snapshot = snapshot
    network.Runner.prepare = prepare
    network.Runner.snapshot = freeze
    network.Runner.native_command = native
    network.Runner.start = start
    network.Runner.case = case
    code, status, error = 1, 'FAIL', None
    runner = None
    try:
        if args.topic_observers_manifest is not None:
            observers, observer_binding = load_observers(args.topic_observers_manifest, source)
        if args.suite in ('network', 'rpc'):
            module = network if args.suite == 'network' else importlib.import_module('run_rpc')
            if rpc_outputs:
                rpc_outputs.bind_rpc(module)
            sys.argv = [str(Path(module.__file__)), '--workspace', str(workspace),
                        '--source', str(source), '--uwvm', str(worker),
                        '--llvm-libs', args.llvm_libs, *(['--wat2wasm', args.wat2wasm]
                                                       if args.suite == 'rpc' else []), *remaining]
            code = module.main()
            status = 'PASS' if code == 0 else 'BLOCKED' if code in (2, 77) else 'FAIL'
        else:
            if remaining:
                raise ValueError('multi-memory suite does not accept extra original-runner arguments')
            fixture_dir = evidence.out/'fixtures'
            if args.topic_observers_manifest is None:
                raise RuntimeError('BLOCKED multi-memory requires --topic-observers-manifest')
            generate(fixture_dir, args.wat2wasm)
            args.only = None
            runner = network.Runner(args)
            runner.prepare({1, 2})
            for version in (1, 2):
                name = f'ros{version}-multi-memory-publish'
                active_gate = DiscoveryGate(runner, evidence, name, version,
                    f'ros{version}/uwvm-ros{version}-topic-observer')
                discovery_rows.append(active_gate.row)
                runner.case(name, str(fixture_dir/(name+'.wasm')), {version: ['publish']},
                            [(version, '--observe', '/uwvm_ros_test/output')],
                            extra=('--wasm-feature-enable-multi-memory',),
                            pre_resume=active_gate.before_resume)
                active_gate.finish()
                active_gate = None
            status, code = 'PASS', 0
    except BaseException as caught:
        error = traceback.format_exc()
        status = 'BLOCKED' if 'BLOCKED' in str(caught) else 'FAIL'
        code = 77 if status == 'BLOCKED' else 1
    finally:
        if active_gate is not None:
            active_gate.stop.set()
            if active_gate.thread:
                active_gate.thread.join(timeout=1)
                if active_gate.thread.is_alive():
                    status, code = 'FAIL', 1
                    error = (error or '')+'\nFAIL discovery gate thread survived cleanup'
        if runner is not None:
            try:
                runner.cleanup()
            except BaseException:
                status, code = 'FAIL', 1
                error = (error or '')+'\n'+traceback.format_exc()
        if pool:
            pool.restore()
        startup.restore()
        rpc_output_evidence = None
        if rpc_outputs:
            rpc_outputs.restore()
            try:
                rpc_output_evidence = rpc_outputs.evidence()
            except (OSError, RuntimeError):
                status, code = 'FAIL', 1
                error = (error or '')+'\n'+traceback.format_exc()
        for cache in environment_caches.values():
            try:
                cache.check()
            except RuntimeError:
                status, code = 'FAIL', 1
                error = (error or '')+'\n'+traceback.format_exc()
        result = evidence.finish(status, {'suite': args.suite,
            'executed_driver_sha256': digest(__file__),
            'evidence_helper_sha256': digest(Path(__file__).with_name('runtime.py')),
            'source': str(source), 'prepared_runs': prepared_runs,
            'launch_requirements': launch_requirements, 'worker_prefix': prefix,
            'native_mode': args.native_mode, 'binfmt_evidence': binfmt, 'error': error,
            'artifact_pool': pool.evidence() if pool else None,
            'rpc_generated_outputs': rpc_output_evidence,
            'topic_observers_manifest': observer_binding,
            'topic_discovery_gates': discovery_rows,
            'original_observer_readiness': original_observer_readiness,
            'native_sdk_environment_cache': [cache.report() for cache in environment_caches.values()],
            'qemu_startup_envelopes': startup.rows,
            'results': runner.results if runner is not None else []})
        if result['status'] != status:
            code = 1
    print(result['status'], evidence.out/'architecture-evidence.json', flush=True)
    return code


if __name__ == '__main__':
    sys.exit(main())
