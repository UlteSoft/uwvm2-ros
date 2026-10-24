#!/usr/bin/env python3
"""Run original extended ROS suites through the existing ARM behavior adapter.

Accepts the source/workspace/worker/native-mode arguments of run.py, followed
by original suite options. Generated compiler outputs stay outside the frozen
source tree. Performance numbers describe QEMU on this host, not physical ARM.
"""
import argparse
import ast
import builtins
import importlib
import importlib.util
import inspect
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys
import textwrap
import time

sys.dont_write_bytecode = True
from runtime import Evidence, digest

SUITES = {
    'additional-security': 'test/0073.ros-tests/support/source/ros/run_additional_security.py',
    'ingress-trap': 'test/0073.ros-tests/support/source/ros/run_ingress.py',
    'revoke-race': 'test/0073.ros-tests/support/source/ros/run_revoke_race.py',
    'flood': 'test/0073.ros-tests/support/source/ros/run_flood.py',
    'payload-security': 'test/0073.ros-tests/support/source/ros/run_payload_security.py',
    'five-module': 'test/0073.ros-tests/support/source/ros_multimodule/run.py',
    'performance': 'test/0073.ros-tests/support/source/ros_perf/run.py',
}
PERFORMANCE_SCOPE = ('Measured latency, throughput, CPU time and RSS are QEMU host simulation '
                     'measurements. They are not physical ARM latency or throughput results.')


