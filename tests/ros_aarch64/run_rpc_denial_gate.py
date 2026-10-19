#!/usr/bin/env python3
"""Run original ARM RPC witnesses with test-owned post-denial Guest stdin gates.

Every original denial/sentinel assertion, native binary and deadline remains.
The two derived denial Guests wait after their assertions until the unchanged
native five-second zero-projection witness passes; their original Worker PID
then exits and the Gateway's required PID binding ends normally.
The default preserves the requested original RPC mode; --selection denied runs
only the two original denial descriptions with their derived post-assert gate.
"""
import argparse
import importlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import traceback

sys.dont_write_bytecode = True
from runtime import digest, read_json_binding
from rebuild_affected import source_state
from rpc_denial_gate import PostDenialGate
from gateway_qualification import pins_preserved
from run_all import (workspace_directory_binding, bind_native_witness_logs,
                     preserved_native_witness_logs)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--suite', choices=('rpc',), default='rpc',
                        help='Accept the existing run_all RPC stage interface')
    parser.add_argument('--selection', choices=('denied', 'all'), default='all')
    own, remaining = parser.parse_known_args()
    if os.environ.get('UWVM_ARM_RPC_DENIAL_GATE_NS') != '1':
        return subprocess.run(['unshare', '--mount', '--propagation', 'private', '--',
            sys.executable, '-B', __file__, *sys.argv[1:]],
            env={**os.environ, 'UWVM_ARM_RPC_DENIAL_GATE_NS': '1'}).returncode
    if sys.platform != 'linux' or os.geteuid() != 0:
        raise RuntimeError('root Linux private namespace required')
    # Resolve ARM helper imports before original tests/ros can shadow them.
    spec = importlib.util.spec_from_file_location('rpc_denial_gate_adapter', Path(__file__).with_name('run.py'))
    adapter = importlib.util.module_from_spec(spec);spec.loader.exec_module(adapter)
    sys.argv = [str(Path(__file__).with_name('run.py')), '--suite', 'rpc', *remaining]
    args, extra = adapter.parse()
    if own.selection == 'denied' and extra:
        raise RuntimeError('denied selection sets original --version2 --mode typed-events itself')
    source = args.source.resolve(strict=True)
    manifest = source.parent/'source-manifest.json';frozen = json.loads(manifest.read_text())
    initial = source_state(source, frozen)
    workspace, outer = args.workspace.resolve(strict=True), args.out.resolve()
    outer.mkdir(parents=True, mode=0o700, exist_ok=False)
    report = {'status': 'RUNNING', 'selection': own.selection,
        'scope': 'Original native binary and denial/sentinel assertions; derived post-assert Guest gate retains Worker PID until native observation passes',
        'source': str(source), 'source_manifest_sha256': digest(manifest), 'events': [],
        'workspace_origin':str(workspace), 'native_witness_log_bindings':{},
        'native_witness_logs_preserved':False, 'native_witness_log_binding_document_pins':[],
        'driver_sha256': digest(__file__), 'adapter_sha256': digest(Path(__file__).with_name('run.py')),
        'gate_helper_sha256': digest(Path(__file__).with_name('rpc_denial_gate.py')),
        'original_rpc_sha256': digest(source/'tests/ros/run_rpc.py'),
        'original_network_sha256': digest(source/'tests/ros/run_network.py'),
        'original_native_witness_header_sha256': digest(source/'tests/ros_resources/typed_event_witness_ros2.h')}
    mounts, alias, status, code = [], None, 'FAIL', 1
    sys.path.insert(0, str(source/'tests/ros'))
    network, rpc, descriptions = (importlib.import_module(name) for name in ('run_network', 'run_rpc', 'typed_parameter_events'))
    original_cases = descriptions.cases
    chosen = [row for row in original_cases() if own.selection == 'all' or row['scenario'] in ('permission-denied', 'no-capability')]
    if len(chosen) != (29 if own.selection == 'all' else 2):
        raise RuntimeError('original typed event case inventory changed')
    report['selected_original_typed_cases'] = chosen
    gate = PostDenialGate(network, rpc)
    original_event = gate.original_event

    def save():
        (outer/'report.json').write_text(json.dumps(report, indent=2)+'\n')

    def checked(command):
        subprocess.run([str(value) for value in command], check=True, capture_output=True, timeout=30)

    # The native executable and C++ header remain original. Only the two
    # test-owned post-assert Wasm gates participate in denial synchronization.
    def event(receiver, description):
        row = {'name': description['name'], 'scenario': description['scenario'], 'raw_run': str(receiver.run), 'status': 'RUNNING'}
        report['events'].append(row);save()
        try:
            result = original_event(receiver, description);row['status'] = 'PASS';return result
        except BaseException:
            row['status'] = 'FAIL';row['error'] = traceback.format_exc();raise
        finally:
            row['witness_log'] = str(receiver.run/(description['name']+'-typed-event-witness.log'))
            log = Path(row['witness_log'])
            if log.is_file():row['witness_log_sha256'] = digest(log)
            save()

    gate.original_event = event
    try:
        checked(['mount', '--make-rprivate', '/'])
        for candidate in ('/gp64', '/pg64', '/rg64', '/gr64'):
            if not Path(candidate).exists():
                alias = Path(candidate);alias.mkdir(mode=0o700);break
        if alias is None:
            raise RuntimeError('no fresh short private RPC workspace alias available')
        checked(['mount', '--bind', workspace, alias]);mounts.append(alias)
        report['workspace_alias_binding'] = {'alias':str(alias), 'origin':str(workspace),
            'alias_directory':workspace_directory_binding(alias),
            'origin_directory':workspace_directory_binding(workspace)}
        if any(report['workspace_alias_binding']['alias_directory'][key] !=
               report['workspace_alias_binding']['origin_directory'][key] for key in ('device','inode')):
            raise RuntimeError('private RPC alias is not the exact incoming workspace')
        roots = {1: (args.ros1_rootfs or workspace/'gateway/noetic-rootfs').resolve(),
                 2: (args.ros2_rootfs or workspace/'gateway/jazzy-rootfs').resolve()}
        versions = (2,) if own.selection == 'denied' else (1, 2)
        for version in versions:
            root = roots[version].resolve(strict=True)
            for name in ('proc', 'sys', 'dev', 'work'):(root/name).mkdir(exist_ok=True)
            for name in ('proc', 'sys', 'dev'):
                checked(['mount', '--rbind', Path('/')/name, root/name]);mounts.append(root/name)
                checked(['mount', '--make-rprivate', root/name])
            checked(['mount', '--bind', source, root/'work']);mounts.append(root/'work')
            checked(['mount', '-o', 'remount,bind,ro', root/'work'])
        if own.selection == 'denied':descriptions.cases = lambda: list(chosen)
        gate.install()
        own_args = ['--suite', 'rpc', *remaining]
        changes = [('--workspace', alias), ('--out', outer/'architecture')]
        changes += [('--ros'+str(version)+'-rootfs', roots[version]) for version in versions]
        for flag, value in changes:
            if flag in own_args:own_args[own_args.index(flag)+1] = str(value)
            else:own_args.extend([flag, str(value)])
        if own.selection == 'denied':own_args.extend(['--version', '2', '--mode', 'typed-events'])
        sys.argv = [str(Path(__file__).with_name('run.py')), *own_args]
        code = adapter.main();status = 'PASS' if code == 0 else 'FAIL'
        arch = outer/'architecture/architecture-evidence.json'
        report['adapter_evidence'] = {'path': str(arch), 'sha256': digest(arch)}
        document, document_pin = read_json_binding(arch, limit=64*1024**2)
        if document_pin['sha256'] != report['adapter_evidence']['sha256']:
            raise RuntimeError('actual RPC adapter report changed before witness capture')
        report['native_witness_log_binding_document_pins'].append(document_pin)
        bind_native_witness_logs(document,report['workspace_alias_binding'],report['native_witness_log_bindings'])
        report['selected_original_native_binaries'] = {label: row for label, row in document['arm_targets'].items()
            if label in ('ros1/uwvm-ros1-resources-probe', 'ros2/uwvm-ros2-resources-probe')}
        mode = extra[extra.index('--mode')+1] if '--mode' in extra else 'all'
        version = extra[extra.index('--version')+1] if '--version' in extra else 'both'
        if status == 'PASS' and (own.selection == 'denied' or
                (version != '1' and mode in ('all', 'resources', 'typed-events'))):
            if len(gate.rows) != 2 or len(gate.compilations) != 2 or any(
                    row['status'] != 'RELEASED' or not row.get('observer_thread_joined') for row in gate.rows):
                raise RuntimeError('both derived denial Gates must complete original native observations and thread cleanup')
    except BaseException:
        report['error'] = traceback.format_exc();status, code = 'FAIL', 1
    finally:
        descriptions.cases = original_cases
        try:
            report['post_denial_gates'] = gate.evidence();gate.restore()
            # restore() reinstalls the outer logging callback, so restore its
            # exact saved original after releasing all gate contexts.
            rpc.RpcRunner.typed_event = original_event
        except BaseException:
            report['gate_cleanup_error'] = traceback.format_exc();status, code = 'FAIL', 1
        report['mount_cleanup'] = []
        for path in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
            report['mount_cleanup'].append({'path': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
            if result.returncode:status, code = 'FAIL', 1
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if any(line.split()[4] == str(path) or line.split()[4].startswith(str(path)+'/') for path in mounts)]
        if report['remaining_owned_mounts']:status, code = 'FAIL', 1
        elif alias is not None:alias.rmdir()
        # The nested /gp64 alias has retired, but the caller's /a64 workspace
        # remains live. Preserve that physical pin for its outer composition;
        # never invent a new opened /gp64 pin from the surviving log bytes.
        try:
            report['native_witness_logs_preserved'] = (
                preserved_native_witness_logs(report['native_witness_log_bindings'])
                and pins_preserved(report['native_witness_log_binding_document_pins']))
            if not report['native_witness_logs_preserved']:status, code = 'FAIL', 1
        except (OSError, ValueError, KeyError, RuntimeError) as error:
            report['native_witness_logs_preserved'] = False
            report['native_witness_log_preservation_error'] = str(error);status, code = 'FAIL', 1
        report['source_preserved'] = source_state(source, frozen) == initial and digest(manifest) == report['source_manifest_sha256']
        if not report['source_preserved']:status, code = 'FAIL', 1
        report['status'] = status;save()
    print(report['status'], outer/'report.json', flush=True)
    return code


if __name__ == '__main__':
    raise SystemExit(main())
