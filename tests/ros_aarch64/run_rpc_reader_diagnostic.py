#!/usr/bin/env python3
"""Run unchanged typed-event denial assertions with rejected-reader diagnostics.

Accepts run.py arguments after its own two options. The default selection runs
the original permission-denied and no-capability cases; --selection all retains
the complete original typed-event ordering to reproduce graph carry-over.
"""
import argparse
import importlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import traceback

sys.dont_write_bytecode = True
from runtime import digest, require_arm
from rebuild_affected import source_state
from build_rpc_reader_diagnostic import derived_header


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--diagnostic-prepared', type=Path, required=True)
    parser.add_argument('--selection', choices=('denied', 'all'), default='denied')
    own, remaining = parser.parse_known_args()
    # Mount mutations are confined to a private namespace, including when a
    # caller omitted unshare. The provided lease remains owned by the parent.
    if os.environ.get('UWVM_ARM_RPC_DIAG_NS') != '1':
        return subprocess.run(['unshare', '--mount', '--propagation', 'private', '--',
            sys.executable, '-B', __file__, *sys.argv[1:]],
            env={**os.environ, 'UWVM_ARM_RPC_DIAG_NS': '1'}).returncode
    if sys.platform != 'linux' or os.geteuid() != 0:
        raise RuntimeError('root Linux private namespace required')
    adapter_spec = importlib.util.spec_from_file_location('rpc_diagnostic_adapter', Path(__file__).with_name('run.py'))
    adapter = importlib.util.module_from_spec(adapter_spec);adapter_spec.loader.exec_module(adapter)
    sys.argv = [str(Path(__file__).with_name('run.py')), '--suite', 'rpc', *remaining]
    args, extra = adapter.parse()
    if extra:
        raise RuntimeError('diagnostic sets the original --version2 --mode typed-events itself')
    source = args.source.resolve(strict=True)
    manifest = source.parent/'source-manifest.json';frozen = json.loads(manifest.read_text())
    initial = source_state(source, frozen)
    prepared_path = own.diagnostic_prepared.resolve(strict=True)
    prepared = json.loads(prepared_path.read_text())
    original = source/'tests/ros_resources/typed_event_witness_ros2.h'
    if (prepared['status'] != 'PASS' or not prepared['source_preserved'] or not prepared['sdk_preserved'] or
            prepared['original_witness']['sha256'] != digest(original)):
        raise RuntimeError('diagnostic preparation/source binding is not qualified')
    architecture = prepared['architecture_evidence'];architecture_path = Path(architecture['path'])
    actual = json.loads(architecture_path.read_text())
    if (digest(architecture_path) != architecture['sha256'] or actual['status'] != 'PASS' or
            actual.get('fatal_errors') or actual.get('remaining_owned_processes')):
        raise RuntimeError('diagnostic preparation lacks valid ARM/cgroup compiler proof')
    binary = Path(prepared['binary']['path'])
    if require_arm(binary)['sha256'] != prepared['binary']['sha256']:
        raise RuntimeError('diagnostic ARM witness binary changed')
    header = Path(prepared['diagnostic_header']['path'])
    if digest(header) != prepared['diagnostic_header']['sha256'] or header.read_text() != derived_header(original.read_text()):
        raise RuntimeError('diagnostic derivation changed from original witness')
    for name in ('flags', 'link', 'compile_commands', 'cmake'):
        item = prepared[name]
        if digest(item['path']) != item['sha256']:
            raise RuntimeError('diagnostic actual compiler metadata changed: '+name)
    dependency = prepared['raw_dependencies']
    if digest(dependency['path']) != dependency['sha256'] or digest(dependency['actual_compile_path']) != dependency['sha256']:
        raise RuntimeError('diagnostic actual dependency file changed')
    for logical, row in dependency['inputs'].items():
        current = source/logical[6:] if logical.startswith('/work/') else Path(row['path'])
        if digest(current) != row['sha256'] or current.stat().st_size != row['bytes']:
            raise RuntimeError('diagnostic compiled input changed: '+logical)
    workspace, outer = args.workspace.resolve(strict=True), args.out.resolve()
    outer.mkdir(parents=True, mode=0o700, exist_ok=False)
    report = {'status': 'RUNNING', 'selection': own.selection,
        'scope': 'Original RpcRunner typed event calls and denial assertions; witness adds failure graph logging only',
        'source': str(source), 'source_manifest_sha256': digest(manifest),
        'driver_sha256': digest(__file__), 'adapter_sha256': digest(Path(__file__).with_name('run.py')),
        'diagnostic_prepared': {'path': str(prepared_path), 'sha256': digest(prepared_path)},
        'probe': prepared['binary'], 'events': [], 'snapshot_override': []}
    mounts, alias = [], None
    status, code = 'FAIL', 1
    sys.path.insert(0, str(source/'tests/ros'))
    network, rpc, descriptions = (importlib.import_module(name) for name in ('run_network', 'run_rpc', 'typed_parameter_events'))
    original_cases, original_snapshot, original_event = descriptions.cases, network.Runner.snapshot, rpc.RpcRunner.typed_event
    chosen = [row for row in original_cases() if own.selection == 'all' or row['scenario'] in ('permission-denied', 'no-capability')]
    if len(chosen) != (29 if own.selection == 'all' else 2):
        raise RuntimeError('original typed event case inventory changed')
    report['selected_original_cases'] = chosen

    def save():
        (outer/'report.json').write_text(json.dumps(report, indent=2)+'\n')

    def checked(command):
        subprocess.run([str(value) for value in command], check=True, capture_output=True, timeout=30)

    def snapshot(receiver, input_path, destination, label):
        selected = binary if label == 'ros2/uwvm-ros2-resources-probe' else input_path
        original_snapshot(receiver, selected, destination, label)
        if selected == binary:
            if digest(destination) != prepared['binary']['sha256']:
                raise RuntimeError('diagnostic snapshot differs from actual compiled witness')
            report['snapshot_override'].append({'label': label, 'original': str(input_path),
                'diagnostic': str(binary), 'snapshot': str(destination), 'sha256': digest(destination)})

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

    try:
        checked(['mount', '--make-rprivate', '/'])
        for candidate in ('/rd64', '/dr64', '/rr64', '/rp64'):
            if not Path(candidate).exists():
                alias = Path(candidate);alias.mkdir(mode=0o700);break
        if alias is None:
            raise RuntimeError('no fresh short private diagnostic workspace alias available')
        checked(['mount', '--bind', workspace, alias]);mounts.append(alias)
        root = (args.ros2_rootfs or workspace/'gateway/jazzy-rootfs').resolve(strict=True)
        for name in ('proc', 'sys', 'dev', 'work'):(root/name).mkdir(exist_ok=True)
        for name in ('proc', 'sys', 'dev'):
            checked(['mount', '--rbind', '/'+name, root/name]);mounts.append(root/name)
            checked(['mount', '--make-rprivate', root/name])
        checked(['mount', '--bind', source, root/'work']);mounts.append(root/'work')
        checked(['mount', '-o', 'remount,bind,ro', root/'work'])
        descriptions.cases = lambda: list(chosen)
        network.Runner.snapshot = snapshot;rpc.RpcRunner.typed_event = event
        own_args = ['--suite', 'rpc', *remaining]
        for flag, value in [('--workspace', alias), ('--out', outer/'architecture'), ('--ros2-rootfs', root)]:
            if flag in own_args:
                own_args[own_args.index(flag)+1] = str(value)
            else:
                own_args.extend([flag, str(value)])
        sys.argv = [str(Path(__file__).with_name('run.py')), *own_args, '--version', '2', '--mode', 'typed-events']
        code = adapter.main();status = 'PASS' if code == 0 else 'FAIL'
        report['adapter_evidence'] = {'path': str(outer/'architecture/architecture-evidence.json'),
            'sha256': digest(outer/'architecture/architecture-evidence.json')}
    except BaseException:
        report['error'] = traceback.format_exc()
    finally:
        descriptions.cases = original_cases;network.Runner.snapshot = original_snapshot;rpc.RpcRunner.typed_event = original_event
        report['mount_cleanup'] = []
        for path in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
            report['mount_cleanup'].append({'path': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
            if result.returncode:status, code = 'FAIL', 1
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if any(line.split()[4] == str(path) or line.split()[4].startswith(str(path)+'/') for path in mounts)]
        if report['remaining_owned_mounts']:status, code = 'FAIL', 1
        elif alias is not None:alias.rmdir()
        report['source_preserved'] = source_state(source, frozen) == initial and digest(manifest) == report['source_manifest_sha256']
        if not report['source_preserved']:status, code = 'FAIL', 1
        report['status'] = status;save()
    print(report['status'], outer/'report.json', flush=True)
    return code


if __name__ == '__main__':
    raise SystemExit(main())