class ExtendedStartupEnvelope:
    """Intercept only verified original startup call sites in one module."""

    def __init__(self, module, gateways=(), observers=()):
        self.module, self.original_time = module, module.time
        self.previous_range = module.__dict__.get('range')
        self.had_range = 'range' in module.__dict__
        self.rows, self.deadlines, self.observers = [], {}, {}
        for function in gateways:
            lines, first = inspect.getsourcelines(function)
            parsed = ast.parse(textwrap.dedent(''.join(lines)))
            assignments = [node for node in ast.walk(parsed) if isinstance(node, ast.Assign)
                and any(isinstance(target, ast.Name) and target.id == 'deadline' for target in node.targets)
                and isinstance(node.value, ast.BinOp) and isinstance(node.value.op, ast.Add)
                and isinstance(node.value.left, ast.Call) and isinstance(node.value.left.func, ast.Attribute)
                and isinstance(node.value.left.func.value, ast.Name) and node.value.left.func.value.id == 'time'
                and node.value.left.func.attr == 'monotonic' and isinstance(node.value.right, ast.Constant)
                and node.value.right.value == 4]
            loops = [node for node in ast.walk(parsed) if isinstance(node, ast.While)
                     and any(isinstance(part, ast.Attribute) and part.attr == 'exists'
                             for part in ast.walk(node.test))]
            if len(assignments) != 1 or not any(loop.lineno == assignments[0].lineno+1 for loop in loops):
                raise RuntimeError('unrecognized original Gateway startup deadline: '+function.__qualname__)
            self.deadlines[(function.__code__, first+assignments[0].lineno-1)] = function.__qualname__
        for function in observers:
            self.observers[function.__code__] = function.__qualname__

    def observe(self, frame):
        if frame is None:
            return
        gateway = frame.f_locals.get('gateway')
        if gateway is None:
            return
        for row in self.rows:
            if row['kind'] != 'gateway' or row['pid'] != gateway.pid or row.get('readiness_observed'):
                continue
            if Path(row['socket']).exists():
                row.update(readiness_observed=True,
                           actual_seconds=round(self.original_time.monotonic()-row['_started'], 6),
                           process_returncode=gateway.poll())

    def observe_callers(self, frame):
        codes = {code for code, line in self.deadlines}
        while frame is not None:
            if frame.f_code in codes:
                self.observe(frame)
                return
            frame = frame.f_back

    def install(self):
        owner = self

        class Clock:
            def __getattr__(self, name):
                return getattr(owner.original_time, name)

            def monotonic(self):
                frame = sys._getframe(1)
                owner.observe(frame)
                real = owner.original_time.monotonic()
                method = owner.deadlines.get((frame.f_code, frame.f_lineno))
                if method is None:
                    return real
                gateway = frame.f_locals['gateway']
                if not hasattr(gateway, 'pid') or not callable(gateway.poll):
                    raise RuntimeError('startup deadline has no actual Gateway process')
                local = frame.f_locals
                socket_path = local.get('socket_path') or local['directory']/f"ros{local['version']}.sock"
                started = getattr(gateway, '_uwvm_arm_launch_time', real)
                owner.rows.append({'kind': 'gateway', 'method': method,
                    'source': frame.f_code.co_filename, 'source_line': frame.f_lineno,
                    'pid': gateway.pid, 'socket': str(socket_path),
                    'old_nominal_seconds': 4, 'qemu_max_seconds': 12,
                    'readiness_observed': False, '_started': started,
                    'scope': 'Outer socket startup only; later native readiness, '
                             'operation, race and side effect deadlines unchanged'})
                # The original assignment adds4. Only that exact call site gets
                # an offset; all subsequent monotonic observations are real.
                return started+12-4

        def observer_range(*arguments):
            frame = sys._getframe(1)
            method = owner.observers.get(frame.f_code)
            if arguments != (200,) or method is None:
                return builtins.range(*arguments)
            process, name = frame.f_locals['proc'], frame.f_locals['name']
            log = frame.f_locals['self'].run/(name+'.log')
            started = getattr(process, '_uwvm_arm_launch_time', owner.original_time.monotonic())
            row = {'kind': 'observer', 'method': method, 'pid': process.pid, 'log': str(log),
                'old_iterations': 200, 'qemu_iterations': 1200, 'old_nominal_seconds': 2,
                'qemu_max_seconds': 12, 'iterations': 0,
                'scope': 'Outer READY startup only; original10second data collection '
                         'and15second process wait remain unchanged'}
            owner.rows.append(row)

            def iterations():
                try:
                    for count in builtins.range(1200):
                        if owner.original_time.monotonic()-started >= 12:
                            row['deadline_reached'] = True
                            break
                        row['iterations'] = count+1
                        yield count
                finally:
                    row.update(actual_seconds=round(owner.original_time.monotonic()-started, 6),
                               readiness_observed=log.is_file() and b'READY' in log.read_bytes(),
                               process_returncode=process.poll())

            return iterations()

        self.module.time = Clock()
        self.module.range = observer_range

    def finish(self):
        now = self.original_time.monotonic()
        for row in self.rows:
            started = row.pop('_started', None)
            if started is not None and 'actual_seconds' not in row:
                row['actual_seconds'] = round(now-started, 6)
                row['startup_did_not_complete'] = True
        return self.rows

    def restore(self):
        self.module.time = self.original_time
        if self.had_range:
            self.module.range = self.previous_range
        else:
            self.module.__dict__.pop('range', None)


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    p = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    p.add_argument('--suite', choices=tuple(SUITES), required=True)
    p.add_argument('--config', type=Path, help='Frozen cross compiler config; required for ingress/revoke helpers')
    p.add_argument('--manifest', type=Path, help='Defaults to source parent/source-manifest.json')
    p.add_argument('--source-addon', type=Path, help='Separately frozen generic fixture inputs omitted from the primary source freeze')
    p.add_argument('--source-addon-manifest', type=Path)
    p.add_argument('--expected-product-sha256', help='Optional selected FiveRunner Worker SHA')
    p.add_argument('--cache-sdk-environment', action='store_true',
                   help='Performance startup only: capture and reuse the exact real SDK setup environment')
    own, remaining = p.parse_known_args()
    saved_argv = sys.argv
    adapter = load('arm_extended_base_adapter', Path(__file__).with_name('run.py'))
    sys.argv = [str(Path(__file__).with_name('run.py')), '--suite', 'network', *remaining]
    args, original_options = adapter.parse()
    source = args.source.resolve(strict=True)
    manifest_path = own.manifest or source.parent/'source-manifest.json'
    manifest = json.loads(manifest_path.read_text())
    manifest_sha = digest(manifest_path)
    if bool(own.source_addon) != bool(own.source_addon_manifest):
        p.error('--source-addon and --source-addon-manifest must be supplied together')
    addon = own.source_addon.resolve(strict=True) if own.source_addon else None
    addon_manifest = json.loads(own.source_addon_manifest.read_text()) if addon else {}
    addon_sha = digest(own.source_addon_manifest) if addon else None
    config = json.loads(own.config.read_text()) if own.config else None
    if own.suite in ('ingress-trap', 'revoke-race') and config is None:
        p.error('--config is required for ARM native helper compilation')
    if own.cache_sdk_environment and (own.suite != 'performance' or args.native_mode != 'binfmt'):
        p.error('--cache-sdk-environment is restricted to performance with verified binfmt execution')
    prefix = json.loads(args.worker_prefix_json)
    if not prefix:
        p.error('extended Worker execution requires a non-empty --worker-prefix-json')
    sys.path.insert(0, str(source/'test/0073.ros-tests/support/source/ros'))
    network = importlib.import_module('run_network')
    selected = load('arm_original_extended_'+own.suite.replace('-', '_'), source/SUITES[own.suite])
    readiness = None
    if own.suite == 'ingress-trap':
        readiness = ExtendedStartupEnvelope(selected, gateways=(selected.IngressRunner.trap,))
    elif own.suite == 'revoke-race':
        readiness = ExtendedStartupEnvelope(selected, gateways=(selected.RaceRunner.race, selected.RaceRunner.dual_revoke))
    elif own.suite == 'performance':
        readiness = ExtendedStartupEnvelope(selected, observers=(selected.Runner.ready,))
    if readiness:
        readiness.install()
    original_popen = subprocess.Popen
    original_snapshot = network.Runner.snapshot
    original_network_main = network.main
    instances, launches, compiles, fixtures, runners = [], [], [], {}, []
    environment_caches = []
    five_startup = []
    failures = []
    saved_parse, saved_evidence = adapter.parse, adapter.Evidence

    def checked(relative):
        expected = manifest[relative] if relative in manifest else addon_manifest[relative]
        expected_sha = expected['sha256'] if isinstance(expected, dict) else expected
        path = (source if relative in manifest else addon)/relative
        if digest(path) != expected_sha or isinstance(expected, dict) and path.stat().st_size != expected['bytes']:
            raise RuntimeError('FAIL frozen source changed: '+relative)
        return expected_sha

    def actual_input(relative):
        return (source if relative in manifest else addon)/relative

    def dependency_name(path):
        for base in (source, addon):
            if base is not None:
                try:
                    return str(path.relative_to(base))
                except ValueError:
                    pass
        raise ValueError('non-project MMD input: '+str(path))

    def latest():
        if not instances:
            raise RuntimeError('FAIL ARM launch before evidence initialization')
        return instances[-1]

    def snapshot(self, input_path, destination, label):
        if self not in runners:
            runners.append(self)
        resolved = Path(input_path).resolve()
        actual = fixtures.get(str(resolved), {}).get('output', str(resolved))
        original_snapshot(self, actual, destination, label)
        with Path(destination).open('rb') as stream:
            is_elf = stream.read(4) == b'\x7fELF'
        if is_elf:
            latest().target(label, destination)

    def matching(command):
        wanted = set()
        tokens = {str(token) for token in command}
        for label, target in latest().targets.items():
            # Strict ingress intentionally rejects DL before the plugin maps.
            if label.startswith(('input-', 'sdk-cache')) or label == 'trusted-dl-fixture':
                continue
            path = Path(target['path'])
            if str(path) in tokens or any(Path(token).name == path.name for token in tokens):
                wanted.add(label)
        return sorted(wanted)

    def factory(command, *positional, **keyword):
        if readiness:
            readiness.observe_callers(sys._getframe(1))
        if not isinstance(command, (list, tuple)):
            return original_popen(command, *positional, **keyword)
        argv = [str(item) for item in command]
        original = list(argv)
        for index, token in enumerate(argv):
            for relative in addon_manifest:
                if token == str(source/relative):
                    checked(relative)
                    argv[index] = str(actual_input(relative))
        if instances and '-o' in argv:
            output_index = argv.index('-o')+1
            output = Path(argv[output_index]).resolve()
            try:
                relative = str(output.relative_to(source))
            except ValueError:
                relative = None
            if relative is not None:
                if output.suffix != '.wasm' or relative not in manifest:
                    raise RuntimeError('FAIL original suite attempted to write frozen source: '+str(output))
                owned = latest().out/'generated-fixtures'/relative
                owned.parent.mkdir(parents=True, exist_ok=True)
                argv[output_index] = str(owned)
                fixtures[str(output)] = {'output': str(owned), 'original_output_sha256': checked(relative),
                                         'command': argv, 'original_command': original}
        helper = next((relative for relative in ('examples/0002.dl/regdl.c',
                       'test/0073.ros-tests/support/source/ros_ingress/revoke_bridge.cpp') if str(source/relative) in original), None)
        if helper is not None:
            if config is None:
                raise RuntimeError('BLOCKED missing frozen ARM cross compiler configuration')
            checked(helper)
            rest = [item for item in argv[1:] if not item.startswith('-stdlib=')]
            dep = latest().out/('helper-'+str(len(compiles))+'.d')
            argv = [*config['compiler'], *config['compile_flags'],
                    *(['-x', 'c'] if helper.endswith('.c') else []),
                    '-MMD', '-MF', str(dep), *rest]
            env = dict(os.environ if keyword.get('env') is None else keyword['env'])
            env.update(config.get('environment', {}))
            keyword['env'] = env
            compiles.append({'original_source': helper, 'source_sha256': checked(helper),
                             'command': argv, 'original_command': original, 'dependency_file': str(dep)})
        required = matching(argv) if instances and helper is None else []
        if required:
            for label in required:
                if label not in ('uwvm', 'native-revoke-bridge'):
                    continue
                artifact = latest().targets[label]['path']
                if artifact in argv and prefix[0] not in argv:
                    at = argv.index(artifact)
                    argv[at:at] = prefix
            cache_launch = None
            for cache in environment_caches:
                argv, keyword, cache_launch = cache.transform(argv, keyword)
                if cache_launch is not None:
                    break
            name = 'extended-launch-'+str(len(launches))+'/'
            frame = inspect.currentframe().f_back
            try:
                while frame is not None:
                    local = frame.f_locals
                    if frame.f_code.co_name in ('start', 'start_input') and isinstance(local.get('self'), network.Runner) and isinstance(local.get('name'), str):
                        # Stable Evidence deduplicates process snapshots without
                        # a name in its key. Use the original Runner name so its
                        # later case check sees the same captured ARM mapping.
                        name = local['name']
                        break
                    frame = frame.f_back
            finally:
                del frame
            process = original_popen(argv, *positional, **keyword)
            row = {'name': name, 'pid': process.pid, 'required': required,
                   'original_command': original, 'command': argv}
            if cache_launch is not None:
                cache_launch['pid'] = process.pid
                row['sdk_environment_cache'] = cache_launch
            launches.append(row)
            latest().start(process, name, argv)
            if cache_launch is not None:
                cache.observe_launch(process, cache_launch, name, required)
            return process
        process = original_popen(argv, *positional, **keyword)
        if helper is not None:
            compiles[-1]['pid'] = process.pid
            latest().start(process, 'extended-helper-compile-'+str(len(compiles))+'/', argv)
        return process

    class ExtendedEvidence(Evidence):
        def __init__(self, *positional, **keyword):
            super().__init__(*positional, **keyword)
            instances.append(self)

        def finish(self, status, extra=None):
            for cache in environment_caches:
                try:
                    cache.check()
                except RuntimeError as error:
                    status = 'FAIL'
                    failures.append(str(error))
            for launch in launches:
                try:
                    self.check(launch['required'], launch['name'])
                    observed = {label for sample in self.samples.values()
                                if sample['name'] == launch['name'] and
                                sample['ancestry'][0] == launch['pid']
                                for label in sample['mapped_arm_targets']}
                    missing = sorted(set(launch['required'])-observed)
                    if missing:
                        raise RuntimeError('missing ARM mapping in actual launch PID subtree '
                                           +str(launch['pid'])+': '+','.join(missing))
                except RuntimeError as error:
                    status = 'FAIL'
                    failures.append(str(error))
            if status == 'PASS' and not launches:
                status = 'FAIL'
                failures.append('no actual ARM executable launches were captured')
            for compile in compiles:
                try:
                    dep = Path(compile['dependency_file'])
                    text = dep.read_text().replace('\\\n', ' ')
                    inputs = {}
                    for token in shlex.split(text.split(':', 1)[1]):
                        path = Path(token).resolve(strict=True)
                        relative = dependency_name(path)
                        inputs[relative] = checked(relative)
                    if compile['original_source'] not in inputs:
                        raise RuntimeError('native helper MMD missing original source')
                    compile['dependencies'] = {'path': str(dep), 'sha256': digest(dep), 'inputs': inputs}
                except (OSError, ValueError, KeyError, RuntimeError) as error:
                    status = 'FAIL'
                    failures.append('native helper dependency freeze failed: '+str(error))
            try:
                for relative in manifest:
                    checked(relative)
                for relative in addon_manifest:
                    checked(relative)
                if digest(manifest_path) != manifest_sha:
                    raise RuntimeError('frozen source manifest changed')
                if addon and digest(own.source_addon_manifest) != addon_sha:
                    raise RuntimeError('frozen source addon manifest changed')
            except (OSError, KeyError, RuntimeError) as error:
                status = 'FAIL'
                failures.append(str(error))
            reports = []
            for runner in runners:
                for filename in ('report.json', 'flood.json', 'payload-security.json', 'performance.json'):
                    path = runner.run/filename
                    if path.is_file():
                        reports.append({'path': str(path), 'sha256': digest(path),
                                        'status': json.loads(path.read_text()).get('status')})
            data = dict(extra or {})
            data.update(extended_suite=own.suite, original_suite=SUITES[own.suite],
                        original_suite_sha256=checked(SUITES[own.suite]),
                        source_manifest_sha256=manifest_sha,
                        source_addon=str(addon) if addon else None,
                        source_addon_manifest_sha256=addon_sha,
                        extended_driver_sha256=digest(__file__), extended_launches=launches,
                        extended_mapping_failures=failures, native_helper_compiles=compiles,
                        generated_fixtures=list(fixtures.values()), original_reports=reports,
                        original_runner_results=[row for runner in runners for row in runner.results],
                        performance_scope=PERFORMANCE_SCOPE if own.suite == 'performance' else None)
            if own.suite == 'five-module':
                data['five_master_startup'] = five_startup
            if readiness:
                data['extended_startup_envelopes'] = readiness.finish()
            if own.suite == 'performance':
                measured = [row for row in reports if Path(row['path']).name == 'performance.json']
                annotated = self.out/'performance-simulation.json'
                annotated.write_text(json.dumps({'measurement_scope': PERFORMANCE_SCOPE,
                    'architecture_evidence': str(self.out/'architecture-evidence.json'),
                    'raw_reports': [{**row, 'result': json.loads(Path(row['path']).read_text())}
                                    for row in measured]}, indent=2)+'\n')
                data['performance_simulation_report'] = {'path': str(annotated), 'sha256': digest(annotated)}
                if own.cache_sdk_environment:
                    data['sdk_environment_cache'] = [cache.report() for cache in environment_caches]
            if own.config:
                data['cross_config_sha256'] = digest(own.config)
            return super().finish(status, data)

    saved_five = None
    saved_performance_prepare = None
    if own.cache_sdk_environment:
        from sdk_env_cache import SdkEnvironmentCache
        saved_performance_prepare = selected.Runner.__dict__.get('prepare')

        def performance_prepare(self, versions):
            result = network.Runner.prepare(self, versions)
            cache = SdkEnvironmentCache(self, latest(), original_popen)
            environment_caches.append(cache)
            for version in versions:
                cache.capture(version)
            return result

        selected.Runner.prepare = performance_prepare
    saved_five_range, had_five_range = None, False
    if own.suite == 'five-module':
        saved_five = selected.FiveRunner.prepare_five
        had_five_range = 'range' in selected.__dict__
        saved_five_range = selected.__dict__.get('range')

        def five_range(*arguments):
            frame = sys._getframe(1)
            if arguments != (200,) or frame.f_code != saved_five.__code__:
                return builtins.range(*arguments)
            master = frame.f_locals['master']
            started = getattr(master, '_uwvm_arm_launch_time', time.monotonic())
            row = {'kind': 'ros1-master', 'port': 11379, 'pid': master.pid,
                   'old_iterations': 200, 'qemu_iterations': 1200,
                   'old_nominal_seconds': 5, 'qemu_max_seconds': 30,
                   'poll_interval_seconds': .025, 'iterations': 0,
                   'readiness_observed': False,
                   'scope': 'FiveRunner outer Master startup only; original Five protocol, '
                            'native probe and CLI timeouts remain unchanged'}
            five_startup.append(row)

            def iterations():
                try:
                    for count in builtins.range(1200):
                        if time.monotonic()-started >= 30:
                            row['deadline_reached'] = True
                            break
                        row['iterations'] = count+1
                        yield count
                finally:
                    row['actual_seconds'] = round(time.monotonic()-started, 6)
                    row['process_returncode'] = master.poll()

            return iterations()

        selected.range = five_range

        def prepare_five(self):
            if args.ros1_rootfs:
                self.roots[1] = args.ros1_rootfs.resolve()
            if args.ros2_rootfs:
                self.roots[2] = args.ros2_rootfs.resolve()
            current = latest()
            python = (self.roots[1]/'usr/bin/python3').resolve(strict=True)
            target = current.target('ros1:python', python)
            first_launch = len(current.processes)
            result = saved_five(self)
            master = next(row for row in reversed(current.processes[first_launch:])
                          if row['name'] == 'roscore')
            pid = master['process'].pid
            current.check(['ros1:python'], 'roscore')
            captured = [sample['pid'] for sample in current.samples.values()
                        if sample['name'] == 'roscore' and sample['ancestry'][0] == pid
                        and 'ros1:python' in sample['mapped_arm_targets']]
            if not captured:
                raise RuntimeError('FAIL missing ARM Five Master Python mapping before Five execution')
            row = next(row for row in reversed(five_startup) if row['pid'] == pid)
            row.update(readiness_observed=True, positive_arm_python_mapping=True,
                       python_target=target, captured_process_pids=sorted(set(captured)),
                       evidence_phase='prepare_five returned before Five protocol execution')
            return result

        selected.FiveRunner.prepare_five = prepare_five
        selected.cgroup_snapshot = lambda: network.cgroup_snapshot()

    def dispatch():
        sys.argv = [str(source/SUITES[own.suite]), *sys.argv[1:]]
        if own.suite in ('flood', 'payload-security', 'performance'):
            sys.argv += ['--wat2wasm', args.wat2wasm]
        if own.suite == 'five-module':
            expected = own.expected_product_sha256 or digest(args.uwvm)
            sys.argv += ['--expected-product-sha256', expected]
        return selected.main()

    try:
        for relative in manifest:
            checked(relative)
        for relative in addon_manifest:
            checked(relative)
        adapter.parse = lambda: (args, original_options)
        adapter.Evidence = ExtendedEvidence
        network.Runner.snapshot = snapshot
        network.main = dispatch
        subprocess.Popen = factory
        return adapter.main()
    finally:
        if readiness:
            readiness.restore()
        subprocess.Popen = original_popen
        network.Runner.snapshot = original_snapshot
        network.main = original_network_main
        adapter.parse, adapter.Evidence = saved_parse, saved_evidence
        if saved_five:
            selected.FiveRunner.prepare_five = saved_five
            if had_five_range:
                selected.range = saved_five_range
            else:
                selected.__dict__.pop('range', None)
        if own.cache_sdk_environment:
            if saved_performance_prepare is None:
                del selected.Runner.prepare
            else:
                selected.Runner.prepare = saved_performance_prepare
        sys.argv = saved_argv


if __name__ == '__main__':
    raise SystemExit(main())
